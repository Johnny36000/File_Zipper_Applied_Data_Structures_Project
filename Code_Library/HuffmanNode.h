
#ifndef MILESTONE_2_ADS_HUFFMANNODE_H
#define MILESTONE_2_ADS_HUFFMANNODE_H

/**
 * Simple Huffman Tree Node
 * Uses unsigned char to handle all 256 possible byte values (0-255)
 */
class HuffmanNode {
public:
    // Node data - unsigned char handles full byte range 0-255
    unsigned char data;      // Character/byte value (meaningful for leaf nodes)
    unsigned int frequency;  // Occurrence count

    // Tree structure
    HuffmanNode* left;       // Left child
    HuffmanNode* right;      // Right child

    // Constructors
    HuffmanNode(unsigned char ch = 0, unsigned int freq = 0);
    HuffmanNode(unsigned int freq, HuffmanNode* l, HuffmanNode* r);

    // Check if node is a leaf
    bool isLeaf() const;

    // Destructor
    ~HuffmanNode();
};

#endif //MILESTONE_2_ADS_HUFFMANNODE_H