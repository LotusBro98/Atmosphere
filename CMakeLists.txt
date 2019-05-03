cmake_minimum_required (VERSION 3.0)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

project(Atmosphere)

set(SOURCES
    src/main.cpp
    )

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)

include_directories(Atmosphere ${GTK_INCLUDE_DIRS})

add_executable(Atmosphere ${SOURCES})

target_link_libraries(Atmosphere ${GTK_LIBRARIES})
