foreach( EXAMPLE example1 example2 example3 example4)
  add_executable( ${EXAMPLE} ${EXAMPLE}.cpp )
endforeach()

add_executable( example5 example5.cpp example5.pb.cc)
target_link_libraries( example5 protobuf)

