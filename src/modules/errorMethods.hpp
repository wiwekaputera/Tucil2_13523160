#ifndef ERRORMETHODS_HPP
#define ERRORMETHODS_HPP

enum ErrorMethod
{
    ERROR_VARIANCE = 1,
    ERROR_MAD = 2,
    ERROR_MAX_DIFF = 3,
    ERROR_ENTROPY = 4
};

// Variance error calculation.
double calculateVarianceError(const unsigned char *block, int numPixels);

// Mean Absolute Deviation error calculation.
double calculateMADError(const unsigned char *block, int numPixels);

// Maximum Pixel Difference error calculation.
double calculateMaxDiffError(const unsigned char *block, int numPixels);

// Entropy error calculation.
double calculateEntropyError(const unsigned char *block, int numPixels);

#endif // ERRORMETHODS_HPP