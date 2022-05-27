
include(FetchContent)
set(CMAKE_TLS_VERIFY true)

function(download url name)
    FetchContent_Declare(${name}
            URL ${url}
    )
    FetchContent_MakeAvailable(${name})
    set(${name}_DIR ${FETCHCONTENT_BASE_DIR}/${name}-src PARENT_SCOPE )
endfunction()

