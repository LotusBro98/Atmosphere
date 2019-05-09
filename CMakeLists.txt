cmake_minimum_required (VERSION 3.0)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

project(AtmosphereClient)

set(CLIENT_SOURCES
        src/client/Client.cpp)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)
pkg_check_modules(VLC REQUIRED libvlc)

include_directories(AtmosphereClient include ${GTK_INCLUDE_DIRS} ${VLC_INCLUDE_DIRS})

add_executable(AtmosphereClient ${CLIENT_SOURCES})

target_link_libraries(AtmosphereClient ${GTK_LIBRARIES} ${VLC_LIBRARIES})




project(AtmosphereServer)

set(SERVER_SOURCES
        src/server/Server.cpp src/server/Room.cpp src/server/include/Room.h src/server/Movie.cpp src/server/include/Movie.h src/server/User.cpp src/server/include/User.h src/server/load_data_from_db.cpp)

find_package(PkgConfig REQUIRED)
pkg_check_modules(VLC REQUIRED libvlc)
pkg_check_modules(SQLITE REQUIRED sqlite3)

include_directories(AtmosphereServer include ${SQLITE_INCLUDE_DIRS} ${VLC_INCLUDE_DIRS})

add_executable(AtmosphereServer ${SERVER_SOURCES})

target_link_libraries(AtmosphereServer ${VLC_LIBRARIES} ${SQLITE_LIBRARIES})
