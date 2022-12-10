cmake_minimum_required (VERSION 3.16)
project(sandbox)
add_executable(sandbox sandbox.cpp)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT sandbox)
