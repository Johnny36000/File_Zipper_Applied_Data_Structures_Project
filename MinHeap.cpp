#include "MinHeap.h"

/**
 * Constructor - Initialize MinHeap with given capacity
 */
MinHeap::MinHeap(int cap) {
    capacity = cap;
    size = 0;
    heapArray = new HuffmanNode*[capacity];
}

/**
 * Destructor - Clean up heap array (nodes are deleted elsewhere)
 */
MinHeap::~MinHeap() {
    delete[] heapArray;
}

/**
 * Get parent index
 */
int MinHeap::parent(int i) {
    return (i - 1) / 2;
}

/**
 * Get left child index
 */
int MinHeap::leftChild(int i) {
    return 2 * i + 1;
}

/**
 * Get right child index
 */
int MinHeap::rightChild(int i) {
    return 2 * i + 2;
}

/**
 * Heapify down - Restore heap property downward from given index
 */
void MinHeap::heapifyDown(int index) {
    int smallest = index;
    int left = leftChild(index);
    int right = rightChild(index);
    
    // Find smallest among node and its children
    if (left < size && heapArray[left]->frequency < heapArray[smallest]->frequency) {
        smallest = left;
    }
    
    if (right < size && heapArray[right]->frequency < heapArray[smallest]->frequency) {
        smallest = right;
    }
    
    // If smallest is not the current node, swap and continue heapifying
    if (smallest != index) {
        HuffmanNode* temp = heapArray[index];
        heapArray[index] = heapArray[smallest];
        heapArray[smallest] = temp;
        
        heapifyDown(smallest);
    }
}

/**
 * Heapify up - Restore heap property upward from given index
 */
void MinHeap::heapifyUp(int index) {
    while (index > 0 && heapArray[parent(index)]->frequency > heapArray[index]->frequency) {
        // Swap with parent
        HuffmanNode* temp = heapArray[index];
        heapArray[index] = heapArray[parent(index)];
        heapArray[parent(index)] = temp;
        
        index = parent(index);
    }
}

/**
 * Resize heap array when capacity is reached
 */
void MinHeap::resize() {
    capacity *= 2;
    HuffmanNode** newArray = new HuffmanNode*[capacity];
    
    for (int i = 0; i < size; i++) {
        newArray[i] = heapArray[i];
    }
    
    delete[] heapArray;
    heapArray = newArray;
}

/**
 * Insert a node into the heap
 */
void MinHeap::insert(HuffmanNode* node) {
    if (size == capacity) {
        resize();
    }
    
    // Insert at end and heapify up
    heapArray[size] = node;
    heapifyUp(size);
    size++;
}

/**
 * Extract and return the minimum element (root)
 */
HuffmanNode* MinHeap::extractMin() {
    if (isEmpty()) {
        return nullptr;
    }
    
    HuffmanNode* minNode = heapArray[0];
    
    // Replace root with last element
    heapArray[0] = heapArray[size - 1];
    size--;
    
    // Restore heap property
    if (size > 0) {
        heapifyDown(0);
    }
    
    return minNode;
}

/**
 * Peek at minimum element without removing it
 */
HuffmanNode* MinHeap::peek() {
    if (isEmpty()) {
        return nullptr;
    }
    return heapArray[0];
}

/**
 * Get current size of heap
 */
int MinHeap::getSize() {
    return size;
}

/**
 * Check if heap is empty
 */
bool MinHeap::isEmpty() {
    return size == 0;
}

/**
 * Clear all elements from heap
 */
void MinHeap::clear() {
    size = 0;
}