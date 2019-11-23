/*
* MIT License
*
* Copyright (c) 2019 Jimmie Bergmann
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files(the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions :
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*/

#ifndef CURSE_CORE_RENDERER_VULKAN_RENDERERVULKAN_HPP
#define CURSE_CORE_RENDERER_VULKAN_RENDERERVULKAN_HPP

#include "Curse/Core.hpp"

#if defined(CURSE_ENABLE_VULKAN)

#include "Curse/Renderer/Renderer.hpp"
#include "Curse/Renderer/Vulkan/VulkanHeaders.hpp"
#include <vector>

namespace Curse
{

    /**
    * @brief Vulkan renderer class.
    */
    class CURSE_API RendererVulkan : public Renderer
    {

    public:

        /**
        * @brief Constructor.
        */
        RendererVulkan();

        /**
        * @brief Constructs and creates renderer.
        *
        * @param window[in] Render target window.
        */
        RendererVulkan(const WindowBase& window, const Version& version, DebugCallback debugCallback = nullptr);

        /**
        * @brief Virtual destructor.
        */
        ~RendererVulkan();

        /**
        * @brief Opens renderer.
        *
        * @param window[in] Render target window.
        */
        virtual void Open(const WindowBase& window, const Version& version = Version::None, DebugCallback debugCallback = nullptr) override;

        /**
        * @brief Closing renderer.
        */
        virtual void Close() override;

        /**
        * @brief Get backend API type.
        */
        virtual BackendApi GetBackendApi() const override;

        /**
        * @brief Get renderer API version.
        */
        virtual Version GetVersion() const override;

        /**
        * @brief Swap buffers.
        *        Necessary to do in order to present the frame, if double/tripple buffering is used.
        */
        virtual void SwapBuffers() override;

        /**
        * @brief Create shader object.
        */
        virtual Shader* CreateShader(const ShaderDescriptor& descriptor) override;

        /**
        * @brief Delete shader object.
        */
        virtual void DeleteShader(Shader* shader) override;

        /**
        * @brief Create texture object.
        */
        virtual Texture* CreateTexture() override;

        /**
        * @brief Delete texture object.
        */
        virtual void DeleteTexture(Texture* texture) override;

    private:

        struct DebugMessenger
        {
            DebugMessenger();
            void Clear();

            VkDebugUtilsMessengerEXT messenger;
            PFN_vkCreateDebugUtilsMessengerEXT CreateDebugUtilsMessengerEXT;
            PFN_vkDestroyDebugUtilsMessengerEXT DestroyDebugUtilsMessengerEXT;
            bool validationDebugger;
            DebugCallback callback;
        };

        struct SwapChainSupport
        {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };

        struct PhysicalDevice
        {
            PhysicalDevice();
            PhysicalDevice(VkPhysicalDevice device);
            PhysicalDevice(VkPhysicalDevice device, uint32_t graphicsQueueIndex, uint32_t presentQueueIndex);
            void Clear();

            VkPhysicalDevice device;
            uint32_t graphicsQueueIndex;
            uint32_t presentQueueIndex;
            SwapChainSupport swapChainSupport;
        };  

        PFN_vkVoidFunction GetVulkanFunction(const char* functionName) const;
        void LoadInstance(const Version& version, DebugCallback debugCallback);
        bool GetRequiredExtensions(std::vector<std::string>& extensions, const bool requestDebugger) const;
        bool LoadDebugger(VkInstanceCreateInfo& instanceInfo, VkDebugUtilsMessengerCreateInfoEXT& debugMessageInfo, DebugCallback debugCallback);     
        void LoadSurface(const WindowBase& window);
        void LoadPhysicalDevice();
        bool ScorePhysicalDevice(PhysicalDevice& physicalDevice, uint32_t & score);
        bool CheckDeviceExtensionSupport(PhysicalDevice & physicalDevice);
        bool FetchSwapChainSupport(PhysicalDevice& physicalDevice);
        void LoadLogicalDevice();
        void LoadSwapChain(const WindowBase& window);
        void LoadImageViews();
        void LoadGraphicsPipeline();

        Version m_version;
        VkInstance m_instance;
        std::vector<const char*> m_validationLayers;
        std::vector<const char*> m_deviceExtensions;
        DebugMessenger m_debugMessenger;
        VkSurfaceKHR m_surface;
        PhysicalDevice m_physicalDevice;
        VkDevice m_logicalDevice;
        VkQueue m_graphicsQueue;
        VkQueue m_presentQueue;
        
        VkSwapchainKHR m_swapChain;
        VkFormat m_swapChainImageFormat;
        VkExtent2D m_swapChainExtent;
        std::vector<VkImage> m_swapChainImages;
        std::vector<VkImageView> m_swapChainImageViews;

    };

}

#endif

#endif