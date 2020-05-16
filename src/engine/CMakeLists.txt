
include_directories(include ../vendor)

file(GLOB ENGINE "src/*.cpp")
set(ROOT_DIR "\"${CMAKE_CURRENT_LIST_DIR}/../../\"" )

add_definitions(-DCMAKE_ROOT_DIR=${ROOT_DIR})

add_library(engine ${ENGINE})
target_link_libraries(engine imgui stb pthread GL GLEW glfw assimp)

target_include_directories(engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
