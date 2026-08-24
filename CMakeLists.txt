cmake_minimum_required(VERSION 3.31)

project(C_Chess LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)

include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        3.1.0
)

FetchContent_MakeAvailable(SFML)

add_executable(C_Chess
        src/app/App.cpp
        src/render/BoardRenderer.cpp
        include/render/BoardRenderer.hpp
        src/app/AssetManager.cpp
        include/app/AssetManager.hpp
        src/input/InputHandler.cpp
        include/input/InputHandler.hpp
        include/app/WindowAspectRatio.hpp
        src/render/PieceRenderer.cpp
        include/render/PieceRenderer.hpp
        include/render/BoardConstants.hpp
)

if(WIN32)
    target_sources(C_Chess PRIVATE
            src/app/WindowAspectRatioWindows.cpp
    )

elseif(APPLE)
    enable_language(OBJCXX)

    target_sources(C_Chess PRIVATE
            src/app/WindowAspectRatioMacOS.mm
    )

elseif(UNIX)
    target_sources(C_Chess PRIVATE
            src/app/WindowAspectRatioLinux.cpp
    )
endif()

target_link_libraries(C_Chess PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
)

add_custom_command(
        TARGET C_Chess POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:C_Chess>/assets
)