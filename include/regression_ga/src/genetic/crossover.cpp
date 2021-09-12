/* Copyright (c) 2021 Krisztián Rugási. */

#include "crossover.h"
#include "ga.h"
#include "../../include/genetic_algorithm/rng.h"

#include <algorithm>
#include <utility>
#include <cstddef>
#include <cassert>

using namespace genetic_algorithm::rng;


mGA::CandidatePair blxAlphaCrossover(const mGA::Candidate& parent1,
                                     const mGA::Candidate& parent2,
                                     double pc,
                                     double alpha,
                                     const mGA::limits_t& bounds)
{
    assert(parent1.chromosome.size() == parent2.chromosome.size());
    assert(0.0 <= pc && pc <= 1.0);
    assert(alpha >= 0.0);

    mGA::Candidate child1(parent1), child2(parent2);

    if (randomReal() <= pc)
    {
        for (size_t i = 0; i < parent1.chromosome.size(); i++)
        {
            /* Coefficients (BLX-a). */
            assert(parent1.chromosome[i].coeffs.size() == bounds.size() && parent2.chromosome[i].coeffs.size() == bounds.size());

            for (size_t j = 0; j < parent1.chromosome[i].coeffs.size(); j++)
            {
                auto [range_min, range_max] = std::minmax(parent1.chromosome[i].coeffs[j], parent2.chromosome[i].coeffs[j]);
                double range_ext = alpha * (range_max - range_min);

                child1.chromosome[i].coeffs[j] = randomReal(range_min - range_ext, range_max + range_ext);
                child2.chromosome[i].coeffs[j] = randomReal(range_min - range_ext, range_max + range_ext);

                child1.chromosome[i].coeffs[j] = std::clamp(child1.chromosome[i].coeffs[j], bounds[j].first, bounds[j].second);
                child2.chromosome[i].coeffs[j] = std::clamp(child2.chromosome[i].coeffs[j], bounds[j].first, bounds[j].second);
            }

            /* Genes (uniform). */
            if (randomBool())
            {
                child1.chromosome[i].fid = parent2.chromosome[i].fid;
                child2.chromosome[i].fid = parent1.chromosome[i].fid;

                child1.chromosome[i].coeffs = parent2.chromosome[i].coeffs;
                child2.chromosome[i].coeffs = parent1.chromosome[i].coeffs;
            }
            if (randomBool())
            {
                child1.chromosome[i].opid = parent2.chromosome[i].opid;
                child2.chromosome[i].opid = parent1.chromosome[i].opid;
            }
        }

        child1.is_evaluated = false;
        child2.is_evaluated = false;
    }

    return std::make_pair(child1, child2);
}

mGA::CandidatePair simulatedBinaryCrossover(const mGA::Candidate& parent1,
                                            const mGA::Candidate& parent2,
                                            double pc,
                                            double eta,
                                            const mGA::limits_t& bounds)
{
    assert(parent1.chromosome.size() == parent2.chromosome.size());
    assert(0.0 <= pc && pc <= 1.0);
    assert(eta >= 0.0);

    mGA::Candidate child1(parent1), child2(parent2);

    if (randomReal() <= pc)
    {
        /* Get beta for the simulated binary crossover. */
        double u = randomReal();
        double beta = (u <= 0.5) ? std::pow(2 * u, 1 / (eta + 1)) : std::pow(1 / (2 * (1 - u)), 1 / (eta + 1));

        for (size_t i = 0; i < parent1.chromosome.size(); i++)
        {
            /* Coefficients (sim_bin). */
            assert(parent1.chromosome[i].coeffs.size() == bounds.size() && parent2.chromosome[i].coeffs.size() == bounds.size());

            for (size_t j = 0; j < parent1.chromosome[i].coeffs.size(); j++)
            {
                child1.chromosome[i].coeffs[j] = 0.5 * ((1 - beta) * parent1.chromosome[i].coeffs[j] + (1 + beta) * parent2.chromosome[i].coeffs[j]);
                child2.chromosome[i].coeffs[j] = 0.5 * ((1 + beta) * parent1.chromosome[i].coeffs[j] + (1 - beta) * parent2.chromosome[i].coeffs[j]);
                /* The childrens genes may be outside the allowed interval. */
                child1.chromosome[i].coeffs[j] = std::clamp(child1.chromosome[i].coeffs[j], bounds[j].first, bounds[j].second);
                child2.chromosome[i].coeffs[j] = std::clamp(child2.chromosome[i].coeffs[j], bounds[j].first, bounds[j].second);
            }

            /* Genes (uniform). */
            if (randomBool())
            {
                child1.chromosome[i].fid = parent2.chromosome[i].fid;
                child2.chromosome[i].fid = parent1.chromosome[i].fid;

                child1.chromosome[i].coeffs = parent2.chromosome[i].coeffs;
                child2.chromosome[i].coeffs = parent1.chromosome[i].coeffs;
            }
            if (randomBool())
            {
                child1.chromosome[i].opid = parent2.chromosome[i].opid;
                child2.chromosome[i].opid = parent1.chromosome[i].opid;
            }
        }

        child1.is_evaluated = false;
        child2.is_evaluated = false;
    }

    return std::make_pair(child1, child2);
}

mGA::CandidatePair wrightCrossover(const mGA::Candidate& parent1,
                                   const mGA::Candidate& parent2,
                                   double pc,
                                   const mGA::limits_t& bounds)
{
    assert(parent1.chromosome.size() == parent2.chromosome.size());
    assert(0.0 <= pc && pc <= 1.0);

    mGA::Candidate child1(parent1), child2(parent2);

    if (randomReal() <= pc)
    {
        /* Determine better parent and weights for wright crossover (single-objective only). */
        const mGA::Candidate* p1 = (parent1.fitness[0] > parent2.fitness[0]) ? &parent1 : &parent2;
        const mGA::Candidate* p2 = (parent1.fitness[0] > parent2.fitness[0]) ? &parent2 : &parent1;
        double w1 = randomReal();
        double w2 = randomReal();

        for (size_t i = 0; i < parent1.chromosome.size(); i++)
        {
            /* Coefficients (wright). */
            assert(parent1.chromosome[i].coeffs.size() == bounds.size() && parent2.chromosome[i].coeffs.size() == bounds.size());

            for (size_t j = 0; j < parent1.chromosome[i].coeffs.size(); j++)
            {
                child1.chromosome[i].coeffs[j] = w1 * (p1->chromosome[i].coeffs[j] - p2->chromosome[i].coeffs[j]) + p1->chromosome[i].coeffs[j];
                child2.chromosome[i].coeffs[j] = w2 * (p1->chromosome[i].coeffs[j] - p2->chromosome[i].coeffs[j]) + p1->chromosome[i].coeffs[j];
                /* The generated childrens genes may be outside the allowed intervals. */
                child1.chromosome[i].coeffs[j] = std::clamp(child1.chromosome[i].coeffs[j], bounds[j].first, bounds[j].second);
                child2.chromosome[i].coeffs[j] = std::clamp(child2.chromosome[i].coeffs[j], bounds[j].first, bounds[j].second);
            }

            /* Genes (uniform). */
            if (randomBool())
            {
                child1.chromosome[i].fid = parent2.chromosome[i].fid;
                child2.chromosome[i].fid = parent1.chromosome[i].fid;

                child1.chromosome[i].coeffs = parent2.chromosome[i].coeffs;
                child2.chromosome[i].coeffs = parent1.chromosome[i].coeffs;
            }
            if (randomBool())
            {
                child1.chromosome[i].opid = parent2.chromosome[i].opid;
                child2.chromosome[i].opid = parent1.chromosome[i].opid;
            }
        }

        child1.is_evaluated = false;
        child2.is_evaluated = false;
    }

    return std::make_pair(child1, child2);
}
