include_directories(include)

file(GLOB DEPTH "src/*.cpp")
add_executable(depth-sampler ${DEPTH})
target_link_libraries(depth-sampler PUBLIC stb imgui engine)
target_include_directories(depth-sampler PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
