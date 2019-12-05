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

CURSE_UNSCOPED_ENUM_BEGIN

namespace Curse
{

    class PipelineVulkan;
    class FramebufferVulkan;

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
        RendererVulkan(const Window& window, const Version& version, Logger* logger = nullptr);

        /**
        * @brief Virtual destructor.
        */
        ~RendererVulkan();

        /**
        * @brief Opens renderer.
        *
        * @param window[in] Render target window.
        */
        virtual bool Open(const Window& window, const Version& version = Version::None, Logger* logger = nullptr) override;

        /**
        * @brief Closing renderer.
        */
        virtual void Close() override;

        /**
        * @brief Resize the framebuffers.
        *        Execute this function as soon as the render target's work area is resized.
        */
        virtual void Resize(const Vector2ui32& size) override;

        /**
        * @brief Get backend API type.
        */
        virtual BackendApi GetBackendApi() const override;

        /**
        * @brief Get renderer API version.
        */
        virtual Version GetVersion() const override;

        /**
        * @brief Compile shader.
        *        Compilation of shaders makes it possible to convert from GLSL to SPIR-V.
        */
        virtual std::vector<uint8_t> CompileShader(const Shader::Format inputFormat, const Shader::Type inputType,
                                                   const std::vector<uint8_t>& inputData, const Shader::Format outputFormat) override;

        /**
        * @brief Create framebuffer object.
        */
        virtual Framebuffer* CreateFramebuffer(const FramebufferDescriptor& descriptor) override;

        /**
        * @brief Create index buffer object.
        */
        virtual IndexBuffer* CreateIndexBuffer(const IndexBufferDescriptor& descriptor) override;

        /**
        * @brief Create pipeline object.
        */
        virtual Pipeline* CreatePipeline(const PipelineDescriptor& descriptor) override;

        /**
        * @brief Create shader object.
        */
        virtual Shader* CreateShader(const ShaderDescriptor& descriptor) override;

        /**
        * @brief Create texture object.
        */
        virtual Texture* CreateTexture() override;

        /**
        * @brief Create vertex buffer object.
        */
        virtual VertexBuffer* CreateVertexBuffer(const VertexBufferDescriptor& descriptor) override;


        /**
        * @brief Destroy framebuffer object.
        */
        virtual void DestroyFramebuffer(Framebuffer* framebuffer) override;
        
        /**
        * @brief Destroy index buffer object.
        */
        virtual void DestroyIndexBuffer(IndexBuffer* indexBuffer) override;

        /**
        * @brief Destroy pipeline object.
        */
        virtual void DestroyPipeline(Pipeline* pipeline) override;

        /**
        * @brief Destroy shader object.
        */
        virtual void DestroyShader(Shader* shader) override;

        /**
        * @brief Destroy texture object.
        */
        virtual void DestroyTexture(Texture* texture) override;

        /**
        * @brief Destroy vertex buffer object.
        */
        virtual void DestroyVertexBuffer(VertexBuffer* vertexBuffer) override;


        /**
        * @brief Bind pipeline to draw queue.
        */
        virtual void BindPipeline(Pipeline* pipeline) override;

        /**
        * @brief Begin and initialize rendering to framebuffers.
        */
        virtual void BeginDraw() override;

        /**
        * @brief Draw vertex buffer, using the current bound pipeline.
        */
        virtual void DrawVertexBuffer(VertexBuffer* vertexBuffer) override;

        /**
        * @brief Draw multiple vertex buffers, using the current bound pipeline.
        */
        virtual void DrawVertexBuffers(VertexBuffer* vertexBuffers, const size_t count) override;

        /**
        * @brief Draw multiple indexed vertex buffers, using the current bound pipeline.
        */
        virtual void DrawVertexBuffers(IndexBuffer* indexBuffer, VertexBuffer* vertexBuffers, const size_t count) override;

        /**
        * @brief Finalize and present rendering.
        */
        virtual void EndDraw() override;
        
        /**
        * @brief Sleep until the graphical device is ready.
        */
        virtual void WaitForDevice() override;

    private:

        struct DebugMessenger
        {
            DebugMessenger();
            void Clear();

            VkDebugUtilsMessengerEXT messenger;
            PFN_vkCreateDebugUtilsMessengerEXT CreateDebugUtilsMessengerEXT;
            PFN_vkDestroyDebugUtilsMessengerEXT DestroyDebugUtilsMessengerEXT;
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

        struct ResourceCounter
        {
            ResourceCounter();
            void Clear(Logger* logger);

            uint32_t framebufferCount;
            uint32_t indexBufferCount;
            uint32_t pipelineCount;
            uint32_t shaderCount;
            uint32_t textureCount;
            uint32_t vertexBufferCount;
        };

        PFN_vkVoidFunction GetVulkanFunction(const char* functionName) const;
        bool LoadInstance(const Version& version);
        bool GetRequiredExtensions(std::vector<std::string>& extensions, const bool requestDebugger) const;
        bool LoadDebugger(VkInstanceCreateInfo& instanceInfo, VkDebugUtilsMessengerCreateInfoEXT& debugMessageInfo);
        bool LoadSurface();
        bool LoadPhysicalDevice();
        bool ScorePhysicalDevice(PhysicalDevice& physicalDevice, uint32_t & score);
        bool CheckDeviceExtensionSupport(PhysicalDevice & physicalDevice);
        bool FetchSwapChainSupport(PhysicalDevice& physicalDevice);
        bool LoadLogicalDevice();
        bool LoadSwapChain();
        bool LoadImageViews();
        bool LoadRenderPass();
        bool LoadPresentFramebuffer();
        bool LoadCommandPool();
        bool LoadSyncObjects();
        bool RecreateSwapChain();
        void UnloadSwapchain();
        bool FindPhysicalDeviceMemoryType(uint32_t& index, const uint32_t filter, const VkMemoryPropertyFlags properties);
 
        Logger* m_logger;
        Version m_version;
        const Window* m_renderTarget;
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
        VkRenderPass m_renderPass;
        std::vector<FramebufferVulkan*> m_presentFramebuffers;
        VkCommandPool m_commandPool;
        std::vector<VkCommandBuffer> m_commandBuffers;
        std::vector<VkSemaphore> m_imageAvailableSemaphores;
        std::vector<VkSemaphore> m_renderFinishedSemaphores;
        std::vector<VkFence> m_inFlightFences;
        std::vector<VkFence> m_imagesInFlight;
        size_t m_maxFramesInFlight;
        size_t m_currentFrame;
        ResourceCounter m_resourceCounter;

        bool m_resized;
        bool m_beginDraw;
        uint32_t m_currentImageIndex;
        VkCommandBuffer* m_currentCommandBuffer;
        VkFramebuffer* m_currentFramebuffer;
        
        
    };

}

CURSE_UNSCOPED_ENUM_END

#endif

#endif