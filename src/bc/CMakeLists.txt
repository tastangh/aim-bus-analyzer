cmake_minimum_required(VERSION 3.21)
project(bc LANGUAGES CXX)

# Set target directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)

# Include SourceFiles.cmake to access the SOURCEFILES and INCLUDEDIRS variables
include(${CMAKE_CURRENT_LIST_DIR}/SourceFiles.cmake)

add_executable(bc ${SOURCEFILES})

target_compile_definitions(bc PUBLIC
    _AIM_LINUX
)

target_link_libraries(bc PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/../../deps/aim-driver/lib/libaim_mil.so
)

target_include_directories(bc PUBLIC ${INCLUDEDIRS}
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../../deps/aim-driver/include/aim_mil_24.22

)