#include "gif_maker.hpp"
#include "gif.h"
#include <cstdio>

using namespace std;

bool createGIF(
    const vector<vector<uint8_t>> &frames,
    int width, int height,
    int delayMs,
    const string &outputPath)
{
    if (frames.empty() || width <= 0 || height <= 0)
        return false;

    GifWriter writer = {};
    if (!GifBegin(&writer, outputPath.c_str(), width, height, delayMs))
        return false;

    // Convert 3 channels (RGB) to 4 channels per pixel (RGBA).
    for (const auto &frame : frames)
    {
        vector<uint8_t> rgba(width * height * 4);
        for (int i = 0; i < width * height; i++)
        {
            rgba[i * 4 + 0] = frame[i * 3 + 2];
            rgba[i * 4 + 1] = frame[i * 3 + 1];
            rgba[i * 4 + 2] = frame[i * 3 + 0];
            rgba[i * 4 + 3] = 255;
        }
        if (!GifWriteFrame(&writer, rgba.data(), width, height, delayMs))
        {
            GifEnd(&writer);
            return false;
        }
    }
    GifEnd(&writer);
    return true;
}