cmake_minimum_required (VERSION 3.0)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

project(AtmosphereClient)

set(CLIENT_SOURCES
        src/Client.cpp
        src/Server.cpp
        src/Room.cpp
        ../src/Message.cpp
        src/main.cpp
        src/Player.cpp
        src/msg_handlers.cpp src/Player_GUI.cpp src/Movie.cpp include/Movie.h)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)
pkg_check_modules(VLC REQUIRED libvlc)
pkg_check_modules(SQLITE REQUIRED sqlite3)

include_directories(AtmosphereClient include ${SQLITE_INCLUDE_DIRS} ${GTK_INCLUDE_DIRS} ${VLC_INCLUDE_DIRS})

add_executable(AtmosphereClient ${CLIENT_SOURCES})

target_link_libraries(AtmosphereClient ${GTK_LIBRARIES} ${VLC_LIBRARIES} ${SQLITE_LIBRARIES} pthread)