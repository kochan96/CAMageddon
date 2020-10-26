#include "campch.h"
#include "Core/Timestep.h"
#include "Scene.h"

#include "glad/glad.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Rendering/AssetsLibrary.h"
#include <Rendering/Primitives/PrimitvesFactory.h>

#include "CAMApplication/Converter/MilimetersGLConverter.h"

namespace CAMageddon
{
	glm::vec3 lightPos(0.0f, 0.0f, 5.0f);

	Scene::Scene(FPSCamera& camera) : m_Camera(camera)
	{
		glm::vec3 lightPosition = { 0.0f, -35.0f, 200.0f };
		glm::vec3 lightDirection = glm::normalize(glm::vec3(0) - lightPosition);
		Light light(lightPosition, lightDirection);
		light.LightType = LightType::Directional;
		light.Ambient = glm::vec3(0.5f, 0.5f, 0.5f);
		light.Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		light.Specular = glm::vec3(1.0f, 1.0f, 1.0f);

		m_Lights.push_back(light);
	}

	void Scene::Init()
	{
		LoadShaders();
		LoadTextures();
		InitPlane();
		InitMaterial();
		InitLight();

		InitTrajectory(std::vector<Instruction>());

		InitSimulation();
	}

	void Scene::SetCutter(Ref<Cutter> cutter)
	{
		if (IsSimulationRunning())
		{
			LOG_ERROR("Simulation is running");
			return;
		}

		m_Cutter = cutter;
		m_Simulation->SetCutter(cutter);
	}

	void Scene::SetMaterial(Ref<Material> material)
	{
		if (IsSimulationRunning())
		{
			LOG_ERROR("Simulation is running");
			return;
		}

		m_Material = material;
		m_Simulation->SetMaterial(material);
	}

	void Scene::SetCutterInstructions(const std::vector<Instruction> instruction)
	{
		if (IsSimulationRunning())
		{
			LOG_ERROR("Simulation is running");
			return;
		}
		m_Simulation->SetInstructions(instruction);
		InitTrajectory(instruction);
	}

	void Scene::LoadShaders()
	{
		AssetsLibrary::Get().LoadShader(AssetsConstants::PlaneShader, AssetsConstants::PlaneShaderPath);
		AssetsLibrary::Get().LoadShader(AssetsConstants::MaterialShader, AssetsConstants::MaterialShaderPath);
		AssetsLibrary::Get().LoadShader(AssetsConstants::LightShader, AssetsConstants::LightShaderPath);
		AssetsLibrary::Get().LoadShader(AssetsConstants::CutterShader, AssetsConstants::CutterShaderPath);
		AssetsLibrary::Get().LoadShader(AssetsConstants::TrajectoryShader, AssetsConstants::TrajectoryShaderPath);
	}

	void Scene::LoadTextures()
	{
		AssetsLibrary::Get().LoadTexture(AssetsConstants::MaterialDiffuseTexture, AssetsConstants::MaterialDiffuseTexturePath);
		AssetsLibrary::Get().LoadTexture(AssetsConstants::MaterialSpecularTexture, AssetsConstants::MaterialSpecularTexturePath);
	}

	void Scene::InitSimulation()
	{
		m_Simulation = CreateScope<CuttingSimulation>();
		m_Simulation->SetMaterial(m_Material);
	}

	void Scene::InitPlane()
	{
		auto patch = PrimitiveFactory::CreateFlatPatchVertices(2.0f);
		auto vertexBuffer = CreateRef<OpenGLVertexBuffer>((float*)patch.vertexBufferData.data(), patch.vertexBufferData.size() * sizeof(Vertex));
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
			});

		m_PlaneVertexArray = CreateRef<OpenGLVertexArray>();
		m_PlaneVertexArray->AddVertexBuffer(vertexBuffer);
	}

	void Scene::InitMaterial()
	{
		m_Material = CreateScope<Material>();
	}

	void Scene::InitLight()
	{
		auto lightCube = PrimitiveFactory::CreateCubeVertices(1.0f);
		auto vertexBuffer = CreateRef<OpenGLVertexBuffer>((float*)lightCube.vertexBufferData.data(), lightCube.vertexBufferData.size() * sizeof(Vertex));
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			});

		m_LightVertexArray = CreateRef<OpenGLVertexArray>();
		m_LightVertexArray->AddVertexBuffer(vertexBuffer);
	}

	void Scene::InitTrajectory(const std::vector<Instruction>& instructions)
	{
		std::vector<glm::vec3> positions;
		std::transform(instructions.begin(), instructions.end(), std::back_inserter(positions),
			[](const Instruction& instruction) {return MilimetersGLConverter::MilimetersToGL(instruction.GetPosition()); });

		auto vertexBuffer = CreateRef<OpenGLVertexBuffer>((float*)positions.data(), positions.size() * 3 * sizeof(float));
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			});

		m_TrajectoryVertexArray = CreateRef<OpenGLVertexArray>();
		m_TrajectoryVertexArray->AddVertexBuffer(vertexBuffer);

		trajectoryCount = positions.size();
	}

	void Scene::Update(Timestep ts)
	{
		m_Simulation->Update(ts);
	}

	void Scene::Render()
	{
		if (m_RenderOptions.RenderPlane)
			RenderPlane();

		if (m_RenderOptions.RenderMaterial && m_Material)
			m_Material->Render(m_Camera, m_Lights);

		if (m_RenderOptions.RenderLight)
			RenderLight();

		if (m_RenderOptions.RenderCutter && m_Cutter)
			m_Cutter->Render(m_Camera, m_Lights);

		if (m_RenderOptions.RenderTrajectory)
			RenderTrajectory();
	}

	void Scene::RenderPlane()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		auto shader = AssetsLibrary::Get().GetShader(AssetsConstants::PlaneShader);
		shader->Bind();
		m_PlaneVertexArray->Bind();

		//COLOR
		shader->UploadUniformFloat4("u_Color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		//VIEW PROJECTION
		shader->UploadUniformMat4("u_ViewProjectionMatrix", m_Camera.GetViewProjectionMatrix());

		//WORLD TRANSFORM
		auto worldMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0, MilimetersGLConverter::MilimetersToGL(m_PlaneHeight)));
		auto scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(200.0f, 200.0f, 0.0f));
		shader->UploadUniformMat4("u_ModelMatrix", worldMatrix * scaleMatrix);

		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glDrawArrays(GL_PATCHES, 0, 4);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void Scene::RenderLight()
	{
		auto shader = AssetsLibrary::Get().GetShader(AssetsConstants::LightShader);
		shader->Bind();
		m_LightVertexArray->Bind();

		//COLOR
		shader->UploadUniformFloat4("u_Color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		//VIEW PROJECTION
		shader->UploadUniformMat4("u_ViewProjectionMatrix", m_Camera.GetViewProjectionMatrix());

		//WORLD TRANSFORM
		for (auto light : m_Lights)
		{

			auto translationMatrix = glm::translate(glm::mat4(1.0f), light.Position);
			auto modelMatrix = translationMatrix;
			shader->UploadUniformMat4("u_ModelMatrix", modelMatrix);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}

	void Scene::RenderTrajectory()
	{
		auto shader = AssetsLibrary::Get().GetShader(AssetsConstants::TrajectoryShader);
		shader->Bind();
		m_TrajectoryVertexArray->Bind();
		//COLOR
		shader->UploadUniformFloat4("u_Color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

		//VIEW PROJECTION
		shader->UploadUniformMat4("u_ViewProjectionMatrix", m_Camera.GetViewProjectionMatrix());

		//WORLD TRANSFORM
		shader->UploadUniformMat4("u_ModelMatrix", glm::mat4(1.0f));

		glDisable(GL_DEPTH_TEST);

		glDrawArrays(GL_LINE_STRIP, 0, trajectoryCount);

		glEnable(GL_DEPTH_TEST);
	}
}
