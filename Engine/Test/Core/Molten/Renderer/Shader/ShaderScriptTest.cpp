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

#include "Test.hpp"
#include "Molten/Renderer/Shader/Visual/VisualShaderScript.hpp"
#include "Molten/Renderer/Shader/Generator/VulkanShaderGenerator.hpp"

namespace Molten::Shader::Visual
{

    TEST(Shader, Script_GenerateGlsl)
    {
        FragmentScript script;

        auto output = script.GetOutputInterface().AddMember<Vector4f32>();
        auto color = script.GetInputInterface().AddMember<Vector4f32>();
        auto mult = script.CreateOperator<Shader::Visual::Operators::MultVec4f32>();
        auto add = script.CreateOperator<Shader::Visual::Operators::AddVec4f32>();
        auto var1 = script.CreateConstantVariable<Vector4f32>({ 0.0f, 0.0f, 0.3f, 0.0f });
        auto var2 = script.CreateConstantVariable<Vector4f32>({ 1.0f, 0.5f, 0.0f, 1.0f });
        script.GetPushConstantInterface().AddMember<Vector4f32>(123);
        script.GetPushConstantInterface().AddMember<Vector2f32>(1234);
        script.GetPushConstantInterface().AddMember<float>(12356);

        output->GetInputPin()->Connect(*add->GetOutputPin());

        add->GetInputPin(0)->Connect(*mult->GetOutputPin());
        add->GetInputPin(1)->Connect(*var1->GetOutputPin());

        mult->GetInputPin(0)->Connect(*color->GetOutputPin());
        mult->GetInputPin(1)->Connect(*var2->GetOutputPin());

        std::vector<uint8_t> source;
        {
            VulkanGenerator::GlslTemplates glslTemplates;
            Molten::Test::Benchmarker bench1("GLSL generator");
            {
                Molten::Test::Benchmarker bench2("GLSL template generator");
                EXPECT_TRUE(VulkanGenerator::GenerateGlslTemplate(glslTemplates, { &script }, nullptr));
            }

            VulkanGenerator::GlslStageTemplates stageTemplates;
            stageTemplates.pushConstantTemplate.blockSource = &glslTemplates.pushConstantTemplate.blockSource;
            stageTemplates.pushConstantTemplate.offsets = &glslTemplates.pushConstantTemplate.stageOffsets[0];

            source = VulkanGenerator::GenerateGlsl(script, &stageTemplates);
        }
        EXPECT_GT(source.size(), size_t(0));
        const std::string sourceStr(source.begin(), source.end());

        static const std::string expectedSource =
            "#version 450\n"
            "#extension GL_ARB_separate_shader_objects : enable\n"
            "layout(location = 0) in vec4 in_0;\n"         
            "layout(std140, push_constant) uniform s_fragment_pc\n"
            "{\n"
            "layout(offset = 0) vec4 mem0;\n"
            "layout(offset = 16) vec2 mem16;\n"
            "layout(offset = 24) float mem24;\n"
            "} pc;\n"
            "layout(location = 0) out vec4 out_0;\n"
            "void main(){\n"
            "vec4 vec4_0 = vec4(1, 0.5, 0, 1);\n"
            "vec4 mul_1 = in_0 * vec4_0;\n"
            "vec4 vec4_2 = vec4(0, 0, 0.3, 0);\n"
            "vec4 add_3 = mul_1 + vec4_2;\n"
            "out_0 = add_3;\n"
            "}\n";

        EXPECT_STREQ(sourceStr.c_str(), expectedSource.c_str());
    }

    TEST(Shader, Script_DefaultPinValue)
    {
        // Cos
        {
            FragmentScript script;
            auto output = script.GetOutputInterface().AddMember<Vector4f32>();
            auto cos = script.CreateFunction<Shader::Visual::Functions::CosVec4f32>();

            output->GetInputPin()->Connect(*cos->GetOutputPin());
            static_cast<Shader::Visual::InputPin<Vector4f32>*>(cos->GetInputPin())->SetDefaultValue({2.1f, 3.5f, 4.7f, 5.2f});

            std::vector<uint8_t> source;
            {
                Molten::Test::Benchmarker bench("GLSL generator - default pin");
                source = VulkanGenerator::GenerateGlsl(script, nullptr);
            }
            EXPECT_GT(source.size(), size_t(0));
            const std::string sourceStr(source.begin(), source.end());

            static const std::string expectedSource =
                "#version 450\n"
                "#extension GL_ARB_separate_shader_objects : enable\n"
                "layout(location = 0) out vec4 out_0;\n"
                "void main(){\n"
                "vec4 cos_0 = cos(vec4(2.1, 3.5, 4.7, 5.2));\n"
                "out_0 = cos_0;\n"
                "}\n";

            EXPECT_STREQ(sourceStr.c_str(), expectedSource.c_str());
        }
    }

    TEST(Shader, Script_Functions)
    {
        // Cos
        {
            FragmentScript script;
            auto output = script.GetOutputInterface().AddMember<Vector4f32>();
            auto var1 = script.CreateConstantVariable<Vector4f32>({ 1.0f, 2.0f, 3.0f, 4.0f });
            auto cos = script.CreateFunction<Shader::Visual::Functions::CosVec4f32>();

            output->GetInputPin()->Connect(*cos->GetOutputPin());
            cos->GetInputPin()->Connect(*var1->GetOutputPin());

            std::vector<uint8_t> source;
            {
                Molten::Test::Benchmarker bench("GLSL generator - function");
                source = VulkanGenerator::GenerateGlsl(script, nullptr);
            }
            const std::string sourceStr(source.begin(), source.end());

            static const std::string expectedSource =
                "#version 450\n"
                "#extension GL_ARB_separate_shader_objects : enable\n"
                "layout(location = 0) out vec4 out_0;\n"
                "void main(){\n"
                "vec4 vec4_0 = vec4(1, 2, 3, 4);\n"
                "vec4 cos_1 = cos(vec4_0);\n"
                "out_0 = cos_1;\n"
                "}\n";

            EXPECT_STREQ(sourceStr.c_str(), expectedSource.c_str());
        }
        // Sin
        {
            FragmentScript script;
            auto output = script.GetOutputInterface().AddMember<Vector3f32>();
            auto var1 = script.CreateConstantVariable<Vector3f32>({ 1.0f, 2.0f, 3.0f });
            auto cos = script.CreateFunction<Shader::Visual::Functions::SinVec3f32>();

            output->GetInputPin()->Connect(*cos->GetOutputPin());
            cos->GetInputPin()->Connect(*var1->GetOutputPin());

            auto source = VulkanGenerator::GenerateGlsl(script, nullptr);
            const std::string sourceStr(source.begin(), source.end());

            static const std::string expectedSource =
                "#version 450\n"
                "#extension GL_ARB_separate_shader_objects : enable\n"
                "layout(location = 0) out vec3 out_0;\n"
                "void main(){\n"
                "vec3 vec3_0 = vec3(1, 2, 3);\n"
                "vec3 sin_1 = sin(vec3_0);\n"
                "out_0 = sin_1;\n"
                "}\n";

            EXPECT_STREQ(sourceStr.c_str(), expectedSource.c_str());
        }
        // Tan
        {
            FragmentScript script;
            auto output = script.GetOutputInterface().AddMember<Vector2f32>();
            auto var1 = script.CreateConstantVariable<Vector2f32>({ 1.0f, 2.0f });
            auto cos = script.CreateFunction<Shader::Visual::Functions::TanVec2f32>();

            output->GetInputPin()->Connect(*cos->GetOutputPin());
            cos->GetInputPin()->Connect(*var1->GetOutputPin());

            auto source = VulkanGenerator::GenerateGlsl(script, nullptr);
            const std::string sourceStr(source.begin(), source.end());

            static const std::string expectedSource =
                "#version 450\n"
                "#extension GL_ARB_separate_shader_objects : enable\n"
                "layout(location = 0) out vec2 out_0;\n"
                "void main(){\n"
                "vec2 vec2_0 = vec2(1, 2);\n"
                "vec2 tan_1 = tan(vec2_0);\n"
                "out_0 = tan_1;\n"
                "}\n";

            EXPECT_STREQ(sourceStr.c_str(), expectedSource.c_str());
        }
    }

}