cmake_minimum_required(VERSION 3.12)

project(qtaskmachine)

option(QTASK_MACHINE_BUILD_TEST "enable qtaskmachine test projects" OFF)

add_subdirectory(src)

if (QTASK_MACHINE_BUILD_TEST)
    add_subdirectory(test)
endif ()