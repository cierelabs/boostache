cmake_minimum_required(VERSION 3.10)
project(booststache CXX)
enable_testing()

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.53 COMPONENTS unit_test_framework system filesystem)

add_definitions( -DBOOST_SPIRIT_USE_PHOENIX_V3=1 )
set( CMAKE_CXX_STANDARD 14 )
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_definitions( -ftemplate-depth=512 -Wno-unused-local-typedefs -Wno-deprecated-declarations )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_definitions( -ftemplate-depth=512 -stdlib=libc++ )
endif()
# Not sure what to do with the <toolset>darwin

include_directories( include )

add_subdirectory(test)
add_subdirectory(examples)
