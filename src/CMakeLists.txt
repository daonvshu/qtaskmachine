cmake_minimum_required(VERSION 3.12)

project(task_machines_lib)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Core REQUIRED)

add_library(${PROJECT_NAME}

    linearstate.h
    linearstate.cpp

    targetsignaltransition.h
    targetsignaltransition.cpp

    delaystate.h
    delaystate.cpp

    groupstate.h
    groupstate.cpp

    directstate.h
    directstate.cpp

    staterecovery.h
    staterecovery.cpp

    conditionaltransition.h
    )

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    )

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})