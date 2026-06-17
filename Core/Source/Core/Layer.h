#pragma once

#include "Event.h"

#include <memory>

namespace Core {

	class Layer
	{
	public:

		virtual ~Layer() = default;

		virtual void OnEvent(Event& event) {}
		virtual void OnUpdate() {}
		virtual void OnRender() {}

		// Add these control functions
		bool IsActive() const { return m_Active; }
		void SetActive(bool active) { m_Active = active; }

	private:

		bool m_Active = true; // Layers are active by default
	};

}

		/*template<std::derived_from<Layer> T>
		void TransitionTo()
		{
			this->SetActive(false);

			auto targetLayer = Application::Get().GetLayer();
			if (targetLayer)
			{
				targetLayer->SetActive(true);
			}
		}*/

		/*virtual ~Layer() = default;

		virtual void OnEvent(Event& event) {}

		virtual void OnUpdate(float ts) {}
		virtual void OnRender() {}

		template<std::derived_from<Layer> T, typename... Args> 
		void TransitionTo(Args&&... args)
		{
			QueueTransition(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
		}


	private:
		void QueueTransition(std::unique_ptr<Layer> layer);*/