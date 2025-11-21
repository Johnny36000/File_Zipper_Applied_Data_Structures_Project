
#ifndef MILESTONE_2_ADS_MINIHEAP_H
#define MILESTONE_2_ADS_MINIHEAP_H

#include "HuffmanNode.h"

class MinHeap {
public:
    MinHeap();                                  // empty heap (starts small, grows)
    ~MinHeap();

    bool empty() const { return size_ == 0; }   // check if MinHeap is empty
    int size()  const { return size_; }         // getter for 'size_'

    void insert(HuffmanNode* node);
    HuffmanNode* extractMin();                  // mimics 'pop' (returns nullptr if empty)
    HuffmanNode* top() const;                   // mimics 'peek' (returns nullptr if empty)

    void clear();                               // drops elements (does not delete nodes)

private:
    HuffmanNode** data_;  // dynamic array of pointers
    int size_;            // number of elements in heap ( '_' to indicate private member)
    int capacity_;        // allocated slots

    // index helpers (returning the index of the respective node)
    int parent(int i) { return (i - 1) / 2; }
    int left(int i)   { return 2 * i + 1; }
    int right(int i)  { return 2 * i + 2; }

    bool less(HuffmanNode* a, HuffmanNode* b) { return a->frequency < b->frequency; } // checks if two nodes satisfy MinHeap conditions

    void siftUp(int i);    // moves newly inserted node up
    void siftDown(int i);  // moves root replacement down (after pop)
    void ensureCapacity(); // capacity management (allocate a larger array by doubling, if needed)
};



#endif //MILESTONE_2_ADS_MINIHEAP_H