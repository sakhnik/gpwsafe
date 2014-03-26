enable_testing()

set(UNIT_TESTS
	KeyStretchTest
	File3Test
	DatabaseTest
	EntryTest
	)

set(LIBRARIES libgpwsafe ${LIBGCRYPT_LIBRARIES} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES})
add_definitions(-DBOOST_TEST_DYN_LINK)

foreach(TEST ${UNIT_TESTS})
	add_executable(${TEST} ${TEST}.cc)
	target_link_libraries(${TEST} ${LIBRARIES})
	add_test(NAME ${TEST} COMMAND ${TEST})
endforeach()
