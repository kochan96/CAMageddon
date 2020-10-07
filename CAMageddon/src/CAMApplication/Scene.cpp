#include "campch.h"
#include "Core/Timestep.h"
#include "Scene.h"
#include "Rendering/ShaderLibrary.h"
#include "glad/glad.h"
#include <glm/gtc/matrix_transform.hpp>

namespace CAMageddon
{
	const std::string PlaneShader = "PlaneShader";
	const std::string PlaneShaderPath = "assets/shaders/PlaneShader.glsl";

	const std::string MaterialShader = "MaterialShader";
	const std::string MaterialShaderPath = "assets/shaders/MaterialShader.glsl";

	const std::string LightShader = "LightShader";
	const std::string LightShaderPath = "assets/shaders/LightShader.glsl";

	glm::vec3 lightPos(5.0f, 10.0f, 2.0f);


	Scene::Scene(FPSCamera& camera) : m_Camera(camera)
	{
	}

	void Scene::Init()
	{
		LoadShaders();
		InitPlane();
		InitMaterial();
		InitLight();
	}

	void Scene::LoadShaders()
	{
		ShaderLibrary::Get().Load(PlaneShader, PlaneShaderPath);
		ShaderLibrary::Get().Load(MaterialShader, MaterialShaderPath);
		ShaderLibrary::Get().Load(LightShader, LightShaderPath);
	}

	void Scene::InitPlane()
	{
		float plane[] = {
			-1.0f,0.0f,1.0f,
			1.0f,0.0f,1.0f,
			1.0f,0.0f,-1.0f,
			-1.0f,0.0f,-1.0f
		};

		auto vertexBuffer = CreateRef<OpenGLVertexBuffer>(plane, sizeof(plane));
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
			});

		m_PlaneVertexArray = CreateRef<OpenGLVertexArray>();
		m_PlaneVertexArray->AddVertexBuffer(vertexBuffer);
	}

	void Scene::InitMaterial()
	{
		float material[] =
		{
			// positions          // normals        
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		};

		auto vertexBuffer = CreateRef<OpenGLVertexBuffer>(material, sizeof(material));
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal"   }
			});

		m_MaterialVertexArray = CreateRef<OpenGLVertexArray>();
		m_MaterialVertexArray->AddVertexBuffer(vertexBuffer);
	}

	void Scene::InitLight()
	{
		float lightCube[] =
		{
			// positions               
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,

			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,

			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
		};

		auto vertexBuffer = CreateRef<OpenGLVertexBuffer>(lightCube, sizeof(lightCube));
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			});

		m_LightVertexArray = CreateRef<OpenGLVertexArray>();
		m_LightVertexArray->AddVertexBuffer(vertexBuffer);
	}

	void Scene::Update(Timestep ts)
	{
	}

	void Scene::Render()
	{
		RenderPlane();
		RenderMaterial();
		RenderLight();
	}

	void Scene::RenderPlane()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		auto shader = ShaderLibrary::Get().GetShader(PlaneShader);
		shader->Bind();
		m_PlaneVertexArray->Bind();

		//COLOR
		shader->UploadUniformFloat4("u_Color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		//VIEW PROJECTION
		shader->UploadUniformMat4("u_ViewProjectionMatrix", m_Camera.GetViewProjectionMatrix());

		//WORLD TRANSFORM
		shader->UploadUniformMat4("u_ModelMatrix", glm::scale(glm::mat4(1.0f), glm::vec3(6.0f, 0.0f, 6.0f)));

		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glDrawArrays(GL_PATCHES, 0, 4);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void Scene::RenderMaterial()
	{
		auto shader = ShaderLibrary::Get().GetShader(MaterialShader);
		shader->Bind();

		glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

		shader->UploadUniformFloat3("v_ViewPosition", m_Camera.GetPosition());

		//LIGHTS
		shader->UploadUniformInt("pointlightCount", 1);
		shader->UploadUniformFloat3("pointLights[0].position", lightPos);

		auto diffuseColor = lightColor * glm::vec3(0.5f);
		auto ambientColor = diffuseColor * glm::vec3(0.2f);
		auto specularColor = glm::vec3(1.0f);
		shader->UploadUniformFloat3("pointLights[0].ambient", ambientColor);
		shader->UploadUniformFloat3("pointLights[0].diffuse", diffuseColor);
		shader->UploadUniformFloat3("pointLights[0].specular", specularColor);

		//MATERIAL
		shader->UploadUniformFloat3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
		shader->UploadUniformFloat3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
		shader->UploadUniformFloat3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		shader->UploadUniformFloat("material.shininess", 32.0f);


		//VIEW PROJECTION
		shader->UploadUniformMat4("u_ViewProjectionMatrix", m_Camera.GetViewProjectionMatrix());

		//WORLD TRANSFORM
		shader->UploadUniformMat4("u_ModelMatrix", glm::mat4(1.0f));

		m_MaterialVertexArray->Bind();

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	void Scene::RenderLight()
	{
		auto shader = ShaderLibrary::Get().GetShader(LightShader);
		shader->Bind();
		m_LightVertexArray->Bind();

		//COLOR
		shader->UploadUniformFloat4("u_Color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		//VIEW PROJECTION
		shader->UploadUniformMat4("u_ViewProjectionMatrix", m_Camera.GetViewProjectionMatrix());

		//WORLD TRANSFORM
		//auto scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		auto translationMatrix = glm::translate(glm::mat4(1.0f), lightPos);
		auto modelMatrix = translationMatrix;
		shader->UploadUniformMat4("u_ModelMatrix", modelMatrix);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}
