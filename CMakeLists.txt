cmake_minimum_required (VERSION 3.0)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set ( CMAKE_CXX_STANDARD 11 )

add_subdirectory(server)

add_subdirectory(client)

project(player_test)

set(PLAYER_SOURCES
        src/test/main.cpp
        )

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)
pkg_check_modules(VLC REQUIRED libvlc)
pkg_check_modules(SQLITE REQUIRED sqlite3)

include_directories(player_test include ${SQLITE_INCLUDE_DIRS} ${GTK_INCLUDE_DIRS} ${VLC_INCLUDE_DIRS})

add_executable(player_test ${PLAYER_SOURCES})

target_link_libraries(player_test ${GTK_LIBRARIES} ${VLC_LIBRARIES} ${SQLITE_LIBRARIES} pthread)