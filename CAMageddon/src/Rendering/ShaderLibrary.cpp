#include "ShaderLibrary.h"

namespace CAMageddon
{
    ShaderLibrary* ShaderLibrary::m_Instance = nullptr;
    
    ShaderLibrary& ShaderLibrary::Get()
    {
        if (!m_Instance)
            m_Instance = new ShaderLibrary();
        
        return *m_Instance;
    }
    Ref<OpenGLShader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
    {
        //TODO error checking e.g. no duplicate load
        auto shader = CreateRef<OpenGLShader>(filepath);
        m_Shaders.insert({ name,shader });
        return shader;
    }

    Ref<OpenGLShader> ShaderLibrary::GetShader(const std::string& name)
    {
        //TODO check if exists
        return m_Shaders[name];
    }
}