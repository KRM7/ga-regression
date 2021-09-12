/* Copyright (c) 2021 Krisztián Rugási. */

#include "ga.h"
#include "crossover.h"
#include "mutation.h"
#include "generate.h"
#include "gene.h"
#include "../fitness/decoder.h"
#include "../fitness/math_ops.h"
#include "../../include/genetic_algorithm/base_ga.h"

#include <algorithm>
#include <vector>
#include <string>
#include <utility>
#include <limits>
#include <cstddef>
#include <cstdlib>
#include <cassert>
#include <stdexcept>

using namespace genetic_algorithm::rng;


/* Contructors. */

mGA::mGA(size_t chrom_len, fitnessFunction_t fitness_function, const std::string& fmask, const std::string& opmask, const limits_t& limits) : 
    GA(chrom_len, fitness_function),
    fmask_(fmask),
    opmask_(opmask),
    limits_(limits)
{
    if (fmask.size() != Decoder::num_base_funcs()) throw std::invalid_argument("Invalid fmask length.");
    if (opmask.size() != size_t(_NUM_OPS)) throw std::invalid_argument("Invalid opmask length.");

    if (std::any_of(limits.begin(), limits.end(), [](const std::pair<double, double>& b) {return b.first > b.second; }))
    {
        throw std::invalid_argument("The lower bound must be lower than the upper bound for each gene.");
    }
}

/* Genetic operators. */

mGA::Candidate mGA::generateCandidate() const
{
    Candidate sol;

    if (!use_preset_form)
    {
        sol = generateRandomSol(chrom_len_, vars_per_func_, limits_, fmask_, opmask_);
    }
    else
    {
        sol = generatePresetSol(chrom_len_, vars_per_func_, limits_, preset_form);
    }

    return sol;
}

mGA::CandidatePair mGA::crossover(const Candidate& p1, const Candidate& p2) const
{
    switch (crossover_method_)
    {
        case CrossoverMethod::simulated_binary:
            return simulatedBinaryCrossover(p1, p2, crossover_rate_, sim_binary_crossover_param_, limits_);
        case CrossoverMethod::BLXa:
            return blxAlphaCrossover(p1, p2, crossover_rate_, blx_crossover_param_, limits_);
        case CrossoverMethod::wright:
            return wrightCrossover(p1, p2, crossover_rate_, limits_);
        default:
            assert(false);	/* Invalid crossover method. Shouldn't get here. */
            std::abort();
    }
}

void mGA::mutate(Candidate& child) const
{
    switch (mutation_method_)
    {
        case MutationMethod::gauss:
            gaussMutateCoeffs(child, mutation_rate_, limits_, gauss_mutation_param_);
            break;
        case MutationMethod::random:
            randomMutateCoeffs(child, mutation_rate_, limits_);
            break;
        case MutationMethod::boundary:
            boundaryMutateCoeffs(child, mutation_rate_, limits_);
            break;
        default:
            assert(false);	/* Invalid mutation method. Shouldn't get here. */
            std::abort();
    }

    if (!use_preset_form)
    {
        mutateForm(child, mutation_rate_, fmask_, opmask_);
    }
}


/* Setters/getters. */

void mGA::fmask(std::string mask)
{
    if (mask.size() != Decoder::num_base_funcs())
    {
        throw std::invalid_argument("Invalid fmask length.");
    }
    if (std::all_of(mask.begin(), mask.end(), [](const char m) { return m == '0'; }))
    {
        throw std::invalid_argument("The fmask must have at least one element that is not 0.");
    }
    if (!std::all_of(mask.begin(), mask.end(), [](const char m) { return (m == '0') || (m == '1'); }))
    {
        throw std::invalid_argument("The fmask must only contain 0s and 1s.");
    }

    fmask_ = std::move(mask);
}

void mGA::opmask(std::string mask)
{
    if (mask.size() != (_OP_MAX - _OP_MIN + 1))
    {
        throw std::invalid_argument("Invalid opmask length.");
    }
    if (std::all_of(mask.begin(), mask.end(), [](const char m) { return m == '0'; }))
    {
        throw std::invalid_argument("The opmask must have at least one element that is not 0.");
    }
    if (!std::all_of(mask.begin(), mask.end(), [](const char m) { return (m == '0') || (m == '1'); }))
    {
        throw std::invalid_argument("The opmask must only contain 0s and 1s.");
    }

    opmask_ = std::move(mask);
}

void mGA::limits(limits_t limits)
{
    if (!std::all_of(limits.begin(), limits.end(), [](const std::pair<double, double>& limit) { return limit.first <= limit.second; }))
    {
        throw std::invalid_argument("The lower bound must be less or equal to the upper bound.");
    }
    if (limits.size() != vars_per_func_)
    {
        throw std::invalid_argument("The limits must have a size of vars_per_func.");
    }
    
    limits_ = std::move(limits);
}


void mGA::crossover_method(CrossoverMethod method)
{
    crossover_method_ = method;
}

void mGA::blx_crossover_param(double alpha)
{
    if (!(0.0 <= alpha && alpha <= std::numeric_limits<double>::max()))
    {
        throw std::invalid_argument("Alpha must be a nonnegative, finite value.");
    }

    blx_crossover_param_ = alpha;
}

void mGA::sim_binary_crossover_param(double eta)
{
    if (!(0.0 <= eta && eta <= std::numeric_limits<double>::max()))
    {
        throw std::invalid_argument("Eta must be a nonnegative, finite value.");
    }

    sim_binary_crossover_param_ = eta;
}


void mGA::mutation_method(MutationMethod method)
{
    mutation_method_ = method;
}

void mGA::gauss_mutation_param(double scale)
{
    if (!(0.0 < scale && scale <= std::numeric_limits<double>::max()))
    {
        throw std::invalid_argument("Eta must be a nonnegative, finite value.");
    }

    gauss_mutation_param_ = scale;
}