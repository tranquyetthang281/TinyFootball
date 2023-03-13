#pragma once
#include "Component.h"
#include "Vector2D.h"
#include "Consts.h"

class TransformComponent : public Component
{
public:
	Vector2D position;
	Vector2D velocity;

	int height = 32;
	int width = 32;

	float speed = 3;
	float deltaSpeed = 0.0f;

	TransformComponent() {}

	TransformComponent(std::string t) : Component(t)
	{
		position.Zero();
	}

	TransformComponent(std::string t, float x, float y) : Component(t)
	{
		position.x = x;
		position.y = y;
	}

	TransformComponent(std::string t, float x, float y, int h, int w) : Component(t)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
	}

	void init() override
	{
		velocity.Zero();
	}

	void update() override
	{
		position.x += static_cast<int>(velocity.x * speed);
		position.y += static_cast<int>(velocity.y * speed);
		if (tag == "ball")
		{
			if (deltaSpeed > EPSILON)
			{
				if (speed > deltaSpeed)
				{
					speed -= deltaSpeed;
				}
				else
				{
					speed = 0.0f;
				}
			}
		}
	}
};