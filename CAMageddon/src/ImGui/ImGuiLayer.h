#pragma once
#include "campch.h"
#include "Core\Layer.h"

namespace CAMageddon
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		ImGuiLayer(const std::string& name);
		virtual ~ImGuiLayer();

		void Begin();
		void End();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnRenderImGui() override;
	private:
		float m_Time = 0.0f;
	};
}