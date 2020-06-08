include_directories(include)

file(GLOB DEPTH_DEP "src/depth_sampler.cpp"  "src/shuffler.cpp")
add_executable(depth-sampler-debug ${DEPTH_DEP} "src/main.cpp")
target_link_libraries(depth-sampler-debug PUBLIC stb imgui engine)
target_include_directories(depth-sampler-debug PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(depth-sampler SHARED ${DEPTH_DEP} "src/depth_interface.cpp")
target_link_libraries(depth-sampler stb imgui engine)
target_include_directories(depth-sampler PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
