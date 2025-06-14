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
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets Network REQUIRED)

include(CPM.cmake)

CPMAddPackage("gh:daonvshu/qlogcollector@1.0.5")
CPMAddPackage("gh:daonvshu/QtDao@2.2.3")
CPMAddPackage("gh:daonvshu/ProtocolCodecEngine@0.0.7")
CPMAddPackage("gh:stdware/qwindowkit#1.2")

add_compile_definitions(ROOT_PROJECT_PATH="${CMAKE_SOURCE_DIR}")

include(db/entity.cmake)

add_executable(${PROJECT_NAME} WIN32
    main.cpp

    app.h
    app.cpp
    app.m.cpp
    app.qrc
    appsettings.h
    appsettings.cpp
    myapplication.h
    myapplication.cpp

    treeview/treeviewitemmodel.h
    treeview/treeviewitemmodel.cpp
    treeview/treeviewitemnode.h
    treeview/treeviewitemnode.cpp
    treeview/configflowtreeview.h
    treeview/configflowtreeview.cpp
    treeview/configflowtreeview.m.cpp
    treeview/styledmenu.h
    treeview/styledmenu.cpp

    dialogs/basepropertyeditdlg.h
    dialogs/basepropertyeditdlg.cpp
    dialogs/nameeditdlg.h
    dialogs/nameeditdlg.cpp
    dialogs/messagedlg.h
    dialogs/messagedlg.cpp

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
    dialogs/nodeedit/groupstatepropeditdlg.h
    dialogs/nodeedit/groupstatepropeditdlg.cpp
    dialogs/nodeedit/recoverystatepropeditdlg.h
    dialogs/nodeedit/recoverystatepropeditdlg.cpp
    dialogs/nodeedit/loopstatepropeditdlg.h
    dialogs/nodeedit/loopstatepropeditdlg.cpp

    graphic/graphicview.h
    graphic/graphicview.cpp
    graphic/graphiccontrol.h
    graphic/graphiclayer.h
    graphic/graphiclayer.cpp
    graphic/graphicobject.h
    graphic/graphicobject.cpp
    graphic/graphicrender.h
    graphic/graphicrender.cpp

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
    graphic/layer/commonnodelayer.h
    graphic/layer/commonnodelayer.cpp
    graphic/layer/multiselectlayer.h
    graphic/layer/multiselectlayer.cpp

    graphic/objects/graphicnode.h
    graphic/objects/graphicnode.cpp
    graphic/objects/graphiclinkline.h
    graphic/objects/graphiclinkline.cpp

    graphic/objects/actions/nodemoveaction.h
    graphic/objects/actions/nodemoveaction.cpp
    graphic/objects/actions/objectremoveaction.h
    graphic/objects/actions/objectremoveaction.cpp
    graphic/objects/actions/objectcopyaction.h
    graphic/objects/actions/objectcopyaction.cpp

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
    graphic/objects/nodes/nodeloopstate.h
    graphic/objects/nodes/nodeloopstate.cpp

    graphic/utils/transform.h
    graphic/utils/transform.cpp
    graphic/utils/bezierdistancecalculator.h
    graphic/utils/bezierdistancecalculator.cpp

    db/viewcentermanager.h
    db/viewcentermanager.cpp

    remote/monitordlg.h
    remote/monitordlg.cpp
    remote/backtrackdlg.h
    remote/backtrackdlg.cpp
    remote/remotecontrol.h
    remote/remotecontrol.cpp

    ${ENTITY_FILE_LIST}
)

target_link_libraries(${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network

    qlogcollectorserver
    qtdao
    protocol_codec_engine
    QWindowKit::Widgets
)