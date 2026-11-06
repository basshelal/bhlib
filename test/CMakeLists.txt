set(TEST_EXECUTABLE "bhlib-test")

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(${TEST_EXECUTABLE} ${TEST_SOURCES})

set_target_properties(${TEST_EXECUTABLE} PROPERTIES
        CXX_STANDARD 23
        CXX_EXTENSIONS OFF
)

target_link_libraries(${TEST_EXECUTABLE} PRIVATE
        bhlib
        Catch2::Catch2
)

# So that we can use #include "test/TestUtils.hpp" rather than relying on relative paths
target_include_directories(${TEST_EXECUTABLE} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

# Without exceptions a failed REQUIRE or a SKIP aborts the entire test run rather than only ending the current test.
# PUBLIC because Catch2 detects exceptions per translation unit, so whatever includes its headers must agree with it.
# This overrides our no exceptions and no RTTI policy only for Catch2 and the tests, bhlib does not link Catch2
target_compile_options(Catch2 PUBLIC -fexceptions -frtti)

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(${TEST_EXECUTABLE})

# See src/CMakeLists.txt
add_dependencies(${TEST_EXECUTABLE} bhlib_verify_interface_header_sets)

# TODO remove the -Wno below
target_compile_options(${TEST_EXECUTABLE} PRIVATE "-Wno-comment")