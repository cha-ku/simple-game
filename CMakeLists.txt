cmake_minimum_required(VERSION 3.10)

project(stabby2d VERSION 0.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_options(-Wall -Wfatal-errors -pedantic)

file(GLOB components CONFIGURE_DEPENDS "src/Components/*")
file(GLOB ecs CONFIGURE_DEPENDS "src/ECS/*")
file(GLOB gamestate CONFIGURE_DEPENDS "src/GameState/*")
file(GLOB logger CONFIGURE_DEPENDS "src/Logger/*")
file(GLOB asset_store CONFIGURE_DEPENDS "src/AssetStore/*")
file(GLOB system CONFIGURE_DEPENDS "src/System/*")

add_executable(${PROJECT_NAME} ${components} ${ecs} ${gamestate} ${logger} ${system} ${asset_store} src/main.cpp)


# SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_image)

target_link_libraries(${PROJECT_NAME} PRIVATE -fsanitize=address)

set_target_properties(stabby2d PROPERTIES LINKER_LANGUAGE CXX)
