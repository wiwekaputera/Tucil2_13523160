#include <iostream>
#include <string>
#include <ctime>
#include <limits>
#include <sys/stat.h>
#include <FreeImage.h>
#include "modules/imageUtils.hpp"
#include "modules/quadTree.hpp"
#include "modules/errorMethods.hpp"

using namespace std;

int main()
{
    FreeImage_Initialise();

    cout << "====================" << "\n";
    cout << "        INPUT       " << "\n";
    cout << "====================" << "\n";

    // 1. INPUT: Absolute path of the image to compress.
    string inputImagePath;
    cout << "Masukkan path gambar yang akan dikompres: ";
    getline(cin, inputImagePath);

    // 2. INPUT: Error calculation method.
    int errorMethodChoice;
    cout << "Pilihan Metode Kalkulasi Eror:\n";
    cout << "1: Variance\n";
    cout << "2: Mean Absolute Deviation (MAD)\n";
    cout << "3: Max Pixel Difference\n";
    cout << "4: Entropy\n";
    cout << "Masukkan pilihan: ";
    cin >> errorMethodChoice;
    if (errorMethodChoice < 1 || errorMethodChoice > 4)
    {
        cout << "Pilihan tidak valid.\n";
        return 1;
    }

    // 3. INPUT: Threshold error.
    double threshold;
    bool validThreshold = false;
    while (!validThreshold)
    {
        if (errorMethodChoice == 1)
            cout << "Masukkan nilai threshold untuk Variance (0-16384): ";
        else if (errorMethodChoice == 2 || errorMethodChoice == 3)
            cout << "Masukkan nilai threshold untuk MAD/Max Pixel Difference (0-255): ";
        else if (errorMethodChoice == 4)
            cout << "Masukkan nilai threshold untuk Entropy (0-8): ";
        cin >> threshold;
        switch (errorMethodChoice)
        {
        case 1:
            // Batas variance yang lebih realistis
            if (threshold >= 0 && threshold <= 16384)
                validThreshold = true;
            else
                cout << "Masukan tidak valid. Untuk Variance, masukkan nilai antara 0 dan 16384.\n";
            break;
        case 2:
        case 3:
            if (threshold >= 0 && threshold <= 255)
                validThreshold = true;
            else
                cout << "Masukan tidak valid. Untuk MAD/Max Pixel Difference, masukkan nilai antara 0 dan 255.\n";
            break;
        case 4:
            if (threshold >= 0 && threshold <= 8)
                validThreshold = true;
            else
                cout << "Masukan tidak valid. Untuk Entropy, masukkan nilai antara 0 dan 8.\n";
            break;
        }
    }

    // 4. INPUT: Minimum block size.
    int minBlockSize;
    cout << "Masukkan minimum block size: ";
    cin >> minBlockSize;

    // 5. INPUT: Output image path.
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string outputImagePath;
    cout << "Masukkan path output image: ";
    getline(cin, outputImagePath);

    clock_t startTime = clock();

    // Load image.
    int width = 0, height = 0;
    FIBITMAP *converted = loadImageAs24Bit(inputImagePath, width, height);
    if (!converted)
    {
        FreeImage_DeInitialise();
        return 1;
    }

    // Copy FI image data to 1D array imgData.
    unsigned char *imgData = new unsigned char[width * height * 3];
    // Convert scanlines from bottom-to-top and BGR -> RGB.
    for (int y = 0; y < height; y++)
    {
        BYTE *bits = FreeImage_GetScanLine(converted, y);
        for (int x = 0; x < width; x++)
        {
            int srcIdx = x * 3;
            int destIdx = (y * width + x) * 3;
            imgData[destIdx + 0] = bits[srcIdx + 2]; // R
            imgData[destIdx + 1] = bits[srcIdx + 1]; // G
            imgData[destIdx + 2] = bits[srcIdx + 0]; // B
        }
    }
    FreeImage_Unload(converted);

    // Get original file size.
    long originalSize = getFileSize(inputImagePath);


    // Build the quadtree using the selected error method.
    QuadTree qt;
    qt.build(imgData, width, height, threshold, minBlockSize, errorMethodChoice);

    // Reconstruct the compressed image.
    unsigned char *compressedData = new unsigned char[width * height * 3];
    qt.reconstructImage(compressedData, width, height);

    // Determine output format based on input's extension.
    FREE_IMAGE_FORMAT outFIF = FreeImage_GetFIFFromFilename(inputImagePath.c_str());
    if (outFIF == FIF_UNKNOWN)
    {
        cerr << "Unknown input image format; defaulting output to JPEG.\n";
        outFIF = FIF_JPEG;
    }

    // Save the compressed image.
    FIBITMAP *outBmp = FreeImage_ConvertFromRawBits(compressedData, width, height, width * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF);

    if (!FreeImage_Save(outFIF, outBmp, outputImagePath.c_str(), 0))
    {
        cerr << "Error saving compressed image to " << outputImagePath << "\n";
    }
    FreeImage_Unload(outBmp);
    delete[] compressedData;
    delete[] imgData;

    clock_t endTime = clock();
    double executionTime = double(endTime - startTime) / CLOCKS_PER_SEC;

    long compressedSize = getFileSize(outputImagePath);
    double compressionPercentage = 0.0;
    if (originalSize > 0)
        compressionPercentage = (1.0 - double(compressedSize) / originalSize) * 100.0;
    
    cout << "\n" << "====================" << "\n";
    cout << "       OUTPUT       " << "\n";
    cout << "====================" << "\n";
    cout << "Waktu Eksekusi (s): " << executionTime << "\n";
    cout << "Ukuran Gambar Sebelum (bytes): " << originalSize << "\n";
    cout << "Ukuran Gambar Setelah (bytes): " << compressedSize << "\n";
    cout << "Persentase Kompresi: " << compressionPercentage << "%\n";
    cout << "Kedalaman Pohon: " << qt.getTreeDepth() << "\n";
    cout << "Banyak Simpul Pohon: " << qt.getNodeCount() << "\n";
    cout << "Gambar Hasil Kompresi Disimpan pada: " << outputImagePath << "\n";

    FreeImage_DeInitialise();
    return 0;
}