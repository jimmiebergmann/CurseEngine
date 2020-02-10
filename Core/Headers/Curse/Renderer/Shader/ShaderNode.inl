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


namespace Curse
{

    namespace Shader
    {

        // Shader script constant node base implementations.
        inline NodeType ConstantNodeBase::GetType() const
        {
            return NodeType::Constant;
        }

        inline ConstantNodeBase::ConstantNodeBase(Script& script) :
            Node(script)
        { }


        // Shader script constant node implementations.
        template<typename T>
        inline PinDataType ConstantNode<T>::GetDataType() const
        {
            return m_output.GetDataType();
        }

        template<typename T>
        inline std::type_index ConstantNode<T>::GetDataTypeIndex() const
        {
            return m_output.GetDataTypeIndex();
        }

        template<typename T>
        inline size_t ConstantNode<T>::GetOutputPinCount() const
        {
            return 1;
        }

        template<typename T>
        inline Pin* ConstantNode<T>::GetOutputPin(const size_t index)
        {
            if (index != 0)
            {
                return nullptr;
            }
            return &m_output;
        }
        template<typename T>
        inline const Pin* ConstantNode<T>::GetOutputPin(const size_t index) const
        {
            if (index != 0)
            {
                return nullptr;
            }
            return &m_output;
        }

        template<typename T>
        inline std::vector<Pin*> ConstantNode<T>::GetOutputPins()
        {
            return { &m_output };
        }
        template<typename T>
        inline std::vector<const Pin*> ConstantNode<T>::GetOutputPins() const
        {
            return { &m_output };
        }

        template<typename T>
        inline const T& ConstantNode<T>::GetValue() const
        {
            return m_value;
        }

        template<typename T>
        inline void ConstantNode<T>::SetValue(const T& value) const
        {
            m_value = value;
        }

        template<typename T>
        inline ConstantNode<T>::ConstantNode(Script& script, const T& value) :
            ConstantNodeBase(script),
            m_output(*this),
            m_value(value)
        { }


        // Shader script function node base implementations.
        inline NodeType FunctionNodeBase::GetType() const
        {
            return NodeType::Function;
        }

        inline FunctionNodeBase::FunctionNodeBase(Script& script) :
            Node(script)
        { }


        // Shader script function node implementations.
        template<FunctionType _FunctionType, typename OutputType, typename ... InputTypes>
        FunctionType FunctionNode<_FunctionType, OutputType, InputTypes...>::GetFunctionType() const
        {
            return _FunctionType;
        }

        template<FunctionType _FunctionType, typename OutputType, typename ... InputTypes>
        inline size_t FunctionNode<_FunctionType, OutputType, InputTypes...>::GetInputPinCount() const
        {
            return InputPinCount;
        }

        template<FunctionType _FunctionType, typename OutputType, typename ... InputTypes>
        inline size_t FunctionNode<_FunctionType, OutputType, InputTypes...>::GetOutputPinCount() const
        {
            return OutputPinCount;
        }

        template<FunctionType _FunctionType, typename OutputType, typename ... InputTypes>
        inline Pin* FunctionNode<_FunctionType, OutputType, InputTypes...>::GetInputPin(const size_t index)
        {
            if (index > m_inputs.size())
            {
                return nullptr;
            }
            return m_inputs[index].get();
        }
        template<FunctionType _FunctionType, typename OutputType, typename ... InputTypes>
        inline const Pin* FunctionNode<_FunctionType, OutputType, InputTypes...>::GetInputPin(const size_t index) const
        {
            if (index > m_inputs.size())
            {
                return nullptr;
            }
            return m_inputs[index].get();
        }

        template<FunctionType _FunctionType, typename OutputType, typename ... InputTypes>
        inline std::vector<Pin*> FunctionNode<_FunctionType, OutputType, InputTypes...>::GetInputPins()
        {
            std::vector<Pin*> pins;
            for (const auto& pin : m_inputs)
            {
                pins.push_back(pin.get());
            }

            return pins;
        }
        template<FunctionType _FunctionType, typename OutputType, typename ... InputTypes>
        inline std::vector<const Pin*> FunctionNode<_FunctionType, OutputType, InputTypes...>::GetInputPins() const
        {
            std::vector<const Pin*> pins;
            for (const auto& pin : m_inputs)
            {
                pins.push_back(pin.get());
            }

            return pins;
        }

        template<FunctionType _FunctionType, typename OutputType, typename ... InputTypes>
        inline Pin* FunctionNode<_FunctionType, OutputType, InputTypes...>::GetOutputPin(const size_t index)
        {
            if (index != 0)
            {
                return nullptr;
            }
            return m_output.get();
        }
        template<FunctionType _FunctionType, typename OutputType, typename ... InputTypes>
        inline const Pin* FunctionNode<_FunctionType, OutputType, InputTypes...>::GetOutputPin(const size_t index) const
        {
            if (index != 0)
            {
                return nullptr;
            }
            return m_output.get();
        }

        template<FunctionType _FunctionType, typename OutputType, typename ... InputTypes>
        inline std::vector<Pin*> FunctionNode<_FunctionType, OutputType, InputTypes...>::GetOutputPins()
        {
            auto output = m_output.get();
            if (!output)
            {
                return {};
            }
            return { output };
        }
        template<FunctionType _FunctionType, typename OutputType, typename ... InputTypes>
        inline std::vector<const Pin*> FunctionNode<_FunctionType, OutputType, InputTypes...>::GetOutputPins() const
        {
            auto output = m_output.get();
            if (!output)
            {
                return {};
            }
            return { output };
        }

        template<FunctionType _FunctionType, typename OutputType, typename ... InputTypes>
        inline FunctionNode<_FunctionType, OutputType, InputTypes...>::FunctionNode(Script& script) :
            FunctionNodeBase(script),
            m_output(std::make_unique<OutputPin<OutputType> >(*this))
        {
            InitInputPin<InputTypes...>();
        }

        template<FunctionType _FunctionType, typename OutputType, typename ... InputTypes>
        template<typename CurrentType, typename ...>
        inline void FunctionNode<_FunctionType, OutputType, InputTypes...>::InitInputPin(const size_t index)
        {
            if (index == InputPinCount)
            {
                return;
            }

            auto inputPin = std::make_unique<InputPin<CurrentType> >(*this);
            m_inputs[index] = std::move(inputPin);

            InitInputPin<CurrentType>(index + 1);
        }


        // Shader script operator node base implementations.
        inline NodeType OperatorNodeBase::GetType() const
        {
            return NodeType::Operator;
        }

        inline Operator OperatorNodeBase::GetOperator() const
        {
            return m_operator;
        }

        inline OperatorNodeBase::OperatorNodeBase(Script& script, const Operator op) :
            Node(script),
            m_operator(op)
        { }


        // Shader script operator node implementations.
        template<typename T>
        inline size_t OperatorNode<T>::GetInputPinCount() const
        {
            return 2;
        }

        template<typename T>
        inline size_t OperatorNode<T>::GetOutputPinCount() const
        {
            return 1;
        }

        template<typename T>
        inline Pin* OperatorNode<T>::GetInputPin(const size_t index)
        {
            if (index > 1)
            {
                return nullptr;
            }
            return std::vector<Pin*>{&m_inputA, &m_inputB}[index];
        }
        template<typename T>
        inline const Pin* OperatorNode<T>::GetInputPin(const size_t index) const
        {
            if (index > 1)
            {
                return nullptr;
            }
            return std::vector<const Pin*>{&m_inputA, &m_inputB}[index];
        }

        template<typename T>
        inline std::vector<Pin*> OperatorNode<T>::GetInputPins()
        {
            return { &m_inputA, &m_inputB };
        }
        template<typename T>
        inline std::vector<const Pin*> OperatorNode<T>::GetInputPins() const
        {
            return { &m_inputA, &m_inputB };
        }

        template<typename T>
        inline Pin* OperatorNode<T>::GetOutputPin(const size_t index)
        {
            if (index != 0)
            {
                return nullptr;
            }
            return &m_output;
        }
        template<typename T>
        inline const Pin* OperatorNode<T>::GetOutputPin(const size_t index) const
        {
            if (index != 0)
            {
                return nullptr;
            }
            return &m_output;
        }

        template<typename T>
        inline std::vector<Pin*> OperatorNode<T>::GetOutputPins()
        {
            return { &m_output };
        }
        template<typename T>
        inline std::vector<const Pin*> OperatorNode<T>::GetOutputPins() const
        {
            return { &m_output };
        }

        template<typename T>
        inline OperatorNode<T>::OperatorNode(Script& script, const Operator op) :
            OperatorNodeBase(script, op),
            m_inputA(*this),
            m_inputB(*this),
            m_inputs{ nullptr, nullptr },
            m_output(*this)
        { }


        // Shader script uniform base implementations.
        inline NodeType UniformNodeBase::GetType() const
        {
            return NodeType::Uniform;
        }

        inline UniformNodeBase::UniformNodeBase(Script& script) :
            Node(script)
        { }

        // Shader script uniform base implementations.
        template<typename T>
        inline size_t UniformNode<T>::GetOutputPinCount() const
        {
            return 1;
        }

        template<typename T>
        inline Pin* UniformNode<T>::GetOutputPin(const size_t index)
        {
            if (index != 0)
            {
                return nullptr;
            }
            return &m_output;
        }
        template<typename T>
        inline const Pin* UniformNode<T>::GetOutputPin(const size_t index) const
        {
            if (index != 0)
            {
                return nullptr;
            }
            return &m_output;
        }

        template<typename T>
        inline std::vector<Pin*> UniformNode<T>::GetOutputPins()
        {
            return { &m_output };
        }
        template<typename T>
        inline std::vector<const Pin*> UniformNode<T>::GetOutputPins() const
        {
            return { &m_output };
        }

        template<typename T>
        inline UniformNode<T>::UniformNode(Script& script) :
            UniformNodeBase(script),
            m_output(*this)
        { }

        // Uniform block implementations.
        inline UniformBlock::~UniformBlock()
        {
            for (auto* uniform : m_uniformNodes)
            {
                delete uniform;
            }
        }

        template<typename T>
        inline UniformNode<T>* UniformBlock::CreateUniformNode()
        {
            auto uniformNode = new UniformNode<T>(m_script);
            m_uniformNodes.push_back(uniformNode);
            return uniformNode;
        }

        inline std::vector<UniformNodeBase*> UniformBlock::GetUniformNodes()
        {
            return m_uniformNodes;
        }

        inline std::vector<const UniformNodeBase*> UniformBlock::GetUniformNodes() const
        {
            return { m_uniformNodes.begin(), m_uniformNodes.end() };
        }

        inline uint32_t UniformBlock::GetId() const
        {
            return m_id;
        }

        inline UniformBlock::UniformBlock(Script& script, const uint32_t id) :
            m_script(script),
            m_id(id)
        { }


        // Shader script varying in node implementations.
        template<typename T>
        inline size_t VaryingInNode<T>::GetOutputPinCount() const
        {
            return 1;
        }

        template<typename T>
        inline Pin* VaryingInNode<T>::GetOutputPin(const size_t index)
        {
            if (index != 0)
            {
                return nullptr;
            }
            return &m_pin;
        }
        template<typename T>
        inline const Pin* VaryingInNode<T>::GetOutputPin(const size_t index) const
        {
            if (index != 0)
            {
                return nullptr;
            }
            return &m_pin;
        }

        template<typename T>
        inline std::vector<Pin*> VaryingInNode<T>::GetOutputPins()
        {
            return { &m_pin };
        }
        template<typename T>
        inline std::vector<const Pin*> VaryingInNode<T>::GetOutputPins() const
        {
            return { &m_pin };
        }

        template<typename T>
        inline NodeType VaryingInNode<T>::GetType() const
        {
            return NodeType::VaryingIn;
        }

        template<typename T>
        inline VaryingInNode<T>::VaryingInNode(Script& script) :
            Node(script),
            m_pin(*this)
        { }


        // Shader script varying out node implementations.
        template<typename T>
        inline size_t VaryingOutNode<T>::GetInputPinCount() const
        {
            return 1;
        }

        template<typename T>
        inline Pin* VaryingOutNode<T>::GetInputPin(const size_t index)
        {
            if (index != 0)
            {
                return nullptr;
            }
            return &m_pin;
        }
        template<typename T>
        inline const Pin* VaryingOutNode<T>::GetInputPin(const size_t index) const
        {
            if (index != 0)
            {
                return nullptr;
            }
            return &m_pin;
        }

        template<typename T>
        inline std::vector<Pin*> VaryingOutNode<T>::GetInputPins()
        {
            return { &m_pin };
        }
        template<typename T>
        inline std::vector<const Pin*> VaryingOutNode<T>::GetInputPins() const
        {
            return { &m_pin };
        }

        template<typename T>
        inline NodeType VaryingOutNode<T>::GetType() const
        {
            return NodeType::VaryingOut;
        }

        template<typename T>
        inline VaryingOutNode<T>::VaryingOutNode(Script& script) :
            Node(script),
            m_pin(*this)
        { }


        // Shader script vertex output node implementations.
        inline size_t VertexOutputNode::GetInputPinCount() const
        {
            return 1;
        }

        inline Pin* VertexOutputNode::GetInputPin(const size_t index)
        {
            if (index != 0)
            {
                return nullptr;
            }
            return &m_pin;
        }
        
        inline const Pin* VertexOutputNode::GetInputPin(const size_t index) const
        {
            if (index != 0)
            {
                return nullptr;
            }
            return &m_pin;
        }

        inline std::vector<Pin*> VertexOutputNode::GetInputPins()
        {
            return { &m_pin };
        }
        inline std::vector<const Pin*> VertexOutputNode::GetInputPins() const
        {
            return { &m_pin };
        }

        inline NodeType VertexOutputNode::GetType() const
        {
            return NodeType::VertexOutput;
        }

        inline VertexOutputNode::VertexOutputNode(Script& script) :
            Node(script),
            m_pin(*this)
        { }

    }

}