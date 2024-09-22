cmake_minimum_required(VERSION 3.10)

project(stabby2d VERSION 0.5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_options(-Wall -Wfatal-errors -pedantic)

add_library(components STATIC include/Components/Position.hpp include/Components/RigidBodyComponent.hpp include/Components/Scale.hpp
        include/Components/SpriteComponent.hpp include/Components/TransformComponent.hpp include/Components/Velocity.hpp)
target_include_directories(components PUBLIC include/Components)
set_target_properties(components PROPERTIES LINKER_LANGUAGE CXX)

add_library(ecs STATIC include/ECS/ECS.hpp src/ECS/ECS.cpp)
target_include_directories(ecs PUBLIC include/ECS include/Logger)
set_target_properties(ecs PROPERTIES LINKER_LANGUAGE CXX)

add_library(game_state STATIC src/GameState/GameState.cpp include/GameState/GameState.hpp)
target_include_directories(game_state PUBLIC include/GameState include/Components include/System include/ECS include/AssetStore)
target_link_libraries(game_state PUBLIC ecs)
set_target_properties(game_state PROPERTIES LINKER_LANGUAGE CXX)

add_library(logger STATIC src/Logger/Logger.cpp include/Logger/Logger.hpp)
target_include_directories(logger PUBLIC include/Logger)
set_target_properties(logger PROPERTIES LINKER_LANGUAGE CXX)

add_library(asset_store STATIC include/AssetStore/AssetManager.hpp src/AssetManager/AssetManager.cpp)
target_include_directories(asset_store PUBLIC include/AssetStore include/Logger)
set_target_properties(asset_store PROPERTIES LINKER_LANGUAGE CXX)

add_library(system STATIC include/System/MovementSystem.hpp include/System/RenderSystem.hpp)
target_include_directories(system PUBLIC include/System include/AssetStore)
set_target_properties(system PROPERTIES LINKER_LANGUAGE CXX)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE -fsanitize=address)
target_link_libraries(${PROJECT_NAME} PUBLIC components ecs game_state logger asset_store system)

# SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_image)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})