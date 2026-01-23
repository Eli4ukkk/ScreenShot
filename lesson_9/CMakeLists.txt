cmake_minimum_required(VERSION 3.16)

# 项目名称
project(learn_qt_class LANGUAGES CXX)

set(CMAKE_AUTOMOC ON) # 自动处理 Q_OBJECT
set(CMAKE_AUTORCC ON) # 自动处理资源文件
set(CMAKE_AUTOUIC ON) # 自动处理 UI 文件

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 指定你的 Qt 路径
set(CMAKE_PREFIX_PATH "E:/qt/6.10.1/mingw_64")

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Core)

# 【关键点】这里要写 main.cpp (或者你实际的文件名)
# WIN32 让你运行时不弹黑框，想看 qDebug 输出可以去掉 WIN32
add_executable(learn_qt_class WIN32 
    main.cpp
    Widget.h
    Widget.cpp
)

target_link_libraries(learn_qt_class PRIVATE Qt6::Widgets Qt6::Gui Qt6::Core)