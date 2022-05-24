cmake_minimum_required(VERSION 3.7)
project(shifter_emu C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

#set(SDL2_DIR "G:/Artemis/Downloads/sdl/SDL2-2.0.22/x86_64-w64-mingw32/lib/cmake/SDL2")
#set(SDL2_DIR "G:/Artemis/Downloads/SDL2-2.0.22/")
#find_package(SDL2 REQUIRED COMPONENTS main)
#
#include_directories(${SDL2_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

include(GenerateExportHeader)
FILE(GLOB manymouse manymouse/*.c)
add_library(manymouse SHARED ${manymouse} manymouse/manymouse.h)
generate_export_header(manymouse)
target_include_directories(manymouse PUBLIC ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR})

add_executable(shifter_emu
        main.c
#        ${manymouse}
)

#target_link_libraries(shifter_emu ${SDL2_LIBRARIES})
target_link_libraries(shifter_emu PRIVATE manymouse)
