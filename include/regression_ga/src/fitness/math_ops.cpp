/* Copyright (c) 2021 Krisztián Rugási. */

#include "math_ops.h"

#include <vector>
#include <cmath>
#include <cassert>
#include <cstdlib>


int precedent(int op)
{
	switch (op)
	{
		case OP_ADD:
            return 10;
        case OP_SUB:
			return 10;
		case OP_MUL:
			return 20;
		case OP_DIV:
			return 20;
		case OP_POW:
			return 30;
		default:
			assert(false);	/* Invalid operator. Shouldn't get here. */
			std::abort();
	}
}

std::vector<double> operator+(const std::vector<double>& lhs, const std::vector<double>& rhs)
{
	assert(lhs.size() == rhs.size());

	std::vector<double> result(lhs.size());
	for (size_t i = 0; i < lhs.size(); i++)
	{
		result[i] = lhs[i] + rhs[i];
	}

	return result;
}

std::vector<double> operator-(const std::vector<double>& lhs, const std::vector<double>& rhs)
{
	assert(lhs.size() == rhs.size());

	std::vector<double> result(lhs.size());
	for (size_t i = 0; i < lhs.size(); i++)
	{
		result[i] = lhs[i] - rhs[i];
	}

	return result;
}

std::vector<double> operator*(const std::vector<double>& lhs, const std::vector<double>& rhs)
{
	assert(lhs.size() == rhs.size());

	std::vector<double> result(lhs.size());
	for (size_t i = 0; i < lhs.size(); i++)
	{
		result[i] = lhs[i] * rhs[i];
	}

	return result;
}

std::vector<double> operator/(const std::vector<double>& lhs, const std::vector<double>& rhs)
{
	assert(lhs.size() == rhs.size());

	std::vector<double> result(lhs.size());
	for (size_t i = 0; i < lhs.size(); i++)
	{
		result[i] = lhs[i] / rhs[i];
	}

	return result;
}

std::vector<double> operator^(const std::vector<double>& lhs, const std::vector<double>& rhs)
{
	assert(lhs.size() == rhs.size());

	std::vector<double> result(lhs.size());
	for (size_t i = 0; i < lhs.size(); i++)
	{
		result[i] = std::pow(lhs[i], rhs[i]);
	}

	return result;
}

std::vector<double> performOperation(const std::vector<double>& lhs, const std::vector<double>& rhs, int op)
{
	switch (op)
	{
		case OP_ADD:
			return lhs + rhs;
		case OP_SUB:
			return lhs - rhs;
		case OP_MUL:
			return lhs * rhs;
		case OP_DIV:
			return lhs / rhs;
		case OP_POW:
			return lhs ^ rhs;
		default:
			assert(false);	/* Invalid operator. Shouldn't get here. */
			std::abort();
	}
}