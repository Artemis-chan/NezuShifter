﻿cmake_minimum_required(VERSION 3.20.2)
project(NezuShifter CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)

#SDL2
if(WIN32)
    set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules)
    set(SDL2_VERSION 2.0.22)
    set(SDL2_ttf_VERSION 2.0.18)
    include(Download)
    download(https://github.com/libsdl-org/SDL/releases/download/release-${SDL2_VERSION}/SDL2-devel-${SDL2_VERSION}-VC.zip SDL2)
    download(https://github.com/libsdl-org/SDL_ttf/releases/download/release-${SDL2_ttf_VERSION}/SDL2_ttf-devel-${SDL2_ttf_VERSION}-VC.zip SDL2_ttf)    
endif()

find_package(SDL2 REQUIRED ttf)
include_directories(${SDL2_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

#ManyMouse
include(GenerateExportHeader)
FILE(GLOB manymouse manymouse/*.c)
add_library(manymouse SHARED ${manymouse} manymouse/manymouse.h)
generate_export_header(manymouse)
target_include_directories(manymouse PUBLIC ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR})

#ViGEmBus
if(WIN32)
    add_compile_definitions("ERROR_INVALID_DEVICE_OBJECT_PARAMETER=__MSABI_LONG(650)")
    include(FetchContent)
    FetchContent_Declare(
            ViGEmClient
            GIT_REPOSITORY https://github.com/ViGEm/ViGEmClient
            GIT_TAG d0b32cfe14db337a071c54ff38451ec2caa4f251
    )
    FetchContent_MakeAvailable(ViGEmClient)
endif()

if(WIN32)
    add_executable(NezuShifter main.cpp controller_emu/controller_emu_win.cpp Shifter.cpp Shifter.hpp)
    target_link_libraries(NezuShifter PUBLIC manymouse ${SDL2_LIBRARIES} ViGEmClient XInput setupapi)
    message(STATUS "Using ViGEmClient ${CMAKE_INSTALL_PREFIX}")
#    install(TARGETS ViGEmClient DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
#    file(INSTALL ${SDL2_LIBRARIES} DESTINATION .)
else()
    add_executable(NezuShifter main.cpp controller_emu/controller_emu_linux.cpp Shifter.cpp Shifter.hpp)
    target_link_libraries(NezuShifter PRIVATE manymouse SDL2 SDL2_ttf)
endif()