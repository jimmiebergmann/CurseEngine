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

#ifndef CURSE_CORE_RENDERER_SHADER_SHADERSCRIPT_HPP
#define CURSE_CORE_RENDERER_SHADER_SHADERSCRIPT_HPP

#include "Curse/Renderer/Shader/ShaderNode.hpp"
#include "Curse/Renderer/Shader.hpp"
#include "Curse/Logger.hpp"
#include <vector>
#include <set>
#include <map>
#include <string>

namespace Curse
{

    namespace Shader
    {

        /**
        * @brief Shader script class, used for generating shaders via virtual script.
        */
        class CURSE_API Script
        {

        public:

            /**
            * @brief Destructor.
            */
            virtual ~Script();

            /**
            * @brief Create new constant node and append it to the shader script.
            *
            * @tparam T Constant node object to create.
            */
            template<typename T>
            ConstantNode<T>* CreateConstantNode(const T& value = PinDefault<T>::Value);

            /**
            * @brief Create new function node and append it to the shader script.
            *
            * @tparam T Function node object to create and data types for parameters.
            */
            template<typename Func>
            auto CreateFunctionNode();

            /**
            * @brief Create new uniform block and append it to the shader script.
            */
            UniformBlock * CreateUniformBlock(const uint32_t id);

            /**
            * @brief Create new operator node and append it to the shader script.
            *
            * @tparam T Operator node object to create.
            */
            template<typename T>
            OperatorNode<T>* CreateOperatorNode(const Operator op);

            /**
            * @brief Create new varying in node and append it to the shader script.
            *        Varying data is sent from the vertex buffer, vertex or geometry shader.
            *
            * @tparam T Varying in node object to create.
            */
            template<typename T>
            VaryingInNode<T>* CreateVaryingInNode();

            /**
            * @brief Create new varying out node and append it to the shader script.
            *        Varying data is sent from the vertex buffer or vertex, geometry or fragment shader.
            *
            * @tparam T Varying out node object to create.
            */
            template<typename T>
            VaryingOutNode<T>* CreateVaryingOutNode();

            /**
            * @brief Removes the node from the script, disconnects all connections of node
            *        and deallocates the pointer.
            */
            void DestroyNode(Node * node);

            /**
            * @brief Get all nodes of shader script.
            */
            std::vector<Node*> GetNodes();

            /**
            * @brief Get all nodes of shader script.
            */
            std::vector<const Node*> GetNodes() const;

            /**
            * @brief Get uniform blocks of shader script.
            */
            std::vector<const UniformBlock*> GetUniformBlocks() const;

            /**
            * @brief Get uniform blocks of shader script.
            */
            std::vector<UniformBlock*> GetUniformBlocks();

            /**
            * @brief Get varying in nodes of shader script.
            */
            std::vector<Node*> GetVaryingInNodes();

            /**
            * @brief Get varying in nodes of shader script.
            */
            std::vector<const Node*> GetVaryingInNodes() const;

            /**
            * @brief Get varying out nodes of shader script.
            */
            std::vector<Node*> GetVaryingOutNodes();

            /**
            * @brief Get varying out nodes of shader script.
            */
            std::vector<const Node*> GetVaryingOutNodes() const;

            /**
            * @brief Generate GLSL code from shader script.
            */
            std::vector<uint8_t> GenerateGlsl(Logger * logger = nullptr) const;

            /**
            * @brief Get type of shader script.
            */
            virtual ShaderType GetType() const = 0;

        protected:

            /**
            * @brief Get vertex output node.
            *
            * @return pointer to vertex output node, nullptr if GetType != ShaderType::Vertex.
            */
            virtual VertexOutputNode* GetVertexOutputNode();

            /**
            * @brief Get vertex output node.
            *
            * @return pointer to vertex output node, nullptr if GetType != ShaderType::Vertex.
            */
            virtual const VertexOutputNode* GetVertexOutputNode() const;

            std::set<Node*> m_allNodes; ///< Set of all nodes of any type.
            std::vector<Node*> m_varyingInNodes;
            std::vector<Node*> m_varyingOutNodes;
            //std::map<size_t, UniformNodeBase*> m_uniformNodes;
            std::map<uint32_t, UniformBlock*> m_uniformBlocks;

        };


        /**
        * @brief Vertex shader script.
        */
        class CURSE_API VertexScript : public Script
        {

        public:

            /**
            * @brief Constructor.
            */
            VertexScript();

            /**
            * @brief Destructor.
            */
            virtual ~VertexScript();

            /**
            * @brief Get type of shader script.
            */
            virtual ShaderType GetType() const override;

            /**
            * @brief Get vertex output node.
            */
            virtual VertexOutputNode* GetVertexOutputNode() override;

            /**
            * @brief Get vertex output node.
            */
            virtual const VertexOutputNode* GetVertexOutputNode() const override;

        private:

            VertexOutputNode m_vertexOutputNode;

        };


        /**
        * @brief Fragment shader script.
        */
        class CURSE_API FragmentScript : public Script
        {

        public:

            /**
            * @brief Destructor.
            */
            virtual ~FragmentScript();

            /**
            * @brief Get type of shader script.
            */
            virtual ShaderType GetType() const override;

        };

    }

}

#include "ShaderScript.inl"

#endif