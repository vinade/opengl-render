cmake_minimum_required(VERSION 3.16)

project(engine VERSION 0.0.1)

include_directories(include ../vendor)

file(GLOB SOURCES "src/*.cpp")
file(GLOB IMGUI "../vendor/imgui/*.cpp")
file(GLOB STB "../vendor/stb/*.cpp")

set(ROOT_DIR "\"${CMAKE_CURRENT_LIST_DIR}/../\"" )

add_definitions(-DCMAKE_ROOT_DIR=${ROOT_DIR})
add_executable(engine ${IMGUI} ${STB} ${SOURCES})
target_link_libraries(engine GL GLEW glfw assimp)
