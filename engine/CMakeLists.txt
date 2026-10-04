add_library(engine STATIC
  core/log.cpp
  platform/window.cpp
  platform/native_handle.cpp
  gfx/gfx_system.cpp
  gfx/cube_renderer.cpp
  engine.cpp

  core/types.h
  core/log.h
  core/assert.h
  platform/window.h
  platform/native_handle.h
  gfx/views.h
  gfx/gfx_system.h
  gfx/cube_renderer.h
  gfx/quitecallback.h
  engine.h
)

target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Kept as an absolute build path so the sandbox can be launched from an IDE,
# its output directory, or the repository root without special working-dir setup.
target_compile_definitions(engine PRIVATE
  ENGINE_SHADER_DIR="${CMAKE_BINARY_DIR}/shaders")

target_link_libraries(engine
  PUBLIC
    bgfx
    bx
    SDL3::SDL3-static
)

if(MSVC)
  target_compile_options(engine PRIVATE /W4 /permissive-)
  target_compile_definitions(engine PRIVATE _CRT_SECURE_NO_WARNINGS NOMINMAX WIN32_LEAN_AND_MEAN)
else()
  target_compile_options(engine PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Keep the IDE tree matching the folders on disk.
get_target_property(_engine_sources engine SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${_engine_sources})
