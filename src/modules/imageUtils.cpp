#include "imageUtils.hpp"
#include <sys/stat.h>
#include <iostream>
#include <vector>
using namespace std;

long getFileSize(const string &filePath)
{
    struct stat st;
    if (stat(filePath.c_str(), &st) == 0)
        return st.st_size;
    return -1;
}

FIBITMAP *loadImageAs24Bit(const string &filePath, int &width, int &height)
{
    // Determine the file format of the input image.
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filePath.c_str());
    if (fif == FIF_UNKNOWN)
        fif = FreeImage_GetFIFFromFilename(filePath.c_str());
    if (fif == FIF_UNKNOWN)
    {
        cerr << "Unknown image format: " << filePath << "\n";
        return nullptr;
    }

    // Load the image.
    FIBITMAP *bitmap = FreeImage_Load(fif, filePath.c_str());
    if (!bitmap)
    {
        cerr << "Error loading image: " << filePath << "\n";
        return nullptr;
    }

    // Convert image to 24-bit RGB.
    FIBITMAP *converted = FreeImage_ConvertTo24Bits(bitmap);
    FreeImage_Unload(bitmap);

    if (!converted)
    {
        cerr << "Error converting image to 24-bit: " << filePath << "\n";
        return nullptr;
    }

    width = FreeImage_GetWidth(converted);
    height = FreeImage_GetHeight(converted);
    if (width == 0 || height == 0)
    {
        cerr << "Error: Invalid image dimensions.\n";
        FreeImage_Unload(converted);
        return nullptr;
    }

    return converted;
}

void flipImageVertically(uint8_t* data, int width, int height) {
    int rowSize = width * 3;
    vector<uint8_t> temp(rowSize);
    for (int y = 0; y < height / 2; y++) {
        uint8_t* rowTop = data + y * rowSize;
        uint8_t* rowBottom = data + (height - 1 - y) * rowSize;
        // Swap the two rows
        memcpy(temp.data(), rowTop, rowSize);
        memcpy(rowTop, rowBottom, rowSize);
        memcpy(rowBottom, temp.data(), rowSize);
    }
}
