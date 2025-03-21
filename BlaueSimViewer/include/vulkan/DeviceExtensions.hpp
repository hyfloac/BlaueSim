#pragma once

#include <vulkan/vulkan.h>
#include <EASTL/vector.h>
#include <cstdint>

namespace blau::vk {

static constexpr const char* RequiredDeviceExtensions[] = {
#if defined(VK_KHR_swapchain) && VK_KHR_swapchain
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
#endif
};

struct EnabledDeviceExtensions final
{
    ::std::uint32_t HasMaintenance1 : 1;
    ::std::uint32_t HasBindMemory2 : 1;
    ::std::uint32_t Pad0 : 30;
};

[[nodiscard]] eastl::vector<const char*> GetRequestedDeviceExtensions(
    VkPhysicalDevice physicalDevice,
    const ::std::uint32_t deviceVulkanVersion,
    ::std::uint32_t* const pExtensionCount,
    EnabledDeviceExtensions& enabledExtensions
) noexcept;

}
