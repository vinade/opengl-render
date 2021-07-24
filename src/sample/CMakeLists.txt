file(GLOB ENGINE_SAMPLE "src/main.cpp" "src/state.cpp")

add_executable(engine-sample ${ENGINE_SAMPLE})
target_link_libraries(engine-sample PRIVATE stb imgui engine)
target_include_directories(engine-sample PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
