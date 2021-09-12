/* Copyright (c) 2021 Krisztián Rugási. */

/* Fitness function class. */

#ifndef FITNESS_FUNCTION_H
#define FITNESS_FUNCTION_H

#include "../genetic/gene.h"

#include <vector>

/* The fitness function used in the GA. */
class FitnessFunction
{
public:
    
    /* The error metrics that can be used for the fitness function. */
    enum class Objective
    {
        LS,
        LAD,
        RMSE,
        MINMAX
    };

    /* Contructors. */
    FitnessFunction() = delete;
    FitnessFunction(const std::vector<double>& x, const std::vector<double>& fx_desired, Objective error_metric);

    /* Setters. */
    void data(const std::vector<double>& x, const std::vector<double>& fx_desired);
    void error_metric(Objective error_metric);

    /* Calc the fitness of chrom (higher is better). */
    std::vector<double> operator()(const std::vector<Gene>& chrom) const;

private:

    std::vector<double> x_;             /* The data points at which to evaluate the chromosomes. */
    std::vector<double> fx_desired_;    /* The value of the data points at each x. */
    Objective error_metric_;            /* The error metric used in the fitness function. */

    /* Objective functions/error metrics. */

    static double squareErrorMean(const std::vector<double>& fx_actual, const std::vector<double>& fx_desired);
    static double absoluteErrorMean(const std::vector<double>& fx_actual, const std::vector<double>& fx_desired);
    static double rootMeanSquareError(const std::vector<double>& fx_actual, const std::vector<double>& fx_desired);
    static double maximumError(const std::vector<double>& fx_actual, const std::vector<double>& fx_desired);
};

#endif // !FITNESS_FUNCTION_H