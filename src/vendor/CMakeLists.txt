
include_directories(imgui stb)

file(GLOB STB "stb/*.cpp")
file(GLOB IMGUI "imgui/*.cpp")

add_library(stb ${STB})
set_property(TARGET stb PROPERTY POSITION_INDEPENDENT_CODE ON)

add_library(imgui ${IMGUI})
set_property(TARGET imgui PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(stb PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/stb")
target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/imgui")
