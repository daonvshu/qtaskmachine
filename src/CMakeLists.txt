cmake_minimum_required(VERSION 3.12)

project(task_machines_lib)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Core REQUIRED)

if (WIN32)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

add_library(${PROJECT_NAME}

    linearstate.h
    linearstate.cpp
    targetsignaltransition.h
    targetsignaltransition.cpp
    conditionaltransition.h

    state/directstate.h
    state/directstate.cpp
    state/delaystate.h
    state/delaystate.cpp
    state/eventstate.h
    state/eventstate.cpp
    state/groupstate.h
    state/groupstate.cpp
    )

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    )

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})