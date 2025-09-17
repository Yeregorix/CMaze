add_executable(CMaze main.cpp
        string_util.hpp
        direction.cpp
        direction.hpp
        random_queue.hpp
        maze.cpp
        maze.hpp
        vector_util.hpp
        chrono.cpp
        chrono.hpp
        image.hpp
        image.cpp
)

target_link_libraries(CMaze LodePNG)
