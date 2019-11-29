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

#ifndef CURSE_CORE_RENDERER_FRAMEBUFFER_HPP
#define CURSE_CORE_RENDERER_FRAMEBUFFER_HPP

#include "Curse/Renderer/Resource.hpp"
#include "Curse/Math/Vector.hpp"

namespace Curse
{

    /**
    * @brief Framebuffer base class.
    */
    class CURSE_API Framebuffer
    {

    protected:

        Framebuffer() = default;
        Framebuffer(const Framebuffer&) = delete;
        Framebuffer(Framebuffer&&) = delete;
        virtual ~Framebuffer() = default;

        Resource resource;

    };

    /**
    * @brief Descriptor class of framebuffer class.
    */
    class CURSE_API FramebufferDescriptor
    {

    public:

        FramebufferDescriptor() = default;

        Resource image;
        Vector2ui32 size;

    };

}

#endif