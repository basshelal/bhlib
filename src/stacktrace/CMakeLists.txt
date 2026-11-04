file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "*.hpp")
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_library(stacktrace STATIC)

target_sources(stacktrace
        PRIVATE
        ${SOURCES}
        PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
        FILES ${HEADERS}
)

target_link_libraries(stacktrace PRIVATE libbacktrace)

# Compile each header on its own to check that it is self-contained, see src/CMakeLists.txt
set_target_properties(stacktrace PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON)
