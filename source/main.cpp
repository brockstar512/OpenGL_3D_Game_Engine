#include "Game.h"
#include <eng.h>
#include <iostream>

int main()
{

	std::cout << "main thread" << std::endl;
	Game* game = new Game();
	engine::Engine& engine = engine::Engine::GetInstance();
	engine.SetApplication(game);

	if (engine.Init(1280,720))
	{
		engine.Run();
	}
	else 
	{
		std::cout << "init failed" << std::endl;

	}

	engine.Destroy();
	return 0;
}