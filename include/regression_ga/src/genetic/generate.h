/* Copyright (c) 2021 Krisztián Rugási. */

/* Functions used in generating the candidates of the of the GA. */

#ifndef GA_UTILS_H
#define GA_UTILS_H

#include <string>
#include <vector>
#include <cstddef>

#include "ga.h"


/* Generate a random operator index for which the value of the mask is 1. */
int randomOperator(const std::string& opmask);

/* Generate a random function index for which the value of the mask is 1. */
int randomFunc(const std::string& fmask);

/* Generate a random coefficient vector within the bounds. */
std::vector<double> generateCoeffs(size_t num_coeffs, const mGA::limits_t& bounds);

/* Generate a completely random candidate. */
mGA::Candidate generateRandomSol(size_t chrom_len,
                                 size_t num_coeffs,
                                 const mGA::limits_t& bounds,
                                 const std::string& fmask,
                                 const std::string& opmask);

/* Generate a candidate solution with a preset function form. */
mGA::Candidate generatePresetSol(size_t chrom_len,
                                 size_t num_coeffs,
                                 const mGA::limits_t& bounds,
                                 const std::vector<int>& presetf);

#endif // !GA_UTILS_H