
#ifndef MILESTONE_2_ADS_DYNAMICARRAY_H
#define MILESTONE_2_ADS_DYNAMICARRAY_H

#include <cstddef>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <cassert>

template <typename T>
class DynamicArray
{
private:
    T *array;        // pointer to elements
    size_t capacity; // maximum number of elements the array can hold
    size_t size;     // current number of elements
    // used size_t instead of int to prevent surprises that might happen
    void resize();

public:
    // Constructor and Destructor
    DynamicArray(size_t cap = 10);
    ~DynamicArray();

    // Core operations
    void pushBack(const T &element);
    void popBack();
    T &operator[](size_t index);
    const T &operator[](size_t index) const;
    T &at(size_t index);
    const T &at(size_t index) const;

    // Utility functions
    size_t getSize() const;
    size_t getCapacity() const;
    bool isEmpty() const;
    void clear();
    T *getData();
    const T *getData() const;
    void reserve(size_t newCapacity);
    void shrinkToFit();

    // Iterator support
    T *begin();
    T *end();
    const T *begin() const;
    const T *end() const;

    DynamicArray(const DynamicArray<T> &other);                // Copy constructor
    DynamicArray &operator=(const DynamicArray<T> &other);     // Copy assignment
    DynamicArray(DynamicArray<T> &&other) noexcept;            // Move constructor
    DynamicArray &operator=(DynamicArray<T> &&other) noexcept; // Move assignment
};


#endif //MILESTONE_2_ADS_DYNAMICARRAY_H