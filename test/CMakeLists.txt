cmake_minimum_required(VERSION 3.12)

project(taskmachine-test)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (WIN32)
    #编译使用utf8文件
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/modules)

add_executable(${PROJECT_NAME} WIN32
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets

    qtaskmachine_lib
)