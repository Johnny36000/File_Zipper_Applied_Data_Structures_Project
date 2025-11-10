#ifndef FILE_ZIPPER_APPLIED_DATA_STRUCTURES_PROJECT_HUFFMANNODE_H
#define FILE_ZIPPER_APPLIED_DATA_STRUCTURES_PROJECT_HUFFMANNODE_H

/**
 * HuffmanNode class - Node structure for Huffman tree
 * Each node contains character data, frequency, and pointers to children
 */
class HuffmanNode {
public:
    unsigned char data;      // Character stored (only for leaf nodes)
    unsigned int frequency;  // Frequency of character or sum of children frequencies
    HuffmanNode* left;       // Left child pointer
    HuffmanNode* right;      // Right child pointer
    bool isLeaf;             // Flag to identify leaf nodes

    // Constructors
    HuffmanNode();
    HuffmanNode(unsigned char ch, unsigned int freq);
    HuffmanNode(unsigned int freq, HuffmanNode* l, HuffmanNode* r);

    // Destructor
    ~HuffmanNode();
};

#endif //FILE_ZIPPER_APPLIED_DATA_STRUCTURES_PROJECT_HUFFMANNODE_H