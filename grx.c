//
// Created by meo-dragon on 7/30/2025.
//
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "grx.h"

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT msg_severity,
                                                     VkDebugUtilsMessageTypeFlagsEXT msgType, const VkDebugUtilsMessengerCallbackDataEXT *callback_data, void *user_data)
{
    printf("validation layer: %s\n", callback_data->pMessage);
    return VK_FALSE;
}

int support_validation_layers(const uint32_t count, const char *layers[])
{
    uint32_t layer_count = 0;
    vkEnumerateInstanceLayerProperties(&layer_count, NULL);
    if (layer_count == 0) return 0;

    VkLayerProperties *available_layers = calloc(layer_count, sizeof(VkLayerProperties));
    if (available_layers == NULL) return 0;

    vkEnumerateInstanceLayerProperties(&layer_count, available_layers);
    for (uint32_t i = 0; i < count; i++)
    {
        bool found = false;
        for (uint32_t j = 0; j < layer_count; j++)
        {
            if (strcmp(layers[i], available_layers[j].layerName) == 0)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            printf("[%s:%d] layer %s could not be found\n", __func__, __LINE__, layers[i]);
            free(available_layers);
            return 0;
        }
    }
    free(available_layers);
    return 1;
}

int support_extensions(const uint32_t count, const char **extensions) {
    uint32_t extension_count = 0;
    vkEnumerateInstanceExtensionProperties(NULL, &extension_count, NULL);
    if (extension_count == 0) return 0;

    VkExtensionProperties* supported_extensions = calloc(extension_count, sizeof(VkExtensionProperties));
    if (supported_extensions == NULL) return 0;

    vkEnumerateInstanceExtensionProperties(NULL, &extension_count, supported_extensions);
    for (uint32_t i = 0; i < count; ++i) {
        bool found = false;
        for (uint32_t j = 0; j < extension_count; ++j)
        {
            if (strcmp(extensions[i], supported_extensions[j].extensionName) == 0)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            printf("[%s:%d] extension %s could not be found\n", __func__, __LINE__, extensions[i]);
            free(supported_extensions);
            return 0;
        }
    }
    free(supported_extensions);
    return 1;
}

void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT *create_info)
{
    create_info->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    create_info->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    create_info->pfnUserCallback = debug_callback;
}

int create_instance(GRX *grx)
{
    const char *validation_layers[] = { "VK_LAYER_KHRONOS_validation" };
    uint32_t layer_count = sizeof(validation_layers) / sizeof(const char *);
    if (!support_validation_layers(layer_count, validation_layers))
    {
        return EXIT_FAILURE;
    }

    const VkApplicationInfo app_info = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = NULL,
        .pApplicationName = "qcr",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "No Engine",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_0
    };

    const char *extensions[] = {
        VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef VK_USE_PLATFORM_WIN32_KHR
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    };
    const uint32_t extensionCount = sizeof(extensions) / sizeof(extensions[0]);
    if (!support_extensions(extensionCount, extensions))
    {
        return EXIT_FAILURE;
    }

    VkInstanceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;
    create_info.enabledExtensionCount = extensionCount;
    create_info.ppEnabledExtensionNames = extensions;
    VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {};
    populate_debug_messenger_create_info(&debug_create_info);
    create_info.pNext = &debug_create_info;
    create_info.enabledLayerCount = layer_count;
    create_info.ppEnabledLayerNames = validation_layers;

    if (vkCreateInstance(&create_info, NULL, &grx->instance) != VK_SUCCESS)
    {
        printf("[%s:%d] vkCreateInstance failed\n", __func__, __LINE__);
        return EXIT_FAILURE;
    }

    return 0;
}

VkResult create_debug_utils_messenger_ext(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* create_info,
    const VkAllocationCallbacks* allocator, VkDebugUtilsMessengerEXT *debug_messenger)
{
    void *func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func == NULL)
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
    printf("[%s:%d] create debug messenger\n", __func__, __LINE__);
    return ((PFN_vkCreateDebugUtilsMessengerEXT)func)(instance, create_info, allocator, debug_messenger);
}

void destroy_debug_utils_messenger_ext(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger,
    const VkAllocationCallbacks* allocator)
{
    void *func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != NULL)
    {
        ((PFN_vkDestroyDebugUtilsMessengerEXT)func)(instance, debug_messenger, allocator);
    }
}

int setup_debug_messenger(GRX *grx)
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
    populate_debug_messenger_create_info(&createInfo);

    if (create_debug_utils_messenger_ext(grx->instance, &createInfo, NULL, &grx->debug_messenger) != VK_SUCCESS)
    {
        printf("[%s:%d] vkCreateDebugUtilsMessengerEXT failed\n", __func__, __LINE__);
        return EXIT_FAILURE;
    }
    printf("[%s:%d] setup debug messenger\n", __func__, __LINE__);
    return EXIT_SUCCESS;
}

bool support_device_extensions(VkPhysicalDevice physical_device, uint32_t count, const char **extensions)
{
    uint32_t extension_count = 0;
    vkEnumerateDeviceExtensionProperties(physical_device, VK_NULL_HANDLE, &extension_count, NULL);
    if (extension_count == 0) return false;

    VkExtensionProperties *supported_extensions = calloc(extension_count, sizeof(VkExtensionProperties));
    if (supported_extensions == NULL) return false;
    vkEnumerateDeviceExtensionProperties(physical_device, VK_NULL_HANDLE, &extension_count, supported_extensions);

    bool found = true;
    for (uint32_t i = 0; i < count; i++)
    {
        bool flag = false;
        for (uint32_t j = 0; j < extension_count; j++)
        {
            if (strcmp(extensions[i], supported_extensions[j].extensionName) == 0)
            {
                flag = true;
                break;
            }
        }
        if (!flag)
        {
            printf("[%s:%d] extension %s could not be found\n", __func__, __LINE__, extensions[i]);
            found = false;
            break;
        }
    }

    free(supported_extensions);
    return found;
}

int pick_physical_device(GRX *grx)
{
    const char *device_extensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    uint32_t device_extension_count = sizeof(device_extensions) / sizeof(const char *);
    int flag = 0;
    grx->physical_device = VK_NULL_HANDLE;

    uint32_t physical_device_count = 0;
    vkEnumeratePhysicalDevices(grx->instance, &physical_device_count, NULL);
    if (physical_device_count == 0)
    {
        printf("[%s:%d] failed to find GPUs with Vulkan support\n", __func__, __LINE__);
        return flag;
    }
    VkPhysicalDevice* physical_devices = calloc(physical_device_count, sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(grx->instance, &physical_device_count, physical_devices);
    for (uint32_t i = 0; i < physical_device_count; i++)
    {
        uint32_t property_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physical_devices[i], &property_count, NULL);
        if (property_count == 0) continue;
        VkQueueFamilyProperties* properties = calloc(property_count, sizeof(VkQueueFamilyProperties));
        vkGetPhysicalDeviceQueueFamilyProperties(physical_devices[i], &property_count, properties);
        for (uint32_t j = 0; j < property_count; j++)
        {
            if (properties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                grx->graphics_queue_index = i;
                flag = 1;
                printf("[%s:%d] physical device found at index %d\n", __func__, __LINE__, i);
            }
        }
        free(properties);

        VkBool32 present_support = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(physical_devices[i], i, grx->surface, &present_support);
        if (present_support)
        {
            grx->present_queue_index = i;
        }

        if (grx->graphics_queue_index == grx->present_queue_index && support_device_extensions(physical_devices[i], device_extension_count, device_extensions))
        {
            grx->physical_device = physical_devices[i];
            break;
        }
    }

    if (grx->physical_device == VK_NULL_HANDLE)
    {
        printf("[%s:%d] failed to find a suitable GPU\n", __func__, __LINE__);
    }

    free(physical_devices);
    return flag;
}

void create_surface(GRX *grx, const SURFACE *surface)
{
#ifdef VK_USE_PLATFORM_WIN32_KHR
    VkWin32SurfaceCreateInfoKHR create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    create_info.hwnd = surface->hwnd;
    create_info.hinstance = surface->h_instance;

    if (vkCreateWin32SurfaceKHR(grx->instance, &create_info, NULL, &grx->surface) != VK_SUCCESS)
    {
        printf("[%s:%d] failed to create surface\n", __func__, __LINE__);
    }
#endif
}

void create_logical_device(GRX *grx)
{
    VkDeviceQueueCreateInfo queue_create_info = {};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = grx->graphics_queue_index;
    queue_create_info.queueCount = 1;

    float queue_priority = 1.0f;
    queue_create_info.pQueuePriorities = &queue_priority;

    VkPhysicalDeviceFeatures device_features = {};

    VkDeviceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.queueCreateInfoCount = 1;
    create_info.pQueueCreateInfos = &queue_create_info;
    create_info.pEnabledFeatures = &device_features;
    create_info.enabledExtensionCount = 0;

    if (vkCreateDevice(grx->physical_device, &create_info, NULL, &grx->logical_device) != VK_SUCCESS)
    {
        printf("[%s:%d] failed to create logical device\n", __func__, __LINE__);
    }

    vkGetDeviceQueue(grx->logical_device, grx->graphics_queue_index, 0, &grx->graphics_queue);
    vkGetDeviceQueue(grx->logical_device, grx->present_queue_index, 0, &grx->present_queue);
}

int init_grx(GRX *grx, const SURFACE *surface)
{
    if (create_instance(grx)) return 1;

    if (setup_debug_messenger(grx)) return 1;

    create_surface(grx, surface);

    if (!pick_physical_device(grx)) return 1;

    return 0;
}

void free_grx(GRX *grx)
{
    vkDestroyDevice(grx->logical_device, NULL);
    // vkDeviceWaitIdle(grx->device);
    destroy_debug_utils_messenger_ext(grx->instance, grx->debug_messenger, NULL);
    vkDestroySurfaceKHR(grx->instance, grx->surface, NULL);
    vkDestroyInstance(grx->instance, NULL);
}
