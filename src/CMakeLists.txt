set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

add_executable(CMaze main.cpp
        direction.cpp
        direction.hpp
        random_queue.hpp
        maze.cpp
        maze.hpp
        vector_util.hpp
        chrono.cpp
        chrono.hpp
        image.cpp
        image.hpp
        user_interface.cpp
        user_interface.hpp
)

target_link_libraries(CMaze LodePNG Qt::Core Qt::Gui Qt::Widgets)
