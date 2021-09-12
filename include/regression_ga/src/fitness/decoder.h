/* Copyright (c) 2021 Krisztián Rugási. */

/* Decoder class for evaluating/parsing postfix expressions. Used in the fitness function to evaluate the function represented by the chromosomes. */

#ifndef DECODER_H
#define DECODER_H

#include "token.h"

#include <vector>
#include <array>
#include <cstddef>


/*
* Decode/evaluate a postfix expression.
* The postfix expressions represent math functions where the operands are the base math functions, and the operators are simple math operators.
* These math function are evaluated for every point in a vector x.
*/
class Decoder
{
public:

    /* Evaluate the math function postfix_expr at every point in x. */
    static std::vector<double> evalPostfixExpr(const std::vector<Token>& postfix_expr, const std::vector<double>& x);

    /* The number of base math functions defined. */
    static constexpr size_t num_base_funcs() noexcept
    {
        return base_functions.size();
    }

private:

    /* Evaluate an operand token (1 base math function) at every point in x. */
    static std::vector<double> evalToken(const Token& token, const std::vector<double>& x);  

    /*
    * BASE MATH FUNCTIONS.
    * Evaluate a base math function with coeffs coefficients at every point in the vector x.
    */

    /* The type of the base math functions. */
    using mathFunction_t = std::vector<double>(*)(const std::vector<double>& x, const std::vector<double>& coeffs);

    /* Simple functions. */
    static std::vector<double> c(const std::vector<double>& x, const std::vector<double>& coeffs);
    static std::vector<double> lin(const std::vector<double>& x, const std::vector<double>& coeffs);

    static std::vector<double> poly(const std::vector<double>& x, const std::vector<double>& coeffs);  //
    static std::vector<double> rec(const std::vector<double>& x, const std::vector<double>& coeffs);   //
    static std::vector<double> root(const std::vector<double>& x, const std::vector<double>& coeffs);  //

    static std::vector<double> exp(const std::vector<double>& x, const std::vector<double>& coeffs);
    static std::vector<double> log(const std::vector<double>& x, const std::vector<double>& coeffs);

    /* Other functions. */
    static std::vector<double> abs(const std::vector<double>& x, const std::vector<double>& coeffs);
    static std::vector<double> sgn(const std::vector<double>& x, const std::vector<double>& coeffs);

    /* Trigonometric functions. */
    static std::vector<double> cos(const std::vector<double>& x, const std::vector<double>& coeffs);

    /* Inverse trigonometric functions. */
    static std::vector<double> arcsin(const std::vector<double>& x, const std::vector<double>& coeffs);
    static std::vector<double> arctan(const std::vector<double>& x, const std::vector<double>& coeffs);
    static std::vector<double> arcsec(const std::vector<double>& x, const std::vector<double>& coeffs);

    /* Inverse hyperbolic functions. */
    static std::vector<double> arsinh(const std::vector<double>& x, const std::vector<double>& coeffs);
    static std::vector<double> arcosh(const std::vector<double>& x, const std::vector<double>& coeffs);
    static std::vector<double> artanh(const std::vector<double>& x, const std::vector<double>& coeffs);
    static std::vector<double> arctgh(const std::vector<double>& x, const std::vector<double>& coeffs);
    static std::vector<double> arsech(const std::vector<double>& x, const std::vector<double>& coeffs);
    static std::vector<double> arcsch(const std::vector<double>& x, const std::vector<double>& coeffs);

    /* Array of all the base functions. */
    static constexpr std::array<mathFunction_t, 19> base_functions =
    {
        c, lin, poly, rec, root, exp, log, abs, sgn, cos,
        arcsin, arctan, arcsec,
        arsinh, arcosh, artanh, arctgh, arsech, arcsch
    };
};

#endif // !DECODER_H