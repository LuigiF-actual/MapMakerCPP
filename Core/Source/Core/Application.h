#pragma once

#include "Layer.h"
#include "Window.h"
#include "Event.h"
#include "SaveFileSystem.hpp"
#include "PopUpWarnings.hpp"

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
		std::string Name = "Application";
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
				std::println("Target layer found");
				for (std::unique_ptr<Layer>& layer : m_LayerStack)
				{
					layer->SetActive(false);
				}
				targetLayer->SetActive(true);
			}
			else
			{
				std::println("ERROR: layer not found");
			}
		}

		std::shared_ptr<Window> GetWindow() const { return m_Window; }

		static Application& Get();

		SavesFilesManager& GetSaveManager();
		PopUpWarnings& GetWarningManager() { return m_WarningManager; }
	
	private:

		ApplicationSpecification m_Specification;

		std::shared_ptr<Window> m_Window;

		bool m_Running = false;

		std::vector< std::unique_ptr<Layer> > m_LayerStack;

		SavesFilesManager m_SavesManager;
		PopUpWarnings m_WarningManager;

		friend class Layer;
		
	};

}


//template<typename TLayer>
//requires(std::is_base_of_v<Layer, TLayer>)
//void PushLayer()
		/*template<typename TLayer>
		requires(std::is_base_of_v<Layer, TLayer>)
		TLayer* GetLayer()
		{
			for (const auto& layer : m_LayerStack)
			{
				if (auto casted = dynamic_cast<TLayer*>(layer.get()))
				{
					return casted;
				}
			}
			return nullptr;
		}

		template<typename T>
		void SwitchLayer()
		{
			// looks for active Layers and deactivate them
			for (auto& layer : m_LayerStack)
			{
				if (layer->IsActive())
				{
					layer->SetActive(false);
				}
			}

			// Finds the layer to activate and sets it to true
			T* targetLayer = GetLayer<T>();
			if (targetLayer != nullptr)
			{
				targetLayer->SetActive(true);
			}
		}*/