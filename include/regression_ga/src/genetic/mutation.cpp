/* Copyright (c) 2021 Krisztián Rugási. */

#include "mutation.h"
#include "generate.h"
#include "ga.h"
#include "../../include/genetic_algorithm/rng.h"  /* Random number generation. */

#include <algorithm>
#include <string>
#include <cassert>
#include <cstddef>

using namespace genetic_algorithm::rng;


void randomMutateCoeffs(mGA::Candidate& child, double pm, const mGA::limits_t& bounds)
{
    assert(0.0 <= pm && pm <= 1.0);

    for (auto& gene : child.chromosome)
    {
        assert(gene.coeffs.size() == bounds.size());

        for (size_t i = 0; i < gene.coeffs.size(); i++)
        {
            if (randomReal() <= pm)
            {
                gene.coeffs[i] = randomReal(bounds[i].first, bounds[i].second);
                child.is_evaluated = false;
            }
        }
    }
}

void boundaryMutateCoeffs(mGA::Candidate& child, double pm, const mGA::limits_t& bounds)
{
    assert(0.0 <= pm && pm <= 1.0);

    for (auto& gene : child.chromosome)
    {
        assert(gene.coeffs.size() == bounds.size());

        for (size_t i = 0; i < gene.coeffs.size(); i++)
        {
            if (randomReal() <= pm)
            {
                gene.coeffs[i] = randomBool() ? bounds[i].first : bounds[i].second;
                child.is_evaluated = false;
            }
        }
    }
}

void gaussMutateCoeffs(mGA::Candidate& child, double pm, const mGA::limits_t& bounds, double scale)
{
    assert(0.0 <= pm && pm <= 1.0);
    assert(scale > 0.0);

    for (auto& gene : child.chromosome)
    {
        assert(gene.coeffs.size() == bounds.size());

        for (size_t i = 0; i < gene.coeffs.size(); i++)
        {
            if (randomReal() <= pm)
            {
                double SD = (bounds[i].second - bounds[i].first) / scale;
                gene.coeffs[i] += randomNormal(0.0, SD);

                /* The mutated gene might be outside the allowed range. */
                gene.coeffs[i] = std::clamp(gene.coeffs[i], bounds[i].first, bounds[i].second);

                child.is_evaluated = false;
            }
        }
    }
}

void mutateForm(mGA::Candidate& child, double pm, const std::string& fmask, const std::string& opmask)
{
    assert(0.0 <= pm && pm <= 1.0);

    for (auto& gene : child.chromosome)
    {
        if (randomReal() <= pm)
        {
            gene.fid = randomFunc(fmask);
            child.is_evaluated = false;
        }
        if (randomReal() <= pm)
        {
            gene.opid = randomOperator(opmask);
            child.is_evaluated = false;
        }
    }
}