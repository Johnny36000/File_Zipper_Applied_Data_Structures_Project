#include "HuffmanNode.h"

/**
 * Default Constructor
 */
HuffmanNode::HuffmanNode() {
    data = '\0';
    frequency = 0;
    left = nullptr;
    right = nullptr;
    isLeaf = false;
}

/**
 * Constructor for leaf node (contains character)
 */
HuffmanNode::HuffmanNode(unsigned char ch, unsigned int freq) {
    data = ch;
    frequency = freq;
    left = nullptr;
    right = nullptr;
    isLeaf = true;
}

/**
 * Constructor for internal node (has children)
 */
HuffmanNode::HuffmanNode(unsigned int freq, HuffmanNode* l, HuffmanNode* r) {
    data = '\0';
    frequency = freq;
    left = l;
    right = r;
    isLeaf = false;
}

/**
 * Destructor - Recursively delete children
 */
HuffmanNode::~HuffmanNode() {
    // Note: Be careful with deletion - only delete if you own the nodes
    // In Huffman tree, we typically delete the entire tree from root
}