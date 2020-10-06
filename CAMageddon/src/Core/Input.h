#pragma once
#include "KeyCodes.h"
#include "MouseCode.h"
#include <utility>

namespace CAMageddon
{
    class Input
    {
    public:
        static bool IsKeyPressed(KeyCode key);

        static bool IsMouseButtonPressed(MouseCode button);
        static std::pair<float, float> GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
    };
}
