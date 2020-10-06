#pragma once
#include <iostream>

namespace CAMageddon
{
    typedef enum class KeyCode : uint16_t
    {
        // From glfw3.h
        Space = 32,
        Apostrophe = 39, /* ' */
        Comma = 44, /* , */
        Minus = 45, /* - */
        Period = 46, /* . */
        Slash = 47, /* / */

        D0 = 48, /* 0 */
        D1 = 49, /* 1 */
        D2 = 50, /* 2 */
        D3 = 51, /* 3 */
        D4 = 52, /* 4 */
        D5 = 53, /* 5 */
        D6 = 54, /* 6 */
        D7 = 55, /* 7 */
        D8 = 56, /* 8 */
        D9 = 57, /* 9 */

        Semicolon = 59, /* ; */
        Equal = 61, /* = */

        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,

        LeftBracket = 91,  /* [ */
        Backslash = 92,  /* \ */
        RightBracket = 93,  /* ] */
        GraveAccent = 96,  /* ` */

        World1 = 161, /* non-US #1 */
        World2 = 162, /* non-US #2 */

        /* Function keys */
        Escape = 256,
        Enter = 257,
        Tab = 258,
        Backspace = 259,
        Insert = 260,
        Delete = 261,
        Right = 262,
        Left = 263,
        Down = 264,
        Up = 265,
        PageUp = 266,
        PageDown = 267,
        Home = 268,
        End = 269,
        CapsLock = 280,
        ScrollLock = 281,
        NumLock = 282,
        PrintScreen = 283,
        Pause = 284,
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301,
        F13 = 302,
        F14 = 303,
        F15 = 304,
        F16 = 305,
        F17 = 306,
        F18 = 307,
        F19 = 308,
        F20 = 309,
        F21 = 310,
        F22 = 311,
        F23 = 312,
        F24 = 313,
        F25 = 314,

        /* Keypad */
        KP0 = 320,
        KP1 = 321,
        KP2 = 322,
        KP3 = 323,
        KP4 = 324,
        KP5 = 325,
        KP6 = 326,
        KP7 = 327,
        KP8 = 328,
        KP9 = 329,
        KPDecimal = 330,
        KPDivide = 331,
        KPMultiply = 332,
        KPSubtract = 333,
        KPAdd = 334,
        KPEnter = 335,
        KPEqual = 336,

        LeftShift = 340,
        LeftControl = 341,
        LeftAlt = 342,
        LeftSuper = 343,
        RightShift = 344,
        RightControl = 345,
        RightAlt = 346,
        RightSuper = 347,
        Menu = 348
    } Key;

    inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
    {
        os << static_cast<int32_t>(keyCode);
        return os;
    }
}

// From glfw3.h
#define CAM_KEY_SPACE           ::CAMageddon::Key::Space
#define CAM_KEY_APOSTROPHE      ::CAMageddon::Key::Apostrophe    /* ' */
#define CAM_KEY_COMMA           ::CAMageddon::Key::Comma         /* , */
#define CAM_KEY_MINUS           ::CAMageddon::Key::Minus         /* - */
#define CAM_KEY_PERIOD          ::CAMageddon::Key::Period        /* . */
#define CAM_KEY_SLASH           ::CAMageddon::Key::Slash         /* / */
#define CAM_KEY_0               ::CAMageddon::Key::D0
#define CAM_KEY_1               ::CAMageddon::Key::D1
#define CAM_KEY_2               ::CAMageddon::Key::D2
#define CAM_KEY_3               ::CAMageddon::Key::D3
#define CAM_KEY_4               ::CAMageddon::Key::D4
#define CAM_KEY_5               ::CAMageddon::Key::D5
#define CAM_KEY_6               ::CAMageddon::Key::D6
#define CAM_KEY_7               ::CAMageddon::Key::D7
#define CAM_KEY_8               ::CAMageddon::Key::D8
#define CAM_KEY_9               ::CAMageddon::Key::D9
#define CAM_KEY_SEMICOLON       ::CAMageddon::Key::Semicolon     /* ; */
#define CAM_KEY_EQUAL           ::CAMageddon::Key::Equal         /* = */
#define CAM_KEY_A               ::CAMageddon::Key::A
#define CAM_KEY_B               ::CAMageddon::Key::B
#define CAM_KEY_C               ::CAMageddon::Key::C
#define CAM_KEY_D               ::CAMageddon::Key::D
#define CAM_KEY_E               ::CAMageddon::Key::E
#define CAM_KEY_F               ::CAMageddon::Key::F
#define CAM_KEY_G               ::CAMageddon::Key::G
#define CAM_KEY_H               ::CAMageddon::Key::H
#define CAM_KEY_I               ::CAMageddon::Key::I
#define CAM_KEY_J               ::CAMageddon::Key::J
#define CAM_KEY_K               ::CAMageddon::Key::K
#define CAM_KEY_L               ::CAMageddon::Key::L
#define CAM_KEY_M               ::CAMageddon::Key::M
#define CAM_KEY_N               ::CAMageddon::Key::N
#define CAM_KEY_O               ::CAMageddon::Key::O
#define CAM_KEY_P               ::CAMageddon::Key::P
#define CAM_KEY_Q               ::CAMageddon::Key::Q
#define CAM_KEY_R               ::CAMageddon::Key::R
#define CAM_KEY_S               ::CAMageddon::Key::S
#define CAM_KEY_T               ::CAMageddon::Key::T
#define CAM_KEY_U               ::CAMageddon::Key::U
#define CAM_KEY_V               ::CAMageddon::Key::V
#define CAM_KEY_W               ::CAMageddon::Key::W
#define CAM_KEY_X               ::CAMageddon::Key::X
#define CAM_KEY_Y               ::CAMageddon::Key::Y
#define CAM_KEY_Z               ::CAMageddon::Key::Z
#define CAM_KEY_LEFT_BRACKET    ::CAMageddon::Key::LeftBracket   /* [ */
#define CAM_KEY_BACKSLASH       ::CAMageddon::Key::Backslash     /* \ */
#define CAM_KEY_RIGHT_BRACKET   ::CAMageddon::Key::RightBracket  /* ] */
#define CAM_KEY_GRAVE_ACCENT    ::CAMageddon::Key::GraveAccent   /* ` */
#define CAM_KEY_WORLD_1         ::CAMageddon::Key::World1        /* non-US #1 */
#define CAM_KEY_WORLD_2         ::CAMageddon::Key::World2        /* non-US #2 */

/* Function keys */
#define CAM_KEY_ESCAPE          ::CAMageddon::Key::Escape
#define CAM_KEY_ENTER           ::CAMageddon::Key::Enter
#define CAM_KEY_TAB             ::CAMageddon::Key::Tab
#define CAM_KEY_BACKSPACE       ::CAMageddon::Key::Backspace
#define CAM_KEY_INSERT          ::CAMageddon::Key::Insert
#define CAM_KEY_DELETE          ::CAMageddon::Key::Delete
#define CAM_KEY_RIGHT           ::CAMageddon::Key::Right
#define CAM_KEY_LEFT            ::CAMageddon::Key::Left
#define CAM_KEY_DOWN            ::CAMageddon::Key::Down
#define CAM_KEY_UP              ::CAMageddon::Key::Up
#define CAM_KEY_PAGE_UP         ::CAMageddon::Key::PageUp
#define CAM_KEY_PAGE_DOWN       ::CAMageddon::Key::PageDown
#define CAM_KEY_HOME            ::CAMageddon::Key::Home
#define CAM_KEY_END             ::CAMageddon::Key::End
#define CAM_KEY_CAPS_LOCK       ::CAMageddon::Key::CapsLock
#define CAM_KEY_SCROLL_LOCK     ::CAMageddon::Key::ScrollLock
#define CAM_KEY_NUM_LOCK        ::CAMageddon::Key::NumLock
#define CAM_KEY_PRINT_SCREEN    ::CAMageddon::Key::PrintScreen
#define CAM_KEY_PAUSE           ::CAMageddon::Key::Pause
#define CAM_KEY_F1              ::CAMageddon::Key::F1
#define CAM_KEY_F2              ::CAMageddon::Key::F2
#define CAM_KEY_F3              ::CAMageddon::Key::F3
#define CAM_KEY_F4              ::CAMageddon::Key::F4
#define CAM_KEY_F5              ::CAMageddon::Key::F5
#define CAM_KEY_F6              ::CAMageddon::Key::F6
#define CAM_KEY_F7              ::CAMageddon::Key::F7
#define CAM_KEY_F8              ::CAMageddon::Key::F8
#define CAM_KEY_F9              ::CAMageddon::Key::F9
#define CAM_KEY_F10             ::CAMageddon::Key::F10
#define CAM_KEY_F11             ::CAMageddon::Key::F11
#define CAM_KEY_F12             ::CAMageddon::Key::F12
#define CAM_KEY_F13             ::CAMageddon::Key::F13
#define CAM_KEY_F14             ::CAMageddon::Key::F14
#define CAM_KEY_F15             ::CAMageddon::Key::F15
#define CAM_KEY_F16             ::CAMageddon::Key::F16
#define CAM_KEY_F17             ::CAMageddon::Key::F17
#define CAM_KEY_F18             ::CAMageddon::Key::F18
#define CAM_KEY_F19             ::CAMageddon::Key::F19
#define CAM_KEY_F20             ::CAMageddon::Key::F20
#define CAM_KEY_F21             ::CAMageddon::Key::F21
#define CAM_KEY_F22             ::CAMageddon::Key::F22
#define CAM_KEY_F23             ::CAMageddon::Key::F23
#define CAM_KEY_F24             ::CAMageddon::Key::F24
#define CAM_KEY_F25             ::CAMageddon::Key::F25

/* Keypad */
#define CAM_KEY_KP_0            ::CAMageddon::Key::KP0
#define CAM_KEY_KP_1            ::CAMageddon::Key::KP1
#define CAM_KEY_KP_2            ::CAMageddon::Key::KP2
#define CAM_KEY_KP_3            ::CAMageddon::Key::KP3
#define CAM_KEY_KP_4            ::CAMageddon::Key::KP4
#define CAM_KEY_KP_5            ::CAMageddon::Key::KP5
#define CAM_KEY_KP_6            ::CAMageddon::Key::KP6
#define CAM_KEY_KP_7            ::CAMageddon::Key::KP7
#define CAM_KEY_KP_8            ::CAMageddon::Key::KP8
#define CAM_KEY_KP_9            ::CAMageddon::Key::KP9
#define CAM_KEY_KP_DECIMAL      ::CAMageddon::Key::KPDecimal
#define CAM_KEY_KP_DIVIDE       ::CAMageddon::Key::KPDivide
#define CAM_KEY_KP_MULTIPLY     ::CAMageddon::Key::KPMultiply
#define CAM_KEY_KP_SUBTRACT     ::CAMageddon::Key::KPSubtract
#define CAM_KEY_KP_ADD          ::CAMageddon::Key::KPAdd
#define CAM_KEY_KP_ENTER        ::CAMageddon::Key::KPEnter
#define CAM_KEY_KP_EQUAL        ::CAMageddon::Key::KPEqual

#define CAM_KEY_LEFT_SHIFT      ::CAMageddon::Key::LeftShift
#define CAM_KEY_LEFT_CONTROL    ::CAMageddon::Key::LeftControl
#define CAM_KEY_LEFT_ALT        ::CAMageddon::Key::LeftAlt
#define CAM_KEY_LEFT_SUPER      ::CAMageddon::Key::LeftSuper
#define CAM_KEY_RIGHT_SHIFT     ::CAMageddon::Key::RightShift
#define CAM_KEY_RIGHT_CONTROL   ::CAMageddon::Key::RightControl
#define CAM_KEY_RIGHT_ALT       ::CAMageddon::Key::RightAlt
#define CAM_KEY_RIGHT_SUPER     ::CAMageddon::Key::RightSuper
#define CAM_KEY_MENU            ::CAMageddon::Key::Menu
