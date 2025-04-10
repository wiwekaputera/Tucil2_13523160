#ifndef IMAGE_UTILS_HPP
#define IMAGE_UTILS_HPP

#include <string>
#include <FreeImage.h>

// Returns file size (in bytes).
long getFileSize(const std::string &filePath);

// Loads an image, converts it to 24-bit RGB,
// returns a pointer to the FreeImage bitmap.
// The function sets width and height.
// Returns nullptr if error.
FIBITMAP *loadImageAs24Bit(const std::string &filePath, int &width, int &height);

void flipImageVertically(uint8_t* data, int width, int height);

#endif // IMAGE_UTILS_HPP