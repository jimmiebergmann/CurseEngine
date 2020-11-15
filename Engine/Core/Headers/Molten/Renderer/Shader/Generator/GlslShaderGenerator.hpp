/*
* MIT License
*
* Copyright (c) 2020 Jimmie Bergmann
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

#ifndef MOLTEN_CORE_RENDERER_SHADER_GLSLSHADERGENERATOR_HPP
#define MOLTEN_CORE_RENDERER_SHADER_GLSLSHADERGENERATOR_HPP

#include "Molten/Renderer/Shader.hpp"
#include "Molten/Renderer/Shader/Generator/ShaderGeneratorNode.hpp"
#include "Molten/Renderer/Shader/Visual/VisualShaderFunction.hpp"
#include "Molten/Renderer/Shader/Visual/VisualShaderOperator.hpp"
#include <string>
#include <vector>
#include <map>

namespace Molten
{
    class Logger;
}
namespace Molten::Shader::Visual
{
    class Pin;
    class Node;
    class Script;
}

namespace Molten::Shader
{

    /** GLSL shader code generator and converter.
    *   This class makes it possible to generate GLSL code from visual shaders and convert it to SPIR-V.
    */
    class MOLTEN_API GlslGenerator
    {

    public:

        /** Enumerator of GLSL compatibilities. */
        enum class Compability : uint8_t
        {
            Glsl,   ///< used for OpenGL.
            SpirV   ///< Used for Vulkan.
        };

        /** Converting GLSL code to SPIR-V binary code. */
        static std::vector<uint8_t> ConvertGlslToSpriV(const std::vector<uint8_t>& code, Type shaderType, Logger* logger = nullptr);

        /** Default constructor. */
        GlslGenerator();

        /** Deleted constructors/operators. */
        /**@{*/
        GlslGenerator(const GlslGenerator&) = delete;
        GlslGenerator(GlslGenerator&&) = delete;
        GlslGenerator& operator =(const GlslGenerator&) = delete;
        GlslGenerator& operator =(GlslGenerator&&) = delete;
        /**@}*/


        /** Generate GLSL code, from a visual shader script.
          *
          * @param script Visual shader script to generate code from.
          * @param compability It's possible to generate plain GLSL or SPIR-V(used for Vulkan) compatible GLSL code.
          * @param logger Optional logger class. 
          */
        std::vector<uint8_t> Generate(const Visual::Script& script, const Compability compability, Logger* logger = nullptr);

    private:

        /** A structure of different counter values. Currently only for variable counters. */
        struct Counters
        {
            Counters();

            /** Resets all counter values to 0. */
            void Reset();
            void ResetVariables();
            void ResetFunctions();
            void ResetOperators();

            /* Get next variable name. */
            std::string GetNextVariableName(const VariableDataType dataType);

            /* Get next variable name. Returns data type name. Name of new variable is set via output parameter name. */
            const std::string& GetNextVariableName(std::string& name, const VariableDataType dataType);

            /* Get next function variable name. Returns function name. Name of new function variable is set via output parameter name. */
            const std::string& GetNextFunctionName(std::string& name, const Visual::FunctionType functionType);

            /* Get next operator variable name. Returns operator token. Name of new operator variable is set via output parameter name. */
            const std::string& GetNextOperatorName(std::string& name, const Visual::ArithmeticOperatorType operatorType);

            uint32_t boolCount;
            uint32_t int32Count;
            uint32_t float32Count;
            uint32_t vector2f32Count;
            uint32_t vector3f32Count;
            uint32_t vector4f32Count;
            uint32_t matrix4x4f32Count;
            uint32_t sampler1DCount;
            uint32_t sampler2DCount;
            uint32_t sampler3DCount;

            uint32_t cosCount;
            uint32_t sinCount;
            uint32_t tanCount;
            uint32_t minCount;
            uint32_t maxCount;
            uint32_t dotCount;
            uint32_t crossCount;
            uint32_t textureCount;

            uint32_t addCount;
            uint32_t subCount;
            uint32_t mulCount;
            uint32_t divCount;
        };

        /** Calculate an estimation of final generated GLSL source code. */
        static size_t CalculateEstimatedScriptSize(const Visual::Script& script);

        /** Prepare and initialize generation. */
        void PrepareGeneration(const Visual::Script& script, Logger* logger);

        /** Generate header data, including input and output data.*/
        bool GenerateHeader(const Compability compability);

        /** Helper functions for generating headers of different compabilities. */
        /**@{*/
        bool GenerateGlslHeader();
        bool GenerateSpirVHeader();
        /**@}*/

        /** Generate main function. */
        bool GenerateMain();

        /** Handle input variable and get next node if possible. */
        GeneratorNodePointer GetNextNode(GeneratorInputVariable * inputVariable);

        /** Generate source code of provided node. GenerateNodeSource will forward to appropriate generator. */
        /**@{*/
        bool GenerateNodeSource(GeneratorNodePointer& node);
        bool GenerateConstantNodeSource(GeneratorNodePointer& node);
        bool GenerateFunctionNodeSource(GeneratorNodePointer& node);
        bool GenerateOperatorNodeSource(GeneratorNodePointer& node);
        bool GenerateOutputNodeSource(GeneratorNodePointer& node);
        /**@}*/

        /** Append string to source buffer. */
        void AppendToSourceBuffer(const std::string& data);

        const Visual::Script* m_script;
        Logger* m_logger;
        std::vector<uint8_t> m_sourceBuffer;
        std::string m_lineBuffer;
        Counters m_counters;
        std::map<const Visual::Pin*, GeneratorOutputVariablePointer> m_visitedOutputVariables;
        std::map<const Visual::Node*, GeneratorNodePointer> m_visitedNodes;
        std::vector<GeneratorOutputNodePointer> m_outputNodes;


        /** Common push constant block data. */
        /*struct PushConstantTemplate
        {
            
            std::vector<uint8_t> blockSource; ///< Common stage source code of push constant block.
            uint32_t blockByteCount; ///< Total number of bytes used as puch constant variables. sizeof(*block*).
            std::vector<PushConstantOffsets> stageOffsets; ///< One item per passed script to GenerateGlslTemplate.
            PushConstantLocations locations; ///< Mapped push constant ids to offsets.
            PushConstantOffsets offsets; ///< List of all variable offsets.
        };*/

        /** Structure for common GLSL data between multiple stages. */
        /*struct GlslTemplates
        {
            PushConstantTemplate pushConstantTemplate;
        };*/

        /** Input */
        /*struct PushConstantStageTemplate
        {
            std::vector<uint8_t>* blockSource; ///< Common stage source code of push constant block. 
            PushConstantOffsets* offsets;
        };*/

        /** 
         * Glsl templates of single stage. 
         * This struct is passed to GenerateGlsl and should be constructed from GlslTemplates 
         * which is provided by GenerateGlslTemplate. */
        /*struct GlslStageTemplates
        {
            PushConstantStageTemplate pushConstantTemplate;
        };*/

        /** Generate GLSL template data from visual shader scripts. */
        //static bool GenerateGlslTemplate(VulkanGenerator::GlslTemplates& glslTemplates, const std::vector<Visual::Script*>& scripts, Logger* logger);

        /** Generate GLSL code, compatible with Spri-V, from a visual shader script. */
        //static std::vector<uint8_t> GenerateGlsl(const Visual::Script& script/*, const GlslStageTemplates* templateData*/, Logger* logger = nullptr);

        

    };

}

#endif