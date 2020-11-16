#include "campch.h"
#include "Core/Layer.h"
#include "Rendering/FPSCameraController.h"
#include "Rendering/FrameBuffer.h"
#include "Scene.h"

#include "CAMApplication/PathGeneration/CAD/SurfaceUV.h"

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
		void RenderSimulationControl();
		void RenderMaterialEdit();
		void RenderCutterInfo();
		void RenderViewport();

		void RenderGeneratePaths();

	private:
		Scope<FPSCameraController> m_CameraController;
		Scope<Scene> m_Scene;
		Ref<OpenGLFramebuffer> m_MsoFramebuffer;
		Ref<OpenGLFramebuffer> m_ViewportFramebuffer;

		std::vector<std::shared_ptr<SurfaceUV>> m_Surfaces;
		
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	};
}