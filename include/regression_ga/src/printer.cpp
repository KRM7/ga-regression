/* Copyright (c) 2021 Krisztián Rugási. */

#include "printer.h"
#include "fitness/token.h"
#include "fitness/math_ops.h"

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cassert>


std::string Printer::print(const std::vector<Token>& infix_expr)
{
    std::ostringstream expression;
    for (const auto& token : infix_expr)
    {
        /* Function. */
        if (!token.is_operator)
        {
            expression << base_function_printers[token.fid](token.coeffs);
        }
        /* Operator. */
        else
        {
            expression << operatorPrint(token.opid);
        }
    }

    return expression.str();
}

std::string Printer::operatorPrint(int op)
{
    switch (op)
    {
        case OP_ADD:
            return "+";
        case OP_SUB:
            return "-";
        case OP_MUL:
            return "*";
        case OP_DIV:
            return "/";
        case OP_POW:
            return "^";
        default:
            assert(false);  /* Invalid operator. Shouldn't get here. */
            std::abort();
    }
}

std::string Printer::sign(double num)
{
    return (num < 0.0) ? "" : "+";
}

/* Base math function printers. */

/* Simple functions. */

std::string Printer::cPrint(const std::vector<double>& coeffs)
{
    /* f(x) = c */
    std::ostringstream sstream;
    sstream << std::setprecision(3);

    sstream << "[" << coeffs[2] << "]";

    return sstream.str();
}

std::string Printer::linPrint(const std::vector<double>& coeffs)
{
    /* f(x) = a*x + d */
    std::ostringstream sstream;
    sstream << std::setprecision(3);

    sstream << "[" << coeffs[0] << "x" << sign(coeffs[3]) << coeffs[3] << "]";

    return sstream.str();
}

std::string Printer::polyPrint(const std::vector<double>& coeffs)
{
    /* f(x) = a*x^n + d */
    std::ostringstream sstream;
    sstream << std::setprecision(3);

    sstream << "[" << coeffs[0] << "x^" << coeffs[4] << sign(coeffs[3]) << coeffs[3] << "]";

    return sstream.str();
}

std::string Printer::recPrint(const std::vector<double>& coeffs)
{
    /* f(x) = a/(b*x + c)^n + d */
    std::ostringstream sstream;
    sstream << std::setprecision(3);

    sstream << "[" << coeffs[0] << "/(" << coeffs[1] << "x" << sign(coeffs[2]) << coeffs[2] << ")^" << coeffs[4] << sign(coeffs[3]) << coeffs[3] << "]";

    return sstream.str();
}

std::string Printer::rootPrint(const std::vector<double>& coeffs)
{
    /* f(x) = a*(b*x + c)^(1/n) + d */
    std::ostringstream sstream;
    sstream << std::setprecision(3);

    sstream << "[" << coeffs[0] << "(" << coeffs[1] << "x" << sign(coeffs[2]) << coeffs[2] << ")^" << 1 / coeffs[4] << ")" << sign(coeffs[3]) << coeffs[3] << "]";

    return sstream.str();
}

std::string Printer::expPrint(const std::vector<double>& coeffs)
{
    /* f(x) = a*e^(b*x + c) + d */
    std::ostringstream sstream;
    sstream << std::setprecision(3);

    sstream << "[" << coeffs[0] << "e^(" << coeffs[1] << "x" << sign(coeffs[2]) << coeffs[2] << ")" << sign(coeffs[3]) << coeffs[3] << "]";

    return sstream.str();
}

std::string Printer::logPrint(const std::vector<double>& coeffs)
{
    /* f(x) = a*ln(b*x + c) + d */
    std::ostringstream sstream;
    sstream << std::setprecision(3);

    sstream << "[" << coeffs[0] << " ln" << "(" << coeffs[1] << "x" << sign(coeffs[2]) << coeffs[2] << ")" << sign(coeffs[3]) << coeffs[3] << "]";

    return sstream.str();
}

/* Other functions. */

std::string Printer::absPrint(const std::vector<double>& coeffs)
{
    /* f(x) = a*|x + c| + d */
    std::ostringstream sstream;
    sstream << std::setprecision(3);

    sstream << "[" << coeffs[0] << "|" << "x" << sign(coeffs[2]) << coeffs[2] << "|" << sign(coeffs[3]) << coeffs[3] << "]";

    return sstream.str();
}

std::string Printer::sgnPrint(const std::vector<double>& coeffs)
{
    /* f(x) = a*sgn(x + c) + d */
    std::ostringstream sstream;
    sstream << std::setprecision(3);

    sstream << "[" << coeffs[0] << "sgn(x" << sign(coeffs[2]) << coeffs[2] << ")" << sign(coeffs[3]) << coeffs[3] << "]";

    return sstream.str();
}

/* Trigonometric functions. */

std::string Printer::cosPrint(const std::vector<double>& coeffs)
{
    /* f(x) = a*cos(b*x + c) + d */
    std::ostringstream sstream;
    sstream << std::setprecision(3);

    sstream << "[" << coeffs[0] << "cos(" << coeffs[1] << "x" << sign(coeffs[2]) << coeffs[2] << ")" << sign(coeffs[3]) << coeffs[3] << "]";

    return sstream.str();
}

/* Inverse trigonometric functions. */

std::string Printer::arcsinPrint(const std::vector<double>& coeffs)
{
    /* f(x) = a*arcsin(bx + c) + d */
    std::ostringstream sstream;
    sstream << std::setprecision(3);

    sstream << "[" << coeffs[0] << "arcsin(" << coeffs[1] << "x" << sign(coeffs[2]) << coeffs[2] << ")" << sign(coeffs[3]) << coeffs[3] << "]";

    return sstream.str();
}

std::string Printer::arctanPrint(const std::vector<double>& coeffs)
{
    /* f(x) = a*arctan(bx + c) + d */
    std::ostringstream sstream;
    sstream << std::setprecision(3);

    sstream << "[" << coeffs[0] << "arctan(" << coeffs[1] << "x" << sign(coeffs[2]) << coeffs[2] << ")" << sign(coeffs[3]) << coeffs[3] << "]";

    return sstream.str();
}

std::string Printer::arcsecPrint(const std::vector<double>& coeffs)
{
    /* f(x) = a*arcsec(b*x + c) + d */
    std::ostringstream sstream;
    sstream << std::setprecision(3);

    sstream << "[" << coeffs[0] << "arcsec(" << coeffs[1] << "x" << sign(coeffs[2]) << coeffs[2] << ")" << sign(coeffs[3]) << coeffs[3] << "]";

    return sstream.str();
}

/* Inverse hyperbolic functions. */

std::string Printer::arsinhPrint(const std::vector<double>& coeffs)
{
    /* f(x) = a*arsh(b*x + c) + d */
    std::ostringstream sstream;
    sstream << std::setprecision(3);

    sstream << "[" << coeffs[0] << "arsh(" << coeffs[1] << "x" << sign(coeffs[2]) << coeffs[2] << ")" << sign(coeffs[3]) << coeffs[3] << "]";

    return sstream.str();
}

std::string Printer::arcoshPrint(const std::vector<double>& coeffs)
{
    /* f(x) = a*arch(b*x + c) + d */
    std::ostringstream sstream;
    sstream << std::setprecision(3);

    sstream << "[" << coeffs[0] << "arch(" << coeffs[1] << "x" << sign(coeffs[2]) << coeffs[2] << ")" << sign(coeffs[3]) << coeffs[3] << "]";

    return sstream.str();
}

std::string Printer::artanhPrint(const std::vector<double>& coeffs)
{
    /* f(x) = a*arth(b*x + c) + d */
    std::ostringstream sstream;
    sstream << std::setprecision(3);

    sstream << "[" << coeffs[0] << "arth(" << coeffs[1] << "x" << sign(coeffs[2]) << coeffs[2] << ")" << sign(coeffs[3]) << coeffs[3] << "]";

    return sstream.str();
}

std::string Printer::arctghPrint(const std::vector<double>& coeffs)
{
    /* f(x) = a*arctgh(b*x + c) + d */
    std::ostringstream sstream;
    sstream << std::setprecision(3);

    sstream << "[" << coeffs[0] << "arctgh(" << coeffs[1] << "x" << sign(coeffs[2]) << coeffs[2] << ")" << sign(coeffs[3]) << coeffs[3] << "]";

    return sstream.str();
}

std::string Printer::arsechPrint(const std::vector<double>& coeffs)
{
    /* f(x) = a*arsech(b*x + c) + d */
    std::ostringstream sstream;
    sstream << std::setprecision(3);

    sstream << "[" << coeffs[0] << "arsech(" << coeffs[1] << "x" << sign(coeffs[2]) << coeffs[2] << ")" << sign(coeffs[3]) << coeffs[3] << "]";

    return sstream.str();
}

std::string Printer::arcschPrint(const std::vector<double>& coeffs)
{
    /* f(x) = a*arcsch(b*x + c) + d */
    std::ostringstream sstream;
    sstream << std::setprecision(3);

    sstream << "[" << coeffs[0] << "arcsch(" << coeffs[1] << "x" << sign(coeffs[2]) << coeffs[2] << ")" << sign(coeffs[3]) << coeffs[3] << "]";

    return sstream.str();
}