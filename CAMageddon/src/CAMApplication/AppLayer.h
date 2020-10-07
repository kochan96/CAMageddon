#include "campch.h"
#include "Core/Layer.h"
#include "Rendering/FPSCameraController.h"
#include "Rendering/FrameBuffer.h"
#include "Scene.h"

namespace CAMageddon
{
	class AppLayer : public Layer
	{
	public:
		AppLayer();
		AppLayer(const std::string& name);

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnRenderImGui() override;
		virtual void OnEvent(Event& event) override;

	private:
		void RenderDebugWindow();
		void RenderViewport();

	private:
		Scope<FPSCameraController> m_CameraController;
		Scope<Scene> m_Scene;
		Ref<OpenGLFramebuffer> m_MsoFramebuffer;
		Ref<OpenGLFramebuffer> m_ViewportFramebuffer;
		
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	};
}