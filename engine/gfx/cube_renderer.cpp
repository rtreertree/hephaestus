#include "gfx/cube_renderer.h"

#include "core/log.h"
#include "gfx/views.h"

#include <bx/math.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <vector>

namespace eng::gfx {
namespace {

struct PosColorVertex {
    f32 x;
    f32 y;
    f32 z;
    u32 abgr;

    static bgfx::VertexLayout Layout() {
        bgfx::VertexLayout layout;
        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();
        return layout;
    }
};

constexpr std::array<PosColorVertex, 8> kVertices = {{
    {-1.0f,  1.0f,  1.0f, 0xff000000}, {-1.0f, -1.0f,  1.0f, 0xff00ff00},
    { 1.0f, -1.0f,  1.0f, 0xff00ffff}, { 1.0f,  1.0f,  1.0f, 0xff0000ff},
    {-1.0f,  1.0f, -1.0f, 0xffff0000}, {-1.0f, -1.0f, -1.0f, 0xffffff00},
    { 1.0f, -1.0f, -1.0f, 0xffffffff}, { 1.0f,  1.0f, -1.0f, 0xffff00ff},
}};

constexpr std::array<uint16_t, 36> kIndices = {{
    0, 1, 2, 0, 2, 3, 3, 2, 6, 3, 6, 7,
    7, 6, 5, 7, 5, 4, 4, 5, 1, 4, 1, 0,
    4, 0, 3, 4, 3, 7, 1, 5, 6, 1, 6, 2,
}};

const char* ShaderDirectory(bgfx::RendererType::Enum renderer) {
    switch (renderer) {
        case bgfx::RendererType::Metal:       return "metal";
        case bgfx::RendererType::Vulkan:      return "spirv";
        case bgfx::RendererType::OpenGL:      return "glsl";
        case bgfx::RendererType::OpenGLES:    return "essl";
        case bgfx::RendererType::Direct3D11:  return "dxbc";
        case bgfx::RendererType::Direct3D12:  return "dxil";
        default:                              return nullptr;
    }
}

bgfx::ShaderHandle LoadShader(const char* name) {
    const char* directory = ShaderDirectory(bgfx::getRendererType());
    if (!directory) {
        ENGINE_LOG_ERROR("No shader binary mapping for renderer %s.",
                         bgfx::getRendererName(bgfx::getRendererType()));
        return BGFX_INVALID_HANDLE;
    }

    const std::filesystem::path path = std::filesystem::path(ENGINE_SHADER_DIR)
        / directory / (std::string(name) + ".sc.bin");
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
        ENGINE_LOG_ERROR("Could not open shader binary: %s", path.string().c_str());
        return BGFX_INVALID_HANDLE;
    }

    const std::streamsize size = file.tellg();
    if (size <= 0) {
        ENGINE_LOG_ERROR("Shader binary is empty: %s", path.string().c_str());
        return BGFX_INVALID_HANDLE;
    }
    file.seekg(0);

    std::vector<char> bytes(static_cast<size_t>(size));
    if (!file.read(bytes.data(), size)) {
        ENGINE_LOG_ERROR("Could not read shader binary: %s", path.string().c_str());
        return BGFX_INVALID_HANDLE;
    }
    return bgfx::createShader(bgfx::copy(bytes.data(), static_cast<uint32_t>(bytes.size())));
}

} // namespace

CubeRenderer::~CubeRenderer() { Shutdown(); }

bool CubeRenderer::Init() {
    const bgfx::ShaderHandle vertexShader = LoadShader("vs_mesh");
    const bgfx::ShaderHandle fragmentShader = LoadShader("fs_mesh");
    if (!bgfx::isValid(vertexShader) || !bgfx::isValid(fragmentShader)) {
        if (bgfx::isValid(vertexShader)) bgfx::destroy(vertexShader);
        if (bgfx::isValid(fragmentShader)) bgfx::destroy(fragmentShader);
        return false;
    }

    m_program = bgfx::createProgram(vertexShader, fragmentShader, true);
    if (!bgfx::isValid(m_program)) {
        ENGINE_LOG_ERROR("Could not create cube shader program.");
        return false;
    }

    const bgfx::VertexLayout layout = PosColorVertex::Layout();
    m_vertexBuffer = bgfx::createVertexBuffer(
        bgfx::copy(kVertices.data(), sizeof(kVertices)), layout);
    m_indexBuffer = bgfx::createIndexBuffer(bgfx::copy(kIndices.data(), sizeof(kIndices)));
    if (!bgfx::isValid(m_vertexBuffer) || !bgfx::isValid(m_indexBuffer)) {
        ENGINE_LOG_ERROR("Could not create cube geometry buffers.");
        Shutdown();
        return false;
    }

    ENGINE_LOG_INFO("Cube renderer initialized.");
    return true;
}

void CubeRenderer::Shutdown() {
    if (bgfx::isValid(m_indexBuffer)) bgfx::destroy(m_indexBuffer);
    if (bgfx::isValid(m_vertexBuffer)) bgfx::destroy(m_vertexBuffer);
    if (bgfx::isValid(m_program)) bgfx::destroy(m_program);
    m_indexBuffer = BGFX_INVALID_HANDLE;
    m_vertexBuffer = BGFX_INVALID_HANDLE;
    m_program = BGFX_INVALID_HANDLE;
    m_elapsedSeconds = 0.0f;
}

void CubeRenderer::Render(f32 deltaSeconds, u32 width, u32 height) {
    if (!bgfx::isValid(m_program) || width == 0 || height == 0) return;
    m_elapsedSeconds += deltaSeconds;

    const bx::Vec3 eye = {0.0f, 0.0f, -5.0f};
    const bx::Vec3 at = {0.0f, 0.0f, 0.0f};
    float view[16];
    float projection[16];
    bx::mtxLookAt(view, eye, at);
    bx::mtxProj(projection, 60.0f, static_cast<f32>(width) / static_cast<f32>(height),
                0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
    bgfx::setViewTransform(views::kOpaque, view, projection);

    float model[16];
    bx::mtxRotateXY(model, m_elapsedSeconds * 0.7f, m_elapsedSeconds);
    bgfx::setTransform(model);
    bgfx::setVertexBuffer(0, m_vertexBuffer);
    bgfx::setIndexBuffer(m_indexBuffer);
    bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z
                   | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CW | BGFX_STATE_MSAA);
    bgfx::submit(views::kOpaque, m_program);
}

} // namespace eng::gfx
