//
// Created by meo-dragon on 7/30/2025.
//

#ifndef GRX_H
#define GRX_H
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
typedef struct grx
{
    VkInstance instance;
    VkDebugUtilsMessengerEXT debug_messenger;
    VkPhysicalDevice physical_device;
} GRX;

int init_grx(GRX *);
void free_grx(GRX *);
#endif //GRX_H
