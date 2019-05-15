cmake_minimum_required(VERSION 2.8)
project(booststache CXX)
enable_testing()

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.67 COMPONENTS unit_test_framework system filesystem)

add_definitions( -DBOOST_SPIRIT_USE_PHOENIX_V3=1 )
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_definitions( -std=c++14 -ftemplate-depth=512 -Wno-unused-local-typedefs -Wno-deprecated-declarations )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_definitions( -std=c++14 -ftemplate-depth=512 -stdlib=libc++ )
endif()
# Not sure what to do with the <toolset>darwin

include_directories( include )
include_directories( SYSTEM ${Boost_INCLUDE_DIRS} )

add_subdirectory(test)
add_subdirectory(examples)
