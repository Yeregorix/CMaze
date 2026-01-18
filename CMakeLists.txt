cmake_minimum_required(VERSION 3.31)
project(CMaze)

set(CMAKE_CXX_STANDARD 23)

# -mwindows: hide console
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mwindows")

add_subdirectory(lib)
add_subdirectory(src)
