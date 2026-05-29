#pragma once
#include "input/InputManager.h"
#include "graphics/GraphicsAPI.h"
#include "render/RenderQueue.h"
#include <memory>
#include <chrono>

struct GLFWwindow;
namespace engine
{
	class Application;

	class Engine
	{
		public:
			static Engine& GetInstance();
		private: 
			
			Engine() = default;
			Engine(const Engine&) = delete;
			Engine(Engine&&) = delete;
			Engine& operator=(Engine&&) = delete;

		public:
			bool Init(int width, int height);
			void Run();
			void Destroy();
			void SetApplication(Application* app);
			Application* GetApplication();
			InputManager& GetInputManager();
			GraphicsAPI& GetGraphicsAPI();
			RenderQueue& GetRenderQueue();

	private:
		std::unique_ptr<Application> m_application;
		std::chrono::steady_clock::time_point m_lastTimePoint;
		GLFWwindow* m_window = nullptr;
		InputManager m_inputManager;
		GraphicsAPI m_graphicsAPI;
		RenderQueue m_renderQueue;
	};
}

	/*
		pointer vs reference member getter 
		* The core question is: can this thing legitimately not exist right now?
		If no, return a reference. If yes, return a pointer.

		Looking at your three getters, the likely reasoning is:

InputManager& — the input manager is almost certainly created once at startup and lives for the entire lifetime of the app. There's never a moment where "input doesn't exist yet" is a meaningful state for callers. Reference.
Application* — probably nullable because there's a global accessor and during very early static init or post-shutdown, there may genuinely be no application. Or the function might be called from places that don't assume one exists.
GraphicsAPI* — graphics backends are typically created late (after window/config), can be swapped (DX12/Vulkan/Metal), can be absent in headless/server modes, and may be torn down before the rest of the engine. So "no graphics API right now" is a real state worth representing.

A few related rules of thumb:

Neither raw pointer nor reference implies ownership. Both say "I'm lending this to you." Ownership lives in unique_ptr / shared_ptr.
If you'd find yourself writing if (&ref == nullptr) or doing weird tricks to represent absence, you wanted a pointer.
If you'd find yourself writing assert(ptr) at the top of every caller, you probably wanted a reference.
std::optional<T&> isn't in the standard yet (proposed for C++26), so a raw pointer is still the idiomatic "optional reference" in modern C++.
Don't pick pointer just because "maybe someday it could be null." YAGNI — change the signature when reality changes. Overly defensive pointer-returning getters pollute calling code with null checks that never fire.
	*/