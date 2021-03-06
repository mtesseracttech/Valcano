//
// Created by MTesseracT on 02/01/2020.
//

#ifndef VALCANO_VULKANDEBUG_HPP
#define VALCANO_VULKANDEBUG_HPP

#include "gfx/vulkan/display/Instance.hpp"

namespace mt::gfx::mtvk {
class VulkanDebug {
protected:
    std::shared_ptr<Instance> instance;
    vk::DebugUtilsMessengerEXT debug_messenger;

    static VKAPI_ATTR vk::Bool32 VKAPI_CALL vulkan_debug_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT vk_message_severity,
        VkDebugUtilsMessageTypeFlagsEXT vk_message_type,
        const VkDebugUtilsMessengerCallbackDataEXT* vk_callback_data,
        void* vk_user_data)
    {
        auto message_severity = vk::DebugUtilsMessageSeverityFlagsEXT(vk_message_severity);
        auto message_type = vk::DebugUtilsMessageTypeFlagsEXT(vk_message_type);
        auto message_callback_data = vk::DebugUtilsMessengerCallbackDataEXT(*vk_callback_data);

        (void)vk_user_data; // To get the compiler to stop complaining about an
            // unused variable.

        LogType log_level = Info;
        if ((uint32_t)message_severity >= (uint32_t)vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning) {
            log_level = Error;
        }

        std::string severity_string = vk::to_string(message_severity);
        severity_string = to_upper(severity_string.substr(2, severity_string.size() - 4));

        std::string type_string = vk::to_string(message_type);
        type_string = to_upper(type_string.substr(2, type_string.size() - 4));

        std::stringstream message;
        message << "[VK-" << type_string << "][" << severity_string << "]:";

        Logger::log(message, log_level);
        Logger::log(message_callback_data.pMessage, log_level);

        return false;
    }

    static vk::Result create_debug_utils_messenger_ext(
        vk::Instance instance,
        const vk::DebugUtilsMessengerCreateInfoEXT* create_info,
        const vk::AllocationCallbacks* allocator,
        vk::DebugUtilsMessengerEXT* debug_messenger)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)instance.getProcAddr(
            "vkCreateDebugUtilsMessengerEXT");

        if (func != nullptr) {
            return vk::Result(func(
                instance,
                reinterpret_cast<const VkDebugUtilsMessengerCreateInfoEXT*>(
                    create_info),
                reinterpret_cast<const VkAllocationCallbacks*>(allocator),
                reinterpret_cast<VkDebugUtilsMessengerEXT_T**>(debug_messenger)));
        } else {
            return vk::Result::eErrorExtensionNotPresent;
        }
    }

    static void destroy_debug_utils_messenger_ext(
        vk::Instance instance, vk::DebugUtilsMessengerEXT debug_messenger,
        const vk::AllocationCallbacks* allocator)
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)instance.getProcAddr(
            "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, static_cast<VkDebugUtilsMessengerEXT>(debug_messenger),
                reinterpret_cast<const VkAllocationCallbacks*>(allocator));
        }
    }

public:
    explicit VulkanDebug(const std::shared_ptr<Instance>& instance,
        bool verbose_mode = false)
        : instance(instance)
    {
        assert(instance);
        vk::DebugUtilsMessengerCreateInfoEXT create_info;
        create_info.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
        if (verbose_mode)
            create_info.messageSeverity |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo;
        create_info.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT ::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT ::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT ::ePerformance;
        create_info.pfnUserCallback = vulkan_debug_callback;
        create_info.pUserData = this;

        if (create_debug_utils_messenger_ext(instance->get_instance(), &create_info,
                nullptr, &debug_messenger)
            != vk::Result::eSuccess) {
            throw std::runtime_error("Failed to set up debug messenger!");
        }

        Logger::log("Set up Vulkan Debug Messenger", Info);
    }

    ~VulkanDebug() = default;

    void destroy()
    {
        Logger::log("Destroyed Vulkan Debug Messenger", Info);
        destroy_debug_utils_messenger_ext(instance->get_instance(), debug_messenger,
            nullptr);
    }
};
} // namespace mt::gfx::mtvk

#endif // VALCANO_DEBUG_HPP
