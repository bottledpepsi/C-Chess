cmake_minimum_required(VERSION 3.31)

project(C_Chess)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(C_Chess
        main.cpp
)

target_link_libraries(C_Chess PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
)