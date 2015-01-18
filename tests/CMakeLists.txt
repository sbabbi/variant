
find_package(GTest REQUIRED)

include_directories(${CMAKE_SOURCE_DIR})

add_executable( variant_storage_test variant_storage_test.cpp )
target_link_libraries( variant_storage_test ${GTEST_BOTH_LIBRARIES} ${VARIANT_LIBCPP} )
add_test(NAME variant_storage_test COMMAND variant_storage_test)

add_executable( variant_test variant_test.cpp )
target_link_libraries( variant_test ${GTEST_BOTH_LIBRARIES} ${VARIANT_LIBCPP} )
add_test(NAME variant_test COMMAND variant_test)

add_executable( visitor_test visitor_test.cpp )
target_link_libraries( visitor_test ${GTEST_BOTH_LIBRARIES} ${VARIANT_LIBCPP} )
add_test(NAME visitor_test COMMAND visitor_test)
