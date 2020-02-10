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

#include "Curse/Renderer/Shader/ShaderScript.hpp"
#include "Curse/System/Exception.hpp"
#include <stack>
#include <map>
#include <memory>
#include <algorithm>

#define CURSE_SHADERSCRIPT_LOG(severity, message) if(logger){ logger->Write(severity, message); }

namespace Curse
{

    namespace Shader
    {

        // Helper functions.
        static const std::string g_emptyString = "";
        static const std::string g_glslDataTypeBool = "bool";
        static const std::string g_glslDataTypeInt = "int";
        static const std::string g_glslDataTypeFloat = "float";
        static const std::string g_glslDataTypeVec2 = "vec2";
        static const std::string g_glslDataTypeVec3 = "vec3";
        static const std::string g_glslDataTypeVec4 = "vec4";

        static const std::string g_glslFunctionCos = "cos";
        static const std::string g_glslFunctionSin = "sin";
        static const std::string g_glslFunctionTan = "tan";
        static const std::string g_glslFunctionMax = "max";
        static const std::string g_glslFunctionMin = "min";
        static const std::string g_glslFunctionCross = "cross";
        static const std::string g_glslFunctionDot = "dot";

        static const std::string& GetGlslPinTypeString(const PinDataType dataType)
        {
            switch (dataType)
            {
                case PinDataType::Bool:       return g_glslDataTypeBool;
                case PinDataType::Int32:      return g_glslDataTypeInt;
                case PinDataType::Float32:    return g_glslDataTypeFloat;
                case PinDataType::Vector2f32: return g_glslDataTypeVec2;
                case PinDataType::Vector3f32: return g_glslDataTypeVec3;
                case PinDataType::Vector4f32: return g_glslDataTypeVec4;
                default: break;
            }
            return g_emptyString;
        }

        static std::string GetGlslOperatorString(const Operator op)
        {
            switch (op)
            {
                case Operator::Addition:        return " + ";
                case Operator::Subtraction:     return " - ";
                case Operator::Multiplication:  return " * ";
                case Operator::Division:        return " / ";
                default: break;
            }
            return g_emptyString;
        }

        static std::string GetGlslFloatAsString(const float value)
        {
            std::string str = std::to_string(value);
            str.erase(str.find_last_not_of('0') + 1, std::string::npos);
            str.erase(str.find_last_not_of('.') + 1, std::string::npos);
            return str;
        }

        static std::string GetGlslInputPinDefaultValue(const Pin& pin)
        {
            if (pin.GetDirection() != PinDirection::In)
            {
                return "";
            }

            switch (pin.GetDataType())
            {
                case PinDataType::Bool:
                    return static_cast<const InputPin<bool>&>(pin).GetDefaultValue() ? "true" : "false";
                case PinDataType::Int32:
                    return std::to_string(static_cast<const InputPin<int32_t>&>(pin).GetDefaultValue());
                case PinDataType::Float32:
                    return GetGlslFloatAsString(static_cast<const InputPin<float>&>(pin).GetDefaultValue());
                case PinDataType::Vector2f32:
                {
                    auto vec = static_cast<const InputPin<Vector2f32>&>(pin).GetDefaultValue();
                    return "vec2(" + GetGlslFloatAsString(vec.x) + ", " + GetGlslFloatAsString(vec.y) + ")";
                }
                case PinDataType::Vector3f32:
                {
                    auto vec = static_cast<const InputPin<Vector3f32>&>(pin).GetDefaultValue();
                    return "vec3(" + GetGlslFloatAsString(vec.x) + ", " + GetGlslFloatAsString(vec.y) + ", " + GetGlslFloatAsString(vec.z) + ")";
                }
                case PinDataType::Vector4f32:
                {
                    auto vec = static_cast<const InputPin<Vector4f32>&>(pin).GetDefaultValue();
                    return "vec4(" + GetGlslFloatAsString(vec.x) + ", " + GetGlslFloatAsString(vec.y) + ", " + GetGlslFloatAsString(vec.z) + ", " + GetGlslFloatAsString(vec.w) + ")";
                }
                default: break;
            }

            return "";
        }

        static std::string GetGlslConstantValue(const ConstantNodeBase& constant)
        {
            switch (constant.GetDataType())
            {
                case PinDataType::Bool:
                    return static_cast<const ConstantNode<bool>&>(constant).GetValue() ? "true" : "false";
                case PinDataType::Int32:
                    return std::to_string(static_cast<const ConstantNode<int32_t>&>(constant).GetValue());
                case PinDataType::Float32:
                    return GetGlslFloatAsString(static_cast<const ConstantNode<float>&>(constant).GetValue());
                case PinDataType::Vector2f32:
                {
                    auto vec = static_cast<const ConstantNode<Vector2f32>&>(constant).GetValue();
                    return "vec2(" + GetGlslFloatAsString(vec.x) + ", " + GetGlslFloatAsString(vec.y) + ")";
                }
                case PinDataType::Vector3f32:
                {
                    auto vec = static_cast<const ConstantNode<Vector3f32>&>(constant).GetValue();
                    return "vec3(" + GetGlslFloatAsString(vec.x) + ", " + GetGlslFloatAsString(vec.y) + ", " + GetGlslFloatAsString(vec.z) + ")";
                }
                case PinDataType::Vector4f32:
                {
                    auto vec = static_cast<const ConstantNode<Vector4f32>&>(constant).GetValue();
                    return "vec4(" + GetGlslFloatAsString(vec.x) + ", " + GetGlslFloatAsString(vec.y) + ", " + GetGlslFloatAsString(vec.z) + ", " + GetGlslFloatAsString(vec.w) + ")";
                }
                default: break;
            }
            return "";
        }

        static const std::string& GetGlslFunctionName(const FunctionType functionType)
        {
            switch (functionType)
            {
                // Trigonometry
                case FunctionType::Cos: return g_glslFunctionCos;
                case FunctionType::Sin: return g_glslFunctionSin;
                case FunctionType::Tan: return g_glslFunctionTan;
                // Mathematics.
                case FunctionType::Max: return g_glslFunctionMax;
                case FunctionType::Min: return g_glslFunctionMin;
                // Vector.
                case FunctionType::Cross: return g_glslFunctionCross;
                case FunctionType::Dot: return g_glslFunctionDot;
                default: break;
            }

            return g_emptyString;
        }

        static void AppendToVector(std::vector<uint8_t>& vector, const std::string& str)
        {
            std::copy(str.begin(), str.end(), std::back_inserter(vector));
        }


        // Shader script implementations.
        Script::~Script()
        {
            for (auto* node : m_allNodes)
            {
                delete node;
            }
        }

        void Script::DestroyNode(Node* node)
        {
            auto itAll = m_allNodes.find(node);
            if (itAll == m_allNodes.end())
            {
                return;
            }

            m_allNodes.erase(itAll);

            switch (node->GetType())
            {
            case NodeType::VaryingIn:
            {
                auto it = std::find(m_varyingInNodes.begin(), m_varyingInNodes.end(), node);
                if (it != m_varyingInNodes.end())
                {
                    m_varyingInNodes.erase(it);
                }
            }
            break;
            case NodeType::VaryingOut:
            {
                auto it = std::find(m_varyingOutNodes.begin(), m_varyingOutNodes.end(), node);
                if (it != m_varyingOutNodes.end())
                {
                    m_varyingOutNodes.erase(it);
                }
            }
            break;
            /*case NodeType::Uniform:
            {
                UniformNodeBase* uniform = static_cast<UniformNodeBase*>(node);
                auto it = m_uniformNodes.find(uniform->GetLocation());
                if (it != m_uniformNodes.end())
                {
                    m_uniformNodes.erase(it);
                }
            }
            break;*/
            default: break;
            }

            delete node;
        }

        std::vector<Node*> Script::GetNodes()
        {
            return { m_allNodes.begin(), m_allNodes.end() };
        }

        std::vector<const Node*> Script::GetNodes() const
        {
            return { m_allNodes.begin(), m_allNodes.end() };
        }

        std::vector<const UniformBlock*> Script::GetUniformBlocks() const
        {
            std::vector<const UniformBlock*> uniformBlocks;
            for (auto& pair : m_uniformBlocks)
            {
                uniformBlocks.push_back(pair.second);
            }
            return uniformBlocks;
        }

        std::vector<UniformBlock*> Script::GetUniformBlocks()
        {
            std::vector<UniformBlock*> uniformBlocks;
            for (auto& pair : m_uniformBlocks)
            {
                uniformBlocks.push_back(pair.second);
            }
            return uniformBlocks;
        }

        std::vector<Node*> Script::GetVaryingInNodes()
        {
            return m_varyingOutNodes;
        }
        std::vector<const Node*> Script::GetVaryingInNodes() const
        {
            return { m_varyingOutNodes.begin(), m_varyingOutNodes.end() };
        }

        std::vector<Node*> Script::GetVaryingOutNodes()
        {
            return m_varyingOutNodes;
        }
        std::vector<const Node*> Script::GetVaryingOutNodes() const
        {
            return { m_varyingOutNodes.begin(), m_varyingOutNodes.end() };
        }

        std::vector<uint8_t> Script::GenerateGlsl(Logger* /*logger*/) const
        {
            struct Variable
            {
                Variable(const std::string& name, const Node* node, const Pin* pin) :
                    name(name),
                    node(node),
                    pin(pin)
                { }

                std::string name;
                const Node* node;
                const Pin* pin;
            };

            using VariablePtr = std::shared_ptr<Variable>;

            std::vector<VariablePtr> outputVars;
            std::vector<VariablePtr> varyingVars;
            std::map<const Pin*, VariablePtr> visitedInputPins;

            // Get varying in variables.        
            size_t index = 0;
            for (auto* node : m_varyingInNodes)
            {
                for (auto* pin : node->GetOutputPins())
                {
                    const std::string name = "v_var_" + std::to_string(index);
                    VariablePtr var = std::make_shared<Variable>(name, node, pin);
                    varyingVars.push_back(var);
                    visitedInputPins.insert({ pin, var });
                    index++;
                }
            }

            // Get uniform variables.
            // ????


            // Get varying out variables. 
            index = 0;
            for (auto* node : m_varyingOutNodes)
            {
                for (auto* pin : node->GetInputPins())
                {
                    const std::string name = "o_var_" + std::to_string(index);
                    VariablePtr var = std::make_shared<Variable>(name, node, pin);
                    outputVars.push_back({ var });
                    index++;
                }
            }           

            // Set version and extensions.
            std::vector<uint8_t> source;
            AppendToVector(source, "#version 450\n");
            AppendToVector(source, "#extension GL_ARB_separate_shader_objects : enable\n");

            // Varying input variables.
            index = 0;
            for (auto& var : varyingVars)
            {
                AppendToVector(source,
                    "layout(location = " + std::to_string(index) + ") in " +
                    GetGlslPinTypeString(var->pin->GetDataType()) + " " + var->name + ";\n");
                index++;
            }

            // Uniform blocks.
            index = 0;
            for (auto& pair : m_uniformBlocks)
            {
                size_t set = pair.first;
                auto block = pair.second;

                std::string blockName = "ubo_" + std::to_string(index);

                AppendToVector(source, "layout(binding= 0, set = " + std::to_string(set) + ") uniform s_" + blockName + "\n{\n");

                size_t uniformIndex = 0;
                for (auto& uniform : block->GetUniformNodes())
                {

                    std::string name = "var_" + std::to_string(uniformIndex);
                    std::string fullName = blockName + "." + name;
                    auto outputPin = uniform->GetOutputPin();
                    VariablePtr var = std::make_shared<Variable>(fullName, uniform, outputPin);
                    visitedInputPins.insert({ outputPin, var });

                    AppendToVector(source,
                        GetGlslPinTypeString(var->pin->GetDataType()) + " " + name + ";\n");

                    uniformIndex++;
                }

                AppendToVector(source, "} " + blockName + ";\n");

                index++;
            }

            // Output variables.
            index = 0;
            for (auto& var : outputVars)
            {
                AppendToVector(source,
                    "layout(location = " + std::to_string(index) + ") out " +
                    GetGlslPinTypeString(var->pin->GetDataType()) + " " + var->name + ";\n");
                index++;
            }

            // Uniform variables.
            /*for (auto& pair : m_uniformNodes)
            {
                auto* node = pair.second;
                auto outputPin = node->GetOutputPin();
                size_t location = node->GetLocation();

                const std::string name = "u_var_l" + std::to_string(location);
                VariablePtr var = std::make_shared<Variable>(name, node, outputPin);
     
                AppendToVector(source,
                    "layout(location = " + std::to_string(location) + ") uniform " +
                    GetGlslPinTypeString(var->pin->GetDataType()) + " " + name + ";\n");

            }*/

            // Main program.
            if (GetType() == ShaderType::Vertex)
            {
                auto vertexOutputNode = GetVertexOutputNode();
                const std::string name = "gl_Position";
                VariablePtr var = std::make_shared<Variable>(name, vertexOutputNode, vertexOutputNode->GetInputPin());
                outputVars.push_back({ var });
            }

            AppendToVector(source, "void main(){\n");

            // Node iteration.
            size_t localVarIndex = 0;
            auto nextLocalName = [&localVarIndex]()
            {
                return std::string("l_var_" + std::to_string(localVarIndex++));
            };
            
            for (auto& outputVar : outputVars)
            {
                class NodeStackObject
                {
                public:

                    NodeStackObject(VariablePtr variable) :
                        outputVar(variable),
                        node(variable->node),
                        currentInputPinIndex(0),
                        inputPinsLeft(variable->node->GetInputPinCount())
                    { }

                    const Pin * GetCurrentInputPin()
                    {
                        return node->GetInputPin(currentInputPinIndex);
                    }

                    size_t GetInputPinsLeftCount() const
                    {
                        return inputPinsLeft;
                    }

                    void MoveToNextInputPin()
                    {
                        if (!inputPinsLeft)
                        {
                            return;
                        }
                        currentInputPinIndex++;
                        inputPinsLeft--;
                    }

                    void AddInputVariable(VariablePtr& variable)
                    {
                        inputVars.push_back(variable);
                    }

                    VariablePtr outputVar;
                    const Node* node;     
                    size_t currentInputPinIndex;
                    size_t inputPinsLeft;
                    std::vector<VariablePtr> inputVars;
                };

                std::stack<NodeStackObject> nodeStack;
                nodeStack.push({ outputVar });

                while (nodeStack.size())
                {
                    auto& stackObject = nodeStack.top();
                    auto node = stackObject.node;

                    if (stackObject.GetInputPinsLeftCount())
                    {
                        auto inputPin = stackObject.GetCurrentInputPin();
                        if (!inputPin)
                        {
                            throw Exception("Node pin is nullptr.");
                        }

                        auto connection = inputPin->GetConnection();

                        // Default value pin.
                        if (!connection)
                        {
                            VariablePtr defaultVar = std::make_shared<Variable>(GetGlslInputPinDefaultValue(*inputPin), nullptr, nullptr);
                            stackObject.AddInputVariable(defaultVar);
                            stackObject.MoveToNextInputPin();
                            continue;
                        }
                      
                        // Already created variable.
                        auto visitedIt = visitedInputPins.find(connection);
                        if (visitedIt != visitedInputPins.end())
                        {
                            stackObject.AddInputVariable(visitedIt->second);
                            stackObject.MoveToNextInputPin();
                            continue;
                        }
                
                        // Create child node.
                        const Node* connectionNode = &connection->GetNode();
                        VariablePtr childVar = std::make_shared<Variable>("", connectionNode, connection);
                        stackObject.AddInputVariable(childVar);
                        nodeStack.push({ childVar });
                        visitedInputPins.insert({ connection, childVar });
                        stackObject.MoveToNextInputPin();
                        continue;
                    }
                    
                    // All input pins of current node is travered, let's create the nodes output variable.
                    switch (node->GetType())
                    {    
                        case NodeType::Constant:
                        {
                            stackObject.outputVar->name = nextLocalName();
                            const ConstantNodeBase* constNode = static_cast<const ConstantNodeBase*>(stackObject.node);

                            AppendToVector(source,
                                GetGlslPinTypeString(constNode->GetDataType()) + " " + stackObject.outputVar->name + " = " +
                                GetGlslConstantValue(*constNode) + ";\n");
                        }
                        break;
                        case NodeType::Function:
                        {
                            stackObject.outputVar->name = nextLocalName();
                            const FunctionNodeBase* funcNode = static_cast<const FunctionNodeBase*>(stackObject.node);

                            AppendToVector(source, 
                                GetGlslPinTypeString(funcNode->GetOutputPin()->GetDataType()) + " " + stackObject.outputVar->name + " = " +
                                GetGlslFunctionName(funcNode->GetFunctionType()) + "(");

                            const size_t inputCount = stackObject.inputVars.size();
                            if (inputCount)
                            {
                                for (size_t i = 0; i < inputCount - 1; i++)
                                {
                                    AppendToVector(source, stackObject.inputVars[i]->name + ", ");
                                }
                                AppendToVector(source, stackObject.inputVars[inputCount - 1]->name);
                            }

                            AppendToVector(source, ");\n");
                        }
                        break;
                        case NodeType::Operator:
                        {
                            if (stackObject.inputVars.size() != 2)
                            {
                                throw Exception("Number of variables for operator variable is " + std::to_string(stackObject.inputVars.size()) + ", expected 2.");
                            }

                            stackObject.outputVar->name = nextLocalName();
                            const OperatorNodeBase* opNode = static_cast<const OperatorNodeBase*>(stackObject.node);

                            AppendToVector(source, 
                                GetGlslPinTypeString(stackObject.outputVar->pin->GetDataType()) + " " + stackObject.outputVar->name + " = " +
                                stackObject.inputVars[0]->name + GetGlslOperatorString(opNode->GetOperator()) + stackObject.inputVars[1]->name + ";\n");
                        }
                        break;
                        case NodeType::VaryingOut:
                        {
                            if (stackObject.inputVars.size() != 1)
                            {
                                throw Exception("Number of variables for varying out variable is " + std::to_string(stackObject.inputVars.size()) + ", expected 1.");
                            }

                            AppendToVector(source, stackObject.outputVar->name + " = " + stackObject.inputVars[0]->name + ";\n");
                        }
                        break;
                        case NodeType::VertexOutput:
                        {
                            if (stackObject.inputVars.size() != 1)
                            {
                                throw Exception("Number of variables for vertex output variable is " + std::to_string(stackObject.inputVars.size()) + ", expected 1.");
                            }

                            AppendToVector(source, stackObject.outputVar->name + " = vec4(" + stackObject.inputVars[0]->name + ", 1);\n");
                        }
                        break;
                        default: break;
                    }

                    nodeStack.pop();
                }
            }

            AppendToVector(source, "}\n");

            return std::move(source);
        }

        VertexOutputNode* Script::GetVertexOutputNode()
        {
            return nullptr;
        }
        const VertexOutputNode* Script::GetVertexOutputNode() const
        {
            return nullptr;
        }


        // Fragment shader script implementations.
        VertexScript::VertexScript() :
            m_vertexOutputNode(*this)
        { }

        VertexScript::~VertexScript()
        { }

        ShaderType VertexScript::GetType() const
        {
            return ShaderType::Vertex;
        }

        VertexOutputNode* VertexScript::GetVertexOutputNode()
        {
            return &m_vertexOutputNode;
        }
        const VertexOutputNode* VertexScript::GetVertexOutputNode() const
        {
            return &m_vertexOutputNode;
        }


        // Fragment shader script implementations.
        FragmentScript::~FragmentScript()
        { }

        ShaderType FragmentScript::GetType() const
        {
            return ShaderType::Fragment;
        }

    }
}