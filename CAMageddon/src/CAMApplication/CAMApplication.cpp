#include "CAMApplication.h"
#include "AppLayer.h"

namespace CAMageddon
{
	CAMApplication::CAMApplication() : Application()
	{
	}

	void CAMApplication::Init()
	{
		Application::Init();

		PushLayer(new AppLayer("AppLayer"));
	}
}
