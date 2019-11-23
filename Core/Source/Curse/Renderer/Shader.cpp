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

#include "Curse/Renderer/Shader.hpp"

namespace Curse
{

    ShaderDescriptor::ShaderDescriptor() :
        type(Shader::Type::Vertex),
        filename(nullptr),
        data(nullptr),
        dataSize(0)
    { }

    ShaderDescriptor::ShaderDescriptor(const Shader::Type type, const char* filename) :
        type(type),
        filename(filename),
        data(nullptr),
        dataSize(0)
    { }

    ShaderDescriptor::ShaderDescriptor(const Shader::Type type, const uint8_t* data, const size_t dataSize) :
        type(type),
        filename(nullptr),
        data(data),
        dataSize(dataSize)
    { }

    Shader::Type Shader::GetType() const
    {
        return type;
    }

}
