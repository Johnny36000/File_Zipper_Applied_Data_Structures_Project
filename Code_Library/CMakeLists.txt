# Create library from all your data structure files
add_library(Code_library
        BitStream.cpp
        BitStream.h
        DynamicArray.cpp
        DynamicArray.h
        HashMap.cpp
        HashMap.h
        HuffmanNode.cpp
        HuffmanNode.h
        HuffmanZipper.cpp
        HuffmanZipper.h
        MiniHeap.cpp
        MiniHeap.h
)

# Make headers accessible to this library and all targets that link to it
target_include_directories(Code_library PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

# Set C++ standard for the library
set_target_properties(Code_library PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)
