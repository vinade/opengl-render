file(GLOB DRONE "src/main.cpp" "src/drone_state.cpp" "src/geometry.cpp")

add_executable(drone ${DRONE})
target_link_libraries(drone PRIVATE stb imgui engine)
target_include_directories(drone PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
