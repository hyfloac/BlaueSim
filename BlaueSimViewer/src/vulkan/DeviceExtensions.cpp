#include "vulkan/DeviceExtensions.hpp"
#include <cstring>

namespace blau::vk {

static constexpr const char* DesiredDeviceExtensions[] = {
};

static constexpr const char* Desired1_0DeviceExtensions[] = {
#if defined(VK_KHR_maintenance1) && VK_KHR_maintenance1
    VK_KHR_MAINTENANCE1_EXTENSION_NAME,
#endif
#if defined(VK_KHR_bind_memory2) && VK_KHR_bind_memory2
    VK_KHR_BIND_MEMORY_2_EXTENSION_NAME
#endif
};

static constexpr const char* Desired1_1DeviceExtensions[] = {
};

static constexpr const char* Desired1_2DeviceExtensions[] = {
};

static constexpr const char* Desired1_3DeviceExtensions[] = {
};

[[nodiscard]] static eastl::vector<VkExtensionProperties> GetDeviceExtensions(VkPhysicalDevice physicalDevice) noexcept
{
    ::std::uint32_t extensionCount;
    VkResult result = vkEnumerateDeviceExtensionProperties(
        physicalDevice,  // physicalDevice
        nullptr,         // pLayerName
        &extensionCount, // pPropertyCount
        nullptr          // pProperties
    );

    if(result != VK_SUCCESS && result != VK_INCOMPLETE)
    {
        return { };
    }

    do
    {
        eastl::vector<VkExtensionProperties> extensions(extensionCount);

        result = vkEnumerateDeviceExtensionProperties(
            physicalDevice,   // physicalDevice
            nullptr,          // pLayerName
            &extensionCount,  // pPropertyCount
            extensions.data() // pProperties
        );

        if(result == VK_SUCCESS)
        {
            return extensions;
        }
    }
    while(result == VK_INCOMPLETE);

    return { };
}

static void ReportEnabledExtension(
    const char* extensionName,
    EnabledDeviceExtensions& enabledExtensions
) noexcept
{
    #if defined(VK_KHR_maintenance1) && VK_KHR_maintenance1
    if(::std::strcmp(extensionName, VK_KHR_MAINTENANCE1_EXTENSION_NAME) == 0)
    {
        enabledExtensions.HasMaintenance1 = true;
        return;
    }
    #endif

    #if defined(VK_KHR_bind_memory2) && VK_KHR_bind_memory2
    if(::std::strcmp(extensionName, VK_KHR_BIND_MEMORY_2_EXTENSION_NAME) == 0)
    {
        enabledExtensions.HasBindMemory2 = true;
        return;
    }
    #endif
}

[[nodiscard]] eastl::vector<const char*> GetRequestedDeviceExtensions(
    VkPhysicalDevice physicalDevice,
    const ::std::uint32_t deviceVulkanVersion,
    ::std::uint32_t* const pExtensionCount,
    EnabledDeviceExtensions& enabledExtensions
) noexcept
{
    if(pExtensionCount)
    {
        *pExtensionCount = 0;
    }

    enabledExtensions.HasMaintenance1 = false;
    enabledExtensions.HasBindMemory2 = false;
    enabledExtensions.Pad0 = 0;

    eastl::vector<VkExtensionProperties> extensions = GetDeviceExtensions(physicalDevice);

    if(extensions.empty())
    {
        return { };
    }

    for(const char* const extensionName : RequiredDeviceExtensions)
    {
        bool found = false;

        for(const VkExtensionProperties& extension : extensions)
        {
            if(::std::strcmp(extension.extensionName, extensionName) == 0)
            {
                found = true;
                break;
            }
        }

        if(!found)
        {
            return { };
        }
    }

    eastl::vector<const char*> enabledDeviceExtensions(extensions.size());

    ::std::size_t insertIndex = 0;

    for(const char* const extensionName : RequiredDeviceExtensions)
    {
        enabledDeviceExtensions[insertIndex++] = extensionName;
    }

    for(const char* extensionName : DesiredDeviceExtensions)
    {
        for(const VkExtensionProperties& availableExtension : extensions)
        {
            if(::std::strcmp(extensionName, availableExtension.extensionName) == 0)
            {
                ReportEnabledExtension(extensionName, enabledExtensions);

                enabledDeviceExtensions[insertIndex++] = extensionName;
                break;
            }
        }
    }

    if(deviceVulkanVersion == VK_API_VERSION_1_0)
    {
        for(const char* const extensionName : Desired1_0DeviceExtensions)
        {
            for(const VkExtensionProperties& availableExtension : extensions)
            {
                if(::std::strcmp(extensionName, availableExtension.extensionName) == 0)
                {
                    enabledDeviceExtensions[insertIndex++] = extensionName;
                    break;
                }
            }
        }
    }
    else if(deviceVulkanVersion == VK_API_VERSION_1_1)
    {
        for(const char* const extensionName : Desired1_1DeviceExtensions)
        {
            for(const VkExtensionProperties& availableExtension : extensions)
            {
                if(::std::strcmp(extensionName, availableExtension.extensionName) == 0)
                {
                    enabledDeviceExtensions[insertIndex++] = extensionName;
                    break;
                }
            }
        }
    }
    else if(deviceVulkanVersion == VK_API_VERSION_1_2)
    {
        for(const char* const extensionName : Desired1_2DeviceExtensions)
        {
            for(const VkExtensionProperties& availableExtension : extensions)
            {
                if(::std::strcmp(extensionName, availableExtension.extensionName) == 0)
                {
                    enabledDeviceExtensions[insertIndex++] = extensionName;
                    break;
                }
            }
        }
    }
    else if(deviceVulkanVersion == VK_API_VERSION_1_3)
    {
        for(const char* const extensionName : Desired1_3DeviceExtensions)
        {
            for(const VkExtensionProperties& availableExtension : extensions)
            {
                if(::std::strcmp(extensionName, availableExtension.extensionName) == 0)
                {
                    enabledDeviceExtensions[insertIndex++] = extensionName;
                    break;
                }
            }
        }
    }

    if(pExtensionCount)
    {
        *pExtensionCount = static_cast<::std::uint32_t>(insertIndex);
    }

    return enabledDeviceExtensions;
}

}
