set(SOURCES
	EvaluatorTest.cpp
	LexerTest.cpp
	ParserTest.cpp)

set(TEST_NAME ${LI_LIBRARY}-test)

find_package(GTest)
enable_testing()

add_executable(${TEST_NAME} ${SOURCES})
target_link_libraries(${TEST_NAME} PRIVATE
	GTest::gtest_main
	${LI_LIBRARY})

include(GoogleTest)
gtest_discover_tests(${TEST_NAME})