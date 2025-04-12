
#pragma once

#include <type_traits>

namespace tire {

struct OpenGLTag {};
struct VulkanTag {};

// Concept for compile time branch or overload by
// render type.
template <typename T>
concept RenderType =
    std::is_same_v<T, OpenGLTag> || std::is_same_v<T, VulkanTag>;

}  // namespace tire