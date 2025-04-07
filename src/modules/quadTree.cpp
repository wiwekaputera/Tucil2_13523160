#include "quadTree.hpp"
#include "errorMethods.hpp"
#include <cstring>
#include <cstdlib>
#include <cmath>

using namespace std;

// QuadNode Implementation
QuadNode::QuadNode(int xPos, int yPos, int w, int h)
    : x(xPos), y(yPos), width(w), height(h),
      meanR(0.0), meanG(0.0), meanB(0.0), isLeaf(false)
{
    memset(children, 0, sizeof(children));
}

QuadNode::~QuadNode()
{
    for (int i = 0; i < 4; i++)
    {
        delete children[i];
        children[i] = nullptr;
    }
}

// QuadTree Implementation
QuadTree::QuadTree() : root(nullptr) {}

QuadTree::~QuadTree()
{
    delete root;
    root = nullptr;
}

void QuadTree::build(
    const unsigned char *imgData, int width, int height,
    double threshold, int minBlockSize, int errorMethod)
{
    delete root;
    root = buildRecursive(imgData, width, height, 0, 0, width, height, threshold, minBlockSize, errorMethod);
}

QuadNode *QuadTree::buildRecursive(
    const unsigned char *imgData,
    int fullWidth, int fullHeight,
    int x, int y, int w, int h,
    double threshold, int minBlockSize, int errorMethod)
{
    // Create a new node at (x, y) with dimensions w x h.
    QuadNode *node = new QuadNode(x, y, w, h);
    // Copy block into temporary buffer for error calculation.
    int numPixels = w * h;
    unsigned char *blockBuffer = new unsigned char[numPixels * 3];
    for (int row = y; row < y + h; row++)
    {
        memcpy(
            blockBuffer + ((row - y) * w * 3),
            imgData + (row * fullWidth + x) * 3,
            w * 3);
    }
    // Calculate error.
    double blockError = 0.0;
    switch (errorMethod)
    {
    case ERROR_VARIANCE:
        blockError = calculateVarianceError(blockBuffer, numPixels);
        break;
    case ERROR_MAD:
        blockError = calculateMADError(blockBuffer, numPixels);
        break;
    case ERROR_MAX_DIFF:
        blockError = calculateMaxDiffError(blockBuffer, numPixels);
        break;
    case ERROR_ENTROPY:
        blockError = calculateEntropyError(blockBuffer, numPixels);
        break;
    default:
        blockError = calculateVarianceError(blockBuffer, numPixels);
        break;
    }
    // Compute mean per channel.
    unsigned long sumR = 0, sumG = 0, sumB = 0;
    for (int i = 0; i < numPixels; i++)
    {
        sumR += blockBuffer[i * 3 + 0];
        sumG += blockBuffer[i * 3 + 1];
        sumB += blockBuffer[i * 3 + 2];
    }
    delete[] blockBuffer;
    node->meanR = double(sumR) / numPixels;
    node->meanG = double(sumG) / numPixels;
    node->meanB = double(sumB) / numPixels;

    // Determine further splitting/not
    if (blockError <= threshold || w <= minBlockSize || h <= minBlockSize)
    {
        node->isLeaf = true;
        return node;
    }

    int halfW = w / 2;
    int halfH = h / 2;
    if (halfW < minBlockSize || halfH < minBlockSize)
    {
        node->isLeaf = true;
        return node;
    }
    node->isLeaf = false;
    node->children[0] = buildRecursive(imgData, fullWidth, fullHeight, x, y, halfW, halfH, threshold, minBlockSize, errorMethod);
    node->children[1] = buildRecursive(imgData, fullWidth, fullHeight, x + halfW, y, w - halfW, halfH, threshold, minBlockSize, errorMethod);
    node->children[2] = buildRecursive(imgData, fullWidth, fullHeight, x, y + halfH, halfW, h - halfH, threshold, minBlockSize, errorMethod);
    node->children[3] = buildRecursive(imgData, fullWidth, fullHeight, x + halfW, y + halfH, w - halfW, h - halfH, threshold, minBlockSize, errorMethod);

    return node;
}

void QuadTree::reconstructRecursive(const QuadNode *node, unsigned char *outBuffer, int fullWidth)
{
    if (!node)
        return;
    if (node->isLeaf)
    {
        unsigned char rVal = (unsigned char)(node->meanR + 0.5);
        unsigned char gVal = (unsigned char)(node->meanG + 0.5);
        unsigned char bVal = (unsigned char)(node->meanB + 0.5);
        for (int row = node->y; row < node->y + node->height; row++)
        {
            for (int col = node->x; col < node->x + node->width; col++)
            {
                int idx = (row * fullWidth + col) * 3;
                outBuffer[idx + 0] = bVal;
                outBuffer[idx + 1] = gVal;
                outBuffer[idx + 2] = rVal;
            }
        }
    }
    else
    {
        for (int i = 0; i < 4; i++)
            reconstructRecursive(node->children[i], outBuffer, fullWidth);
    }
}

void QuadTree::reconstructImage(unsigned char *outBuffer, int fullWidth, int fullHeight)
{
    memset(outBuffer, 0, fullWidth * fullHeight * 3);
    reconstructRecursive(root, outBuffer, fullWidth);
}

int QuadTree::getTreeDepth() const
{
    return getTreeDepth(root);
}
int QuadTree::getTreeDepth(const QuadNode *node) const
{
    if (!node)
        return 0;
    if (node->isLeaf)
        return 1;
    int maxDepth = 0;
    for (int i = 0; i < 4; i++)
    {
        int d = getTreeDepth(node->children[i]);
        if (d > maxDepth)
            maxDepth = d;
    }
    return maxDepth + 1;
}

int QuadTree::getNodeCount() const
{
    return getNodeCount(root);
}
int QuadTree::getNodeCount(const QuadNode *node) const
{
    if (!node)
        return 0;
    int count = 1;
    for (int i = 0; i < 4; i++)
    {
        count += getNodeCount(node->children[i]);
    }
    return count;
}