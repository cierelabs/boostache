add_custom_target(check COMMAND make all test)

add_library(liaw STATIC
  liaw2014/renderer.hpp
  liaw2014/renderer.cpp
  liaw2014/render_ast.hpp
  liaw2014/render_data.hpp
)

add_executable(test_liaw
  test_liaw.cpp
)
add_dependencies(test_liaw liaw)
target_include_directories(test_liaw PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(test_liaw liaw boost_unit_test_framework)
add_test(test_liaw test_liaw)
