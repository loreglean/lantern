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
#
if (MSVC)
    # TODO: setup vc++ flags
    set(COMPILER_FLAGS
        "")
else()
    set(COMPILER_FLAGS
        "-std=gnu++0x -Wall -Wno-comment -O3")
endif()

# Add compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILER_FLAGS}")

# Additional MSVC-related stuff
#
if (MSVC)
    # Setup dependencies folder
    #
    set(MSVC_DEPENDENCIES_FOLDER
        "${CMAKE_BINARY_DIR}/dependencies/")
    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${MSVC_DEPENDENCIES_FOLDER}/include/)
    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${MSVC_DEPENDENCIES_FOLDER}/lib/)
    include_directories(${MSVC_DEPENDENCIES_FOLDER}/include/)
    link_directories(${MSVC_DEPENDENCIES_FOLDER}/lib/)

    # Add math defines
    add_definitions(-D_USE_MATH_DEFINES)
endif()

# Targets
#

# Library
#

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

set(TESTS_SOURCES
    tests/src/main.cpp
    tests/src/vector3.cpp
    tests/src/vector4.cpp
    tests/src/matrix3x3.cpp
    tests/src/matrix4x4.cpp
    tests/src/camera.cpp
    tests/src/bitmap_painter.cpp
    tests/src/obj_import.cpp)
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

if (MSVC)
    target_link_libraries(tests lantern gtest)
else()
    target_link_libraries(tests lantern gtest pthread)
endif()
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


if (MINGW)
    target_link_libraries(empty_app lantern mingw32 SDL2main SDL2)
elseif (APPLE)
    target_link_libraries(empty_app lantern /Library/Frameworks/SDL2.framework)
else()
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

if (MINGW)
    target_link_libraries(rotating_car_app lantern mingw32 SDL2main SDL2)
elseif (APPLE)
    target_link_libraries(rotating_car_app lantern /Library/Frameworks/SDL2.framework)
else()
    target_link_libraries(rotating_car_app lantern SDL2main SDL2)
endif()
