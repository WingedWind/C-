cmake_minimum_required(VERSION 3.16)

project(matrix)

set(CMAKE_CXX_STANDARD 17)

# collecting libs
add_subdirectory(source)

# common includes
foreach(LIB ${LIBLIST})
  target_include_directories(${LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
endforeach()



set(SRCS_MATRIX
  source/Matrix/main.cpp
)

add_executable(matrix ${SRCS_MATRIX})

target_include_directories(matrix PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_link_libraries (matrix ${LIBLIST})


set(SRCS_UNIT_TESTS
  source/Unit_tests/unit_tests.cpp
)

add_executable(unit_tests ${SRCS_UNIT_TESTS})

target_include_directories(unit_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_link_libraries (unit_tests ${LIBLIST})