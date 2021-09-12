/* Copyright (c) 2021 Krisztián Rugási. */

#include "generate.h"
#include "ga.h"
#include "../fitness/math_ops.h"
#include "../fitness/decoder.h"
#include "../../include/genetic_algorithm/rng.h"  /* Random number generation. */

#include <algorithm>
#include <vector>
#include <string>
#include <cstddef>

using namespace genetic_algorithm::rng;


int randomOperator(const std::string& opmask)
{
    assert(opmask.size() == size_t(_NUM_OPS));
	assert(std::any_of(opmask.begin(), opmask.end(), [](const char m) { return m == '1'; }));
    assert(std::all_of(opmask.begin(), opmask.end(), [](const char m) { return m == '1' || m == '0'; }));

    std::vector<int> operators;
	operators.reserve(opmask.size());

	for (size_t i = 0; i < opmask.size(); i++)
	{
        if (opmask[i] == '1') operators.emplace_back(_OP_MIN + int(i));
	}

    return operators[randomIdx(operators.size())];
}

int randomFunc(const std::string& fmask)
{
    assert(fmask.size() == Decoder::num_base_funcs());
	assert(std::any_of(fmask.begin(), fmask.end(), [](const char m) { return m == '1'; }));
    assert(std::all_of(fmask.begin(), fmask.end(), [](const char m) { return m == '1' || m == '0'; }));

    std::vector<int> base_func_ids;
    base_func_ids.reserve(fmask.size());

	for (size_t i = 0; i < fmask.size(); i++)
	{
        if (fmask[i] == '1') base_func_ids.push_back(int(i));
	}

    return base_func_ids[randomIdx(base_func_ids.size())];
}

std::vector<double> generateCoeffs(size_t num_coeffs, const mGA::limits_t& bounds)
{
    assert(num_coeffs == bounds.size());

    std::vector<double> coeffs;
    coeffs.reserve(num_coeffs);

    for (size_t i = 0; i < num_coeffs; i++)
    {
        coeffs.push_back(randomReal(bounds[i].first, bounds[i].second));
    }

    return coeffs;
}

mGA::Candidate generateRandomSol(size_t chrom_len,
                                 size_t num_coeffs,
                                 const mGA::limits_t& bounds,
                                 const std::string& fmask,
                                 const std::string& opmask)
{
    mGA::Candidate sol;
    sol.chromosome.reserve(chrom_len);

    for (size_t i = 0; i < chrom_len; i++)
    {
        std::vector<double> coeffs = generateCoeffs(num_coeffs, bounds);
        int fid = randomFunc(fmask);
        int opid = randomOperator(opmask);

        sol.chromosome.emplace_back(fid, coeffs, opid);
    }

    return sol;
}

mGA::Candidate generatePresetSol(size_t chrom_len,
                                 size_t num_coeffs,
                                 const mGA::limits_t& bounds,
                                 const std::vector<int>& presetf)
{
    assert((presetf.size() + 1)/2 == chrom_len);

    mGA::Candidate sol;
    sol.chromosome.reserve(chrom_len);

    for (size_t i = 0; i < chrom_len; i++)
    {
        std::vector<double> coeffs = generateCoeffs(num_coeffs, bounds);

        int fid = presetf[2 * i];

        int opid;
        if (i != chrom_len - 1)
        {
            opid = presetf[2*i + 1];
        }
        /* The operator of the last gene is not used and is not in the preset form. */
        else
        {
            opid = _OP_MIN;
        }

        sol.chromosome.emplace_back(fid, coeffs, opid);
    }

    return sol;
}