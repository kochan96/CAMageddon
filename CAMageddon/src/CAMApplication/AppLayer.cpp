#include "AppLayer.h"
#include "imgui.h"
#include "glad/glad.h"
#include "Core/Application.h"

#include "Helpers/FileDialog.h"
#include "Simulation/CuttingLoader.h"


namespace CAMageddon
{
	AppLayer::AppLayer() : Layer() {}
	AppLayer::AppLayer(const std::string& name) : Layer(name) {}

	void AppLayer::OnAttach()
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_DEPTH_TEST);

		auto& window = Application::Get().GetMainWindow();
		float aspectRatio = window.GetWidth() / window.GetHeight();
		const float fov = 45.0f;
		const float nearPlane = 0.1f;
		const float farPlane = 1000.0f;
		m_CameraController = CreateScope<FPSCameraController>(fov, aspectRatio, nearPlane, farPlane);

		FramebufferSpecification msoFbSpec;
		msoFbSpec.Width = window.GetWidth();
		msoFbSpec.Height = window.GetHeight();
		msoFbSpec.Samples = 4;
		m_MsoFramebuffer = CreateRef<OpenGLFramebuffer>(msoFbSpec);

		FramebufferSpecification fbSpec;
		fbSpec.Width = window.GetWidth();
		fbSpec.Height = window.GetHeight();
		m_ViewportFramebuffer = CreateRef< OpenGLFramebuffer>(fbSpec);

		m_Scene = CreateScope<Scene>(m_CameraController->GetCamera());
		m_Scene->Init();
	}

	void AppLayer::OnDetach()
	{

	}

	void AppLayer::OnUpdate(Timestep ts)
	{
		if (FramebufferSpecification spec = m_MsoFramebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_MsoFramebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_ViewportFramebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController->OnResize(m_ViewportSize.x, m_ViewportSize.y);
		}

		m_CameraController->OnUpdate(ts);
		m_Scene->Update(ts);

		m_MsoFramebuffer->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_Scene->Render();

		m_MsoFramebuffer->UnBind();
	}

	void AppLayer::OnRenderImGui()
	{
		RenderDebugWindow();
		RenderSimulationControl();
		RenderViewport();
	}

	static void RenderPointLightEdit(Light& light)
	{
		ImGui::DragFloat3("Position", &light.Position.x, 0.1f);
		ImGui::ColorEdit3("Ambient", &light.Ambient.x);
		ImGui::ColorEdit3("Diffuse", &light.Diffuse.x);
		ImGui::ColorEdit3("Specular", &light.Specular.x);

		ImGui::Checkbox("Attenuation", &light.AttenuationEnabled);
		if (light.AttenuationEnabled)
		{
			ImGui::DragFloat("Linear", &light.Linear);
			ImGui::DragFloat("Quadratic", &light.Quadratic);
		}
	}

	static void RenderDirLightEdit(Light& light)
	{
		if (ImGui::DragFloat3("Position", &light.Position.x, 0.1f))
		{
			light.Direction = glm::normalize(glm::vec3(0) - light.Position);
		}
		ImGui::ColorEdit3("Ambient", &light.Ambient.x);
		ImGui::ColorEdit3("Diffuse", &light.Diffuse.x);
		ImGui::ColorEdit3("Specular", &light.Specular.x);
	}

	static void RenderSpotLightEdit(Light& light)
	{

	}

	static void RenderLightEdit(Light& light)
	{
		//TODO edit for lights types;

		if (light.LightType == LightType::Point)
			RenderPointLightEdit(light);
		else if (light.LightType == LightType::Directional)
			RenderDirLightEdit(light);
		else if (light.LightType == LightType::Spot)
			RenderSpotLightEdit(light);
	}

	void AppLayer::RenderDebugWindow()
	{
		ImGui::Begin("Debug");
		auto framerate = ImGui::GetIO().Framerate;
		ImGui::Text("Application framerate %.2f", framerate);


		if (ImGui::TreeNode("RenderOptions"))
		{

			auto& renderOptions = m_Scene->GetRenderOptions();

			ImGui::Checkbox("Render Plane", &renderOptions.RenderPlane);
			ImGui::Checkbox("Render Material", &renderOptions.RenderMaterial);
			ImGui::Checkbox("Render Light", &renderOptions.RenderLight);
			ImGui::Checkbox("Render Path", &renderOptions.RenderTrajectory);
			ImGui::Checkbox("Render Cutter", &renderOptions.RenderCutter);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Lights"))
		{
			auto& lights = m_Scene->GetLights();
			for (int i = 0; i < lights.size(); i++)
			{
				ImGui::PushID(i);
				if (ImGui::TreeNode(lights[i].Name.c_str()))
				{
					RenderLightEdit(lights[i]);

					ImGui::TreePop();
				}

				ImGui::PopID();
			}

			ImGui::TreePop();
		}

		ImGui::End();
	}

	void AppLayer::RenderSimulationControl()
	{
		ImGui::Begin("Simulation");

		ImGui::Text("Load cutter first");
		if (ImGui::Button("Load"))
		{
			const char* filters[] = { "*.k*","*.f*" };

			auto filePath = FileDialog::OpenFile("SelectPath", "assets/paths/", 2, filters);
			if (filePath)
			{
				auto cutter = CuttingLoader::LoadCutter(filePath);
				auto instructions = CuttingLoader::LoadInstructions(filePath);
				m_Scene->SetCutter(std::move(cutter));
				m_Scene->SetCutterInstructions(instructions);
			}
		}

		if (ImGui::TreeNode("Simulation Controls"))
		{
			if (m_Scene->IsSimulationRunning())
			{
				if (ImGui::Button("Pause"))
				{
					m_Scene->PauseSimulation();
				}

				if (ImGui::Button("FastForward"))
				{
					m_Scene->FastForwardSimulation();
				}

				float speed = m_Scene->GetSimulationSpeed();
				if (ImGui::SliderFloat("Speed", &speed, 0.1f, 2.0f))
				{
					speed = std::clamp(speed, 0.1f, 2.0f);
					m_Scene->SetSimulationSpeed(speed);
				}

				ImGui::ProgressBar(m_Scene->GetSimulationProgress());
			}
			else if (m_Scene->IsSimulationPaused())
			{
				if (ImGui::Button("Resume"))
				{
					m_Scene->ResumeSimulation();
				}

				if (ImGui::Button("FastForward"))
				{
					m_Scene->FastForwardSimulation();
				}

				float speed = m_Scene->GetSimulationSpeed();
				if (ImGui::SliderFloat("Speed", &speed, 0.1f, 2.0f))
				{
					speed = std::clamp(speed, 0.1f, 2.0f);
					m_Scene->SetSimulationSpeed(speed);
				}
			}
			else if (m_Scene->IsSimulationReady())
			{
				if (ImGui::Button("Start"))
				{
					m_Scene->StartSimulation();
				}

				float speed = m_Scene->GetSimulationSpeed();
				if (ImGui::SliderFloat("Speed", &speed, 0.1f, 2.0f))
				{
					speed = std::clamp(speed, 0.1f, 2.0f);
					m_Scene->SetSimulationSpeed(speed);
				}
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.6f, 0.4f, 1.0f));
				ImGui::Text("Load simulation first");
				ImGui::PopStyleColor();
			}

			ImGui::TreePop();
		}


		RenderCutterInfo();
		RenderMaterialEdit();

		ImGui::End();
	}

	void AppLayer::RenderMaterialEdit()
	{
		if (ImGui::TreeNode("Material"))
		{
			if (m_Scene->IsSimulationRunning() || m_Scene->IsSimulationPaused())
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.6f, 0.4f, 1.0f));
				ImGui::Text("Simulation is running or paused");
				ImGui::Text("Cannot edit material");
				ImGui::PopStyleColor();

				ImGui::TreePop();
				return;
			}

			auto material = m_Scene->GetMaterial();
			if (material)
			{
				auto& materialSpec = material->GetSpecification();

				if (ImGui::DragFloat("MaxDepth", &materialSpec.MaxDepth, 1.0f, 0.0f, materialSpec.SizeZ))
				{
					materialSpec.MaxDepth = std::clamp(materialSpec.MaxDepth, 0.0f, materialSpec.SizeZ);
				}

				if (ImGui::DragFloat("Width", &materialSpec.SizeX, 1.0f, 10.0f, 500.0f))
				{
					materialSpec.SizeX = std::clamp(materialSpec.SizeX, 10.0f, 500.0f);
				}

				if (ImGui::DragInt("Width Precision", &materialSpec.PrecisionX, 1.0f, 300, 1000))
				{
					materialSpec.PrecisionX = std::clamp(materialSpec.PrecisionX, 300, 1000);
				}

				if (ImGui::DragFloat("Length", &materialSpec.SizeY, 1.0f, 10.0f, 500.0f))
				{
					materialSpec.SizeY = std::clamp(materialSpec.SizeY, 10.0f, 500.0f);
				}

				if (ImGui::DragInt("Length Precision", &materialSpec.PrecisionY, 1.0f, 300, 1000))
				{
					materialSpec.PrecisionY = std::clamp(materialSpec.PrecisionY, 300, 1000);
				}

				if (ImGui::DragFloat("Height", &materialSpec.SizeZ, 1.0f, 10.0f, 500.0f))
				{
					materialSpec.SizeZ = std::clamp(materialSpec.SizeZ, 10.0f, 500.0f);
				}

				if (ImGui::Button("Reset"))
				{
					auto material = CreateRef<Material>(materialSpec);
					m_Scene->SetMaterial(material);
					m_Scene->SetPlaneHeight(materialSpec.SizeZ - materialSpec.MaxDepth);
				}
			}

			ImGui::TreePop();
		}
	}

	void AppLayer::RenderCutterInfo()
	{
		if (ImGui::TreeNode("Cutter"))
		{
			auto cutter = m_Scene->GetCutter();
			if (cutter)
			{
				ImGui::Text("Cutter Type %s", cutter->GetType() == CutterType::FLAT ? "Flat" : "Spherical");
				ImGui::Text("Cutter diameter %f", cutter->GetDiameter());
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.6f, 0.4f, 1.0f));
				ImGui::Text("Load Cutter First");
				ImGui::PopStyleColor();
			}

			ImGui::TreePop();
		}
	}

	void AppLayer::RenderViewport()
	{
		m_MsoFramebuffer->BindRead();
		m_ViewportFramebuffer->BindDraw();
		auto msoFbSpec = m_MsoFramebuffer->GetSpecification();
		auto viewFbSpec = m_ViewportFramebuffer->GetSpecification();
		glBlitFramebuffer(0, 0, msoFbSpec.Width, msoFbSpec.Height, 0, 0, viewFbSpec.Width, viewFbSpec.Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		m_MsoFramebuffer->UnBind();
		m_ViewportFramebuffer->UnBind();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = m_ViewportFramebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void AppLayer::OnEvent(Event& event)
	{
		m_CameraController->OnEvent(event);
	}
}
