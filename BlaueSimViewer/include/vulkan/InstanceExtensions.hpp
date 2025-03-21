#pragma once

#include <vulkan/vulkan.h>
#include <EASTL/vector.h>
#include <cstdint>

namespace blau::vk {

static constexpr const char* RequiredInstanceExtensions[] = {
#if defined(VK_KHR_surface) && VK_KHR_surface
    VK_KHR_SURFACE_EXTENSION_NAME,
#endif
#if defined(VK_KHR_win32_surface) && VK_KHR_win32_surface
    VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
#if defined(VK_EXT_metal_surface) && VK_EXT_metal_surface
    VK_EXT_METAL_SURFACE_EXTENSION_NAME,
#endif
};

struct EnabledInstanceExtensions final
{
    ::std::uint32_t HasDebugUtils : 1;
    ::std::uint32_t HasGetSurfaceCapabilities2 : 1;
    ::std::uint32_t HasGetPhysicalDeviceProperties2 : 1;
    ::std::uint32_t Pad0 : 29;
};

[[nodiscard]] eastl::vector<const char*> GetRequestedInstanceExtensions(
    const ::std::uint32_t vulkanVersion,
    ::std::uint32_t* const pExtensionCount,
    EnabledInstanceExtensions& enabledExtensions
) noexcept;

}
