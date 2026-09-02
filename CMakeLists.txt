cmake_minimum_required(VERSION 3.25)
project(MyEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

# Single output dir so the exe is easy to find on every generator.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
foreach(cfg IN ITEMS DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${cfg} ${CMAKE_BINARY_DIR}/bin)
endforeach()

# ---------------------------------------------------------------- third_party
set(BGFX_BUILD_EXAMPLES   OFF CACHE BOOL "" FORCE)
set(BGFX_BUILD_TESTS      OFF CACHE BOOL "" FORCE)
set(BGFX_BUILD_TOOLS      ON  CACHE BOOL "" FORCE)  # shaderc, needed next milestone
set(BGFX_INSTALL          OFF CACHE BOOL "" FORCE)
set(BGFX_CUSTOM_TARGETS   OFF CACHE BOOL "" FORCE)

# Uncomment if bgfx.cmake cannot auto-locate the sibling repos:
# set(BX_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/third_party/bx   CACHE PATH "" FORCE)
# set(BIMG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/bimg CACHE PATH "" FORCE)
# set(BGFX_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/bgfx CACHE PATH "" FORCE)

add_subdirectory(third_party/bgfx.cmake)

set(SDL_SHARED  OFF CACHE BOOL "" FORCE)
set(SDL_STATIC  ON  CACHE BOOL "" FORCE)
set(SDL_TEST    OFF CACHE BOOL "" FORCE)
set(SDL_TESTS   OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/SDL)

# ---------------------------------------------------------------------- engine
add_subdirectory(engine)
add_subdirectory(sandbox)