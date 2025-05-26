cmake_minimum_required(VERSION 3.21)
project(app LANGUAGES CXX)

# Set target directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)

# Include SourceFiles.cmake to access the SOURCEFILES and INCLUDEDIRS variables
include(${CMAKE_SOURCE_DIR}/src/SourceFiles.cmake)

add_executable(app ${SOURCEFILES})

target_compile_definitions(app PUBLIC
    _AIM_LINUX
)

target_link_libraries(app PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/deps/aim-driver/lib/libaim_mil.so
)

target_include_directories(app PUBLIC ${INCLUDEDIRS}
    ${CMAKE_CURRENT_LIST_DIR}/deps/aim-driver/include/aim_mil_24.22
)