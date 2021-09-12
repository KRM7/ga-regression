/* Copyright (c) 2021 Krisztián Rugási. */

/* Tokens for the infix/postfix expressions. */

#ifndef TOKEN_H_
#define TOKEN_H_

#include "math_ops.h"

#include <vector>


/* A token used in the postfix and infix expressions. It can either be an operator or an operand. */
struct Token
{
    Token() = delete;

    /* Construct operator. */
    Token(int opid) :
        is_operator(true),
        fid(0),
        opid(opid)
    {}

    /* Construct operand. */
    Token(int fid, const std::vector<double>& coeffs) :
        is_operator(false),
        fid(fid),
        coeffs(coeffs),
        opid(_OP_MIN)
    {}

    /* Operator or operand. */
    bool is_operator;
    
    /* Operand. */
    int fid;
    std::vector<double> coeffs;

    /* Operator. */
    int opid;
};

#endif // !TOKEN_H_