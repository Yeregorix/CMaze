cmake_minimum_required(VERSION 3.31)
project(CMaze)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

add_executable(CMaze main.cpp
        string_util.hpp
        direction.cpp
        direction.hpp
        random_queue.hpp
        maze.cpp
        maze.hpp
        vector_util.hpp
        lodepng.cpp
        lodepng.h
        chrono.cpp
        chrono.hpp
        image.hpp
        image.cpp
)
