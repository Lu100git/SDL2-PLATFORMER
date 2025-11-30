cmake_minimum_required(VERSION 3.31.6)
project(cathedral-level)

# Find SDL2 and image library
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Gather all source files in src/
file(GLOB SOURCES "src/*.cpp")

# Add main.cpp to the list (if it's not in src/)
list(APPEND SOURCES "main.cpp")


# Add the include directory so headers are visible to all source files
include_directories(${PROJECT_SOURCE_DIR}/include)

# Create the executable
add_executable(main ${SOURCES})

# Link SDL2
target_link_libraries(main SDL2::SDL2 SDL2::SDL2main SDL2_image::SDL2_image SDL2_mixer::SDL2_mixer)

