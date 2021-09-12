#include "genetic_regression.h"
#include "ui_genetic_regression.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QPixmap>
#include <QModelIndex>
#include <QPen>
#include <QRgba64>

#include "include/regression_ga/src/genetic/ga.h"
#include "include/regression_ga/src/fitness/fitness_function.h"
#include "include/regression_ga/src/fitness/converter.h"
#include "include/regression_ga/src/fitness/token.h"
#include "include/regression_ga/src/printer.h"
#include "include/regression_ga/src/io_utils.h"

#include <algorithm>
#include <vector>
#include <string>
#include <utility>
#include <numeric>
#include <exception>
#include <stdexcept>
#include <cmath>
#include <cstddef>


QVector<QPointF> GeneticRegression::statsToPoints(const std::vector<double>& stats)
{
    QVector<QPointF> points;
    points.reserve(stats.size());
    for (size_t i = 0; i < stats.size(); i++)
    {
        points.emplace_back(double(i + 1), stats[i]);
    }

    return points;
}


/* Initialize UI. */
GeneticRegression::GeneticRegression(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GeneticRegression)
{
    ui->setupUi(this);

    ui->tabSettings->setAutoFillBackground(true);
    ui->tabResults->setAutoFillBackground(true);
    ui->tabStats->setAutoFillBackground(true);

    /* Setup results chart. */

    data_points->setName("Data");
    data_points->setColor(QRgb(0x6232a8));
    data_points->setBorderColor(Qt::transparent);
    data_points->setMarkerSize(6.5);
    data_points->setMarkerShape(QScatterSeries::MarkerShapeRectangle);

    function_points->setName("Function");
    QPen result_pen = function_points->pen();
    result_pen.setWidthF(2.25);
    result_pen.setColor(QRgb(0xde1010));
    function_points->setPen(result_pen);

    results_chart->addSeries(data_points);
    results_chart->addSeries(function_points);
    results_chart->legend()->setAlignment(Qt::AlignRight);

    resultAxisX->setTitleText("x");
    resultAxisX->setTickCount(10);
    resultAxisX->setMinorTickCount(9);
    resultAxisX->setGridLineColor(QRgb(0x808080));
    results_chart->addAxis(resultAxisX, Qt::AlignBottom);
    data_points->attachAxis(resultAxisX);
    function_points->attachAxis(resultAxisX);

    resultAxisY->setTitleText("f(x)");
    resultAxisY->setTickCount(5);
    resultAxisY->setMinorTickCount(9);
    resultAxisY->setGridLineColor(QRgb(0x808080));
    results_chart->addAxis(resultAxisY, Qt::AlignLeft);
    data_points->attachAxis(resultAxisY);
    function_points->attachAxis(resultAxisY);

    ui->resultChartView->setChart(results_chart);


    /* Setup stats chart. */

    fitness_best->setName("Best fitness");
    QPen best_pen = fitness_best->pen();
    best_pen.setWidthF(2.0);
    best_pen.setColor(QRgb(0x1fc224));
    fitness_best->setPen(best_pen);

    fitness_mean->setName("Mean fitness");
    QPen mean_pen = fitness_mean->pen();
    mean_pen.setWidthF(2.0);
    mean_pen.setColor(QRgb(0x391bcf));
    fitness_mean->setPen(mean_pen);

    fitness_sd->setName("Fitness SD");
    fitness_sd->setColor(QRgb(0xd41717));

    stats_chart->addSeries(fitness_best);
    stats_chart->addSeries(fitness_mean);
    stats_chart->addSeries(fitness_sd);
    stats_chart->legend()->setAlignment(Qt::AlignRight);

    statAxisX->setTitleText("Generation");
    statAxisX->setTickCount(10);
    statAxisX->setMinorTickCount(4);
    statAxisX->setGridLineColor(QRgb(0xb3b3b3));
    statAxisX->setLabelFormat("%d");
    stats_chart->addAxis(statAxisX, Qt::AlignBottom);
    fitness_best->attachAxis(statAxisX);
    fitness_mean->attachAxis(statAxisX);
    fitness_sd->attachAxis(statAxisX);

    statAxisY->setTitleText("Fitness");
    statAxisY->setTickCount(6);
    statAxisY->setMinorTickCount(4);
    statAxisY->setGridLineColor(QRgb(0xb3b3b3));
    stats_chart->addAxis(statAxisY, Qt::AlignLeft);
    fitness_best->attachAxis(statAxisY);
    fitness_mean->attachAxis(statAxisY);
    fitness_sd->attachAxis(statAxisY);

    ui->statsChartView->setChart(stats_chart);
}

/* Destructor. */
GeneticRegression::~GeneticRegression()
{
    delete ui;
}


/* Run the genetic algorithm and display the results. */
void GeneticRegression::on_pushButtonRUN_clicked()
{
    /* Setup fitness function. */

    std::vector<double> x, fx_desired;

    /* Fit function based on all of the data points. */
    if (!ui->panelRange->isChecked())
    {
        x = x_data;
        fx_desired = fx_data;

        if (x.size() < 2)
        {
            QMessageBox::critical(this, "Error", "There are not enough data points to fit a function (need at least 2).");
            return;
        }
    }
    /* Only use the data points in the specified range for fitting the function. */
    else
    {
        for (size_t i = 0; i < x_data.size(); i++)
        {
            if (ui->inputRangeMin->value() <= x_data[i] && x_data[i] <= ui->inputRangeMax->value())
            {
                x.push_back(x_data[i]);
                fx_desired.push_back(fx_data[i]);
            }
        }
        if (x.size() < 2)
        {
            QMessageBox::critical(this, "Error", "There are not enough data points in the specified x interval to fit a function (need at least 2).");
            return;
        }
    }
    FitnessFunction fitness_function(x, fx_desired, static_cast<FitnessFunction::Objective>(ui->comboBoxObjective->currentIndex()));

    /* Setup GA. */
    size_t chrom_len = size_t(ui->inputNumFuncs->value());

    std::vector<std::pair<double, double>> bounds;
    bounds.push_back({ ui->inputCoeffAmin->value(),
                       ui->inputCoeffAmax->value() });
    bounds.push_back({ ui->inputCoeffBmin->value(),
                       ui->inputCoeffBmax->value() });
    bounds.push_back({ ui->inputCoeffCmin->value(),
                       ui->inputCoeffCmax->value() });
    bounds.push_back({ ui->inputCoeffDmin->value(),
                       ui->inputCoeffDmax->value() });
    bounds.push_back({ ui->inputCoeffNmin->value(),
                       ui->inputCoeffNmax->value() });

    mGA algorithm(chrom_len, fitness_function, fmask, opmask, bounds);

    /* General settings. */
    algorithm.population_size(size_t(ui->inputPopsize->value()));
    algorithm.max_gen(size_t(ui->inputMaxgen->value()));
    algorithm.crossover_rate(ui->inputCxRate->value());
    algorithm.mutation_rate(ui->inputMxRate->value());

    /* Selection settings. */
    size_t selection_method = size_t(ui->comboBoxSelection->currentIndex());
    switch (selection_method)
    {
        case 0:
            algorithm.selection_method(mGA::SogaSelection::roulette);
            break;
        case 1:
            algorithm.selection_method(mGA::SogaSelection::tournament);
            break;
        case 2:
            algorithm.selection_method(mGA::SogaSelection::rank);
            break;
        case 3:
            algorithm.selection_method(mGA::SogaSelection::sigma);
            break;
        case 4:
            algorithm.selection_method(mGA::SogaSelection::boltzmann);
            break;
        default:
            QMessageBox::critical(this, "Error", "Invalid selection method.");
            return;
    }

    /* Crossover settings. */
    size_t crossover_method = size_t(ui->comboBoxCrossover->currentIndex());
    switch (crossover_method)
    {
        case 0:
            algorithm.crossover_method(mGA::CrossoverMethod::BLXa);
            algorithm.blx_crossover_param(ui->inputCxParam->value());
            break;
        case 1:
            algorithm.crossover_method(mGA::CrossoverMethod::simulated_binary);
            algorithm.sim_binary_crossover_param(ui->inputCxParam->value());
            break;
        case 2:
            algorithm.crossover_method(mGA::CrossoverMethod::wright);
            break;
        default:
            QMessageBox::critical(this, "Error", "Invalid crossover method.");
            return;
    }

    /* Mutation settings. */
    size_t mutation_method = size_t(ui->comboBoxMutation->currentIndex());
    switch (mutation_method)
    {
        case 0:
            algorithm.mutation_method(mGA::MutationMethod::random);
            break;
        case 1:
            algorithm.mutation_method(mGA::MutationMethod::boundary);
            break;
        case 2:
            algorithm.mutation_method(mGA::MutationMethod::gauss);
            algorithm.gauss_mutation_param(ui->inputMxParam->value());
            break;
        default:
            QMessageBox::critical(this, "Error", "Invalid mutation method.");
            return;
    }

    /* Handle preset function form if used. */
    if (ui->panelPresetF->isChecked())
    {
        std::string preset_input = ui->inputPresetF->toPlainText().toStdString();
        try
        {
            std::vector<int> preset_form = presetStringToFForm(preset_input);
            algorithm.use_preset_form = true;
            algorithm.chrom_len((preset_form.size() + 1)/2);
            algorithm.preset_form = preset_form;
        }
        catch (const std::exception& e)
        {
            std::string error_msg = std::string(e.what());
            error_msg.append("\nThe input can only contain valid function indices and valid operators forming a valid infix expression.");
            error_msg.append("\neg.: 1+5 or 2*4-1/6");
            QMessageBox::critical(this, "Error", QString(error_msg.c_str()));

            return;
        }
    }

    /* Handle progress bar. */
    ui->progressBar->setFormat(" Generation %v/%m");
    ui->progressBar->setValue(1);
    ui->progressBar->setMaximum(int(algorithm.max_gen()));

    auto updateProgressBar =
    [this](const mGA::GA* ga) -> void
    {
        ui->progressBar->setValue(ui->progressBar->value() + 1);
        if (ga->generation_cntr() % 50 == 0)
        {
            QApplication::processEvents();
        }
    };
    algorithm.endOfGenerationCallback = updateProgressBar;

    /* Update UI. */
    ui->tabControl->setCurrentIndex(1);
    ui->labelResult->setText(" f(x) = ");
    function_points->clear();

    /* Set result chart axisX range (custom interval case). */
    if (ui->panelRange->isChecked())
    {
        auto [x_min, x_max] = axisMinMax(x, 0.0);
        auto [fx_min, fx_max] = axisMinMax(fx_desired, 0.0);
        ui->resultChartView->chart()->axes(Qt::Horizontal)[0]->setRange(x_min, x_max);
        ui->resultChartView->chart()->axes(Qt::Vertical)[0]->setRange(fx_min, fx_max);
    }

    auto sols = algorithm.run();

    /* Display results. */
    std::vector<Token> sol_infix = Converter::chromosomeToInfix(sols[0].chromosome);
    std::vector<Token> sol_postfix = Converter::infixToPostfix(sol_infix);
    std::string sol_str = Printer::print(sol_infix);
    std::vector<std::pair<double, double>> sol_points = drawFunction(sol_postfix, resultAxisX->min(), resultAxisX->max(), 1000);

    /* Display function on results chart. */
    QVector<QPointF> points;
    points.reserve(sol_points.size());
    for (const auto& point : sol_points)
    {
      points.emplace_back(point.first, point.second);
    }
    function_points->replace(points);

    /* Display function as text. */
    ui->labelResult->setText(ui->labelResult->text().append(sol_str.c_str()));

    /* Display stats. */
    auto history = algorithm.soga_history();
    fitness_best->replace(statsToPoints(history.fitness_max));
    fitness_mean->replace(statsToPoints(history.fitness_mean));
    fitness_sd->replace(statsToPoints(history.fitness_sd));
    /* Update axis limits. */
    auto [best_min, best_max] = axisMinMax(history.fitness_max);
    auto [sd_min, sd_max] = axisMinMax(history.fitness_sd);
    ui->statsChartView->chart()->axes(Qt::Horizontal)[0]->setRange(1.0, double(history.fitness_max.size()));
    ui->statsChartView->chart()->axes(Qt::Vertical)[0]->setRange(std::min(best_min, sd_min), std::max(best_max, sd_max));
    statAxisX->applyNiceNumbers();
    statAxisY->applyNiceNumbers();

    ui->actionSaveResults->setEnabled(true);
    ui->actionSaveStats->setEnabled(true);

    /* Everything done. */
    ui->progressBar->setFormat("Done!");
}

/* Load data points from file. */
void GeneticRegression::on_actionOpenData_triggered()
{
    QString file_name = QFileDialog::getOpenFileName(this,
                                                     "Open the data file",
                                                     QCoreApplication::applicationDirPath(),
                                                     "(*.txt; *.csv)");

    if (file_name.toStdString() != "")
    {
        try
        {
            /* Read data points. */
            std::tie(x_data, fx_data) = readData(file_name.toStdString());
        }
        catch (const std::exception& e)
        {
            std::string msg = "Couldn't read data from file.\n";
            msg.append(e.what());
            QMessageBox::critical(this, "Error", QString(msg.c_str()));
            return;
        }

        /* Run button. */
        data_loaded = true;
        bool ready = data_loaded && operator_selected && function_selected;
        ui->pushButtonRUN->setEnabled(ready);

        /* Display data file name. */
        QString file_label = "Data file:  ";
        file_label.append(file_name);
        ui->labelFilepath->setText(file_label);

        /* Display data points on chart. */
        QVector<QPointF> points;
        points.reserve(x_data.size());
        for (size_t i = 0; i < x_data.size(); i++)
        {
            points.emplace_back(x_data[i], fx_data[i]);
        }
        data_points->replace(points);
        function_points->clear();
        fitness_best->clear();
        fitness_mean->clear();
        fitness_sd->clear();

        /* Update axis limits */
        auto [x_min, x_max] = axisMinMax(x_data);
        auto [fx_min, fx_max] = axisMinMax(fx_data);
        ui->resultChartView->chart()->axes(Qt::Horizontal)[0]->setRange(x_min, x_max);
        ui->resultChartView->chart()->axes(Qt::Vertical)[0]->setRange(fx_min, fx_max);
        resultAxisX->applyNiceNumbers();
        resultAxisY->applyNiceNumbers();

        /* Switch to results tab to show the data points on the chart. */
        ui->tabControl->setCurrentIndex(1);

        ui->actionSaveResults->setEnabled(false);
        ui->actionSaveStats->setEnabled(false);
    }
}

/* Save results chart as an image. */
void GeneticRegression::on_actionSaveResults_triggered()
{
    QPixmap img(ui->resultChartView->size());
    QPainter p(&img);
    ui->resultChartView->render(&p);

    QString filename = QFileDialog::getSaveFileName(this,
                                                    "Save results chart",
                                                    QCoreApplication::applicationDirPath(),
                                                    "(*.png)");

    img.save(filename);
}

/* Save stats chart as an image. */
void GeneticRegression::on_actionSaveStats_triggered()
{
    QPixmap img(ui->statsChartView->size());
    QPainter p(&img);
    ui->statsChartView->render(&p);

    QString filename = QFileDialog::getSaveFileName(this,
                                                    "Save stats chart",
                                                    QCoreApplication::applicationDirPath(),
                                                    "(*.png)");

    img.save(filename);
}

/* Check how many base functions are selected and enable run button accordingly. */
void GeneticRegression::on_listFunctions_itemSelectionChanged()
{
    QModelIndexList selected_funcs = ui->listFunctions->selectionModel()->selectedIndexes();

    /* Update fmask. */
    fmask = std::string(ui->listFunctions->count(), '0');
    for (int i = 0; i < selected_funcs.size(); i++)
    {
        int idx = selected_funcs[i].row();
        fmask[idx] = '1';
    }

    /* Update run button. */
    function_selected = selected_funcs.size() != 0;
    bool ready = data_loaded && operator_selected && function_selected;
    ui->pushButtonRUN->setEnabled(ready);
}

/* Function list selection buttons */
void GeneticRegression::on_pushButtonSelectFuncs_clicked()
{
    ui->listFunctions->selectAll();
}
void GeneticRegression::on_pushButtonClearFuncs_clicked()
{
    ui->listFunctions->clearSelection();
}

/* Disable/enable function list and operators, use preset field to determine function form. */
void GeneticRegression::on_panelPresetF_toggled(bool on)
{
    ui->panelFunctions->setEnabled(!on);
    ui->panelOperators->setEnabled(!on);

    function_selected = on;
    operator_selected = on ||
                        ui->checkBoxOpAdd->checkState() == 2 ||
                        ui->checkBoxOpMul->checkState() == 2 ||
                        ui->checkBoxOpDiv->checkState() == 2 ||
                        ui->checkBoxOpPow->checkState() == 2;
    bool ready = data_loaded && operator_selected && function_selected;
    ui->pushButtonRUN->setEnabled(ready);   
}

/* Check if there are any operators selected after change, and update opmask. */
void GeneticRegression::on_checkBoxOpAdd_stateChanged(int new_state)
{
    operator_selected = new_state == 2 ||
                        ui->checkBoxOpMul->checkState() == 2 ||
                        ui->checkBoxOpDiv->checkState() == 2 ||
                        ui->checkBoxOpPow->checkState() == 2;

    /* Update run button */
    bool ready = data_loaded && operator_selected && function_selected;
    ui->pushButtonRUN->setEnabled(ready);

    /* Update opmask. */
    opmask[0] = new_state ? '1' : '0';
    opmask[1] = new_state ? '1' : '0';
}
void GeneticRegression::on_checkBoxOpMul_stateChanged(int new_state)
{
    operator_selected = ui->checkBoxOpAdd->checkState() == 2 ||
                        new_state == 2 ||
                        ui->checkBoxOpDiv->checkState() == 2 ||
                        ui->checkBoxOpPow->checkState() == 2;

    /* Update run button */
    bool ready = data_loaded && operator_selected && function_selected;
    ui->pushButtonRUN->setEnabled(ready);

    /* Update opmask. */
    opmask[2] = new_state ? '1' : '0';
}
void GeneticRegression::on_checkBoxOpDiv_stateChanged(int new_state)
{
    operator_selected = ui->checkBoxOpAdd->checkState() == 2 ||
                        ui->checkBoxOpMul->checkState() == 2 ||
                        new_state == 2 ||
                        ui->checkBoxOpPow->checkState() == 2;

    /* Update run button */
    bool ready = data_loaded && operator_selected && function_selected;
    ui->pushButtonRUN->setEnabled(ready);

    /* Update opmask. */
    opmask[3] = new_state ? '1' : '0';
}
void GeneticRegression::on_checkBoxOpPow_stateChanged(int new_state)
{
    operator_selected = ui->checkBoxOpAdd->checkState() == 2 ||
                        ui->checkBoxOpMul->checkState() == 2 ||
                        ui->checkBoxOpDiv->checkState() == 2 ||
                        new_state == 2;

    /* Update run button */
    bool ready = data_loaded && operator_selected && function_selected;
    ui->pushButtonRUN->setEnabled(ready);

    /* Update opmask. */
    opmask[4] = new_state ? '1' : '0';

}

/* Update crossover parameter stuff according to the crossover method chosen. */
void GeneticRegression::on_comboBoxCrossover_currentIndexChanged(int selected_index)
{
    /* Enable/disable parameter stuff. */
    bool has_param = selected_index == 0 || selected_index == 1;

    ui->labelCxParam->setEnabled(has_param);
    ui->labelCxParam->setVisible(has_param);
    ui->inputCxParam->setEnabled(has_param);
    ui->inputCxParam->setVisible(has_param);

    /* BLX-alpha crossover specific. */
    if (selected_index == 0)
    {
        ui->labelCxParam->setText("Parameter alpha: ");
        ui->inputCxParam->setMinimum(0.0);
        ui->inputCxParam->setMaximum(2.0);
        ui->inputCxParam->setSingleStep(0.1);
        ui->inputCxParam->setValue(0.5);
        ui->inputCxParam->setDecimals(2);
    }
    /* Simulated binary crossover specific. */
    else if (selected_index == 1)
    {
        ui->labelCxParam->setText("Parameter eta: ");
        ui->inputCxParam->setMinimum(0.0);
        ui->inputCxParam->setMaximum(200.0);
        ui->inputCxParam->setSingleStep(0.1);
        ui->inputCxParam->setValue(4.0);
        ui->inputCxParam->setDecimals(1);
    }
}

/* Update mutation parameter stuff according to the mutation method chosen. */
void GeneticRegression::on_comboBoxMutation_currentIndexChanged(int selected_index)
{
    /* Enable/disable parameter stuff. */
    bool has_param = selected_index == 2;

    ui->labelMxParam->setEnabled(has_param);
    ui->labelMxParam->setVisible(has_param);
    ui->inputMxParam->setEnabled(has_param);
    ui->inputMxParam->setVisible(has_param);

    /* Gauss mutation method specific. */
    if (selected_index == 2)
    {
        ui->labelMxParam->setText("Sigma parameter: ");
        ui->inputMxParam->setMinimum(1.0);
        ui->inputMxParam->setMaximum(100.0);
        ui->inputMxParam->setSingleStep(0.1);
        ui->inputMxParam->setValue(6.0);
        ui->inputMxParam->setDecimals(1);
    }
}

/* Update coefficient intervals so the maximum can never be smaller than the minimum. */
void GeneticRegression::on_inputCoeffAmin_valueChanged(double new_value)
{
    ui->inputCoeffAmax->setMinimum(new_value);
}
void GeneticRegression::on_inputCoeffAmax_valueChanged(double new_value)
{
    ui->inputCoeffAmin->setMaximum(new_value);
}
void GeneticRegression::on_inputCoeffBmin_valueChanged(double new_value)
{
    ui->inputCoeffBmax->setMinimum(new_value);
}
void GeneticRegression::on_inputCoeffBmax_valueChanged(double new_value)
{
    ui->inputCoeffBmin->setMaximum(new_value);
}
void GeneticRegression::on_inputCoeffCmin_valueChanged(double new_value)
{
    ui->inputCoeffCmax->setMinimum(new_value);
}
void GeneticRegression::on_inputCoeffCmax_valueChanged(double new_value)
{
    ui->inputCoeffCmin->setMaximum(new_value);
}
void GeneticRegression::on_inputCoeffDmin_valueChanged(double new_value)
{
    ui->inputCoeffDmax->setMinimum(new_value);
}
void GeneticRegression::on_inputCoeffDmax_valueChanged(double new_value)
{
    ui->inputCoeffDmin->setMaximum(new_value);
}
void GeneticRegression::on_inputCoeffNmin_valueChanged(double new_value)
{
    ui->inputCoeffNmax->setMinimum(new_value);
}
void GeneticRegression::on_inputCoeffNmax_valueChanged(double new_value)
{
    ui->inputCoeffNmin->setMaximum(new_value);
}

/* Reset results graph x axis min, max values if there is already data loaded. */
void GeneticRegression::on_panelRange_toggled(bool on)
{
    if (!on && data_loaded)
    {
        auto [x_min, x_max] = axisMinMax(x_data);
        auto [fx_min, fx_max] = axisMinMax(fx_data);
        ui->resultChartView->chart()->axes(Qt::Horizontal)[0]->setRange(x_min, x_max);
        ui->resultChartView->chart()->axes(Qt::Vertical)[0]->setRange(fx_min, fx_max);
        resultAxisX->applyNiceNumbers();
        resultAxisY->applyNiceNumbers();
    }
}

/* Update range min/max intervals so the minimum can never be larger than the maximum. */
void GeneticRegression::on_inputRangeMin_valueChanged(double new_value)
{
    ui->inputRangeMax->setMinimum(new_value);
}
void GeneticRegression::on_inputRangeMax_valueChanged(double new_value)
{
    ui->inputRangeMin->setMaximum(new_value);
}

