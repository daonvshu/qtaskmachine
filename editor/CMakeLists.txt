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
    graphic/graphicobject.h
    graphic/graphicobject.cpp

    graphic/controls/mouseactioncontrol.h
    graphic/controls/mouseactioncontrol.cpp
    graphic/controls/transformcontrol.h
    graphic/controls/transformcontrol.cpp
    graphic/controls/graphiclayercontrol.h
    graphic/controls/graphiclayercontrol.cpp
    graphic/controls/graphicobjcreatecontrol.h
    graphic/controls/graphicobjcreatecontrol.cpp

    graphic/layer/orthogonalgridlayer.h
    graphic/layer/orthogonalgridlayer.cpp
    graphic/layer/activenodelayer.h
    graphic/layer/activenodelayer.cpp
    graphic/layer/activelinklinelayer.h
    graphic/layer/activelinklinelayer.cpp
    graphic/layer/staticnodelayer.h
    graphic/layer/staticnodelayer.cpp
    graphic/layer/staticlinklinelayer.h
    graphic/layer/staticlinklinelayer.cpp

    graphic/objects/graphicnode.h
    graphic/objects/graphicnode.cpp
    graphic/objects/graphiclinkline.h
    graphic/objects/graphiclinkline.cpp

    graphic/objects/nodes/nodebeginstate.h
    graphic/objects/nodes/nodebeginstate.cpp
    graphic/objects/nodes/nodeendstate.h
    graphic/objects/nodes/nodeendstate.cpp
    graphic/objects/nodes/nodenormalstate.h
    graphic/objects/nodes/nodenormalstate.cpp
    graphic/objects/nodes/nodedelaystate.h
    graphic/objects/nodes/nodedelaystate.cpp
    graphic/objects/nodes/nodeeventstate.h
    graphic/objects/nodes/nodeeventstate.cpp
    graphic/objects/nodes/nodemultieventstate.h
    graphic/objects/nodes/nodemultieventstate.cpp
    graphic/objects/nodes/nodeconditionstate.h
    graphic/objects/nodes/nodeconditionstate.cpp
    graphic/objects/nodes/nodestategroup.h
    graphic/objects/nodes/nodestategroup.cpp
    graphic/objects/nodes/noderecoverystate.h
    graphic/objects/nodes/noderecoverystate.cpp

    graphic/render/graphicrender.h
    graphic/render/graphicrender.cpp
    graphic/render/nodes/nodenormalstaterender.h
    graphic/render/nodes/nodenormalstaterender.cpp

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