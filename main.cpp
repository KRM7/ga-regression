#include "genetic_regression.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GeneticRegression w;
    w.show();

    return a.exec();
}
