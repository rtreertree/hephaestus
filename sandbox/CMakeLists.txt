add_executable(sandbox WIN32 MACOSX_BUNDLE main.cpp)

target_link_libraries(sandbox PRIVATE engine)
add_dependencies(sandbox engine_shaders)

# We provide our own main(); SDL3's entry-point shim would rename it.
target_compile_definitions(sandbox PRIVATE SDL_MAIN_HANDLED)

# Keep a console on Windows so ENGINE_LOG output is visible during development.
if(MSVC)
  set_target_properties(sandbox PROPERTIES
    LINK_FLAGS "/SUBSYSTEM:CONSOLE /ENTRY:mainCRTStartup"
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
endif()

if(APPLE)
  set_target_properties(sandbox PROPERTIES
    MACOSX_BUNDLE FALSE)   # plain CLI binary for now; bundle it when you ship
endif()

set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT sandbox)
