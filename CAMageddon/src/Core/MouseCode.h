#pragma once
#include <iostream>
namespace CAMageddon
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

#define CAM_MOUSE_BUTTON_0      ::CAMageddon::Mouse::Button0
#define CAM_MOUSE_BUTTON_1      ::CAMageddon::Mouse::Button1
#define CAM_MOUSE_BUTTON_2      ::CAMageddon::Mouse::Button2
#define CAM_MOUSE_BUTTON_3      ::CAMageddon::Mouse::Button3
#define CAM_MOUSE_BUTTON_4      ::CAMageddon::Mouse::Button4
#define CAM_MOUSE_BUTTON_5      ::CAMageddon::Mouse::Button5
#define CAM_MOUSE_BUTTON_6      ::CAMageddon::Mouse::Button6
#define CAM_MOUSE_BUTTON_7      ::CAMageddon::Mouse::Button7
#define CAM_MOUSE_BUTTON_LAST   ::CAMageddon::Mouse::ButtonLast
#define CAM_MOUSE_BUTTON_LEFT   ::CAMageddon::Mouse::ButtonLeft
#define CAM_MOUSE_BUTTON_RIGHT  ::CAMageddon::Mouse::ButtonRight
#define CAM_MOUSE_BUTTON_MIDDLE ::CAMageddon::Mouse::ButtonMiddle
