cmake_minimum_required(VERSION 3.20)
project(RealgarEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Output directory pattern (mirrors Premake's OutputDir) ───────────────────
# Binaries/<system>-x86_64/<config>/<ProjectName>
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(PLATFORM_DIR "windows")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(PLATFORM_DIR "linux")
else()
    set(PLATFORM_DIR "${CMAKE_SYSTEM_NAME}")
endif()

set(OUTPUT_BASE "${CMAKE_SOURCE_DIR}/Binaries/${PLATFORM_DIR}-x86_64")

# Multi-config generators (Visual Studio, Xcode, Ninja Multi-Config)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY         "${OUTPUT_BASE}/$<CONFIG>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${OUTPUT_BASE}/Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_BASE}/Release")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DIST    "${OUTPUT_BASE}/Dist")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY         "${OUTPUT_BASE}/$<CONFIG>")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${OUTPUT_BASE}/Debug")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_BASE}/Release")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DIST    "${OUTPUT_BASE}/Dist")

# ── Build configurations ──────────────────────────────────────────────────────
# For single-config generators (Makefiles / Ninja) default to Debug
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose: Debug Release Dist" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "Dist")
endif()

# Expose Dist as a valid config for multi-config generators
get_property(_isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(_isMultiConfig)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Dist" CACHE STRING "" FORCE)
endif()

# ── Dist = Release-like but strip symbols ────────────────────────────────────
set(CMAKE_CXX_FLAGS_DIST   "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_C_FLAGS_DIST     "${CMAKE_C_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_DIST "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")

# ── Windows compiler options (mirrors /EHsc /Zc:preprocessor /Zc:__cplusplus)
if(MSVC)
    add_compile_options(/EHsc /Zc:preprocessor /Zc:__cplusplus)
endif()

# ── Extract Vulkan lib on Windows ─────────────────────────────────────────────
if(WIN32)
    execute_process(
            COMMAND "${CMAKE_SOURCE_DIR}/Libraries/vulkan/Lib/7-Zip/7z.exe"
            e "${CMAKE_SOURCE_DIR}/Libraries/vulkan/Lib/vulkan-1.7z"
            "-o${CMAKE_SOURCE_DIR}/Libraries/vulkan/Lib/"
            -aoa
            RESULT_VARIABLE _7z_result
    )
    if(NOT _7z_result EQUAL 0)
        message(WARNING "7-Zip extraction of vulkan-1.7z failed (exit ${_7z_result}). "
                "Make sure vulkan-1.lib is already present.")
    endif()
endif()

# ── Third-party ───────────────────────────────────────────────────────────────
add_subdirectory(ThirdParty/Lua)
add_subdirectory(ThirdParty/freetype2)
add_subdirectory(ThirdParty/glfw3)

# glfw overrides the global archive output dir internally, so redirect it
# explicitly so the linker can find libglfw3.a alongside the other libraries.
set_target_properties(glfw PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY         "${OUTPUT_BASE}/$<CONFIG>"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${OUTPUT_BASE}/Debug"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_BASE}/Release"
        ARCHIVE_OUTPUT_DIRECTORY_DIST    "${OUTPUT_BASE}/Dist"
)

# ── Engine core ───────────────────────────────────────────────────────────────
add_subdirectory(Core)

# ── Projects ──────────────────────────────────────────────────────────────────
add_subdirectory(Game)
add_subdirectory(Editor)

# ── Default startup project (Visual Studio) ──────────────────────────────────
set_property(DIRECTORY "${CMAKE_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT Game)