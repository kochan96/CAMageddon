#pragma once
#include "campch.h"
#include "Shader.h"
#include "Texture.h"

namespace CAMageddon
{

    namespace AssetsConstants
    {
        const std::string PlaneShader = "PlaneShader";
        const std::string PlaneShaderPath = "assets/shaders/PlaneShader.glsl";

        const std::string MaterialShader = "MaterialShader";
        const std::string MaterialShaderPath = "assets/shaders/MaterialShader.glsl";

        const std::string LightShader = "LightShader";
        const std::string LightShaderPath = "assets/shaders/LightShader.glsl";

        const std::string TrajectoryShader = "TrajectoryShader";
        const std::string TrajectoryShaderPath = "assets/shaders/TrajectoryShader.glsl";

        const std::string CutterShader = "CutterShader";
        const std::string CutterShaderPath = "assets/shaders/CutterShader.glsl";

        const std::string MaterialDiffuseTexture = "wood_diffuse";
        const std::string MaterialDiffuseTexturePath = "assets/textures/wood/wood_diffuse.jpg";

        const std::string MaterialSpecularTexture = "wood_specular";
        const std::string MaterialSpecularTexturePath = "assets/textures/wood/wood_specular.jpg";

        const std::string MaterialBumpTexture = "wood_bump";
        const std::string MaterialBumpTexturePath = "assets/textures/wood/wood_bump.jpg";


    }

    class AssetsLibrary
    {
    public:
        static AssetsLibrary& Get();

        Ref<OpenGLShader> LoadShader(const std::string& name, const std::string& filepath);
        Ref<OpenGLShader> GetShader(const std::string& name);

		Ref<OpenGLTexture2D> LoadTexture(const std::string& name, const std::string& filepath);
		Ref<OpenGLTexture2D> GetTexture(const std::string& name);

    private:
        AssetsLibrary(){}

        static AssetsLibrary* m_Instance;

        std::unordered_map<std::string, Ref<OpenGLShader>> m_Shaders;
		std::unordered_map<std::string, Ref<OpenGLTexture2D>> m_Textures;
    };
}