#pragma once

#include "Event.h"

#include <format>

namespace Core {

	//
	// Key Events
	//
	/*
	
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
	};


	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }
	
	*/

	// Changed from macro to explicit function calls, because it was hard to understand

	class KeyEvent : public Event
	{
	public:
		int GetKeyCode() const { return m_KeyCode; }
	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}

		int m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, bool isRepeat)
			: KeyEvent(keycode), m_IsRepeat(isRepeat) {}

		bool IsRepeat() const { return m_IsRepeat; }

		std::string ToString() const override
		{
			return std::format("KeyPressedEvent: {} (repeat={})", m_KeyCode, m_IsRepeat);
		}

		//EVENT_CLASS_TYPE(KeyPressed)

		static EventType GetStaticType()
		{
			return EventType::KeyPressed;
		}
		EventType GetEventType() const override
		{
			return GetStaticType();
		}
		const char* GetName() const override
		{
			return "KeyPressed";
		}

	private:
		bool m_IsRepeat;
	};

	class IsKeyPressedEvent : public KeyEvent
	{
	public:
		IsKeyPressedEvent(int keycode, bool isRepeat)
			: KeyEvent(keycode), m_IsRepeat(isRepeat) {}

		bool IsRepeat() const { return m_IsRepeat; }

		std::string ToString() const override
		{
			return std::format("KeyPressedEvent: {} (repeat={})", m_KeyCode, m_IsRepeat);
		}

		//EVENT_CLASS_TYPE(KeyPressed)

		static EventType GetStaticType()
		{
			return EventType::IsKeyPressed;
		}
		EventType GetEventType() const override
		{
			return GetStaticType();
		}
		const char* GetName() const override
		{
			return "IsKeyPressed";
		}

	private:
		bool m_IsRepeat;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			return std::format("KeyReleasedEvent: {}", m_KeyCode);
		}

		int GetKeyCode() { return m_KeyCode; }

		//EVENT_CLASS_TYPE(KeyReleased)
		static EventType GetStaticType()
		{
			return EventType::KeyReleased;
		}
		EventType GetEventType() const override
		{
			return GetStaticType();
		}
		const char* GetName() const override
		{
			return "KeyReleased";
		}

		

	};



	// ================              ================ //
	// //////////////// Mouse Events //////////////// //
	// ================              ================ //

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float xPos, float yPos)
			: m_MouseX(xPos), m_MouseY(yPos) {}

		float GetX() const { return m_MouseX; }
		float GetY() const { return m_MouseY; }

		std::string ToString() const override
		{
			return std::format("MouseMovedEvent: {}, {}", m_MouseX, m_MouseY);
		}

		//EVENT_CLASS_TYPE(MouseMoved)

		static EventType GetStaticType()
		{
			return EventType::MouseMoved;
		}
		EventType GetEventType() const override
		{
			return GetStaticType();
		}
		const char* GetName() const override
		{
			return "MouseMoved";
		}

	private:
		float m_MouseX, m_MouseY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {}

		float GetXOffset() const { return m_XOffset; }
		float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override
		{
			return std::format("MouseScrolledEvent: {}, {}", m_XOffset, m_YOffset);
		}

		//EVENT_CLASS_TYPE(MouseScrolled)

		static EventType GetStaticType()
		{
			return EventType::MouseScrolled;
		}
		EventType GetEventType() const override
		{
			return GetStaticType();
		}
		const char* GetName() const override
		{
			return "MouseScrolled";
		}

	private:
		float m_XOffset, m_YOffset;
	};

	class MouseButtonEvent : public Event
	{
	public:
		int GetMouseButton() const { return m_Button; }
	protected:
		MouseButtonEvent(int button)
			: m_Button(button) {}

		int m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			return std::format("MouseButtonPressedEvent: {}", m_Button);
		}

		//EVENT_CLASS_TYPE(MouseButtonPressed)

		static EventType GetStaticType()
		{
			return EventType::MouseButtonPressed;
		}
		 EventType GetEventType() const override
		{
			return GetStaticType();
		}
		 const char* GetName() const override
		{
			return "MouseButtonPressed";
		}

	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			return std::format("MouseButtonReleasedEvent: {}", m_Button);
		}

		//EVENT_CLASS_TYPE(MouseButtonReleased)


		static EventType GetStaticType()
		{
			return EventType::MouseButtonReleased;
		}
		 EventType GetEventType() const override
		{
			return GetStaticType();
		}
		 const char* GetName() const override
		{
			return "MouseButtonReleased";
		}


	};

	
}
