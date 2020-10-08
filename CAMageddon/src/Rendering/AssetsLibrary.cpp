#include "AssetsLibrary.h"

namespace CAMageddon
{
    AssetsLibrary* AssetsLibrary::m_Instance = nullptr;
    
    AssetsLibrary& AssetsLibrary::Get()
    {
        if (!m_Instance)
            m_Instance = new AssetsLibrary();
        
        return *m_Instance;
    }
    Ref<OpenGLShader> AssetsLibrary::LoadShader(const std::string& name, const std::string& filepath)
    {
        //TODO error checking e.g. no duplicate load
        auto shader = CreateRef<OpenGLShader>(filepath);
        m_Shaders.insert({ name,shader });
        return shader;
    }

    Ref<OpenGLShader> AssetsLibrary::GetShader(const std::string& name)
    {
        //TODO check if exists
        return m_Shaders[name];
    }

    Ref<OpenGLTexture2D> AssetsLibrary::LoadTexture(const std::string& name, const std::string& filepath)
    {
        //TODO error checking e.g. no duplicate load
        auto texture = CreateRef<OpenGLTexture2D>(filepath);
        m_Textures.insert({ name,texture });
        return texture;
    }

    Ref<OpenGLTexture2D> AssetsLibrary::GetTexture(const std::string& name)
    {
        //TODO check if exists
        return m_Textures[name];
    }
}