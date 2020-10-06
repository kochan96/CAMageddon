#pragma once
#include "campch.h"

namespace CAMageddon
{
	class ImGuiLayer
	{
	public:
		ImGuiLayer();
		ImGuiLayer(const std::string& name);
		virtual ~ImGuiLayer();

		void Begin();
		void End();

		virtual void Init();
		virtual void Shutdown();
	private:
		void InitImGuiStyle();
	};
}