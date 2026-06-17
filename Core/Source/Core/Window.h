#pragma once

#include "Event.h"

#include <raylib.h>

#include <string>
#include <functional>


namespace Core {

	struct WindowSpecification
	{
		std::string Title;
		uint32_t Height = 720;
		uint32_t Width = 1280;

		bool IsResizeable = true;
		bool VSync = true;

		using EventCallbackFn = std::function<void(Event&)>;
		EventCallbackFn EventCallback;
	};

	class Window
	{
	public:
		Window(const WindowSpecification& specification = WindowSpecification());
		~Window();

		void Create();
		void Destroy();

		void Update();

		void RaiseEvent(Event& event);

		void PollEvents();


		//Vector2 GetFramebufferSize() const;
		Vector2 GetMousePos() const;

		bool ShouldClose() const;

	private:
		WindowSpecification m_Specification;

	};

}