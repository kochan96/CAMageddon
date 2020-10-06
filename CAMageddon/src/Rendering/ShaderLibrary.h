#pragma once
#include "campch.h"
#include "Shader.h"

namespace CAMageddon
{
    class ShaderLibrary
    {
    public:
        static ShaderLibrary& Get();

        Ref<OpenGLShader> Load(const std::string& name, const std::string& filepath);
        Ref<OpenGLShader> GetShader(const std::string& name);

    private:
        ShaderLibrary(){}

        static ShaderLibrary* m_Instance;

        std::unordered_map<std::string, Ref<OpenGLShader>> m_Shaders;
    };
}