cmake_minimum_required(VERSION 3.31)

project(C_Chess)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        3.1.0
)
FetchContent_MakeAvailable(SFML)

add_executable(C_Chess
        main.cpp
)

target_link_libraries(C_Chess PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
)