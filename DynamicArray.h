#ifndef FILE_ZIPPER_APPLIED_DATA_STRUCTURES_PROJECT_DYNAMICARRAY_H
#define FILE_ZIPPER_APPLIED_DATA_STRUCTURES_PROJECT_DYNAMICARRAY_H

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

// Method Implementations
template <typename T>
DynamicArray<T>::DynamicArray(size_t cap)
{
    capacity = (cap > 0) ? cap : 1; // check if the capacity is a valid positive number
    size = 0;
    array = new T[capacity];
}

template <typename T>
DynamicArray<T>::~DynamicArray()
{
    delete[] array;
}

template <typename T>
void DynamicArray<T>::resize()
{
    size_t newCap = (capacity == 0) ? 1 : capacity * 2; // scale factor is subject to change as doubling is not very effective for large capacities
    reserve(newCap);
}

template <typename T>
void DynamicArray<T>::reserve(size_t newCapacity) // A function for adjusting the capacity
{
    if (newCapacity <= capacity)
        return;
    T *newArray = new T[newCapacity];
    for (size_t i = 0; i < size; ++i)
        newArray[i] = array[i];
    delete[] array;
    array = newArray;
    capacity = newCapacity;
}

template <typename T>
void DynamicArray<T>::shrinkToFit()
{
    if (size == capacity)
        return;
    size_t newCap = (size > 0) ? size : 1; // shrinking the capacity to free memory
    T *newArray = new T[newCap];
    for (size_t i = 0; i < size; ++i)
        newArray[i] = array[i];
    delete[] array;
    array = newArray;
    capacity = newCap;
}

template <typename T>
void DynamicArray<T>::pushBack(const T &element)
{
    if (size == capacity) // to avoid overflow
        resize();
    array[size++] = element;
}

template <typename T>
void DynamicArray<T>::popBack()
{
    if (isEmpty())
        throw std::out_of_range("DynamicArray::popBack: array is empty");
    --size; // will throw an error if empty
    if (size > 0)
        --size;
}

template <typename T>
T &DynamicArray<T>::operator[](size_t index)
{
    assert(index < size);
    return array[index];
}

template <typename T>
const T &DynamicArray<T>::operator[](size_t index) const
{
    assert(index < size);
    return array[index];
}

template <typename T>
T &DynamicArray<T>::at(size_t index)
{
    if (index >= size)
        throw std::out_of_range("DynamicArray::at: index out of range");
    return array[index];
}

template <typename T>
const T &DynamicArray<T>::at(size_t index) const
{
    if (index >= size)
        throw std::out_of_range("DynamicArray::at: index out of range");
    return array[index];
}

template <typename T>
size_t DynamicArray<T>::getSize() const
{
    return size;
}

template <typename T>
size_t DynamicArray<T>::getCapacity() const
{
    return capacity;
}

template <typename T>
bool DynamicArray<T>::isEmpty() const
{
    return size == 0;
}

template <typename T>
void DynamicArray<T>::clear()
{
    size = 0;
}

template <typename T>
T *DynamicArray<T>::getData()
{
    return array; // return pointer to elements
}

template <typename T>
const T *DynamicArray<T>::getData() const
{
    return array;
}

template <typename T>
T *DynamicArray<T>::begin()
{
    return array; // return pointer to the first element
}

template <typename T>
T *DynamicArray<T>::end()
{
    return array + size; // return pointer to the last element
}

template <typename T>
const T *DynamicArray<T>::begin() const
{
    return array; // const method to get the first element
}

template <typename T>
const T *DynamicArray<T>::end() const
{
    return array + size; // const method to get the last element
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T> &other) // A function to copy another array
{
    capacity = other.capacity;
    size = other.size;
    array = new T[capacity];
    for (size_t i = 0; i < size; ++i)
        array[i] = other.array[i];
}

template <typename T>
DynamicArray<T> &DynamicArray<T>::operator=(const DynamicArray<T> &other) // A function to overload the = operator
{
    if (this == &other)
        return *this;
    T *newArray = new T[other.capacity];
    for (size_t i = 0; i < other.size; ++i)
        newArray[i] = other.array[i];
    delete[] array;
    array = newArray;
    capacity = other.capacity;
    size = other.size;
    return *this;
}

template <typename T>
DynamicArray<T>::DynamicArray(DynamicArray<T> &&other) noexcept
{
    array = other.array;
    capacity = other.capacity;
    size = other.size;
    other.array = nullptr;
    other.capacity = 0;
    other.size = 0;
}

template <typename T>
DynamicArray<T> &DynamicArray<T>::operator=(DynamicArray<T> &&other) noexcept
{
    if (this == &other)
        return *this;
    delete[] array;
    array = other.array;
    capacity = other.capacity;
    size = other.size;
    other.array = nullptr;
    other.capacity = 0;
    other.size = 0;
    return *this;
}

#endif // FILE_ZIPPER_APPLIED_DATA_STRUCTURES_PROJECT_DYNAMICARRAY_H