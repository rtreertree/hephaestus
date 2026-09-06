add_library(sandbox_compile OBJECT main.cpp)

# Keep the sandbox source compilable without requiring the executable link.
target_compile_definitions(sandbox_compile PRIVATE SDL_MAIN_HANDLED)

add_executable(sandbox WIN32 MACOSX_BUNDLE $<TARGET_OBJECTS:sandbox_compile>)

target_link_libraries(sandbox PRIVATE engine)

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