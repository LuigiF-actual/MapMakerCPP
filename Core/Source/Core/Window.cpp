#include "Window.h"
#include "WindowEvents.h"
#include "InputEvents.h"
#include <iostream>

namespace Core {

    Window::Window(const WindowSpecification& specification)
        : m_Specification(specification)
    {}

    Window::~Window()
    {
        Destroy();
    }

    void Window::Create()
    {
        if (m_Specification.IsResizeable)
        {
            SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        }
        if (m_Specification.VSync)
        {
            SetConfigFlags(FLAG_VSYNC_HINT);
        }
        InitWindow(m_Specification.Width, m_Specification.Height, m_Specification.Title.c_str());

        SetTargetFPS(60);

        SetExitKey(KEY_NULL);
    }

    void Window::Destroy()
    {
        if (IsWindowReady())
        {
            CloseWindow();
        }
    }

    void Window::Update()
    {
        //nothings happerns for now
    }

    void Window::RaiseEvent(Event& event)
    {
        if (m_Specification.EventCallback)
            m_Specification.EventCallback(event);
    }

    Vector2 Window::GetMousePos() const
    {
        return GetMousePosition();
    }

    bool Window::ShouldClose() const
    {
        return WindowShouldClose();
    }

    void Core::Window::PollEvents()
    {
        int key = GetKeyPressed();

        while (key != 0)
        {
            KeyPressedEvent event(key, false);
            RaiseEvent(event);

            key = GetKeyPressed();
        }


        int latestPKey = 0;
        int PKey = 0;
        auto keypresedEvent = [&]() {
            for (int keycode = 8; keycode <= 348; keycode++)
            {
                //Check every keycode for a press
                if (IsKeyDown(keycode))
                {
                    KeyPressedEvent event(keycode, true);
                    RaiseEvent(event);
                }

            }
        };

        keypresedEvent();

        Vector2 mousePos = GetMousePosition();
        
        {
            MouseMovedEvent event(mousePos.x, mousePos.y);
            RaiseEvent(event);
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            MouseButtonPressedEvent event(MOUSE_BUTTON_LEFT);
            RaiseEvent(event);
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            MouseButtonPressedEvent event(MOUSE_BUTTON_RIGHT);
            RaiseEvent(event);
        }

        float wheel = GetMouseWheelMove();

        if (wheel != 0.0f)
        {
            MouseScrolledEvent event(0.0, wheel);
            RaiseEvent(event);
        }

        if (IsWindowResized())
        {
            WindowResizeEvent event(GetScreenWidth(), GetScreenHeight());
            RaiseEvent(event);
        }
    }

}