file(GLOB_RECURSE BHLIB_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE BHLIB_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

# Static by default, set BUILD_SHARED_LIBS to ON for a shared library, see README.md for why static is preferred
add_library(bhlib)
add_library(bhlib::bhlib ALIAS bhlib)

target_sources(bhlib
        PRIVATE
        ${BHLIB_SOURCES}
        PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES ${BHLIB_HEADERS}
)

# Our headers are compiled as part of whatever includes them, so their requirements must reach our consumers
target_compile_features(bhlib PUBLIC cxx_std_23)
target_compile_options(bhlib PUBLIC -Wno-interference-size)

# That also exposes our headers to our consumers' warning flags, so have them treat our headers as system headers.
# Our own tests must not, they are what holds our headers to our warning flags
if (NOT PROJECT_IS_TOP_LEVEL)
    set_target_properties(bhlib PROPERTIES SYSTEM ON)
endif ()

target_link_libraries(bhlib
        PUBLIC
        fmt::fmt
        magic_enum::magic_enum
        PRIVATE
        libbacktrace
)

# The heap sealing only works if the final binary wraps malloc and friends too, so this is PUBLIC, see bhlib/heap/Heap.hpp
# --undefined pulls in the object file with all of the __wrap_* symbols. Without it, if only a library linked after us
# (such as fmt) allocates, then order-sensitive linkers like GNU ld fail on undefined references to __wrap_malloc
target_link_options(bhlib PUBLIC
        "LINKER:--wrap=malloc"
        "LINKER:--wrap=free"
        "LINKER:--wrap=calloc"
        "LINKER:--wrap=realloc"
        "LINKER:--wrap=aligned_alloc"
        "LINKER:--undefined=__wrap_malloc"
)

# Header-only code is only compiled when something includes it, so a header that nothing includes yet could silently
# rot. VERIFY_INTERFACE_HEADER_SETS generates a target that compiles every header on its own, which also checks that
# each header is self-contained (includes everything it needs).
# Building the tests builds this too.
set_target_properties(bhlib PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON)
