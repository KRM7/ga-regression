/* Copyright (c) 2021 Krisztián Rugási. */

/* Utility functions used during fitness evaluation. */

#ifndef FITNESS_UTILS_H
#define FITNESS_UTILS_H

#include <vector>


/* Codes for the different math operators used in the fitted math functions. */
constexpr int OP_ADD = 3001;
constexpr int OP_SUB = 3002;
constexpr int OP_MUL = 3003;
constexpr int OP_DIV = 3004;
constexpr int OP_POW = 3005;

constexpr int _OP_MIN = OP_ADD;
constexpr int _OP_MAX = OP_POW;
constexpr int _NUM_OPS = _OP_MAX - _OP_MIN + 1;

/* Return the precedent of a math operator. */
int precedent(int op);

/* Math operators for double vectors (of equal size). */
std::vector<double> operator+(const std::vector<double>& lhs, const std::vector<double>& rhs);
std::vector<double> operator-(const std::vector<double>& lhs, const std::vector<double>& rhs);
std::vector<double> operator*(const std::vector<double>& lhs, const std::vector<double>& rhs);
std::vector<double> operator/(const std::vector<double>& lhs, const std::vector<double>& rhs);
std::vector<double> operator^(const std::vector<double>& lhs, const std::vector<double>& rhs);

/* Perform the math operation op on the 2 double vectors lhs and rhs. */
std::vector<double> performOperation(const std::vector<double>& lhs, const std::vector<double>& rhs, int op);

#endif // !FITNESS_UTILS_H