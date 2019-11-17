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

#ifndef CURSE_CORE_RENDERER_OPENGL_RENDEREROPENGLWIN32_HPP
#define CURSE_CORE_RENDERER_OPENGL_RENDEREROPENGLWIN32_HPP

#include "Curse/Core.hpp"

#if CURSE_OPENGL_IS_AVAILABLE
#if defined(CURSE_PLATFORM_WINDOWS)

#include "Curse/Renderer/Renderer.hpp"
#include "Curse/Platform/Win32Headers.hpp"

namespace Curse
{

    /**
    * @brief OpenGL renderer class for Win32.
    */
    class CURSE_API RendererOpenGLWin32 : public Renderer
    {

    public:

        /**
        * @brief Constructor.
        */
        RendererOpenGLWin32();

        /**
        * @brief Constructs and creates renderer.
        *
        * @param window[in] Render target window.
        */
        RendererOpenGLWin32(const WindowBase & window, const Version& version);

        /**
        * @brief Virtual destructor.
        */
        ~RendererOpenGLWin32();

        /**
        * @brief Opens renderer.
        *
        * @param window[in] Render target window.
        */
        virtual void Open(const WindowBase & window, const Version& version = Version::None);

        /**
        * @brief Closing renderer.
        */
        virtual void Close();

        /**
        * @brief Get renderer API type.
        */
        virtual Type GetType() const;

        /**
        * @brief Get renderer API version.
        */
        virtual Version GetVersion() const;

        /**
        * @brief Swap buffers.
        *        Necessary to do in order to present the frame, if double/tripple buffering is used.
        */
        virtual void SwapBuffers();

    private:

        /**
        * @brief Open opengl context by provided version.
        *
        * @param version[in] Opened version.
        *
        * @throw Exception If failed to open provided version.
        */
        bool OpenVersion(HDC deviceContext, const Version& version);

        /**
        * @brief Open the best available opengl context.
        *
        * @param version[out] Opened version.
        *
        * @throw Exception If failed to open any opengl context.
        */
        void OpenBestVersion(HDC deviceContext, Version& version);

        Version m_version;
        HDC m_deviceContext;
        HGLRC m_context;

    };

}

#endif

#endif

#endif
