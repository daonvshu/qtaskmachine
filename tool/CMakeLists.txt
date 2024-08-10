cmake_minimum_required(VERSION 3.12)

project(qtask_machines_tool)

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

add_executable(${PROJECT_NAME} WIN32
    main.cpp

    app.h
    app.cpp
    app.m.cpp

    res/res.qrc

    flowchart/flowchartitemdata.h
    flowchart/flowchartitemdata.cpp
    flowchart/flowchartwidget.h
    flowchart/flowchartwidget.cpp

    flowchart/elements/fcbranchitem.h
    flowchart/elements/fcbranchitem.cpp
    flowchart/elements/fcconnectline.h
    flowchart/elements/fcconnectline.cpp
    flowchart/elements/fcexecutoritem.h
    flowchart/elements/fcexecutoritem.cpp
    flowchart/elements/fciconitem.h
    flowchart/elements/fciconitem.cpp
    flowchart/elements/flowchartexecutoritem.h
    flowchart/elements/flowchartexecutoritem.cpp

    widgets/flowlistitemdelegate.h
    widgets/flowlistitemdelegate.cpp
    widgets/statetypelistview.h
    widgets/statetypelistview.cpp

    subpage/commonpropmanager.h
    subpage/commonpropmanager.cpp
    subpage/configdelaystate.h
    subpage/configdelaystate.cpp
    subpage/configeventstate.h
    subpage/configeventstate.cpp
    subpage/configmultieventstate.h
    subpage/configmultieventstate.cpp

    utils/curvemovecontrol.h
    utils/curvemovecontrol.cpp
    utils/xmath.h
    utils/xmath.cpp
)

target_link_libraries(${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
)