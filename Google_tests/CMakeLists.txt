# Find or fetch Google Test
find_package(GTest QUIET)

if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.12.1
    )
    FetchContent_MakeAvailable(googletest)
endif()

# Create test executable
add_executable(HuffmanZipperTests HuffmanZipperTest.cpp)

# Link with Code_library and Google Test
target_link_libraries(HuffmanZipperTests PRIVATE
        Code_library
        GTest::gtest
        GTest::gtest_main
)

# Explicitly include Code_library headers
target_include_directories(HuffmanZipperTests PRIVATE
        ${CMAKE_SOURCE_DIR}/Code_library
)

# Set C++ standard for tests
target_compile_features(HuffmanZipperTests PRIVATE cxx_std_20)

# Enable testing
enable_testing()
add_test(NAME HuffmanZipperTests COMMAND HuffmanZipperTests)
