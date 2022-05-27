function(find_component comp)
    set(COMP_DIR ${SDL2_${comp}_DIR})
#    message(STATUS "Checking for ${comp} in ${COMP_DIR}")

    string(APPEND SDL2_INCLUDE_DIRS ";${COMP_DIR}/include")
    set(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS} PARENT_SCOPE)

    # Support both 32 and 64 bit builds
    if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
        string(APPEND SDL2_LIBRARIES ";${COMP_DIR}/lib/x64/SDL2_${comp}.lib")
    else ()
        string(APPEND SDL2_LIBRARIES ";${COMP_DIR}/lib/x86/SDL2_${comp}.lib")
    endif ()
    
    set(SDL2_LIBRARIES ${SDL2_LIBRARIES} PARENT_SCOPE)
endfunction()

set(SDL2_INCLUDE_DIRS "${SDL2_DIR}/include")

# Support both 32 and 64 bit builds
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    set(SDL2_LIBRARIES "${SDL2_DIR}/lib/x64/SDL2.lib;${SDL2_DIR}/lib/x64/SDL2main.lib")
else ()
    set(SDL2_LIBRARIES "${SDL2_DIR}/lib/x86/SDL2.lib;${SDL2_DIR}/lib/x86/SDL2main.lib")
endif ()

foreach(comp ${SDL2_FIND_COMPONENTS})
    find_component(${comp})
endforeach()

string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)

message(STATUS "SDL2_INCLUDE_DIRS: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2_LIBRARIES: ${SDL2_LIBRARIES}")