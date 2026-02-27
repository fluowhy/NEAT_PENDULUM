cmake_minimum_required(VERSION 3.28)
project(NEAT-PENDULUM LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD=23)
set(CMAKE_CXX_COMPILER=g++)
set(CMAKE_BUILD_TYPE=Debug)
# set(CMAKE_CXX_FLAGS(_DEBUG/_RELEASE)=-march=native)
# release

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

set(MY_LIBRARY_SOURCES src/neat/neat.cpp src/neat/topological_sort.cpp)
add_library(neat STATIC ${MY_LIBRARY_SOURCES})

set(MY_EXECUTABLE_SOURCES src/main.cpp src/events.cpp)
add_executable(main ${MY_EXECUTABLE_SOURCES})
target_compile_features(main PRIVATE cxx_std_23)
# add_compile_options(-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -DNDEBUG -O3)
add_compile_options(-fsanitize=undefined)
target_link_libraries(main PRIVATE SFML::Graphics)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)
