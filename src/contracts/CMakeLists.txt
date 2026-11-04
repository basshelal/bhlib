file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "*.hpp")

add_library(contracts STATIC ${SOURCES})

target_sources(contracts PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
        FILES ${HEADERS}
)

target_link_libraries(contracts PUBLIC stacktrace)

# Compile each header on its own to check that it is self-contained, see src/CMakeLists.txt
set_target_properties(contracts PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON)
