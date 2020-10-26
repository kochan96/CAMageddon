#include "Cutter.h"
#include "CAMApplication/Converter/MilimetersGLConverter.h"
#include "Rendering/AssetsLibrary.h"
#include "Rendering/Camera.h"
#include "Rendering/Primitives/PrimitvesFactory.h"
#include <glm/gtc/matrix_transform.hpp>

namespace CAMageddon
{
	Cutter::Cutter(CutterType type, float diameter)
		:m_Type(type), m_Diameter(diameter)
	{
		Init();
	}

	void Cutter::Init()
	{
		if (m_Type == CutterType::FLAT)
			InitFlat();
		else
			InitSphere();
	}

	void Cutter::InitFlat()
	{
		const int divisionCount = 20;

		auto cutterData = PrimitiveFactory::CreateOpenCylinderVerticesNormals(MilimetersGLConverter::MilimetersToGL(GetRadius()), MilimetersGLConverter::MilimetersToGL(60), divisionCount);
		auto cutterEndData = PrimitiveFactory::CreateFlatDiscNormals(MilimetersGLConverter::MilimetersToGL(GetRadius()), divisionCount);

		auto cutterHandleVertexBuffer = CreateRef<OpenGLVertexBuffer>((float*)cutterData.vertexBufferData.data(), cutterData.vertexBufferData.size() * sizeof(VertexN));
		cutterHandleVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal"   },
			});

		auto cutterEndVertexBuffer = CreateRef<OpenGLVertexBuffer>((float*)cutterEndData.vertexBufferData.data(), cutterEndData.vertexBufferData.size() * sizeof(VertexN));
		cutterEndVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal"   },
			});

		m_CutterHandleVertexArray = CreateRef<OpenGLVertexArray>();
		m_CutterHandleVertexArray->AddVertexBuffer(cutterHandleVertexBuffer);
		auto indexBuffer = CreateRef<OpenGLIndexBuffer>(cutterData.indexBufferData.data(), cutterData.indexBufferData.size());
		m_CutterHandleVertexArray->SetIndexBuffer(indexBuffer);

		m_CutterEndVertexArray = CreateRef<OpenGLVertexArray>();
		m_CutterEndVertexArray->AddVertexBuffer(cutterEndVertexBuffer);
		auto indexBuffer2 = CreateRef<OpenGLIndexBuffer>(cutterEndData.indexBufferData.data(), cutterEndData.indexBufferData.size());
		m_CutterEndVertexArray->SetIndexBuffer(indexBuffer2);
	}

	void Cutter::InitSphere()
	{
		const int divisionCount = 20;
		float glRadius = MilimetersGLConverter::MilimetersToGL(GetRadius());

		auto cutterData = PrimitiveFactory::CreateOpenCylinderVerticesNormals(glRadius, MilimetersGLConverter::MilimetersToGL(60), divisionCount, glRadius);
		auto cutterEndData = PrimitiveFactory::CreateHalfSphereVerticesNormals(glRadius, divisionCount, divisionCount, glRadius);

		auto cutterHandleVertexBuffer = CreateRef<OpenGLVertexBuffer>((float*)cutterData.vertexBufferData.data(), cutterData.vertexBufferData.size() * sizeof(VertexN));
		cutterHandleVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal"   },
			});

		auto cutterEndVertexBuffer = CreateRef<OpenGLVertexBuffer>((float*)cutterEndData.vertexBufferData.data(), cutterEndData.vertexBufferData.size() * sizeof(VertexN));
		cutterEndVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal"   },
			});

		m_CutterHandleVertexArray = CreateRef<OpenGLVertexArray>();
		m_CutterHandleVertexArray->AddVertexBuffer(cutterHandleVertexBuffer);
		auto indexBuffer = CreateRef<OpenGLIndexBuffer>(cutterData.indexBufferData.data(), cutterData.indexBufferData.size());
		m_CutterHandleVertexArray->SetIndexBuffer(indexBuffer);

		m_CutterEndVertexArray = CreateRef<OpenGLVertexArray>();
		m_CutterEndVertexArray->AddVertexBuffer(cutterEndVertexBuffer);
		auto indexBuffer2 = CreateRef<OpenGLIndexBuffer>(cutterEndData.indexBufferData.data(), cutterEndData.indexBufferData.size());
		m_CutterEndVertexArray->SetIndexBuffer(indexBuffer2);
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

	void Cutter::Render(const FPSCamera& camera, std::vector<Light> lights)
	{
		auto shader = AssetsLibrary::Get().GetShader(AssetsConstants::CutterShader);
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

		m_CutterHandleVertexArray->Bind();

		glDrawElements(GL_TRIANGLES, m_CutterHandleVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);

		m_CutterEndVertexArray->Bind();

		glDrawElements(GL_TRIANGLES, m_CutterEndVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
	}
}
