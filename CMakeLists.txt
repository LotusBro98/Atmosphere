cmake_minimum_required (VERSION 3.0)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

project(Atmosphere)

set(SOURCES
    #src/main.cpp
        src/player.cpp
   )

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)
pkg_check_modules(VLC REQUIRED libvlc)

include_directories(Atmosphere include ${GTK_INCLUDE_DIRS} ${VLC_INCLUDE_DIRS})

add_executable(Atmosphere ${SOURCES})

target_link_libraries(Atmosphere ${GTK_LIBRARIES} ${VLC_LIBRARIES})
