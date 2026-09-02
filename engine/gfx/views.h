#pragma once
#include "core/types.h"
#include <bgfx/bgfx.h>

namespace eng::gfx {

// Reserved now so later milestones never renumber. bgfx submits views in
// ascending ID order unless you override with bgfx::setViewOrder.
namespace views {
    constexpr bgfx::ViewId kShadow    = 0;
    constexpr bgfx::ViewId kOpaque    = 10;
    constexpr bgfx::ViewId kSkybox    = 20;
    constexpr bgfx::ViewId kTransparent = 30;
    constexpr bgfx::ViewId kPostFx    = 100;
    constexpr bgfx::ViewId kDebugDraw = 200;
    constexpr bgfx::ViewId kUi        = 255;
}

} // namespace eng::gfx