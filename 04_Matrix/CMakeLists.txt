cmake_minimum_required (VERSION 3.16)
project ("LIRS CACHE" VERSION 1.0.0) 

set (EXEC_FILE "matrix")

set (INCLUDE 

    include
)

set (SOURCES 

    source/main.cpp
)

add_executable (${EXEC_FILE} ${SOURCES})
target_include_directories (${EXEC_FILE} PRIVATE ${INCLUDE})

set (COMPILE_SETTINGS

    -Wall
    -Wextra
    -Wpedantic
    -O2
)

target_compile_options (${EXEC_FILE} PRIVATE ${COMPILE_SETTINGS})