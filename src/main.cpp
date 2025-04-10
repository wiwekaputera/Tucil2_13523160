#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <limits>
#include <sys/stat.h>
#include <FreeImage.h>
#include "modules/imageUtils.hpp"
#include "modules/quadTree.hpp"
#include "modules/errorMethods.hpp"
#include "modules/gif_maker.hpp"

using namespace std;

// Debugging function
void printFramePixels(const vector<uint8_t> &frame, int width, int height, int numPixelsToPrint = 10)
{
    // Each pixel consists of 3 bytes.
    int totalPixels = width * height;
    int pixelsToPrint = (numPixelsToPrint < totalPixels) ? numPixelsToPrint : totalPixels;

    for (int i = 0; i < pixelsToPrint; i++)
    {
        int idx = i * 3;
        int blue = static_cast<int>(frame[idx + 0]);
        int green = static_cast<int>(frame[idx + 1]);
        int red = static_cast<int>(frame[idx + 2]);
        cout << "Pixel " << i << ": B=" << blue << " G=" << green << " R=" << red << "\n";
    }
}

// Check if file exists using stat.
bool fileExists(const string &filename)
{
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}

// Simple function to convert a string to lower-case.
string toLower(const string &s)
{
    string result;
    for (char c : s)
        result.push_back(tolower(c));
    return result;
}

int main()
{
    // Initialize FreeImage.
    FreeImage_Initialise();

    cout << "====================\n";
    cout << "        INPUT       \n";
    cout << "====================\n";

    // 1. Input: Image path.
    string inputImagePath;
    while (true)
    {
        cout << "Masukkan path gambar yang akan dikompres: ";
        getline(cin, inputImagePath);
        if (!fileExists(inputImagePath))
        {
            cout << "File tidak ditemukan. Silakan masukkan path yang valid.\n";
            continue;
        }
        // Check if the file path contains an extension
        size_t pos = inputImagePath.find_last_of(".");
        if (pos == string::npos)
        {
            cout << "Path harus memiliki ekstensi .jpg, .jpeg, atau .png.\n";
            continue;
        }
        string ext = toLower(inputImagePath.substr(pos));
        if (ext == ".jpg" || ext == ".jpeg" || ext == ".png")
            break;
        else
            cout << "Ekstensi file tidak didukung. Hanya .jpg, .jpeg, atau .png yang diterima.\n";
    }

    // 2. Input: Error calculation method.
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

    // 3. Input: Threshold.
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
            validThreshold = (threshold >= 0 && threshold <= 16384);
            if (!validThreshold)
                cout << "Masukan tidak valid. Untuk Variance, masukkan nilai antara 0 dan 16384.\n";
            break;
        case 2:
        case 3:
            validThreshold = (threshold >= 0 && threshold <= 255);
            if (!validThreshold)
                cout << "Masukan tidak valid. Untuk MAD/Max Pixel Difference, masukkan nilai antara 0 dan 255.\n";
            break;
        case 4:
            validThreshold = (threshold >= 0 && threshold <= 8);
            if (!validThreshold)
                cout << "Masukan tidak valid. Untuk Entropy, masukkan nilai antara 0 dan 8.\n";
            break;
        }
    }

    // 4. Input: Minimum block size.
    int minBlockSize;
    cout << "Masukkan minimum block size: ";
    cin >> minBlockSize;

    // 5. Input: Output image path.
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string outputImagePath;
    while (true)
    {
        cout << "Masukkan path output image: ";
        getline(cin, outputImagePath);
        size_t pos = outputImagePath.find_last_of(".");
        if (pos == string::npos)
        {
            cout << "Path harus memiliki ekstensi .jpg, .jpeg, atau .png.\n";
            continue;
        }
        string ext = toLower(outputImagePath.substr(pos));
        if (ext == ".jpg" || ext == ".jpeg" || ext == ".png")
            break;
        else
            cout << "Ekstensi file tidak didukung. Hanya .jpg, .jpeg, atau .png yang diterima.\n";
    }

    // 6. Input: Output GIF path.
    string outputGIFPath;
    while (true)
    {
        cout << "Masukkan path output GIF: ";
        getline(cin, outputGIFPath);
        size_t pos = outputGIFPath.find_last_of(".");
        if (pos == string::npos)
        {
            cout << "Path harus memiliki ekstensi .gif.\n";
            continue;
        }
        string ext = toLower(outputGIFPath.substr(pos));
        if (ext == ".gif")
            break;
        else
            cout << "Ekstensi file tidak didukung. Hanya .gif yang diterima.\n";
    }

    // Start time
    clock_t startTime = clock();

    // Load image using FreeImage.
    int width = 0, height = 0;
    FIBITMAP *converted = loadImageAs24Bit(inputImagePath, width, height);
    if (!converted)
    {
        FreeImage_DeInitialise();
        return 1;
    }

    // Copy image data into a 1D array and convert BGR -> RGB.
    unsigned char *imgData = new unsigned char[width * height * 3];
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

    // Build quadtree and record frames.
    // Frame: vector<uint8_t> = an image of every QuadTree level
    vector<vector<uint8_t>> frames;
    QuadTree qt;
    qt.build(imgData, width, height, threshold, minBlockSize, errorMethodChoice);

    vector<vector<uint8_t>> levelFrames = qt.captureFramesPerLevel(width, height);

    // Push the last frame 3 more times so that the final image appears longer
    if (!levelFrames.empty())
    {
        for (int i = 0; i < 3; i++)
        {
            levelFrames.push_back(levelFrames.back());
        }
    }

    // Debugging statement
    // if (!frames.empty())
    // {
    //     cout << "First frame pixel values:\n";
    //     printFramePixels(frames[0], width, height, 10);
    // }
    // cout << "EXECUTED" << endl;

    // Reconstruct compressed image.
    unsigned char *compressedData = new unsigned char[width * height * 3];
    qt.reconstructImage(compressedData, width, height);

    // Determine output format.
    FREE_IMAGE_FORMAT outFIF = FreeImage_GetFIFFromFilename(outputImagePath.c_str());
    if (outFIF == FIF_UNKNOWN)
    {
        cerr << "Unknown input image format; defaulting output to JPEG.\n";
        outFIF = FIF_JPEG;
    }

    // Save the compressed image.
    FIBITMAP *outBmp = FreeImage_ConvertFromRawBits(compressedData, width, height, width * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF);
    if (!FreeImage_Save(outFIF, outBmp, outputImagePath.c_str(), 0))
        cerr << "Error saving compressed image to " << outputImagePath << "\n";
    FreeImage_Unload(outBmp);

    delete[] compressedData;
    delete[] imgData;

    clock_t endTime = clock();
    double executionTime = double(endTime - startTime) / CLOCKS_PER_SEC;

    // Create animated GIF from recorded frames.
    if (!createGIF(levelFrames, width, height, 50, outputGIFPath))
        cerr << "GIF creation failed." << "\n";

    endTime = clock();
    double GIFexecutionTime = double(endTime - startTime) / CLOCKS_PER_SEC;

    long compressedSize = getFileSize(outputImagePath);
    double compressionPercentage = 0.0;
    if (originalSize > 0)
        compressionPercentage = (1.0 - double(compressedSize) / originalSize) * 100.0;

    cout << "\n====================\n";
    cout << "       OUTPUT       \n";
    cout << "====================\n";
    cout << "Waktu Eksekusi Kompresi (s): " << executionTime << "\n";
    cout << "Waktu Eksekusi hingga Pembuatan GIF (s): " << GIFexecutionTime << "\n";
    cout << "Ukuran Gambar Sebelum (bytes): " << originalSize << "\n";
    cout << "Ukuran Gambar Setelah (bytes): " << compressedSize << "\n";
    cout << "Persentase Kompresi: " << compressionPercentage << "%\n";
    cout << "Kedalaman Pohon: " << qt.getTreeDepth() << "\n";
    cout << "Banyak Simpul Pohon: " << qt.getNodeCount() << "\n";
    cout << "Gambar Hasil Kompresi Disimpan pada: " << outputImagePath << "\n";
    cout << "GIF Disimpan pada: " << outputGIFPath << "\n";

    FreeImage_DeInitialise();
    return 0;
}