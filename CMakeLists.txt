cmake_minimum_required(VERSION 3.2)

project(BoskeopolisWorld)
include(engine/engine.cmake)
add_executable(BoskeopolisWorld
    block_behavior.cpp
    block_component.cpp
    block_condition.cpp
    block_type.cpp
    dir.cpp
    game.cpp
    input.cpp
    json.cpp
    layer.cpp
    map.cpp
    main.cpp
    renderer.cpp
    sprite_system.cpp
    tileset.cpp
    utility.cpp
    ${engine_sources}
)
set_property(TARGET BoskeopolisWorld PROPERTY CXX_STANDARD 23)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
include_directories(include include/nasringine)
include_directories(${OPENGL_INCLUDE_DIRS})
target_link_libraries(BoskeopolisWorld ${OPENGL_LIBRARIES} glfw)
file(COPY ${CMAKE_CURRENT_LIST_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb")