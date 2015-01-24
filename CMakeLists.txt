cmake_minimum_required(VERSION 2.8)

# Project name
project(lantern)

# Add testing tool
include(CTest)

# SDL2 look up ==============================
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED)
# ===========================================

# Compiler setup ============================
if (MSVC)
    set(COMPILER_FLAGS
        "")

    add_definitions(-D_USE_MATH_DEFINES)
    add_definitions(-DNOMINMAX)
else()
    set(COMPILER_FLAGS
        "-std=gnu++0x -Wall -Wno-comment")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILER_FLAGS}")
# ===========================================

# Library target ============================
set(LANTERN_SOURCES
    lantern/src/app.cpp
    lantern/src/camera.cpp
    lantern/src/color.cpp
    lantern/src/matrix3x3.cpp
    lantern/src/matrix4x4.cpp
    lantern/src/mesh.cpp
    lantern/src/obj_import.cpp
    lantern/src/pipeline.cpp
    lantern/src/texture.cpp
    lantern/src/vector3.cpp
    lantern/src/vector4.cpp)

set(LANTERN_HEADERS
    lantern/include/app.h
    lantern/include/camera.h
    lantern/include/color.h
    lantern/include/color_shader.h
    lantern/include/geometry_utils.h
    lantern/include/matrix3x3.h
    lantern/include/matrix4x4.h
    lantern/include/mesh.h
    lantern/include/mesh_attribute_info.h
    lantern/include/obj_import.h
    lantern/include/pipeline.h
    lantern/include/shader.h
    lantern/include/texture.h
    lantern/include/vector2.h
    lantern/include/vector3.h
    lantern/include/vector4.h)

add_library(
    lantern STATIC
    ${LANTERN_SOURCES}
    ${LANTERN_HEADERS})

target_include_directories(lantern PUBLIC lantern/include)
target_include_directories(lantern PRIVATE ${SDL2_INCLUDE_DIR})

set_target_properties(
    lantern PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
# ===========================================

# Gooogle C++ testing framework target ======
if(DEFINED ENV{GTEST_ROOT})

    string(REPLACE "\\" "/" GTEST_SOURCES "$ENV{GTEST_ROOT}/src/gtest-all.cc")

    add_library(
        gtest STATIC
        ${GTEST_SOURCES})

    target_include_directories(gtest PRIVATE $ENV{GTEST_ROOT}/include $ENV{GTEST_ROOT})

    if(UNIX)
        target_link_libraries(gtest pthread)
    endif()

endif()
# ===========================================

# Tests target ==============================
set(TESTS_SOURCES
    tests/src/camera.cpp
    tests/src/main.cpp
    tests/src/matrix3x3.cpp
    tests/src/matrix4x4.cpp
    tests/src/obj_import.cpp
    tests/src/pipeline.cpp
    tests/src/vector3.cpp
    tests/src/vector4.cpp)

set(TESTS_HEADERS
    tests/include/assert_utils.h)

add_executable(
    tests
    ${TESTS_SOURCES}
    ${TESTS_HEADERS}
    ${LANTERN_HEADERS})

target_include_directories(tests PRIVATE lantern/include tests/include)

if (DEFINED ENV{GTEST_ROOT})
    target_include_directories(tests PRIVATE $ENV{GTEST_ROOT}/include)
endif()

set_target_properties(
    tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")

target_link_libraries(tests lantern gtest)

add_custom_command(
    TARGET tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/tests/resources"
    $<TARGET_FILE_DIR:tests>/resources)
# ===========================================

# Empty app target ==========================
add_executable(
    empty_app
    examples/empty_app/main.cpp)

set_target_properties(
    empty_app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/empty_app")

target_include_directories(empty_app PRIVATE lantern/include)
target_include_directories(empty_app PRIVATE ${SDL2_INCLUDE_DIR})

target_link_libraries(empty_app lantern ${SDL2_LIBRARY})

if (WIN32)
    add_custom_command(
        TARGET empty_app
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        "${SDL2_DLL}"
        $<TARGET_FILE_DIR:empty_app>)
endif()
# ===========================================

# Rasterized triangle app target ======
add_executable(
    rasterized_triangle_app
    examples/rasterized_triangle_app/main.cpp)

set_target_properties(
    rasterized_triangle_app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/rasterized_triangle_app")

target_include_directories(rasterized_triangle_app PRIVATE lantern/include)
target_include_directories(rasterized_triangle_app PRIVATE ${SDL2_INCLUDE_DIR})

target_link_libraries(rasterized_triangle_app lantern ${SDL2_LIBRARY})

if (WIN32)
    add_custom_command(
        TARGET rasterized_triangle_app
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        "${SDL2_DLL}"
        $<TARGET_FILE_DIR:rasterized_triangle_app>)
endif()

add_custom_command(
    TARGET rasterized_triangle_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/examples/rasterized_triangle_app/resources"
    $<TARGET_FILE_DIR:rasterized_triangle_app>/resources)
# ===========================================