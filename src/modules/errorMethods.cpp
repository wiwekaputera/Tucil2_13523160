#include "errorMethods.hpp"
#include <cmath>
#include <cstdlib>

using namespace std;

// Variance method.
double calculateVarianceError(const unsigned char *block, int numPixels)
{
    double sumR = 0, sumG = 0, sumB = 0;
    for (int i = 0; i < numPixels; i++)
    {
        sumR += block[i * 3 + 0];
        sumG += block[i * 3 + 1];
        sumB += block[i * 3 + 2];
    }
    double meanR = sumR / numPixels;
    double meanG = sumG / numPixels;
    double meanB = sumB / numPixels;

    double varR = 0, varG = 0, varB = 0;
    for (int i = 0; i < numPixels; i++)
    {
        double diffR = block[i * 3 + 0] - meanR;
        double diffG = block[i * 3 + 1] - meanG;
        double diffB = block[i * 3 + 2] - meanB;
        varR += diffR * diffR;
        varG += diffG * diffG;
        varB += diffB * diffB;
    }
    varR /= numPixels;
    varG /= numPixels;
    varB /= numPixels;
    return (varR + varG + varB) / 3.0;
}

// MAD method.
double calculateMADError(const unsigned char *block, int numPixels)
{
    double sumR = 0, sumG = 0, sumB = 0;
    for (int i = 0; i < numPixels; i++)
    {
        sumR += block[i * 3 + 0];
        sumG += block[i * 3 + 1];
        sumB += block[i * 3 + 2];
    }
    double meanR = sumR / numPixels;
    double meanG = sumG / numPixels;
    double meanB = sumB / numPixels;

    double madR = 0, madG = 0, madB = 0;
    for (int i = 0; i < numPixels; i++)
    {
        madR += fabs(block[i * 3 + 0] - meanR);
        madG += fabs(block[i * 3 + 1] - meanG);
        madB += fabs(block[i * 3 + 2] - meanB);
    }
    madR /= numPixels;
    madG /= numPixels;
    madB /= numPixels;
    return (madR + madG + madB) / 3.0;
}

// Maximum Pixel Difference method.
double calculateMaxDiffError(const unsigned char *block, int numPixels)
{
    unsigned char minR = 255, minG = 255, minB = 255;
    unsigned char maxR = 0, maxG = 0, maxB = 0;
    for (int i = 0; i < numPixels; i++)
    {
        unsigned char r = block[i * 3 + 0];
        unsigned char g = block[i * 3 + 1];
        unsigned char b = block[i * 3 + 2];
        if (r < minR)
            minR = r;
        if (r > maxR)
            maxR = r;
        if (g < minG)
            minG = g;
        if (g > maxG)
            maxG = g;
        if (b < minB)
            minB = b;
        if (b > maxB)
            maxB = b;
    }
    double diffR = maxR - minR;
    double diffG = maxG - minG;
    double diffB = maxB - minB;
    return (diffR + diffG + diffB) / 3.0;
}

// Entropy method.
double calculateEntropyError(const unsigned char *block, int numPixels)
{
    int histR[256] = {0}, histG[256] = {0}, histB[256] = {0};
    for (int i = 0; i < numPixels; i++)
    {
        histR[block[i * 3 + 0]]++;
        histG[block[i * 3 + 1]]++;
        histB[block[i * 3 + 2]]++;
    }
    double entropyR = 0, entropyG = 0, entropyB = 0;
    for (int i = 0; i < 256; i++)
    {
        if (histR[i] > 0)
        {
            double p = double(histR[i]) / numPixels;
            entropyR -= p * log2(p);
        }
        if (histG[i] > 0)
        {
            double p = double(histG[i]) / numPixels;
            entropyG -= p * log2(p);
        }
        if (histB[i] > 0)
        {
            double p = double(histB[i]) / numPixels;
            entropyB -= p * log2(p);
        }
    }
    return (entropyR + entropyG + entropyB) / 3.0;
}