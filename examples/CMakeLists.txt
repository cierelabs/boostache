foreach( EXAMPLE example1 example2 example3 )
  add_executable( ${EXAMPLE} ${EXAMPLE}.cpp )
endforeach()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/footer.mst ${CMAKE_CURRENT_BINARY_DIR}/footer.mst COPYONLY)