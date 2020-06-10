#pragma once

#include "glm/glm.hpp"

struct Transformation
{
	glm::vec3 Translation;
	float Rotation;
	glm::vec3 Scale;
	bool Flipped;

	Transformation()
	{
		Translation = glm::vec3(0.0f, 0.0f, 0.0f);
		Rotation = 0.0f;
		Scale = glm::vec3(1.0f, 1.0f, 0.0f);
		Flipped = false;
	}
};

class Drawable
{
public:
	Drawable() {}
	virtual ~Drawable() {}
	
	// Interface methods
	virtual void OnUpdate(float timeStep) {}
	virtual void OnRender() {}
	virtual void OnImGuiRender() {}
};