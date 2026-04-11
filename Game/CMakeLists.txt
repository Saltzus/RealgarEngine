cmake_minimum_required(VERSION 3.20)

# ── Collect sources ───────────────────────────────────────────────────────────
file(GLOB_RECURSE GAME_SOURCES
        "Source/*.h"
        "Source/*.cpp"
)

file(GLOB_RECURSE IMGUI_SOURCES
        "${CMAKE_SOURCE_DIR}/Libraries/ImGui/*.h"
        "${CMAKE_SOURCE_DIR}/Libraries/ImGui/*.cpp"
)

# ── Target ────────────────────────────────────────────────────────────────────
add_executable(Game
        ${GAME_SOURCES}
        ${IMGUI_SOURCES}
)

# ── Include directories ───────────────────────────────────────────────────────
target_include_directories(Game PRIVATE
        Source

        # Lua
        ${CMAKE_SOURCE_DIR}/ThirdParty/Lua/Source
        # FreeType 2
        ${CMAKE_SOURCE_DIR}/ThirdParty/freetype2/src
        ${CMAKE_SOURCE_DIR}/ThirdParty/freetype2/include
        # Core
        ${CMAKE_SOURCE_DIR}/Core/Source
        # GLFW
        ${CMAKE_SOURCE_DIR}/ThirdParty/glfw3/include
        # glad / OpenGL
        ${CMAKE_SOURCE_DIR}/Libraries/glad/include
        # Vulkan
        ${CMAKE_SOURCE_DIR}/Libraries/vulkan/Include
        # GLM
        ${CMAKE_SOURCE_DIR}/Libraries/glm
        # stb_image
        ${CMAKE_SOURCE_DIR}/Libraries/stb_image
        # miniaudio
        ${CMAKE_SOURCE_DIR}/Libraries/miniaudio
        # nlohmann JSON
        ${CMAKE_SOURCE_DIR}/Libraries/nlohmann_json
        # ImGui
        ${CMAKE_SOURCE_DIR}/Libraries/ImGui
        ${CMAKE_SOURCE_DIR}/Libraries/ImGui/backends
)

# ── Dependencies ──────────────────────────────────────────────────────────────
add_dependencies(Game Core)

# ── Link libraries ────────────────────────────────────────────────────────────
target_link_directories(Game PRIVATE
        ${CMAKE_SOURCE_DIR}/Libraries/vulkan/Lib
        ${OUTPUT_BASE}/$<CONFIG>
)

target_link_libraries(Game PRIVATE
        Core
        Lua
        glfw
        freetype
        vulkan-1
)

if(UNIX AND NOT APPLE)
    target_link_libraries(Game PRIVATE png z bz2)
endif()

# ── Platform defines ──────────────────────────────────────────────────────────
if(WIN32)
    target_compile_definitions(Game PRIVATE WINDOWS)
endif()

# ── Per-configuration defines ─────────────────────────────────────────────────
target_compile_definitions(Game PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:RELEASE>
        $<$<CONFIG:Dist>:DIST>
)

# ── Optimisation / debug symbols per config ───────────────────────────────────
target_compile_options(Game PRIVATE
        $<$<AND:$<CXX_COMPILER_ID:GNU,Clang>,$<CONFIG:Debug>>:   -g -O0>
        $<$<AND:$<CXX_COMPILER_ID:GNU,Clang>,$<CONFIG:Release>>: -g -O2>
        $<$<AND:$<CXX_COMPILER_ID:GNU,Clang>,$<CONFIG:Dist>>:    -O2>
        $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:        /Od /Zi>
        $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:      /O2 /Zi>
        $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Dist>>:         /O2>
)

# ── Output paths ──────────────────────────────────────────────────────────────
set_target_properties(Game PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${OUTPUT_BASE}/Debug/Game"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_BASE}/Release/Game"
        RUNTIME_OUTPUT_DIRECTORY_DIST    "${OUTPUT_BASE}/Dist/Game"
        RUNTIME_OUTPUT_DIRECTORY         "${OUTPUT_BASE}/$<CONFIG>/Game"
)

# ── Post-build: copy Resources (Windows) ─────────────────────────────────────
if(WIN32)
    add_custom_command(TARGET Game POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/Game/Resources"
            "${OUTPUT_BASE}/$<CONFIG>/Game"
            COMMENT "Copying Game/Resources → Binaries output"
    )
endif()