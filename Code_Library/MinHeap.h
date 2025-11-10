#ifndef FILE_ZIPPER_APPLIED_DATA_STRUCTURES_PROJECT_MINHEAP_H
#define FILE_ZIPPER_APPLIED_DATA_STRUCTURES_PROJECT_MINHEAP_H

#include "HuffmanNode.h"

/**
 * MinHeap class - A min-heap implementation for Huffman tree construction
 * Used as a priority queue to select nodes with minimum frequency
 */
class MinHeap {
private:
    HuffmanNode** heapArray;  // Array of pointers to HuffmanNodes
    int capacity;              // Maximum size of heap
    int size;                  // Current number of elements in heap

    // Helper functions
    int parent(int i);
    int leftChild(int i);
    int rightChild(int i);
    void heapifyDown(int index);
    void heapifyUp(int index);
    void resize();

public:
    // Constructor and Destructor
    MinHeap(int cap = 256);
    ~MinHeap();

    // Core operations
    void insert(HuffmanNode* node);
    HuffmanNode* extractMin();
    HuffmanNode* peek();

    // Utility functions
    int getSize();
    bool isEmpty();
    void clear();
};


#endif //FILE_ZIPPER_APPLIED_DATA_STRUCTURES_PROJECT_MINHEAP_H