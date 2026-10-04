#pragma once

#include "core/types.h"

#include <bgfx/bgfx.h>

namespace eng::gfx {

// A deliberately small render vertical slice. It owns the GPU resources needed
// to submit one indexed, vertex-coloured cube to the opaque view.
class CubeRenderer {
public:
    CubeRenderer() = default;
    ~CubeRenderer();

    CubeRenderer(const CubeRenderer&) = delete;
    CubeRenderer& operator=(const CubeRenderer&) = delete;

    bool Init();
    void Shutdown();
    void Render(f32 deltaSeconds, u32 width, u32 height);

private:
    bgfx::VertexBufferHandle m_vertexBuffer = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle  m_indexBuffer  = BGFX_INVALID_HANDLE;
    bgfx::ProgramHandle      m_program      = BGFX_INVALID_HANDLE;
    f32 m_elapsedSeconds = 0.0f;
};

} // namespace eng::gfx
