cmake_minimum_required(VERSION 3.21)
project(bm LANGUAGES CXX)

# Set target directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)

# Include SourceFiles.cmake to access the SOURCEFILES and INCLUDEDIRS variables
include(${CMAKE_CURRENT_LIST_DIR}/SourceFiles.cmake)

add_executable(bm ${SOURCEFILES})

target_compile_definitions(bm PUBLIC
    _FILE_OFFSET_BITS=64 WXUSINGDLL __WXGTK__ _AIM_LINUX
)

target_link_libraries(bm PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/../../deps/wxWidgets/lib/libwx_baseu_net-3.2.so.0
    ${CMAKE_CURRENT_LIST_DIR}/../../deps/wxWidgets/lib/libwx_baseu-3.2.so.0
    ${CMAKE_CURRENT_LIST_DIR}/../../deps/wxWidgets/lib/libwx_gtk3u_core-3.2.so.0
    ${CMAKE_CURRENT_LIST_DIR}/../../deps/aim-driver/lib/libaim_mil.so
)

target_include_directories(bm PUBLIC ${INCLUDEDIRS}
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../../deps/aim-driver/include/aim_mil_24.22
    ${CMAKE_CURRENT_LIST_DIR}/../../deps/wxWidgets/include
    ${CMAKE_CURRENT_LIST_DIR}/../../deps/wxWidgets/lib/wx/include/gtk3-unicode-3.2
    ${CMAKE_CURRENT_LIST_DIR}/ui
)