cmake_minimum_required(VERSION 2.8)

# Project name
project(lantern)

# Add testing tool
include(CTest)

# Project directories
include_directories(
    examples/empty_app
    examples/rotating_car_app
    lantern/src lantern/include
    tests/src tests/include)

# Additional compiler flags
set(COMPILER_FLAGS)
if(MSVC)
  set(COMPILER_FLAGS "${COMPILER_FLAGS} /W4 /GS")
  set(LINK_FLAGS "${LINK_FLAGS} /SafeSEH /DynamicBase /NXCompat")
  add_definitions("-D_USE_MATH_DEFINES") # http://msdn.microsoft.com/en-us/library/4hwaceh6.aspx
else()
  set(COMPILER_FLAGS
    "-std=gnu++0x -flto -Wall -Wextra -Wno-comment -Werror -Wno-unused-parameter -Wno-unknown-pragmas")
endif(MSVC)

# Add compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DEFAULT_CMAKE_C_FLAGS} ${COMPILER_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DEFAULT_CMAKE_CXX_FLAGS} ${COMPILER_FLAGS}")

# Targets
#

# Library
#
if(CMAKE_HOST_WIN32)
   set(SDL2Path ${CMAKE_SOURCE_DIR}/SDL2-devel-2.0.3-VC)

   include_directories(SYSTEM ${SDL2Path}/include)
   link_directories(${SDL2Path}/lib/x86)
endif(CMAKE_HOST_WIN32)

set(LANTERN_SOURCES
    lantern/src/vector3.cpp
    lantern/src/vector4.cpp
    lantern/src/matrix3x3.cpp
    lantern/src/matrix4x4.cpp
    lantern/src/camera.cpp
    lantern/src/app.cpp
    lantern/src/point.cpp
    lantern/src/color.cpp
    lantern/src/bitmap_painter.cpp
    lantern/src/mesh.cpp
    lantern/src/obj_import.cpp
    lantern/src/scene_object.cpp
    lantern/src/scene.cpp
    lantern/src/pipeline.cpp)
set(LANTERN_HEADERS
    lantern/include/vector3.h
    lantern/include/vector4.h
    lantern/include/matrix3x3.h
    lantern/include/matrix4x4.h
    lantern/include/point.h
    lantern/include/color.h
    lantern/include/camera.h
    lantern/include/app.h
    lantern/include/bitmap_painter.h
    lantern/include/mesh.h
    lantern/include/obj_import.h
    lantern/include/scene_object.h
    lantern/include/scene.h
    lantern/include/pipeline.h)

add_library(
	lantern STATIC
	${LANTERN_SOURCES}
	${LANTERN_HEADERS})

set_target_properties(
    lantern PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Tests
#

if(CMAKE_HOST_UNIX)
    set(GTestPath /usr/src/gtest/)
endif(CMAKE_HOST_UNIX)
if(CMAKE_HOST_WIN32)
    set(GTestPath ${CMAKE_SOURCE_DIR}/gtest-1.7.0)
endif(CMAKE_HOST_WIN32)
set(GTEST_FILES ${GTestPath}/src/gtest-all.cc ${GTestPath}/src/gtest_main.cc)

include_directories(SYSTEM ${GTestPath}/)
include_directories(SYSTEM ${GTestPath}/include/)


set(TESTS_SOURCES
    tests/src/main.cpp
    tests/src/vector3.cpp
    tests/src/vector4.cpp
    tests/src/matrix3x3.cpp
    tests/src/matrix4x4.cpp
    tests/src/camera.cpp
    tests/src/bitmap_painter.cpp
    tests/src/obj_import.cpp
    ${GTEST_FILES})

set(TESTS_HEADERS
    tests/include/common.h)

add_executable(
	tests
	${TESTS_SOURCES}
	${TESTS_HEADERS}
	${LANTERN_HEADERS})

set_target_properties(
    tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/")

add_custom_command(
    TARGET tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/tests/resources"
    $<TARGET_FILE_DIR:tests>/resources)
if(MSVC)
    target_link_libraries(tests lantern)
else()
    target_link_libraries(tests lantern pthread)
endif(MSVC)

add_test(lantern_tests tests)

# Empty app example
#

add_executable(
    empty_app
    examples/empty_app/main.cpp
    ${LANTERN_HEADERS})

set_target_properties(
    empty_app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/empty_app")

if (UNIX)
    target_link_libraries(empty_app lantern SDL2)
else ()
    target_link_libraries(empty_app lantern SDL2main SDL2)
endif()

# Rotating car app example
#

add_executable(
    rotating_car_app
    examples/rotating_car_app/main.cpp
    ${LANTERN_HEADERS})

set_target_properties(
    rotating_car_app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/rotating_car_app")

add_custom_command(
    TARGET rotating_car_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/examples/rotating_car_app/resources"
    $<TARGET_FILE_DIR:rotating_car_app>/resources)

if (UNIX)
    target_link_libraries(rotating_car_app lantern SDL2)
else ()
    target_link_libraries(rotating_car_app lantern SDL2main SDL2)
endif()
