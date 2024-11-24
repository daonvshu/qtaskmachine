cmake_minimum_required(VERSION 3.12)

project(qtask_machines VERSION 1.0.4)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

if (Qt6_FOUND)
    find_package(Qt6 REQUIRED COMPONENTS StateMachine)
endif ()

if (WIN32)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

set(CMAKE_DEBUG_POSTFIX "d")

add_library(${PROJECT_NAME}

    linearstate.h
    linearstate.cpp
    targetsignaltransition.h
    targetsignaltransition.cpp
    conditionaltransition.h

    stateutils/taskmachinerunner.h
    stateutils/taskmachinerunner.cpp
    stateutils/taskmachinesteputil.h
    stateutils/taskmachinesteputil.cpp

    state/directstate.h
    state/directstate.cpp
    state/delaystate.h
    state/delaystate.cpp
    state/eventstate.h
    state/eventstate.cpp
    state/groupstate.h
    state/groupstate.cpp
    state/multieventstate.h
    state/multieventstate.cpp
    )

target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Core)

if (Qt6_FOUND)
    target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::StateMachine)
endif ()

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

include(CMakePackageConfigHelpers)

install(TARGETS ${PROJECT_NAME}
    EXPORT QTaskMachinesTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/QTaskMachinesConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/QTaskMachinesConfig.cmake"
    INSTALL_DESTINATION lib/cmake/QTaskMachines
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/QTaskMachinesConfig.cmake"
    DESTINATION lib/cmake/QTaskMachines
)

install(EXPORT QTaskMachinesTargets
    FILE QTaskMachinesTargets.cmake
    DESTINATION lib/cmake/QTaskMachines
)