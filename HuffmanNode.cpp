#include "HuffmanNode.h"

/**
 * Constructor for leaf nodes or default initialization
 * Handles all byte values 0-255
 */
HuffmanNode::HuffmanNode(unsigned char ch, unsigned int freq) {
    data = ch;
    frequency = freq;
    left = nullptr;
    right = nullptr;
}

/**
 * Constructor for internal nodes with children
 * Used when merging two nodes in Huffman tree construction
 */
HuffmanNode::HuffmanNode(unsigned int freq, HuffmanNode* l, HuffmanNode* r) {
    data = 0;
    frequency = freq;
    left = l;
    right = r;
}

/**
 * Check if this is a leaf node (contains actual data)
 */
bool HuffmanNode::isLeaf() const {
    return (left == nullptr && right == nullptr);
}

/**
 * Recursively delete all child nodes
 */
HuffmanNode::~HuffmanNode() {
    delete left;
    delete right;
}
