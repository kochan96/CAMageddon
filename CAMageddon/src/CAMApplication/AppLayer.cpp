#include "AppLayer.h"
#include "imgui.h"
#include "Rendering/ShaderLibrary.h"
#include "Rendering/VertexArray.h"
#include "glad/glad.h"

#include "Core/Application.h"

namespace CAMageddon
{
	const std::string FlatColorShaderPath = "assets/shaders/FlatColorShader.glsl";
	const std::string FlatColorShaderName = "FlatColorShader";

	AppLayer::AppLayer() : Layer() {}
	AppLayer::AppLayer(const std::string& name) : Layer(name) {}

	void AppLayer::OnAttach()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		ShaderLibrary::Get().Load(FlatColorShaderName, FlatColorShaderPath);
		auto& window = Application::Get().GetMainWindow();
		float aspectRatio = window.GetWidth() / window.GetHeight();
		const float fov = 45.0f;
		const float nearPlane = 0.1f;
		const float farPlane = 1000.0f;
		m_CameraController = CreateScope<FPSCameraController>(fov, aspectRatio, nearPlane, farPlane);

		FramebufferSpecification fbSpec;
		fbSpec.Width = window.GetWidth();
		fbSpec.Height = window.GetHeight();

		m_Framebuffer = CreateRef<OpenGLFramebuffer>(fbSpec);
	}

	void AppLayer::OnDetach()
	{

	}

	void AppLayer::OnUpdate(Timestep ts)
	{
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController->OnResize(m_ViewportSize.x, m_ViewportSize.y);
		}

		m_CameraController->OnUpdate(ts);

		m_Framebuffer->Bind();
		glClear(GL_COLOR_BUFFER_BIT);

		float triangle[] = {
			-0.5f,0.0f,0.0f,
			0.5f,0.0f,0.0f,
			0.0f,0.5f,0.0f
		};

		auto vertexBuffer = CreateRef<OpenGLVertexBuffer>(triangle, sizeof(triangle));
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
			});

		auto vertexArray = CreateRef<OpenGLVertexArray>();
		vertexArray->AddVertexBuffer(vertexBuffer);

		auto shader = ShaderLibrary::Get().GetShader(FlatColorShaderName);
		shader->Bind();
		vertexArray->Bind();
		shader->UploadUniformFloat4("u_Color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		shader->UploadUniformMat4("u_ViewProjectionMatrix", m_CameraController->GetCamera().GetViewProjectionMatrix());
		shader->UploadUniformMat4("u_ModelMatrix", glm::mat4(1.0f));

		glDrawArrays(GL_TRIANGLES, 0, 3);

		m_Framebuffer->UnBind();
	}

	void AppLayer::OnRenderImGui()
	{
		ImGui::Begin("Test window");

		ImGui::Text("Test ");

		ImGui::End();

		RenderViewport();
	}

	void AppLayer::RenderViewport()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void AppLayer::OnEvent(Event& event)
	{
		m_CameraController->OnEvent(event);
	}
}
