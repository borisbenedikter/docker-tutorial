cmake_minimum_required(VERSION 3.0)

# Project name
project(random_matrix_generator)

# Find the Boost libraries
find_package(Boost REQUIRED)

# Find the Eigen library
find_package(Eigen3 REQUIRED)

# Source files
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SOURCES
    ${SRC_DIR}/main.cpp
)

# Add the executable
set(EXE_NAME "random_matrix_generator")
add_executable(${EXE_NAME} ${SOURCES})

# Link against the libraries (this will also include the include directories)
target_link_libraries(${EXE_NAME} Boost::boost)
target_link_libraries(${EXE_NAME} Eigen3::Eigen)

# Executable output directory
set_target_properties(${EXE_NAME} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
