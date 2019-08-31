    
cmake_minimum_required(VERSION 3.0)
project(raytracer)

set(CMAKE_CXX_STANDARD_REQUIRED 17)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall")
elseif(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()

file(GLOB_RECURSE SRCS "src/*.h" "src/*.cpp")
add_executable(raytracer ${SRCS})