cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_PREFIX_PATH "E:/qt/6.10.1/mingw_64")
project(Practice)
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
add_executable(Practice
    main.cpp
    Widget.h
    Widget.cpp
    ShotWidget.h
    ShotWidget.cpp
)
target_link_libraries(Practice Qt6::Core Qt6::Gui Qt6::Widgets)

