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

		bool m_Active = true; 
	};

}

