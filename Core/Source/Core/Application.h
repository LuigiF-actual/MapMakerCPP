#pragma once

#include "Layer.h"
#include "Window.h"
#include "Event.h"
#include "SaveFileSystem.hpp"

#include <raylib.h>
#include <string>
#include <memory>
#include <vector>
#include <set>
#include <functional>
#include <print>

namespace Core {

	struct ApplicationSpecification
	{
		std::string Name = "MapMakerC++";
		WindowSpecification WindowSpec;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& specification = ApplicationSpecification());
		~Application();

		void Run();
		void Stop();

		void RaiseEvent(Event& event);

		void PushLayer(std::unique_ptr<Layer> layer)
		{
			//m_LayerStack.push_back(std::make_unique<TLayer>());
			m_LayerStack.push_back(std::move(layer));
		}

		template<typename TLayer>
		requires(std::is_base_of_v<Layer, TLayer>)
		TLayer* GetLayer()
		{
			for (const std::unique_ptr<Layer>& layer : m_LayerStack)
			{
				TLayer* casted = dynamic_cast<TLayer*>(layer.get());
				if (casted != nullptr)
				{
					return casted;
				}
			}
			return nullptr;
		}

		template<typename T>
		void SwitchLayer()
		{
			T* targetLayer = GetLayer<T>();
			if (targetLayer != nullptr)
			{
				for (std::unique_ptr<Layer>& layer : m_LayerStack)
				{
					layer->SetActive(false);
				}
				targetLayer->SetActive(true);
			}
		}

		std::shared_ptr<Window> GetWindow() const { return m_Window; }

		static Application& Get();

		SavesFilesManager& GetSaveManager();
	
	private:

		ApplicationSpecification m_Specification;

		std::shared_ptr<Window> m_Window;

		bool m_Running = false;

		std::vector< std::unique_ptr<Layer> > m_LayerStack;

		SavesFilesManager m_SavesManager;

		friend class Layer;
		
	};

}

