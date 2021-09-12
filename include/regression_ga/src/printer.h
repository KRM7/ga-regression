/* Copyright (c) 2021 Krisztián Rugási. */

#ifndef PRINTER_H
#define PRINTER_H

#include "fitness/token.h"

#include <vector>
#include <string>
#include <array>

/* Converts an infix expression of base math functions to a string for printing. */
class Printer
{
public:

    /* Convert the infix expression to a string. */
    static std::string print(const std::vector<Token>& infix_expr);

private:

    using mathFunctionPrinter_t = std::string(*)(const std::vector<double>& coeffs);

    /* For displaying + signs (returns empty string on negative num). */
    static std::string sign(double num);

    /* Return the operator op in string form. */
    static std::string operatorPrint(int op);

    /* Printers for the base math functions used. */

    /* Simple functions. */
    static std::string cPrint(const std::vector<double>& coeffs);
    static std::string linPrint(const std::vector<double>& coeffs);

    static std::string polyPrint(const std::vector<double>& coeffs);
    static std::string recPrint(const std::vector<double>& coeffs);
    static std::string rootPrint(const std::vector<double>& coeffs);

    static std::string expPrint(const std::vector<double>& coeffs);
    static std::string logPrint(const std::vector<double>& coeffs);

    /* Other functions. */
    static std::string absPrint(const std::vector<double>& coeffs);
    static std::string sgnPrint(const std::vector<double>& coeffs);

    /* Trigonometric functions. */
    static std::string cosPrint(const std::vector<double>& coeffs);

    /* Inverse trigonometric functions. */
    static std::string arcsinPrint(const std::vector<double>& coeffs);
    static std::string arctanPrint(const std::vector<double>& coeffs);
    static std::string arcsecPrint(const std::vector<double>& coeffs);

    /* Inverse hyperbolic functions. */
    static std::string arsinhPrint(const std::vector<double>& coeffs);
    static std::string arcoshPrint(const std::vector<double>& coeffs);
    static std::string artanhPrint(const std::vector<double>& coeffs);
    static std::string arctghPrint(const std::vector<double>& coeffs);
    static std::string arsechPrint(const std::vector<double>& coeffs);
    static std::string arcschPrint(const std::vector<double>& coeffs);

    /* Array of all the base math function printers. */
    static constexpr std::array<mathFunctionPrinter_t, 19> base_function_printers =
    {
        cPrint, linPrint, polyPrint, recPrint,
        rootPrint, expPrint, logPrint, absPrint,
        sgnPrint, cosPrint, arcsinPrint, arctanPrint,
        arcsecPrint, arsinhPrint, arcoshPrint,
        artanhPrint, arctghPrint, arsechPrint, arcschPrint
    };
};

#endif // !PRINTER_H