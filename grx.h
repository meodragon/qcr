//
// Created by meo-dragon on 7/30/2025.
//

#ifndef GRX_H
#define GRX_H
#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#else
#endif
#include <vulkan/vulkan.h>

#include "surface.h"

typedef struct grx
{
    VkInstance instance;
    VkDebugUtilsMessengerEXT debug_messenger;
    VkSurfaceKHR surface;
    const uint32_t *surface_width;
    const uint32_t *surface_height;
    VkPhysicalDevice physical_device;
    uint32_t graphics_queue_index;
    uint32_t present_queue_index;
    VkDevice logical_device;
    VkQueue graphics_queue;
    VkQueue present_queue;
    VkSurfaceCapabilitiesKHR *surface_capabilities;
    uint32_t surface_format_count;
    VkSurfaceFormatKHR *surface_formats;
    uint32_t present_mode_count;
    VkPresentModeKHR *present_modes;
    VkSwapchainKHR swap_chain;
    uint32_t image_count;
    VkImage *swap_chain_images;
    VkFormat swap_chain_image_format;
    VkExtent2D swap_chain_extent;
    VkImageView *swap_chain_image_views;
} GRX;

int init_grx(GRX *, const SURFACE *);
void free_grx(GRX *);
#endif //GRX_H
