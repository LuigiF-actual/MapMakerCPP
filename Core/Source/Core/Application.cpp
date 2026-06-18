#include "Application.h"

#include <assert.h>
#include <iostream>
#include <ranges>

namespace Core {

	static Application* s_Application = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{

		s_Application = this;

		// Set window title to app name if empty
		if (m_Specification.WindowSpec.Title.empty())
			m_Specification.WindowSpec.Title = m_Specification.Name;

		m_Specification.WindowSpec.EventCallback = [this](Event& event) { RaiseEvent(event); };

		m_Window = std::make_shared<Window>(m_Specification.WindowSpec);
		m_Window->Create();

	}

	Application::~Application()
	{
		m_Window->Destroy();

		s_Application = nullptr;
	}

	void Application::Run()
	{

		while (!m_Window->ShouldClose())
		{
			
			m_Window->PollEvents(); 

			for (const std::unique_ptr<Layer>& layer : m_LayerStack)
			{
				if (layer->IsActive())
					layer->OnUpdate();
			}
			
			BeginDrawing();

			ClearBackground(RAYWHITE);

			for (const std::unique_ptr<Layer>& layer : m_LayerStack)
			{
				if (layer->IsActive())
					layer->OnRender();
			}
			
			EndDrawing();


			m_Window->Update();
		}
	}

	void Application::Stop()
	{
		m_Running = false;
	}

	void Application::RaiseEvent(Event& event)
	{

		bool IsWindowResizeEvent = false;

		for (auto& layer : std::views::reverse(m_LayerStack))
		{
			if (event.GetEventType() == EventType::WindowResize)
			{
				layer->OnEvent(event);
				IsWindowResizeEvent = true;
			}

			if (layer->IsActive() && !IsWindowResizeEvent)
			{
				layer->OnEvent(event);
				if (event.Handled)
					break;
			}
		}
	}

	Application& Application::Get()
	{
		assert(s_Application);
		return *s_Application;
	}


	SavesFilesManager& Application::GetSaveManager()
	{
		return m_SavesManager;
	}

}