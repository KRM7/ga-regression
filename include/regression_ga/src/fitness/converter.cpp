/* Copyright (c) 2021 Krisztián Rugási. */

#include "converter.h"
#include "token.h"
#include "math_ops.h"
#include "../genetic/gene.h"

#include <vector>
#include <stack>
#include <utility>
#include <cstddef>
#include <cassert>


std::vector<Token> Converter::chromosomeToInfix(const std::vector<Gene>& chrom)
{
    assert(chrom.size() > 0);

    std::vector<Token> infix_expr;
    infix_expr.reserve(2 * chrom.size());

    for (size_t i = 0; i < chrom.size() - 1; i++)
    {
        infix_expr.emplace_back(chrom[i].fid, chrom[i].coeffs);
        infix_expr.emplace_back(chrom[i].opid);
    }
    infix_expr.emplace_back(chrom.back().fid, chrom.back().coeffs);
    /* The operator in the last gene is discarded. */

    return infix_expr;
}

std::vector<Token> Converter::infixToPostfix(const std::vector<Token>& infix_expr)
{
    std::vector<Token> postfix_expr;
    std::stack<Token> operator_stack;
    postfix_expr.reserve(infix_expr.size());

    for (const auto& token : infix_expr)
    {
        /* Operand. */
        if (!token.is_operator)
        {
            postfix_expr.push_back(token);
        }
        /* Operator. */
        else
        {
            while (!operator_stack.empty() && precedent(operator_stack.top().opid) >= precedent(token.opid))
            {
                postfix_expr.push_back(std::move(operator_stack.top()));
                operator_stack.pop();
            }
            operator_stack.push(token);
        }
    }
    while (!operator_stack.empty())
    {
        postfix_expr.push_back(std::move(operator_stack.top()));
        operator_stack.pop();
    }

    return postfix_expr;
}