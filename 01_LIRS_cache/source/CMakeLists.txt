set(LIBLIST
    Lirs
    Belady
)

foreach(LIB ${LIBLIST})
  add_subdirectory(${LIB})
endforeach()

set(LIBLIST "${LIBLIST}" PARENT_SCOPE)