cmake_minimum_required(VERSION 3.21)
project(rt LANGUAGES CXX)

# Set target directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)

# Include SourceFiles.cmake to access the SOURCEFILES and INCLUDEDIRS variables
include(${CMAKE_CURRENT_LIST_DIR}/SourceFiles.cmake)

add_executable(rt ${SOURCEFILES})

target_compile_definitions(rt PUBLIC
    _AIM_LINUX
)

target_link_libraries(rt PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/../../deps/aim-driver/lib/libaim_mil.so
)

target_include_directories(rt PUBLIC ${INCLUDEDIRS}
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../../deps/aim-driver/include/aim_mil_24.22
)