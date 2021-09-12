/* Copyright (c) 2021 Krisztián Rugási. */

/*
* Functions to perform conversions between: chromosomes of the GA, infix expressions, postfix expressions.
* These function are used during fitness evaluation in the decoding process (and for printing the results).
*/

#ifndef CONVERTER_H
#define CONVERTER_H

#include "token.h"
#include "../genetic/gene.h"

#include <vector>


/* Conversions between chromosomes, infix, and postfix expressions. */
class Converter
{
public:

    /* Convert a chromosome of a solution in the GA to an infix expression. */
    static std::vector<Token> chromosomeToInfix(const std::vector<Gene>& chrom);

    /* Convert an infix expression to a postfix expression for evaluation. */
    static std::vector<Token> infixToPostfix(const std::vector<Token>& infix_expr);

};

#endif // !CONVERTER_H