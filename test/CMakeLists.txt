add_executable( test-key_format key_format.cpp )
target_link_libraries(
  test-key_format
  Boost::unit_test_framework
)
add_test( NAME "key_format" COMMAND test-key_format )

add_executable( test-camap camap.cpp )
target_link_libraries(
  test-camap
  Boost::unit_test_framework
)
add_test( NAME "camap" COMMAND test-camap )

add_executable( test-key key.cpp )
target_link_libraries(
  test-key
  Boost::unit_test_framework
)
add_test( NAME "key" COMMAND test-key )

add_executable( test-insert insert.cpp )
target_link_libraries(
  test-insert
  Boost::unit_test_framework
)
add_test( NAME "insert" COMMAND test-insert )

add_executable( test-erase erase.cpp )
target_link_libraries(
  test-erase
  Boost::unit_test_framework
)
add_test( NAME "erase" COMMAND test-erase )

add_executable( test-find find.cpp )
target_link_libraries(
  test-find
  Boost::unit_test_framework
)
add_test( NAME "find" COMMAND test-find )

add_executable( test-copy copy.cpp )
target_link_libraries(
  test-copy
  Boost::unit_test_framework
)
add_test( NAME "copy" COMMAND test-copy )

add_executable( test-update update.cpp )
target_link_libraries(
  test-update
  Boost::unit_test_framework
)
add_test( NAME "update" COMMAND test-update )

add_executable( test-update_range update_range.cpp )
target_link_libraries(
  test-update_range
  Boost::unit_test_framework
)
add_test( NAME "update_range" COMMAND test-update_range )

add_executable( test-copy_range copy_range.cpp )
target_link_libraries(
  test-copy_range
  Boost::unit_test_framework
)
add_test( NAME "copy_range" COMMAND test-copy_range )

add_executable( test-rectangle_view rectangle_view.cpp )
target_link_libraries(
  test-rectangle_view
  Boost::unit_test_framework
)
add_test( NAME "rectangle_view" COMMAND test-rectangle_view )

add_executable( test-cast cast.cpp )
target_link_libraries(
  test-cast
  Boost::unit_test_framework
)
add_test( NAME "cast" COMMAND test-cast )

add_executable( test-hdmap hdmap.cpp )
target_link_libraries(
  test-hdmap
  Boost::unit_test_framework
)
add_test( NAME "hdmap" COMMAND test-hdmap )

add_executable( test-cross cross.cpp )
target_link_libraries(
  test-cross
  Boost::unit_test_framework
)
add_test( NAME "cross" COMMAND test-cross )
