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

	void AppLayer::RenderDebugWindow()
	{
		ImGui::Begin("Debug");
		auto framerate = ImGui::GetIO().Framerate;
		ImGui::Text("Application framerate %.2f", framerate);
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
			if (ImGui::Button("Start"))
			{
				m_Scene->StartSimulation();
			}
			if (ImGui::Button("Pause"))
			{
				m_Scene->PauseSimulation();
			}
			if (ImGui::Button("FastForward"))
			{
				m_Scene->FastForwardSimulation();
			}

			ImGui::TreePop();
		}

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

			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Material"))
		{
			auto material = m_Scene->GetMaterial();
			if (material)
			{
				ImGui::Text("Material loaded");
			}

			ImGui::TreePop();
		}

		ImGui::End();
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
