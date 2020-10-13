#include "Material.h"
#include "Rendering/Primitives/PrimitvesFactory.h"
#include "Rendering/AssetsLibrary.h"
#include "Rendering/Camera.h"
#include <CAMApplication/Converter/MilimetersGLConverter.h>

#include "../../Scene.h"

namespace CAMageddon
{
	Material::Material()
	{
		m_Specification.MaxDepth = 30.0f;
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

	void Material::Render(const FPSCamera& camera, std::vector<Light> lights)
	{
		auto shader = AssetsLibrary::Get().GetShader(AssetsConstants::MaterialShader);
		shader->Bind();

		glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

		shader->UploadUniformFloat3("u_ViewPosition", camera.GetPosition());

		//LIGHTS
		shader->UploadUniformInt("u_PointlightCount", lights.size());
		for (int i = 0; i < lights.size(); i++)
		{

			shader->UploadUniformFloat3("u_PointLights[" + std::to_string(i) + "].position", lights[i].Position);
			shader->UploadUniformFloat3("u_PointLights[" + std::to_string(i) + "].ambient", lights[i].Ambient);
			shader->UploadUniformFloat3("u_PointLights[" + std::to_string(i) + "].diffuse", lights[i].Diffuse);
			shader->UploadUniformFloat3("u_PointLights[" + std::to_string(i) + "].specular", lights[i].Specular);
		}

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

	std::vector<Index> Material::GetIndices(glm::vec2 leftBottom, glm::vec2 rightTop)
	{
		std::vector<Index> indices;

		int maxRow = m_Specification.PrecisionY - 1;
		int maxColumn = m_Specification.PrecisionX - 1;

		float halfWidth = m_Specification.SizeX / 2.0f;
		float halfHeight = m_Specification.SizeY / 2.0f;

		int startRow = leftBottom.y / m_Specification.SizeY * maxRow + 0.5f;
		int endRow = rightTop.y / m_Specification.SizeY * maxRow + 0.5f;

		if (endRow < 0 || startRow >= m_Specification.PrecisionY)
			return indices;

		int startColumn = leftBottom.x / m_Specification.SizeX * maxColumn + 0.5f;
		int endColumn = rightTop.x / m_Specification.SizeX * maxColumn + 0.5f;

		if (endColumn < 0 || startColumn >= m_Specification.PrecisionX)
			return indices;

		startRow = std::min(startRow, 0);
		endRow = std::min(endRow, maxRow);

		startColumn = std::min(startColumn, 0);
		endColumn = std::min(endColumn, maxColumn);

		for (int row = startRow; row <= endRow; row++)
		{
			for (int column = startColumn; column <= endColumn; column++)
			{
				indices.push_back({ row,column });
			}
		}

		return indices;
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
