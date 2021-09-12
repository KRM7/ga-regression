#ifndef GENETICREGRESSION_H
#define GENETICREGRESSION_H

#include <QMainWindow>
#include <QChart>
#include <QScatterSeries>
#include <QLineSeries>
#include <QValueAxis>

#include <vector>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class GeneticRegression; }
QT_END_NAMESPACE

class GeneticRegression : public QMainWindow
{
    Q_OBJECT

public:
    GeneticRegression(QWidget *parent = nullptr);
    ~GeneticRegression();

private slots:

    void on_pushButtonRUN_clicked();


    void on_actionOpenData_triggered();

    void on_actionSaveResults_triggered();

    void on_actionSaveStats_triggered();


    void on_listFunctions_itemSelectionChanged();

    void on_pushButtonSelectFuncs_clicked();

    void on_pushButtonClearFuncs_clicked();

    void on_panelPresetF_toggled(bool on);


    void on_checkBoxOpAdd_stateChanged(int new_state);

    void on_checkBoxOpMul_stateChanged(int new_state);

    void on_checkBoxOpDiv_stateChanged(int new_state);

    void on_checkBoxOpPow_stateChanged(int new_state);


    void on_comboBoxCrossover_currentIndexChanged(int selected_index);

    void on_comboBoxMutation_currentIndexChanged(int selected_index);


    void on_inputCoeffAmin_valueChanged(double new_value);

    void on_inputCoeffAmax_valueChanged(double new_value);

    void on_inputCoeffBmin_valueChanged(double new_value);

    void on_inputCoeffBmax_valueChanged(double new_value);

    void on_inputCoeffCmin_valueChanged(double new_value);

    void on_inputCoeffCmax_valueChanged(double new_value);

    void on_inputCoeffDmin_valueChanged(double new_value);

    void on_inputCoeffDmax_valueChanged(double new_value);

    void on_inputCoeffNmin_valueChanged(double new_value);

    void on_inputCoeffNmax_valueChanged(double new_value);


    void on_panelRange_toggled(bool on);

    void on_inputRangeMin_valueChanged(double new_value);

    void on_inputRangeMax_valueChanged(double arg1);

private:

    Ui::GeneticRegression *ui;

    bool data_loaded = false;
    bool operator_selected = true;
    bool function_selected = false;

    std::vector<double> x_data;
    std::vector<double> fx_data;

    std::string opmask = "11000";
    std::string fmask = std::string(19, '0');

    QChart* results_chart = new QChart();
    QChart* stats_chart = new QChart();

    QValueAxis *resultAxisX = new QValueAxis();
    QValueAxis *resultAxisY = new QValueAxis();
    QValueAxis *statAxisX = new QValueAxis();
    QValueAxis *statAxisY = new QValueAxis();

    QScatterSeries* data_points = new QScatterSeries();
    QLineSeries* function_points = new QLineSeries();
    QLineSeries* fitness_best = new QLineSeries();
    QLineSeries* fitness_mean = new QLineSeries();
    QLineSeries* fitness_sd = new QLineSeries();

    QVector<QPointF> statsToPoints(const std::vector<double>& stats);

};

#endif // GENETICREGRESSION_H
