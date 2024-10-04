cmake_minimum_required(VERSION 3.12)

project(TaskMachinesFlowEditor)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (WIN32)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)

include(CPM.cmake)

CPMAddPackage("gh:daonvshu/qlogcollector@1.0.5")
CPMAddPackage("gh:stdware/qwindowkit#1.2")

add_compile_definitions(ROOT_PROJECT_PATH="${CMAKE_SOURCE_DIR}")

add_executable(${PROJECT_NAME} WIN32
    main.cpp

    app.h
    app.cpp
    app.qrc
    myapplication.h
    myapplication.cpp

    graphic/graphicview.h
    graphic/graphicview.cpp
    graphic/graphiccontrol.h
    graphic/graphiclayer.h
    graphic/graphiclayer.cpp

    graphic/controls/mouseactioncontrol.h
    graphic/controls/mouseactioncontrol.cpp
    graphic/controls/transformcontrol.h
    graphic/controls/transformcontrol.cpp
    graphic/controls/graphiclayercontrol.h
    graphic/controls/graphiclayercontrol.cpp

    graphic/layer/orthogonalgridlayer.h
    graphic/layer/orthogonalgridlayer.cpp

    graphic/utils/transform.h
    graphic/utils/transform.cpp
)

target_link_libraries(${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets

    qlogcollectorserver
    QWindowKit::Widgets
)