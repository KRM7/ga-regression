/* Copyright (c) 2021 Krisztián Rugási. */

/*
* Mutation function used in the mixed-coded genetic algorithm.
* The real-encoded coefficients of the genes use the real mutations, while the genes encoding the
* form of the function use a different mutation operator.
*/

#ifndef MUTATION_H
#define MUTATION_H

#include "ga.h"

#include <string>


/* Performs random mutation on the coefficients in the genes. Mutated coefficients are randomly chosen from within the allowed range. */
void randomMutateCoeffs(mGA::Candidate& child, double pm, const mGA::limits_t& bounds);

/* Performs boundary mutation on the coefficients in the genes. Mutated coefficients are either the lower or upper bound of the allowed range. */
void boundaryMutateCoeffs(mGA::Candidate& child, double pm, const mGA::limits_t& bounds);

/* Performs gauss mutation on the coefficients in the genes. Mutated coefficients are randomly chosen from a normal distribution around the original value. */
void gaussMutateCoeffs(mGA::Candidate& child, double pm, const mGA::limits_t& bounds, double scale);

/* Performs mutation on the parts of the genes which encode the form of the function. */
void mutateForm(mGA::Candidate& child, double pm, const std::string& fmask, const std::string& opmask);

#endif // !MUTATION_H