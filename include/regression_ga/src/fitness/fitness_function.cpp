/* Copyright (c) 2021 Krisztián Rugási. */

#include "fitness_function.h"
#include "converter.h"
#include "decoder.h"
#include "token.h"
#include "../genetic/gene.h"

#include <algorithm>
#include <vector>
#include <limits>
#include <cmath>
#include <cstddef>
#include <cassert>
#include <cstdlib>

/* Constructors. */

FitnessFunction::FitnessFunction(const std::vector<double>& x, const std::vector<double>& fx_desired, Objective error_metric) :
	x_(x),
	fx_desired_(fx_desired),
	error_metric_(error_metric)
{
	assert(x.size() == fx_desired.size());
}

/* Setters. */

void FitnessFunction::data(const std::vector<double>& x, const std::vector<double>& fx_desired)
{
	assert(x.size() == fx_desired.size());

	x_ = x;
	fx_desired_ = fx_desired;
}

void FitnessFunction::error_metric(Objective error_metric)
{
    error_metric_ = error_metric;
}

/* Fitness function call. */
std::vector<double> FitnessFunction::operator()(const std::vector<Gene>& chrom) const
{
	std::vector<Token> infix_expr = Converter::chromosomeToInfix(chrom);
    std::vector<Token> postfix_expr = Converter::infixToPostfix(infix_expr);

    std::vector<double> fx_actual = Decoder::evalPostfixExpr(postfix_expr, x_);

	double error;
	switch (error_metric_)
	{
		case Objective::LS:
			error = squareErrorMean(fx_actual, fx_desired_);
			break;
		case Objective::LAD:
			error = absoluteErrorMean(fx_actual, fx_desired_);
			break;
		case Objective::RMSE:
			error = rootMeanSquareError(fx_actual, fx_desired_);
			break;
		case Objective::MINMAX:
			error = maximumError(fx_actual, fx_desired_);
			break;
		default:
			assert(false);	/* Invalid objective, shouldn't get here. */
			std::abort();
	}

    double fitness = std::isnan(error) ? 0.0 : 1.0 / error;

    return { fitness };
}

/* Objective functions. */

double FitnessFunction::squareErrorMean(const std::vector<double>& fx_actual, const std::vector<double>& fx_desired)
{
    assert(fx_actual.size() == fx_desired.size());

    double mean = 0.0;
    for (size_t i = 0; i < fx_actual.size(); i++)
    {
        double error_sq = (fx_actual[i] - fx_desired[i]) * (fx_actual[i] - fx_desired[i]);
        error_sq = std::min(error_sq, std::numeric_limits<double>::max());

        mean += error_sq / double(fx_actual.size());
    }

    return mean;
}

double FitnessFunction::absoluteErrorMean(const std::vector<double>& fx_actual, const std::vector<double>& fx_desired)
{
    assert(fx_actual.size() == fx_desired.size());

    double mean = 0.0;
    for (size_t i = 0; i < fx_actual.size(); i++)
    {
        double error_abs = std::abs(fx_actual[i] - fx_desired[i]);
        error_abs = std::min(error_abs, std::numeric_limits<double>::max());

        mean += error_abs / double(fx_actual.size());
    }

    return mean;
}

double FitnessFunction::rootMeanSquareError(const std::vector<double>& fx_actual, const std::vector<double>& fx_desired)
{
    assert(fx_actual.size() == fx_desired.size());

    double mean = 0.0;
    for (size_t i = 0; i < fx_actual.size(); i++)
    {
        double error_sq = (fx_actual[i] - fx_desired[i]) * (fx_actual[i] - fx_desired[i]);
        error_sq = std::min(error_sq, std::numeric_limits<double>::max());

        mean += error_sq / double(fx_actual.size());
    }

    return std::sqrt(mean);
}

double FitnessFunction::maximumError(const std::vector<double>& fx_actual, const std::vector<double>& fx_desired)
{
    assert(fx_actual.size() == fx_desired.size());

    double error_max = std::abs(fx_actual[0] - fx_desired[0]);
    for (size_t i = 1; i < fx_actual.size(); i++)
    {
        error_max = std::max(error_max, std::abs(fx_actual[i] - fx_desired[i]));
    }

    return std::min(error_max, std::numeric_limits<double>::max());
}