cmake_minimum_required(VERSION 2.8)

project(lantern)

include(CTest)

include_directories(src)
include_directories(include)

set(LANTERN_SOURCES src/vector3.cpp)
set(LANTERN_HEADERS include/vector3.h include/vector4.h)
set(TESTS_SOURCES tests_src/main.cpp tests_src/vector3.cpp tests_src/vector4.cpp)

add_library(
	lantern STATIC
	${LANTERN_SOURCES}
	${LANTERN_HEADERS})

add_executable(
	tests
	${TESTS_SOURCES}
	${LANTERN_HEADERS})
target_link_libraries(tests lantern gtest pthread)

add_test(lantern_tests tests)
