function(add_compiler_warnings)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "CLANG")
        set(WARNINGS
            -Wall
            -Wextra
            -Wpedantic)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(WARNINGS
            /W4
            /permissive-)
    else()
        message(WARNING "Warnings not supported for this compiler")
        return()
    endif()

    add_compile_options(${WARNINGS})
    message(STATUS "Compiler Warnings Enabled")
endfunction()
