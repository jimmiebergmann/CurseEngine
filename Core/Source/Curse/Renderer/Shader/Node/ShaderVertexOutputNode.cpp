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

#include "Curse/Renderer/Shader/Node/ShaderVertexOutputNode.hpp"

namespace Curse
{

    namespace Shader
    {

        // Vertex output node implementations.
        NodeType VertexOutputNode::GetType() const
        {
            return NodeType::VertexOutput;
        }

        size_t VertexOutputNode::GetInputPinCount() const
        {
            return 1;
        }

        Pin* VertexOutputNode::GetInputPin(const size_t index)
        {
            if (index != 0)
            {
                return nullptr;
            }
            return &m_input;
        }
        const Pin* VertexOutputNode::GetInputPin(const size_t index) const
        {
            if (index != 0)
            {
                return nullptr;
            }
            return &m_input;
        }

        std::vector<Pin*> VertexOutputNode::GetInputPins()
        {
            return { &m_input };
        }
        std::vector<const Pin*> VertexOutputNode::GetInputPins() const
        {
            return { &m_input };
        }

        VertexOutputNode::VertexOutputNode(Script& script) :
            Node(script),
            m_input(*this)
        { }

        VertexOutputNode::~VertexOutputNode()
        { }

    }

}