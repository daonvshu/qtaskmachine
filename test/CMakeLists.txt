cmake_minimum_required(VERSION 3.12)

project(taskmachine-test)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (WIN32)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_compile_definitions(STATE_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/states")
add_compile_definitions(FEATURES_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/features")
add_compile_definitions(SCENE_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/scene")

add_executable(${PROJECT_NAME} WIN32
    main.cpp

    demos.h
    demos.cpp

    highlighter.h
    highlighter.cpp

    states/directstatetest.h
    states/directstatetest.cpp
    states/delaystatetest.h
    states/delaystatetest.cpp
    states/eventstaterun.h
    states/eventstatetest.h
    states/eventstatetest.cpp

    features/groupstaterun2.h
    features/groupstatetest.h
    features/groupstatetest.cpp
    features/conditionalstatetest.h
    features/conditionalstatetest.cpp
    features/recoverstaterun.h
    features/recoverstatetest.h
    features/recoverstatetest.cpp
    features/debugstatetest.h
    features/debugstatetest.cpp
    features/targetlessstatetest.h
    features/targetlessstaterun.h
    features/targetlessstatetest.cpp

    scene/countdownrun.h
    scene/countdowntest.h
    scene/countdowntest.cpp
    scene/signalpsceventtrigger.h
    scene/signalparamschecktest.h
    scene/signalparamschecktest.cpp
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets

    task_machines_lib
)