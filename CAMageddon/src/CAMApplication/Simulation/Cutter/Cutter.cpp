#include "Cutter.h"
#include "CAMApplication/Converter/MilimetersGLConverter.h"
#include "Rendering/AssetsLibrary.h"
#include "Rendering/Camera.h"
#include "Rendering/Primitives/PrimitvesFactory.h"
#include <glm/gtc/matrix_transform.hpp>

#include "../../Scene.h"

namespace CAMageddon
{
	Cutter::Cutter(CutterType type, float diameter)
		:m_Type(type), m_Diameter(diameter)
	{
		Init();
	}

	void Cutter::Init()
	{
		auto cutterData = PrimitiveFactory::CreateOpenCylinderVerticesNormalsTexture(MilimetersGLConverter::MilimetersToGL(GetRadius()), MilimetersGLConverter::MilimetersToGL(60), 20);
		auto vertexBuffer = CreateRef<OpenGLVertexBuffer>((float*)cutterData.vertexBufferData.data(), cutterData.vertexBufferData.size() * sizeof(VertexNT));
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal"   },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});

		m_VertexArray = CreateRef<OpenGLVertexArray>();
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		auto indexBuffer = CreateRef<OpenGLIndexBuffer>(cutterData.indexBufferData.data(), cutterData.indexBufferData.size());
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}

	void Cutter::Render(const FPSCamera& camera, std::vector<Light> lights)
	{
		auto shader = AssetsLibrary::Get().GetShader(AssetsConstants::CutterShader);
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


		glm::vec3 ambient = { 0.25f, 0.25f, 0.25f, };
		glm::vec3 diffuse = { 0.4f, 0.4f, 0.4f };
		glm::vec3 specular = { 0.774597f, 0.774597f, 0.774597f };
		shader->SetFloat3("u_Material.ambient", ambient);
		shader->SetFloat3("u_Material.diffuse", diffuse);
		shader->SetFloat3("u_Material.specular", specular);
		shader->UploadUniformFloat("u_Material.shininess", 76.8f);


		//VIEW PROJECTION
		shader->UploadUniformMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());

		//WORLD TRANSFORM
		auto worldMatrix = glm::translate(glm::mat4(1.0f), GetPosition());
		shader->UploadUniformMat4("u_ModelMatrix", worldMatrix);

		m_VertexArray->Bind();

		glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
	}
}
