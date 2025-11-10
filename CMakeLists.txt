cmake_minimum_required(VERSION 3.20)
project(File_Zipper_Applied_Data_Structures_Project)

set(CMAKE_CXX_STANDARD 20)

include_directories(Code_Library)

add_executable(File_Zipper_Applied_Data_Structures_Project
        Code_Library/HuffmanNode.h
        Code_Library/HuffmanNode.cpp
        Code_Library/MinHeap.h
        Code_Library/MinHeap.cpp
        Code_Library/DynamicArray.h
        Code_Library/HashMap.h
        Code_Library/BitStream.h
        Code_Library/BitStream.cpp
        Code_Library/main.cpp
)
