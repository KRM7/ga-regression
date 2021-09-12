/* Copyright (c) 2021 Krisztián Rugási. */

/* 
* Crossover functions used in the mixed-coded genetic algorithm. 
* The real-encdoded coefficients in the genes use the different real crossovers the function are
* named after, while the parts of the genes encoding the function's form use uniform crossover.
*/

#ifndef CROSSOVER_H
#define CROSSOVER_H

#include "ga.h"


/* BLX-alpha crossover used for the real encoded coefficients. (With uniform crossover for the function form.) */
mGA::CandidatePair blxAlphaCrossover(const mGA::Candidate& parent1,
                                     const mGA::Candidate& parent2,
                                     double pc,
                                     double alpha,
                                     const mGA::limits_t& bounds);

/* Simulated binary crossover used for the real encdoded coefficients. (With uniform crossover for the function form.) */
mGA::CandidatePair simulatedBinaryCrossover(const mGA::Candidate& parent1,
                                            const mGA::Candidate& parent2,
                                            double pc,
                                            double eta,
                                            const mGA::limits_t& bounds);

/* Wright crossover used for the real encdoded coefficients. (Added uniform crossover for the function form.) (Only works with 1 objective.) */
mGA::CandidatePair wrightCrossover(const mGA::Candidate& parent1,
                                   const mGA::Candidate& parent2,
                                   double pc,
                                   const mGA::limits_t& bounds);

#endif // !CROSSOVER_H