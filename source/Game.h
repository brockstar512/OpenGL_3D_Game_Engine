#pragma once
#include <eng.h>
#include <memory>

class Game : public engine::Application
{

public:
	bool Init() override;
	void Update(float deltaTime) override;
	void Destroy() override;

private:
	engine::Material m_material;
	std::unique_ptr<engine::Mesh> m_mesh;
	float m_offsetX = 0.0f;
	float m_offsetY = 0.0f;
};