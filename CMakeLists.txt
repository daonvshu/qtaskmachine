cmake_minimum_required(VERSION 3.12)

project(qtaskmachine)

option(QTASK_MACHINE_BUILD_TEST "enable qtaskmachine test projects" OFF)
option(QTASK_MACHINE_BUILD_TOOL "enable generator tool project" OFF)

add_subdirectory(src)

if (QTASK_MACHINE_BUILD_TEST)
    add_subdirectory(test)
endif ()

if (QTASK_MACHINE_BUILD_TOOL)
    add_subdirectory(tool)
    add_subdirectory(editor)
endif ()