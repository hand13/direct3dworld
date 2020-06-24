cmake_minimum_required(VERSION 3.15)
project(World)
set(CMAKE_CXX_STANDARD 17)
include_directories("e:\\usr\\deb\\FMT\\include" "e:\\usr\\include" "e:\\usr\\include\\DXH\\Inc")
link_directories("e:\\usr\\deb\\FMT\\lib" "e:\\usr\\lib" "e:\\usr\\DXH\\Inc")
add_executable(godlike godlike.cpp image.cpp base_direct.cpp im_direct.cpp screenutils.cpp)
add_executable(test test.cpp screenutils.cpp)
add_library(imgui STATIC imgui.cpp imgui_demo.cpp imgui_draw.cpp imgui_impl_dx11.cpp
  imgui_impl_win32.cpp imgui_widgets.cpp)
target_link_libraries(imgui d3d11 d3dcompiler)
target_link_libraries(godlike fmtd imgui d3d11 d3dcompiler DirectXTK ole32)
add_custom_command(
    TARGET godlike
    COMMAND powershell cp ${CMAKE_SOURCE_DIR}/*.hlsl ${CMAKE_SOURCE_DIR}/build/Debug)