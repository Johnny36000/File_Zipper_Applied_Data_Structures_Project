#include "MinHeap.h"

static const int initialCapacity = 16; // choosing a small starting capacity to keep it light

// Constructor
MinHeap::MinHeap() {
    size_ = 0;
    capacity_ = initialCapacity;
    data_ = new HuffmanNode*[capacity_];
}

// Destructor
MinHeap::~MinHeap() {
    // Do not delete HuffmanNode*s (ownershp is external)
    // Huffman tree destructor will free them later
    delete[] data_; // only internal array is freed
}

void MinHeap::ensureCapacity() {
    if (size_ < capacity_) return; // capacity not reached, can exit safely

    int newCap = (capacity_ == 0) ? initialCapacity : capacity_ * 2; // double array size
    HuffmanNode** newData = new HuffmanNode*[newCap];

    for (int i = 0; i < size_; ++i) {
        newData[i] = data_[i];
    }

    delete[] data_;
    data_ = newData;
    capacity_ = newCap;
}

void MinHeap::insert(HuffmanNode* node) {

    if (!node) return;                // ignore nulls
    ensureCapacity();

    // insertion process:
    data_[size_] = node; // place at the end
    siftUp(size_);       // sift up if needed
    ++size_;             // increment size
}

HuffmanNode* MinHeap::extractMin() {
    if (size_ == 0) return 0;         // nullptr if empty

    HuffmanNode* minNode = data_[0];  // root is the smallest
    data_[0] = data_[size_ - 1];      // move last element to the root
    --size_;                          // decrement size

    if (size_ > 0) siftDown(0);       // restore heap property
    return minNode;                   // caller owns this pointer
}

HuffmanNode* MinHeap::top() const {
    if (size_ == 0) return 0;
    return data_[0];
}

void MinHeap::clear() {
    // nodes are not deleted, ownership is external (Huffman tree will clean up)
    // simply set size = 0
    size_ = 0;
}

void MinHeap::siftUp(int i) {
    // while node is smaller than parent, swap upward
    while (i > 0) {
        int p = parent(i);
        if (less(data_[i], data_[p])) {
            // swap pointers
            HuffmanNode* tmp = data_[i];
            data_[i] = data_[p];
            data_[p] = tmp;

            i = p; // continue from parent's index

        } else {
            break; // heap property satisfied
        }
    }
}

void MinHeap::siftDown(int i) {
    // swap down with smaller child until heap property holds
    while (true) {
        int l = left(i);
        int r = right(i);
        int smallest = i;

        if (l < size_ && less(data_[l], data_[smallest])) smallest = l;
        if (r < size_ && less(data_[r], data_[smallest])) smallest = r;

        if (smallest == i) break;

        // swap with smallest child
        HuffmanNode* tmp = data_[i];
        data_[i] = data_[smallest];
        data_[smallest] = tmp;

        i = smallest; // continue from child swapped with
    }
}