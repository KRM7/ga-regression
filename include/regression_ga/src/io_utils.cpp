/* Copyright (c) 2021 Krisztián Rugási. */

#include "io_utils.h"
#include "fitness/decoder.h"
#include "fitness/token.h"
#include "fitness/math_ops.h"

#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <utility>
#include <cwctype>
#include <cstddef>
#include <cstdlib>
#include <cassert>
#include <stdexcept>


std::vector<std::pair<double, double>> drawFunction(const std::vector<Token>& postfix_expr, double lbound, double ubound, size_t num_points)
{
    assert(lbound < ubound);
    assert(num_points > 0);

    double increment = (ubound - lbound) / num_points;

    std::vector<double> x(num_points);
    for (size_t i = 0; i < x.size(); i++)
    {
        x[i] = lbound;
        lbound += increment;
    }
    std::vector<double> fx = Decoder::evalPostfixExpr(postfix_expr, x);

    std::vector<std::pair<double, double>> points(num_points);
    for (size_t i = 0; i < points.size(); i++)
    {
        points[i].first = x[i];
        points[i].second = fx[i];
    }

    return points;
}

std::pair<std::vector<double>, std::vector<double>> readData(const std::string& path)
{
    std::vector<double> x, fx;

    std::string filetype = path.substr(path.find_last_of("."));

    std::string separator;
    if (filetype == ".txt")
    {
        separator = "\t";
    }
    else if (filetype == ".csv")
    {
        separator = ",";
    }
    else
    {
        throw std::domain_error("Only .txt and .csv files are supported.");
    }

    std::ifstream file(path, std::ios::in);

    std::string line;
    while (std::getline(file, line))
    {
        std::string x_str, fx_str;

        x_str = line.substr(0, line.find(separator));
        fx_str = line.substr(line.find(separator) + 1);

        x.push_back(atof(x_str.c_str()));
        fx.push_back(atof(fx_str.c_str()));
    }

    if (x.size() == 0)
    {
        throw std::domain_error("No data points were read from the file.");
    }
    if (x.size() != fx.size())
    {
        throw std::domain_error("Not every x value has a corresponding fx value in the file.");
    }

    return { x, fx };
}

std::pair<double, double> axisMinMax(const std::vector<double>& values, double pad)
{
    double min_val = *std::min_element(values.begin(), values.end());
    double max_val = *std::max_element(values.begin(), values.end());

    double interval_len = max_val - min_val;

    double int_min = min_val - pad*interval_len;
    double int_max = max_val + pad*interval_len;

    return std::make_pair(int_min, int_max);
}

std::vector<int> presetStringToFForm(std::string& preset)
{
    /* Convert the input string to a vector of strings (integers and other characters) */
    std::vector<std::string> expr;
    while (!preset.empty())
    {
        std::string token;
        while (!preset.empty() && std::iswdigit(preset.front()))
        {
            token.push_back(preset.front());
            preset.erase(0, 1);
        }
        expr.push_back(token);

        if (!preset.empty())
        {
            expr.push_back(std::string(1, preset.front()));
            preset.erase(0, 1);
        }
    }
    if (expr.size() % 2 == 0)
    {
        throw std::invalid_argument("Invalid preset function form (even number of tokens).");
    }

    std::vector<int> preset_form;
    for (size_t i = 0; i < expr.size(); i++)
    {
        /* Even indices must be operands (valid base function indices). */
        if (i % 2 == 0)
        {
            int idx = std::atoi(expr[i].c_str());
            if (!(1 <= idx && idx <= Decoder::num_base_funcs()))
            {
                throw std::invalid_argument("Invalid preset function form (invalid base function index).");
            }

            preset_form.push_back(idx - 1);
        }
        /* Odd indices must be valid operators. */
        else
        {
            if (expr[i].size() != 1)
            {
                throw std::invalid_argument("Invalid preset function form (multi-character non-integer token/operator).");
            }
            switch (expr[i][0])
            {
                case '+':
                    preset_form.push_back(OP_ADD);
                    break;
                case '-':
                    preset_form.push_back(OP_SUB);
                    break;
                case '*':
                    preset_form.push_back(OP_MUL);
                    break;
                case '/':
                    preset_form.push_back(OP_DIV);
                    break;
                case '^':
                    preset_form.push_back(OP_POW);
                    break;
                default:
                    throw std::invalid_argument("Invalid preset function form (invalid operator).");
            }
        }
    }

    return preset_form;
}
