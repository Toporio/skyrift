cmake_minimum_required(VERSION 3.28)
project(skyrift LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

set( PROJECT_SOURCES
    src/main.cpp
        src/Game.cpp
        src/Stage.cpp
        src/Player.cpp
        src/Projectile.cpp
        src/Tile.cpp
        src/Entity.cpp
        src/Server.cpp
        src/Client.cpp
)

add_executable(skyrift ${PROJECT_SOURCES})
target_include_directories(skyrift PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_features(skyrift PRIVATE cxx_std_17)
target_link_libraries(skyrift PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Network
)
