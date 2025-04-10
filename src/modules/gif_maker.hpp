#ifndef GIF_MAKER_HPP
#define GIF_MAKER_HPP

#include <vector>
#include <string>
#include <cstdint>

bool createGIF(
    const std::vector<std::vector<uint8_t>> &frames,
    int width, int height,
    int delayMs,
    const std::string &outputPath);

#endif // GIF_MAKER_HPP