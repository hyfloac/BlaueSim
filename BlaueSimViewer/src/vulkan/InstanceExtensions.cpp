#include "vulkan/InstanceExtensions.hpp"
#include <cstring>

namespace blau::vk {

static constexpr const char* DesiredInstanceExtensions[] = {
#if defined(VK_EXT_debug_utils) && VK_EXT_debug_utils
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
#if defined(VK_KHR_get_surface_capabilities2) && VK_KHR_get_surface_capabilities2
    VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME,
#endif
};

static constexpr const char* Desired1_0InstanceExtensions[] = {
#if defined(VK_KHR_get_physical_device_properties2) && VK_KHR_get_physical_device_properties2
    VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
#endif
};

static constexpr const char* Desired1_1InstanceExtensions[] = {
};

static constexpr const char* Desired1_2InstanceExtensions[] = {
};

static constexpr const char* Desired1_3InstanceExtensions[] = {
};

[[nodiscard]] static eastl::vector<VkExtensionProperties> GetInstanceExtensions() noexcept
{
    ::std::uint32_t extensionCount;
    VkResult result = vkEnumerateInstanceExtensionProperties(
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

        result = vkEnumerateInstanceExtensionProperties(
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
    EnabledInstanceExtensions& enabledExtensions
) noexcept
{
    #if defined(VK_EXT_debug_utils) && VK_EXT_debug_utils
    if(::std::strcmp(extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0)
    {
        enabledExtensions.HasDebugUtils = true;
        return;
    }
    #endif

    #if defined(VK_KHR_get_surface_capabilities2) && VK_KHR_get_surface_capabilities2
    if(::std::strcmp(extensionName, VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME) == 0)
    {
        enabledExtensions.HasGetSurfaceCapabilities2 = true;
        return;
    }
    #endif

    #if defined(VK_KHR_get_physical_device_properties2) && VK_KHR_get_physical_device_properties2
    if(::std::strcmp(extensionName, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME) == 0)
    {
        enabledExtensions.HasGetPhysicalDeviceProperties2 = true;
        return;
    }
    #endif
}

[[nodiscard]] eastl::vector<const char*> GetRequestedInstanceExtensions(
    const ::std::uint32_t vulkanVersion,
    ::std::uint32_t* const pExtensionCount,
    EnabledInstanceExtensions& enabledExtensions
) noexcept
{
    if(pExtensionCount)
    {
        *pExtensionCount = 0;
    }

    enabledExtensions.HasDebugUtils = false;
    enabledExtensions.Pad0 = 0;

    eastl::vector<VkExtensionProperties> extensions = GetInstanceExtensions();

    if(extensions.empty())
    {
        return { };
    }

    for(const char* const extensionName : RequiredInstanceExtensions)
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

    eastl::vector<const char*> enabledInstanceExtensions(extensions.size());

    ::std::size_t insertIndex = 0;

    for(const char* const extensionName : RequiredInstanceExtensions)
    {
        enabledInstanceExtensions[insertIndex++] = extensionName;
    }

    for(const char* extensionName : DesiredInstanceExtensions)
    {
        for(const VkExtensionProperties& availableExtension : extensions)
        {
            if(::std::strcmp(extensionName, availableExtension.extensionName) == 0)
            {
                ReportEnabledExtension(extensionName, enabledExtensions);

                enabledInstanceExtensions[insertIndex++] = extensionName;
                break;
            }
        }
    }

    if(vulkanVersion == VK_API_VERSION_1_0)
    {
        for(const char* const extensionName : Desired1_0InstanceExtensions)
        {
            for(const VkExtensionProperties& availableExtension : extensions)
            {
                if(::std::strcmp(extensionName, availableExtension.extensionName) == 0)
                {
                    enabledInstanceExtensions[insertIndex++] = extensionName;
                    break;
                }
            }
        }
    }
    else if(vulkanVersion == VK_API_VERSION_1_1)
    {
        for(const char* const extensionName : Desired1_1InstanceExtensions)
        {
            for(const VkExtensionProperties& availableExtension : extensions)
            {
                if(::std::strcmp(extensionName, availableExtension.extensionName) == 0)
                {
                    enabledInstanceExtensions[insertIndex++] = extensionName;
                    break;
                }
            }
        }
    }
    else if(vulkanVersion == VK_API_VERSION_1_2)
    {
        for(const char* const extensionName : Desired1_2InstanceExtensions)
        {
            for(const VkExtensionProperties& availableExtension : extensions)
            {
                if(::std::strcmp(extensionName, availableExtension.extensionName) == 0)
                {
                    enabledInstanceExtensions[insertIndex++] = extensionName;
                    break;
                }
            }
        }
    }
    else if(vulkanVersion == VK_API_VERSION_1_3)
    {
        for(const char* const extensionName : Desired1_3InstanceExtensions)
        {
            for(const VkExtensionProperties& availableExtension : extensions)
            {
                if(::std::strcmp(extensionName, availableExtension.extensionName) == 0)
                {
                    enabledInstanceExtensions[insertIndex++] = extensionName;
                    break;
                }
            }
        }
    }

    if(pExtensionCount)
    {
        *pExtensionCount = static_cast<::std::uint32_t>(insertIndex);
    }

    return enabledInstanceExtensions;
}

}
