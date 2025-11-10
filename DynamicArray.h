#ifndef FILE_ZIPPER_APPLIED_DATA_STRUCTURES_PROJECT_DYNAMICARRAY_H
#define FILE_ZIPPER_APPLIED_DATA_STRUCTURES_PROJECT_DYNAMICARRAY_H

template <typename T>
class DynamicArray {
private:
    T* array;
    int capacity;
    int size;

    void resize();

public:
    // Constructor and Destructor
    DynamicArray(int cap = 10);
    ~DynamicArray();

    // Core operations
    void pushBack(T element);
    void popBack();
    T& operator[](int index);
    const T& operator[](int index) const;

    // Utility functions
    int getSize() const;
    int getCapacity() const;
    bool isEmpty() const;
    void clear();
    T* getData();
};

// Template implementation in header

/**
 * Constructor
 */
template <typename T>
DynamicArray<T>::DynamicArray(int cap) {
    capacity = cap;
    size = 0;
    array = new T[capacity];
}

/**
 * Destructor
 */
template <typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] array;
}

/**
 * Resize array when capacity is reached
 */
template <typename T>
void DynamicArray<T>::resize() {
    capacity *= 2;
    T* newArray = new T[capacity];

    for (int i = 0; i < size; i++) {
        newArray[i] = array[i];
    }

    delete[] array;
    array = newArray;
}

/**
 * Add element to end
 */
template <typename T>
void DynamicArray<T>::pushBack(T element) {
    if (size == capacity) {
        resize();
    }
    array[size++] = element;
}

/**
 * Remove element from end
 */
template <typename T>
void DynamicArray<T>::popBack() {
    if (size > 0) {
        size--;
    }
}

/**
 * Array access operator
 */
template <typename T>
T& DynamicArray<T>::operator[](int index) {
    return array[index];
}

/**
 * Const array access operator
 */
template <typename T>
const T& DynamicArray<T>::operator[](int index) const {
    return array[index];
}

/**
 * Get current size
 */
template <typename T>
int DynamicArray<T>::getSize() const {
    return size;
}

/**
 * Get capacity
 */
template <typename T>
int DynamicArray<T>::getCapacity() const {
    return capacity;
}

/**
 * Check if empty
 */
template <typename T>
bool DynamicArray<T>::isEmpty() const {
    return size == 0;
}

/**
 * Clear all elements
 */
template <typename T>
void DynamicArray<T>::clear() {
    size = 0;
}

/**
 * Get raw array pointer
 */
template <typename T>
T* DynamicArray<T>::getData() {
    return array;
}

#endif //FILE_ZIPPER_APPLIED_DATA_STRUCTURES_PROJECT_DYNAMICARRAY_H