cmake_minimum_required(VERSION 3.16)

project(foobar)

set(CMAKE_CXX_STANDARD 17)

# collecting libs
add_subdirectory(source)

# common includes
foreach(LIB ${LIBLIST})
  target_include_directories(${LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/${LIB})
endforeach()



set(SRCS_LIRS
  source/Lirs/cache.cpp
)

add_executable(lirs ${SRCS_LIRS})

target_include_directories(lirs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_link_libraries (lirs ${LIBLIST})


set(SRCS_BELADY
  source/Belady/belady.cpp
)

add_executable(belady ${SRCS_BELADY})

target_include_directories(belady PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_link_libraries (belady ${LIBLIST})