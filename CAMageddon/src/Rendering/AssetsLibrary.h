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

        const std::string WoodDiffuseTexture = "wood_diffuse";
        const std::string WoodDiffuseTexturePath = "assets/textures/wood/wood_diffuse.jpg";
        
        const std::string BoxDiffuseTexture = "box_diffuse";
        const std::string BoxDiffuseTexturePath = "assets/textures/other/box4_diff.jpg";

        const std::string WoodSpecularTexture = "wood_specular";
        const std::string WoodSpecularTexturePath = "assets/textures/wood/wood_specular.jpg";
        
        const std::string BoxSpecularTexture = "box_specular";
        const std::string BoxSpecularTexturePath = "assets/textures/other/box4_spec.jpg";
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