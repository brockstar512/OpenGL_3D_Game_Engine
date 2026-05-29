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
/*
  - Take a reference when null is never valid — the caller must always provide a real object. The type system enforces this and the
  caller can't accidentally pass null.
  - Take a pointer when null is intentionally meaningful — e.g. Unbind() behavior, or an optional parameter.
  */