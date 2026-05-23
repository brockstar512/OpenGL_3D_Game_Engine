#include "InputManager.h"

namespace engine {
	void InputManager::SetKeyPressed(int keyCode, bool isPressed)
	{
		if (keyCode < 0 || keyCode > static_cast<int>(m_keys.size()))
		{
			return;
		}
		m_keys[keyCode] = isPressed;
	}

	bool InputManager::isKeyPressed(int keyCode)
	{
		if (keyCode < 0 || keyCode > static_cast<int>(m_keys.size()))
		{
			return false;
		}
		return m_keys[keyCode];;
	}
}