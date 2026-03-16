cmake_minimum_required(VERSION 3.28)
project(NEAT-PENDULUM LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_COMPILER=g++)
set(CMAKE_BUILD_TYPE Release)

find_package(Threads REQUIRED)

# Release
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

set(NEAT_SOURCES src/neat/neat.cpp src/neat/topological_sort.cpp)
set(CART_POLE_SOURCES src/cart_pole/cart_pole.cpp)
set(EVENTS_SOURCES src/events/events.cpp)
set(DISPLAY_SOURCES src/display/display.cpp)

add_library(neat STATIC ${NEAT_SOURCES})
target_include_directories(neat PRIVATE src/neat src/)

add_library(cart_pole STATIC ${CART_POLE_SOURCES})
target_include_directories(cart_pole PRIVATE src/cart_pole src/)
target_link_libraries(cart_pole PRIVATE SFML::Graphics)

add_library(events STATIC ${EVENTS_SOURCES})
target_include_directories(events PRIVATE src/cart_pole src/)
target_link_libraries(events PRIVATE SFML::Graphics)

add_library(display STATIC ${DISPLAY_SOURCES})
# target_include_directories(display PRIVATE src/cart_pole src/)
target_link_libraries(display PRIVATE SFML::Graphics)

set(MY_EXECUTABLE_SOURCES src/main.cpp)
add_executable(main ${MY_EXECUTABLE_SOURCES})
target_include_directories(main PRIVATE src/cart_pole src/neat src/events src/neat src/display src/)
target_link_libraries(main PRIVATE SFML::Graphics cart_pole events neat display Threads::Threads)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-fsanitize=undefined -g)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-Wall -Wextra -Wshadow -Wnon-virtual-dtor -fsanitize=undefined -pedantic -DNDEBUG -O3 -march=native)
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)
