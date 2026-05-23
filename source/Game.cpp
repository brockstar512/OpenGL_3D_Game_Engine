#include "Game.h"
#include <GLFW/glfw3.h>
#include <iostream>

bool Game::Init()
{
	return true;
}

void Game::Update(float deltaTime)
{
	//std::cout << "Current deltaTime: " << deltaTime << std::endl;
	auto& input = engine::Engine::GetInstance().GetInputManager();
	if (input.isKeyPressed(GLFW_KEY_A))
	{
		std::cout << "[A] Button is pressed " << std::endl;

	}
}

void Game::Destroy()
{

}
