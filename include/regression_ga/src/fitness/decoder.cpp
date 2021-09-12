/* Copyright (c) 2021 Krisztián Rugási. */

#include "decoder.h"
#include "token.h"
#include "math_ops.h"

#include <vector>
#include <stack>
#include <cmath>
#include <cstddef>
#include <cassert>


std::vector<double> Decoder::evalToken(const Token& token, const std::vector<double>& x)
{
    assert(!token.is_operator);
    assert(token.coeffs.size() >= 5);
    assert(token.fid < base_functions.size());

    return base_functions[token.fid](x, token.coeffs);
}

std::vector<double> Decoder::evalPostfixExpr(const std::vector<Token>& postfix_expr, const std::vector<double>& x)
{
    std::stack<std::vector<double>> operand_stack;

    for (const auto& token : postfix_expr)
    {
        /* Operand. */
        if (!token.is_operator)
        {
            operand_stack.push(evalToken(token, x));
        }
        /* Operator. */
        else
        {
            std::vector<double> rhs = operand_stack.top();
            operand_stack.pop();

            std::vector<double> lhs = operand_stack.top();
            operand_stack.pop();

            std::vector<double> result = performOperation(lhs, rhs, token.opid);
            operand_stack.push(result);
        }
    }
    assert(operand_stack.size() == 1);

    return operand_stack.top();
}

/* Base functions. */

/* Simple functions. */

std::vector<double> Decoder::c(const std::vector<double>& x, const std::vector<double>& coeffs)
{
    /* f(x) = c */

    std::vector<double> fx(x.size(), coeffs[2]);

    return fx;
}

std::vector<double> Decoder::lin(const std::vector<double>& x, const std::vector<double>& coeffs)
{
    /* f(x) = a*x + d */

    std::vector<double> fx(x.size());
    for (size_t i = 0; i < x.size(); i++)
    {
        fx[i] = coeffs[0] * x[i] + coeffs[3];
    }

    return fx;
}

std::vector<double> Decoder::poly(const std::vector<double>& x, const std::vector<double>& coeffs)
{
    /* f(x) = a*x^n + d */

    std::vector<double> fx(x.size());
    for (size_t i = 0; i < x.size(); i++)
    {
        fx[i] = coeffs[0] * std::pow(x[i], coeffs[4]) + coeffs[3];
    }

    return fx;
}

std::vector<double> Decoder::rec(const std::vector<double>& x, const std::vector<double>& coeffs)
{
    /* f(x) = a/(b*x + c)^n + d */

    std::vector<double> fx(x.size());
    for (size_t i = 0; i < x.size(); i++)
    {
        fx[i] = coeffs[0] / std::pow(coeffs[1] * x[i] + coeffs[2], coeffs[4]) + coeffs[3];
    }

    return fx;
}

std::vector<double> Decoder::root(const std::vector<double>& x, const std::vector<double>& coeffs)
{
    /* f(x) = a*(b*x + c)^(1/n) + d */

    std::vector<double> fx(x.size());
    for (size_t i = 0; i < x.size(); i++)
    {
        fx[i] = coeffs[0] * std::pow(coeffs[1] * x[i] + coeffs[2], 1.0 / coeffs[4]) + coeffs[3];
    }

    return fx;
}

std::vector<double> Decoder::exp(const std::vector<double>& x, const std::vector<double>& coeffs)
{
    /* f(x) = a*e^(b*x + c) + d */

    std::vector<double> fx(x.size());
    for (size_t i = 0; i < x.size(); i++)
    {
        fx[i] = coeffs[0] * std::exp(coeffs[1] * x[i] + coeffs[2]) + coeffs[3];
    }

    return fx;
}

std::vector<double> Decoder::log(const std::vector<double>& x, const std::vector<double>& coeffs)
{
    /* f(x) = a*ln(b*x + c) + d,   x > -c/b */

    std::vector<double> fx(x.size());
    for (size_t i = 0; i < x.size(); i++)
    {
        fx[i] = coeffs[0] * std::log(coeffs[1] * x[i] + coeffs[2]) + coeffs[3];
    }

    return fx;
}

/* Other functions. */

std::vector<double> Decoder::abs(const std::vector<double>& x, const std::vector<double>& coeffs)
{
    /* f(x) = a*|x + c| + d */

    std::vector<double> fx(x.size());
    for (size_t i = 0; i < x.size(); i++)
    {
        fx[i] = coeffs[0] * std::abs(x[i] + coeffs[2]) + coeffs[3];
    }

    return fx;
}

std::vector<double> Decoder::sgn(const std::vector<double>& x, const std::vector<double>& coeffs)
{
    /* f(x) = a*sgn(x + c) + d */

    std::vector<double> fx(x.size());
    for (size_t i = 0; i < x.size(); i++)
    {
        if (x[i] - coeffs[2] < 0)
        {
            fx[i] = coeffs[3];
        }
        else if (x[i] - coeffs[2] == 0)
        {
            fx[i] = coeffs[0] / 2 + coeffs[3];
        }
        else
        {
            fx[i] = coeffs[0] + coeffs[3];
        }
    }

    return fx;
}

/* Trigonometric functions. */

std::vector<double> Decoder::cos(const std::vector<double>& x, const std::vector<double>& coeffs)
{
    /* f(x) = a*cos(b*x + c) + d */

    std::vector<double> fx(x.size());
    for (size_t i = 0; i < x.size(); i++)
    {
        fx[i] = coeffs[0] * std::cos(coeffs[1] * x[i] + coeffs[2]) + coeffs[3];
    }

    return fx;
}

/* Inverse trigonometric functions. */

std::vector<double> Decoder::arcsin(const std::vector<double>& x, const std::vector<double>& coeffs)
{
    /* f(x) = a*arcsin(bx + c) + d,  -1 <= b*x+c <= 1 */

    std::vector<double> fx(x.size());
    for (size_t i = 0; i < x.size(); i++)
    {
        fx[i] = coeffs[0] * std::asin(coeffs[1] * x[i] + coeffs[2]) + coeffs[3];
    }

    return fx;
}

std::vector<double> Decoder::arctan(const std::vector<double>& x, const std::vector<double>& coeffs)
{
    /* f(x) = a*arctan(bx + c) + d */

    std::vector<double> fx(x.size());
    for (size_t i = 0; i < x.size(); i++)
    {
        fx[i] = coeffs[0] * std::atan(coeffs[1] * x[i] + coeffs[2]) + coeffs[3];
    }

    return fx;
}

std::vector<double> Decoder::arcsec(const std::vector<double>& x, const std::vector<double>& coeffs)
{
    /* f(x) = a*arcsec(b*x + c) + d = a*acos(1/(b*x + c)) + d,  b*x + c >= 1.0, or <= -1.0 */

    std::vector<double> fx(x.size());
    for (size_t i = 0; i < x.size(); i++)
    {
        fx[i] = coeffs[0] * std::acos(1.0 / (coeffs[1] * x[i] + coeffs[2])) + coeffs[3];
    }

    return fx;
}

/* Inverse hyperbolic functions. */

std::vector<double> Decoder::arsinh(const std::vector<double>& x, const std::vector<double>& coeffs)
{
    /* f(x) = a*arsh(b*x + c) + d */

    std::vector<double> fx(x.size());
    for (size_t i = 0; i < x.size(); i++)
    {
        fx[i] = coeffs[0] * std::asinh(coeffs[1] * x[i] + coeffs[2]) + coeffs[3];
    }

    return fx;
}

std::vector<double> Decoder::arcosh(const std::vector<double>& x, const std::vector<double>& coeffs)
{
    /* f(x) = a*arch(b*x + c) + d,   b*x + c >= 1.0 */

    std::vector<double> fx(x.size());
    for (size_t i = 0; i < x.size(); i++)
    {
        fx[i] = coeffs[0] * std::acosh(coeffs[1] * x[i] + coeffs[2]) + coeffs[3];
    }

    return fx;
}

std::vector<double> Decoder::artanh(const std::vector<double>& x, const std::vector<double>& coeffs)
{
    /* f(x) = a*arth(b*x + c) + d,   0 <= b*x+c <= 1 */

    std::vector<double> fx(x.size());
    for (size_t i = 0; i < x.size(); i++)
    {
        fx[i] = coeffs[0] * std::atanh(coeffs[1] * x[i] * coeffs[2]) + coeffs[3];
    }

    return fx;
}

std::vector<double> Decoder::arctgh(const std::vector<double>& x, const std::vector<double>& coeffs)
{
    /* f(x) = a*arctgh(b*x + c) + d = a/2 * ln((b*x + c + 1)/(b*x + c - 1)) + d,   b*x + c < -1.0, or > 1.0 */

    std::vector<double> fx(x.size());
    for (size_t i = 0; i < x.size(); i++)
    {
        fx[i] = coeffs[0] / 2.0 * std::log((coeffs[1] * x[i] + coeffs[2] + 1.0) / (coeffs[1] * x[i] + coeffs[2] - 1.0)) + coeffs[3];
    }

    return fx;
}

std::vector<double> Decoder::arsech(const std::vector<double>& x, const std::vector<double>& coeffs)
{
    /* f(x) = a*arsech(b*x + c) + d = a*ln((1 + sqrt(1 - (b*x+c)^2))/(b*x+c)) + d,   0 < b*x+c <= 1 */

    std::vector<double> fx(x.size());
    for (size_t i = 0; i < x.size(); i++)
    {
        double xi = coeffs[1] * x[i] + coeffs[2];
        xi = (1.0 + std::sqrt(1.0 - xi * xi)) / xi;
        fx[i] = coeffs[0] * std::log(xi) + coeffs[3];
    }

    return fx;
}

std::vector<double> Decoder::arcsch(const std::vector<double>& x, const std::vector<double>& coeffs)
{
    /* f(x) = a*arcsch(b*x + c) + d = a*ln((1 + sqrt(1 - (b*x+c)^2))/(b*x+c)) + d,   b*x+c != 0 */

    std::vector<double> fx(x.size());
    for (size_t i = 0; i < x.size(); i++)
    {
        double xi = coeffs[1] * x[i] + coeffs[2];
        xi = (1.0 + std::sqrt(1.0 + xi * xi)) / xi;
        fx[i] = coeffs[0] * std::log(xi) + coeffs[3];
    }

    return fx;
}