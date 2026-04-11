cmake_minimum_required(VERSION 3.20)

# ── Collect sources ───────────────────────────────────────────────────────────
file(GLOB LUA_SOURCES
        "Source/*.c"
        "Source/*.h"
        "Source/*.hpp"
)

# Remove standalone interpreter and compiler entry points
list(FILTER LUA_SOURCES EXCLUDE REGEX "/(lua|luac)\\.c$")

# ── Target ────────────────────────────────────────────────────────────────────
add_library(Lua STATIC ${LUA_SOURCES})

# ── Include directories ───────────────────────────────────────────────────────
target_include_directories(Lua PUBLIC Source)

# ── Platform links ────────────────────────────────────────────────────────────
if(UNIX AND NOT APPLE)
    target_link_libraries(Lua PRIVATE png z bz2)
endif()

# ── Per-configuration defines ─────────────────────────────────────────────────
target_compile_definitions(Lua PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:RELEASE>
        $<$<CONFIG:Dist>:DIST>
)

# ── Optimisation / debug symbols per config ───────────────────────────────────
target_compile_options(Lua PRIVATE
        $<$<AND:$<C_COMPILER_ID:GNU,Clang>,$<CONFIG:Debug>>:   -g -O0>
        $<$<AND:$<C_COMPILER_ID:GNU,Clang>,$<CONFIG:Release>>: -g -O2>
        $<$<AND:$<C_COMPILER_ID:GNU,Clang>,$<CONFIG:Dist>>:    -O2>
        $<$<AND:$<C_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:        /Od /Zi>
        $<$<AND:$<C_COMPILER_ID:MSVC>,$<CONFIG:Release>>:      /O2 /Zi>
        $<$<AND:$<C_COMPILER_ID:MSVC>,$<CONFIG:Dist>>:         /O2>
)

# ── Output paths ──────────────────────────────────────────────────────────────
set_target_properties(Lua PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${OUTPUT_BASE}/Debug/Lua"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_BASE}/Release/Lua"
        ARCHIVE_OUTPUT_DIRECTORY_DIST    "${OUTPUT_BASE}/Dist/Lua"
        ARCHIVE_OUTPUT_DIRECTORY         "${OUTPUT_BASE}/$<CONFIG>/Lua"
)