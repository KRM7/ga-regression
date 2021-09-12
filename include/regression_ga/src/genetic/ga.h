/* Copyright (c) 2021 Krisztián Rugási. */

/* The mixed-encoded genetic algorithm class used for fitting functions. */

#ifndef GA_H_
#define GA_H_

#include <vector>
#include <utility>
#include <string>
#include <cstddef>

#include "../../include/genetic_algorithm/base_ga.h"
#include "gene.h"


/* 
* The mixed-encoded genetic algorithm used for interpolating functions.
* Each chromosome encodes a math function made up of base functions and operators (encoded in the genes).
*/
class mGA : public genetic_algorithm::GA<Gene>
{
public:

    /* Type containing the lower and upper bounds of each real coefficient in a gene. */
    using limits_t = std::vector<std::pair<double, double>>;

    /* The possible crossover methods used in the GA. Uniform crossover is used for the non-real components of the genes. */
    enum class CrossoverMethod 
    { 
        BLXa, wright, simulated_binary
    };

    /* The possible mutation method used in the GA. The non-real components of the genes use a simple mutation operator. */
    enum class MutationMethod 
    { 
        random, gauss, boundary
    };

    /* Use preset function form. (Affects initial Candidate generation and mutations.) */
    bool use_preset_form = false;

    /* Preset function form */
    std::vector<int> preset_form;

    /* Contructors. */
    mGA() = delete;
    mGA(size_t chrom_len, fitnessFunction_t fitness_function, const std::string& fmask, const std::string& opmask, const limits_t& limits);

    /* Setters. */
    void fmask(std::string mask);
    void opmask(std::string mask);
    void limits(limits_t limits);

    void crossover_method(CrossoverMethod method);
    void blx_crossover_param(double alpha);
    void sim_binary_crossover_param(double eta);

    void mutation_method(MutationMethod method);
    void gauss_mutation_param(double scale);

private:

    std::string fmask_;                 /* Mask for the possible function indices to use. */
    std::string opmask_;                /* Mask for the possible operators to use. */
    limits_t limits_;                   /* The lower and upper bounds of each real-encoded coefficient of the genes. */
    const size_t vars_per_func_ = 5;    /* The number of coefficients stores in a gene for a math function. */

    CrossoverMethod crossover_method_ = CrossoverMethod::simulated_binary;  /* The crossover method used in the GA. */
    double blx_crossover_param_ = 0.5;          /* The parameter of the BLX-alpha crossover. */
    double sim_binary_crossover_param_ = 4.0;   /* The parameeter of the simulated binary crossover. */

    MutationMethod mutation_method_ = MutationMethod::gauss;    /* The mutation method used in the GA. */
    double gauss_mutation_param_ = 6.0;                         /* The parameter of the gauss mutation operator. */

    /* Used to randomly generate the Candidates of the initial population. */
    Candidate generateCandidate() const override;

    /* Used to perform crossovers in the GA. */
    CandidatePair crossover(const Candidate& p1, const Candidate& p2) const override;

    /* Used to perform mutations in the GA. */
    void mutate(Candidate& child) const override;

};

#endif // !GA_H_
