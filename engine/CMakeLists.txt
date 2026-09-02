add_library(engine STATIC
  core/log.cpp
  platform/window.cpp
  platform/native_handle.cpp
  gfx/gfx_system.cpp
  engine.cpp

  core/types.h
  core/log.h
  core/assert.h
  platform/window.h
  platform/native_handle.h
  gfx/views.h
  gfx/gfx_system.h
  engine.h
)

target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

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