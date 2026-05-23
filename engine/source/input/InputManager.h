#pragma once
#include <array>
namespace engine {
	class InputManager {
		//this class is going to be owned and created by the engine.
		//so we will enforce that by creating the rule of 5
	private:
		InputManager() = default;
		InputManager(const InputManager&) = delete;
		InputManager(InputManager&&) = delete;
		InputManager& operator=(InputManager&&) = delete;
	public:
		void SetKeyPressed(int keyCode, bool isPressed);
		bool isKeyPressed(int keyCode);
	private:
		std::array<bool, 256>m_keys = { false};
		friend class Engine;
	};
}