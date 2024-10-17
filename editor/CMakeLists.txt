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

    dialogs/basepropertyeditdlg.h
    dialogs/basepropertyeditdlg.cpp

    dialogs/nodeedit/normalstatepropeditdlg.h
    dialogs/nodeedit/normalstatepropeditdlg.cpp
    dialogs/nodeedit/beginstatepropeditdlg.h
    dialogs/nodeedit/beginstatepropeditdlg.cpp
    dialogs/nodeedit/endstatepropeditdlg.h
    dialogs/nodeedit/endstatepropeditdlg.cpp
    dialogs/nodeedit/delaystatepropeditdlg.h
    dialogs/nodeedit/delaystatepropeditdlg.cpp
    dialogs/nodeedit/conditionstatepropeditdlg.h
    dialogs/nodeedit/conditionstatepropeditdlg.cpp
    dialogs/nodeedit/eventstatepropeditdlg.h
    dialogs/nodeedit/eventstatepropeditdlg.cpp
    dialogs/nodeedit/eventtriggerinputwidget.h
    dialogs/nodeedit/eventtriggerinputwidget.cpp
    dialogs/nodeedit/multieventstatepropeditdlg.h
    dialogs/nodeedit/multieventstatepropeditdlg.cpp

    widget/flowlayout.h
    widget/flowlayout.cpp

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
    graphic/render/graphiclinklinerender.h
    graphic/render/graphiclinklinerender.cpp
    graphic/render/commonnodestaterender.h
    graphic/render/commonnodestaterender.cpp
    graphic/render/nodes/nodenormalstaterender.h
    graphic/render/nodes/nodenormalstaterender.cpp
    graphic/render/nodes/nodebeginstaterender.h
    graphic/render/nodes/nodebeginstaterender.cpp
    graphic/render/nodes/nodeendstaterender.h
    graphic/render/nodes/nodeendstaterender.cpp
    graphic/render/nodes/nodedelaystaterender.h
    graphic/render/nodes/nodedelaystaterender.cpp
    graphic/render/nodes/nodeconditionstaterender.h
    graphic/render/nodes/nodeconditionstaterender.cpp
    graphic/render/nodes/nodeeventstaterender.h
    graphic/render/nodes/nodeeventstaterender.cpp
    graphic/render/nodes/nodemultieventstaterender.h
    graphic/render/nodes/nodemultieventstaterender.cpp

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