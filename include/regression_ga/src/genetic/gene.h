/* Copyright (c) 2021 Krisztián Rugási. */

/*
* Contains the type of the genes used in the genetic algorithm.
* Each gene encodes a math function consisting of the ID of the function, its coefficients, and an operator.
*/

#ifndef GENE_H
#define GENE_H

#include <vector>
#include <functional>
#include <cstddef>


/* The gene type used in the GA. */
struct Gene
{
    Gene() = delete;
    Gene(int fid, const std::vector<double>& coeffs, int opid) : fid(fid), coeffs(coeffs), opid(opid) {}

    /* Function. */
    int fid;
    std::vector<double> coeffs;

    /* Operator. */
    int opid;


    bool operator==(const Gene& rhs) const noexcept
    {
        return (this->coeffs == rhs.coeffs) && (this->fid == rhs.fid) && (this->opid == rhs.opid);
    }
    bool operator!=(const Gene& rhs) const noexcept
    {
        return !(*this == rhs);
    }
};

namespace std
{
    /* Hasher needs to be defined for the gene type for the GA to work. */
    template<>
    struct hash<Gene>
    {
        size_t operator()(const Gene& gene) const noexcept
        {
            size_t seed = gene.coeffs.size();
            for (const auto& coeff : gene.coeffs)
            {
                seed ^= std::hash<double>()(coeff) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            seed ^= std::hash<int>()(gene.fid) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= std::hash<int>()(gene.opid) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

            return seed;
        }
    };
}

#endif // !GENE_H
