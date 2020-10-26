#include "Material.h"
#include "Rendering/Primitives/PrimitvesFactory.h"
#include "Rendering/AssetsLibrary.h"
#include "Rendering/Camera.h"
#include <CAMApplication/Converter/MilimetersGLConverter.h>

namespace CAMageddon
{
	Material::Material()
	{
		m_Specification.MaxDepth = 40.0f;
		m_Specification.SizeZ = 50.0f;

		m_Specification.SizeX = 150.0f;
		m_Specification.SizeY = 150.0f;
		m_Specification.PrecisionX = 500;
		m_Specification.PrecisionY = 500;

		Init();
	}

	Material::Material(const MaterialSpecifiaction& specification)
		:m_Specification(specification)
	{
		Init();
	}

	static void AddPointLight(const Light& light, const Ref<OpenGLShader> shader, int& pointLightCount)
	{
		int i = pointLightCount;
		shader->UploadUniformFloat3("u_PointLights[" + std::to_string(i) + "].position", light.Position);

		shader->UploadUniformFloat3("u_PointLights[" + std::to_string(i) + "].ambient", light.Ambient);
		shader->UploadUniformFloat3("u_PointLights[" + std::to_string(i) + "].diffuse", light.Diffuse);
		shader->UploadUniformFloat3("u_PointLights[" + std::to_string(i) + "].specular", light.Specular);

		shader->SetBool("u_PointLights[" + std::to_string(i) + "].attenuationEnabled", light.AttenuationEnabled);
		shader->UploadUniformFloat("u_PointLights[" + std::to_string(i) + "].constant", light.Constant);
		shader->UploadUniformFloat("u_PointLights[" + std::to_string(i) + "].linear", light.Linear);
		shader->UploadUniformFloat("u_PointLights[" + std::to_string(i) + "].quadratic", light.Quadratic);

		pointLightCount++;
	}

	static void AddDirLight(const Light& light, const Ref<OpenGLShader> shader, int& dirLightCount)
	{
		int i = dirLightCount;

		shader->UploadUniformFloat3("u_DirLights[" + std::to_string(i) + "].direction", light.Direction);

		shader->UploadUniformFloat3("u_DirLights[" + std::to_string(i) + "].ambient", light.Ambient);
		shader->UploadUniformFloat3("u_DirLights[" + std::to_string(i) + "].diffuse", light.Diffuse);
		shader->UploadUniformFloat3("u_DirLights[" + std::to_string(i) + "].specular", light.Specular);

		dirLightCount++;
	}

	static void AddSpotLight(const Light& light, const Ref<OpenGLShader> shader, int& spotLightCount)
	{
		int i = spotLightCount;

		shader->UploadUniformFloat3("u_SpotLights[" + std::to_string(i) + "].position", light.Position);
		shader->UploadUniformFloat3("u_SpotLights[" + std::to_string(i) + "].direction", light.Direction);

		shader->UploadUniformFloat3("u_SpotLights[" + std::to_string(i) + "].ambient", light.Ambient);
		shader->UploadUniformFloat3("u_SpotLights[" + std::to_string(i) + "].diffuse", light.Diffuse);
		shader->UploadUniformFloat3("u_SpotLights[" + std::to_string(i) + "].specular", light.Specular);

		shader->UploadUniformFloat("u_SpotLights[" + std::to_string(i) + "].cutOff", light.CutOff);
		shader->UploadUniformFloat("u_SpotLights[" + std::to_string(i) + "].outerCutOff", light.OuterCutOff);


		shader->SetBool("u_SpotLights[" + std::to_string(i) + "].attenuationEnabled", light.AttenuationEnabled);
		shader->UploadUniformFloat("u_SpotLights[" + std::to_string(i) + "].constant", light.Constant);
		shader->UploadUniformFloat("u_SpotLights[" + std::to_string(i) + "].linear", light.Linear);
		shader->UploadUniformFloat("u_SpotLights[" + std::to_string(i) + "].quadratic", light.Quadratic);

		spotLightCount++;
	}

	void Material::Render(const FPSCamera& camera, std::vector<Light> lights)
	{
		auto shader = AssetsLibrary::Get().GetShader(AssetsConstants::MaterialShader);
		shader->Bind();

		glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

		shader->UploadUniformFloat3("u_ViewPosition", camera.GetPosition());

		//LIGHTS
		int pointLightCount = 0, dirLightCount = 0, spotLightCount = 0;

		for (int i = 0; i < lights.size(); i++)
		{
			if (lights[i].LightType == LightType::Point)
				AddPointLight(lights[i], shader, pointLightCount);
			else if (lights[i].LightType == LightType::Directional)
				AddDirLight(lights[i], shader, dirLightCount);
			else if (lights[i].LightType == LightType::Spot)
				AddSpotLight(lights[i], shader, spotLightCount);
		}

		shader->UploadUniformInt("u_PointlightCount", pointLightCount);
		shader->UploadUniformInt("u_DirlightCount", dirLightCount);
		shader->UploadUniformInt("u_SpotlightCount", spotLightCount);

		//MATERIAL
		const int materialDiffuseTextureSlot = 0;
		const int materialSpecularTextureSlot = 1;

		AssetsLibrary::Get().GetTexture(AssetsConstants::MaterialDiffuseTexture)->Bind(materialDiffuseTextureSlot);
		AssetsLibrary::Get().GetTexture(AssetsConstants::MaterialSpecularTexture)->Bind(materialSpecularTextureSlot);
		shader->SetInt("u_Material.diffuse", materialDiffuseTextureSlot);
		shader->SetInt("u_Material.specular", materialSpecularTextureSlot);
		shader->UploadUniformFloat("u_Material.shininess", 32.0f);

		//HeightMap
		const int heightMapTextureSlot = 2;
		m_HeightMap->Bind(heightMapTextureSlot);
		shader->SetInt("u_HeightMap", heightMapTextureSlot);

		//VIEW PROJECTION
		shader->UploadUniformMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());

		//WORLD TRANSFORM
		shader->UploadUniformMat4("u_ModelMatrix", glm::mat4(1.0f));

		m_VertexArray->Bind();

		glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
	}

	float Material::GetHeigth(int row, int column)
	{
		return m_HeightMap->GetHeightValue(row, column);
	}

	void Material::SetHeight(int row, int column, float value)
	{
		m_HeightMap->SetHeightValue(row, column, value);
	}

	glm::vec2 Material::GetPosition(int row, int column)
	{
		float deltaWidth = 1.0f / (m_Specification.PrecisionX - 1);
		float deltaHeight = 1.0f / (m_Specification.PrecisionY - 1);

		float u = column * deltaWidth;
		float v = row * deltaHeight;

		float x = (2 * u - 1) * m_Specification.SizeX / 2.0f;
		float y = (2 * v - 1) * m_Specification.SizeY / 2.0f;

		return { x,y };
	}

	BoundingIndices Material::GetIndices(glm::vec2 leftBottom, glm::vec2 rightTop)
	{
		int maxRow = m_Specification.PrecisionY - 1;
		int maxColumn = m_Specification.PrecisionX - 1;

		float halfWidth = m_Specification.SizeX / 2.0f;
		float halfHeight = m_Specification.SizeY / 2.0f;

		int startRow = (leftBottom.y / m_Specification.SizeY + 0.5f) * maxRow - 0.5f;
		int endRow = (rightTop.y / m_Specification.SizeY + 0.5f) * maxRow + 0.5f;

		if (endRow < 0 || startRow >= m_Specification.PrecisionY)
			return { 0,0,0,0 };

		int startColumn = (leftBottom.x / m_Specification.SizeX + 0.5f) * maxColumn - 0.5f;
		int endColumn = (rightTop.x / m_Specification.SizeX + 0.5f) * maxColumn + 0.5f;

		if (endColumn < 0 || startColumn >= m_Specification.PrecisionX)
			return { 0,0,0,0 };


		startColumn = std::max(startColumn, 0);
		endColumn = std::min(endColumn, maxColumn + 1);

		startRow = std::max(startRow, 0);
		endRow = std::min(endRow, maxRow + 1);

		return { startRow,endRow,startColumn,endColumn };
	}

	void Material::Update()
	{
		m_HeightMap->Update();
	}

	void Material::Init()
	{

		HeightMapSpecification hmSpec;
		hmSpec.Value = m_Specification.SizeZ;
		hmSpec.HeightCount = m_Specification.PrecisionY;
		hmSpec.WidthCount = m_Specification.PrecisionX;

		m_HeightMap = CreateScope<HeightMap>(hmSpec);

		auto rowCount = m_HeightMap->GetTextureHeight();
		auto columnCount = m_HeightMap->GetTextureWidth();
		auto count = rowCount * columnCount;

		for (int row = 0; row < rowCount; row++)
		{
			m_HeightMap->SetHeightValue(row, 0, 0);
			m_HeightMap->SetHeightValue(row, columnCount - 1, 0);
		}

		for (int column = 0; column < columnCount; column++)
		{
			m_HeightMap->SetHeightValue(0, column, 0);
			m_HeightMap->SetHeightValue(rowCount - 1, column, 0);
		}

		m_HeightMap->Update();

		auto materialTop = PrimitiveFactory::CreateFlatPlaneVerticesNormalsTexture(
			MilimetersGLConverter::MilimetersToGL(m_Specification.SizeX),
			MilimetersGLConverter::MilimetersToGL(m_Specification.SizeY),
			m_Specification.PrecisionX,
			m_Specification.PrecisionY);


		auto vertexBuffer = CreateRef<OpenGLVertexBuffer>((float*)materialTop.vertexBufferData.data(), materialTop.vertexBufferData.size() * sizeof(VertexNT));
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal"   },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});

		m_VertexArray = CreateScope<OpenGLVertexArray>();
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		auto indexBuffer = CreateRef<OpenGLIndexBuffer>(materialTop.indexBufferData.data(), materialTop.indexBufferData.size());
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}
}
