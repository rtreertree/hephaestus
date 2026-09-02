set(SHADER_OUT ${CMAKE_BINARY_DIR}/shaders)

bgfx_compile_shaders(
  TYPE VERTEX
  SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/vs_mesh.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${SHADER_OUT}
  INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/third_party/bgfx/src
               ${CMAKE_SOURCE_DIR}/third_party/bgfx/examples/common
  OUT_FILES_VAR VS_OUT
)

bgfx_compile_shaders(
  TYPE FRAGMENT
  SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/fs_mesh.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${SHADER_OUT}
  INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/third_party/bgfx/src
               ${CMAKE_SOURCE_DIR}/third_party/bgfx/examples/common
  OUT_FILES_VAR FS_OUT
)

# Custom commands alone never run — a target must own the outputs
add_custom_target(engine_shaders ALL DEPENDS ${VS_OUT} ${FS_OUT})