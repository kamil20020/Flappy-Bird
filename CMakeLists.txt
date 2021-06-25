cmake_minimum_required(VERSION 3.1)
project (box)

set(CMAKE_CXX_STANDARD 14)

if (UNIX)

    set ( CMAKE_MODULES_PATH "/usr/share/SFML/cmake/Modules" ${CMAKE_MODULE_PATH})

endif(UNIX)

if(WIN32)

    set( SFML_STATIC_LIBRARIES TRUE )

    set ( SFML_DIR "./sfml/lib/cmake/SFML")
endif(WIN32)

find_package(SFML 2 COMPONENTS window graphics network audio REQUIRED)

add_executable(box main.cpp)

target_link_libraries(box sfml-window sfml-graphics sfml-network sfml-audio)