#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include "errorMethods.hpp"

struct QuadNode
{
    int x, y;                   // Top-left position of the block.
    int width, height;          // Dimensions of the block.
    double meanR, meanG, meanB; // Average intensity per channel.
    bool isLeaf;                // True if no further subdivision.
    QuadNode *children[4];      // Child nodes (nullptr if leaf).

    QuadNode(int xPos, int yPos, int w, int h);
    ~QuadNode();
};

class QuadTree
{
public:
    QuadTree();
    ~QuadTree();

    // Build the quadtree from an RGB image buffer.
    // imgData: pointer to image data (RGB, row-major; 3 bytes per pixel).
    void build(const unsigned char *imgData, int width, int height,
               double threshold, int minBlockSize, int errorMethod);

    // Reconstruct the compressed image into outBuffer.
    // outBuffer allocated with size (width*height*3).
    void reconstructImage(unsigned char *outBuffer, int width, int height);

    // Member functions to retrieve quadtree metrics.
    int getTreeDepth() const;
    int getNodeCount() const;

    QuadNode *root;

private:
    // Recursive helper to build the quadtree.
    QuadNode *buildRecursive(
        const unsigned char *imgData,
        int fullWidth, int fullHeight,
        int x, int y, int w, int h,
        double threshold, int minBlockSize, int errorMethod);

    // Recursive helper to reconstruct image from quadtree leaves.
    static void reconstructRecursive(const QuadNode *node, unsigned char *outBuffer, int fullWidth);

    // Recursive helper functions for tree metrics.
    int getTreeDepth(const QuadNode *node) const;
    int getNodeCount(const QuadNode *node) const;
};

#endif // QUADTREE_HPP