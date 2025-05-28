cmake_minimum_required(VERSION 3.21)
project(aim-bus-analyzer)


# Bus Controller
add_subdirectory(src/bc/)

# BM 
 add_subdirectory(src/bm/)

# RT Emulator
 add_subdirectory(src/rt/)