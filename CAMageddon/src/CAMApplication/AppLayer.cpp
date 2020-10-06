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
		glEnable(GL_MULTISAMPLE);

		ShaderLibrary::Get().Load(FlatColorShaderName, FlatColorShaderPath);
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

		m_MsoFramebuffer->Bind();
		glClear(GL_COLOR_BUFFER_BIT);

		float triangle[] = {
			-1.0f,0.0f,0.0f,
			1.0f,0.0f,0.0f,
			0.0f,1.0f,0.0f
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

		m_MsoFramebuffer->UnBind();
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
