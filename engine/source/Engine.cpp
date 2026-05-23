#include "Engine.h"
#include "Application.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace engine 
{
	Engine& Engine::GetInstance()
	{
		static Engine instance;
		return instance;
	}

	void keyCallback(GLFWwindow* window, int keyCode, int, int action, int)
	{

		auto& inputManager = engine::Engine::GetInstance().GetInputManager();
		if (action == GLFW_PRESS)
		{
			inputManager.SetKeyPressed(keyCode, true);
		}
		else if (action == GLFW_RELEASE)
		{
			inputManager.SetKeyPressed(keyCode, false);

		}
	}
	bool Engine::Init(int width, int height)
	{
		if (!m_application)
		{

			return false;
		}

		#if defined (__linux__)
			glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
		#endif

		if (!glfwInit())
		{
			return false;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow(width, height, "GameDevelopmentProject", nullptr, nullptr);

		if (m_window == nullptr)
		{
			std::cout << "Error creating window" << std::endl;
			glfwTerminate();
			return false;
		}

		glfwSetKeyCallback(m_window, keyCallback);


		glfwMakeContextCurrent(m_window);

		if (glewInit() != GLEW_OK)
		{
			glfwTerminate();
			return false;
		}
		return m_application->Init();
	}

	void Engine::Run()
	{
		if (!m_application)
		{
			return;
		}

		m_lastTimePoint = std::chrono::high_resolution_clock::now();

		while (!glfwWindowShouldClose(m_window) && !m_application->NeedsToBeClosed())
		{
			glfwPollEvents();

			auto now = std::chrono::high_resolution_clock::now();
			float deltaTime = std::chrono::duration<float>(now - m_lastTimePoint).count();
			m_lastTimePoint = now;

			m_application->Update(deltaTime);
			glfwSwapBuffers(m_window);
		}
	}

	void Engine::Destroy()
	{
		if (m_application)
		{
			m_application->Destroy();
			m_application.reset();
			glfwTerminate();
			m_window = nullptr;
		}
	}

	void Engine::SetApplication(Application* app)
	{
		m_application.reset(app);
	}

	Application* Engine::GetApplication()
	{
		return m_application.get();
	}
	InputManager& Engine::GetInputManager()
	{
		return m_inputManager;
	}
}
