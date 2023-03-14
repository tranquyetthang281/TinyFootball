#pragma once
#include "Component.h"
#include "Vector2D.h"
#include "Consts.h"

class TransformComponent : public Component
{
public:
	int id = 1;
	Vector2D position;
	Vector2D velocity;

	int height = 32;
	int width = 32;

	float speed = 0;
	float deltaSpeed = 0.0f;
	double facing = 0.5;

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

	TransformComponent(std::string t, float x, float y, int h, int w, int i) : Component(t)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		id = i;
	}

	void init() override
	{
		velocity.Zero();
	}

	void update() override
	{

		// position += VelocityVect();
		// std::cout << id << " " << position << '\n';
		if (tag == "ball")
		{
			position.x += static_cast<int>(cos(facing) * speed);
			position.y += static_cast<int>(sin(facing) * speed);
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
			return;
		}
	}
	double DistanceTo(const TransformComponent &t)
	{
		Vector2D v(position.x - t.position.x, position.y - t.position.y);
		return v.Length();
	}
	Vector2D &VelocityVect()
	{
		return Vector2D(speed * cos(facing), speed * sin(facing));
	}
	bool OnBorder()
	{
		int x = position.x / CELL_SIZE;
		int y = position.y / CELL_SIZE;
		// std::cout << x << " " << y << '\n';
		return (x <= 0) || (y >= WIDTH / CELL_SIZE - 1) ||
					 (y <= 0) || (y >= HEIGHT / CELL_SIZE - 1);
	}
};