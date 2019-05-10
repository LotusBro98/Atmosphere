cmake_minimum_required (VERSION 3.0)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

project(AtmosphereServer)

set(SERVER_SOURCES
        src/Server.cpp src/Room.cpp src/Movie.cpp src/User.cpp src/load_data_from_db.cpp src/tcp_socket.cpp ../src/Message.cpp)

find_package(PkgConfig REQUIRED)
pkg_check_modules(VLC REQUIRED libvlc)
pkg_check_modules(SQLITE REQUIRED sqlite3)

include_directories(AtmosphereServer include ${SQLITE_INCLUDE_DIRS} ${VLC_INCLUDE_DIRS})

add_executable(AtmosphereServer ${SERVER_SOURCES})

target_link_libraries(AtmosphereServer ${VLC_LIBRARIES} ${SQLITE_LIBRARIES} pthread)
