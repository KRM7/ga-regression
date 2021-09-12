/* Copyright (c) 2021 Krisztián Rugási. */

#ifndef UTILS_H_
#define UTILS_H_

#include "fitness/token.h"

#include <vector>
#include <string>
#include <utility>
#include <cstddef>


/* Return the points of the function represented by postfix_expr (x, fx values) for num_points number of equally spaced points between lbound and ubound. */
std::vector<std::pair<double, double>> drawFunction(const std::vector<Token>& postfix_expr, double lbound, double ubound, size_t num_points);

/* Read data points from a file, returning a vector of the x and the fx values of the points in the file. */
std::pair<std::vector<double>, std::vector<double>> readData(const std::string& path);

/* Calc the minimum and maximum values for a chart's axis values from the points displayed. */
std::pair<double, double> axisMinMax(const std::vector<double>& values, double pad = 0.1);

/* Convert the string from the preset field to a vector of operators and function indices. */
std::vector<int> presetStringToFForm(std::string& preset);


#endif // !UTILS_H_
