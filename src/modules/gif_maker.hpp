// gif_maker.hpp
#pragma once
#include <vector>
#include <string>
#include <cstdint>

bool createGIF(const std::vector<std::vector<uint8_t>> &frames,
               int width, int height,
               int delayMs,
               const std::string &outputPath);