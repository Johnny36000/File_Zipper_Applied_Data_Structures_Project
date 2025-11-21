cmake_minimum_required(VERSION 3.14)
project(Milestone_2_ADS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add Code_library subdirectory first
add_subdirectory(Code_library)

# Main executable (compression/decompression program)
add_executable(Milestone_2_ADS main.cpp)
target_link_libraries(Milestone_2_ADS PRIVATE Code_library)

# Add Google_tests subdirectory
add_subdirectory(Google_tests)
