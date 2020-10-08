#include "Material.h"
#include "Rendering/Primitives/PrimitvesFactory.h"
#include "Rendering/AssetsLibrary.h"
#include "Rendering/Camera.h"
#include <CAMApplication/Converter/MilimetersGLConverter.h>


namespace CAMageddon
{
	Material::Material()
	{
		m_Specification.MaxDepth = 30.0f;
		m_Specification.SizeY = 50.0f;

		m_Specification.SizeX = 150.0f;
		m_Specification.SizeZ = 150.0f;
		m_Specification.PrecisionX = 500;
		m_Specification.PrecisionZ = 500;

		Init();
	}

	Material::Material(const MaterialSpecifiaction& specification)
		:m_Specification(specification)
	{
		Init();
	}

	void Material::Render(const FPSCamera& camera, const glm::vec3& lightPos)
	{
		auto shader = AssetsLibrary::Get().GetShader(AssetsConstants::MaterialShader);
		shader->Bind();

		glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

		shader->UploadUniformFloat3("u_ViewPosition", camera.GetPosition());

		//LIGHTS
		shader->UploadUniformInt("u_PointlightCount", 1);
		shader->UploadUniformFloat3("u_PointLights[0].position", lightPos);

		auto diffuseColor = lightColor * glm::vec3(0.5f);
		auto ambientColor = diffuseColor * glm::vec3(0.2f);
		auto specularColor = glm::vec3(1.0f);
		shader->UploadUniformFloat3("u_PointLights[0].ambient", ambientColor);
		shader->UploadUniformFloat3("u_PointLights[0].diffuse", diffuseColor);
		shader->UploadUniformFloat3("u_PointLights[0].specular", specularColor);

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

	void Material::Init()
	{

		HeightMapSpecification hmSpec;
		hmSpec.Width = m_Specification.SizeX;
		hmSpec.Height = m_Specification.SizeZ;
		hmSpec.Value = m_Specification.SizeY;
		hmSpec.HeightCount = m_Specification.PrecisionZ;
		hmSpec.WidthCount = m_Specification.PrecisionX;

		m_HeightMap = CreateScope<HeightMap>(hmSpec);

		auto material = PrimitiveFactory::CreateFlatPlaneVerticesNormalsTexture(
			MilimetersGLConverter::MilimetersToGL(m_Specification.SizeX),
			MilimetersGLConverter::MilimetersToGL(m_Specification.SizeZ),
			m_Specification.PrecisionX, 
			m_Specification.PrecisionZ);

		auto vertexBuffer = CreateRef<OpenGLVertexBuffer>((float*)material.vertexBufferData.data(), material.vertexBufferData.size() * sizeof(VertexNT));
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal"   },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});

		m_VertexArray = CreateScope<OpenGLVertexArray>();
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		auto indexBuffer = CreateRef<OpenGLIndexBuffer>(material.indexBufferData.data(), material.indexBufferData.size());
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}
}
