#pragma once
#include "Component.h"
#include "Vector2D.h"
#include "Consts.h"
#include "Game.h"
class TransformComponent : public Component
{
public:
	int id;
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
		position.x += static_cast<int>(velocity.x * speed);
		position.y += static_cast<int>(velocity.y * speed);

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
		if (tag == "ball")
		{
			if (position.x < 10 && position.y >= 280 && position.y <= 400)
			{
				// std::cout << "in ball1\n";
				Game::pointM++;
				position.x = 525;
				position.y = 340;
				speed = 0;
				deltaSpeed = 0;
				std::cout << "Messi* - Ronaldo :" << Game::pointM << "-" << Game::pointR << '\n';
				return;
			}
			if (position.x > 1010 && position.y >= 280 && position.y <= 400)
			{
				std::cout << "in ball2\n";
				Game::pointR++;
				position.x = 525;
				position.y = 340;
				speed = 0;
				deltaSpeed = 0;
				std::cout << "Messi - Ronaldo* :" << Game::pointM << "-" << Game::pointR << '\n';
			}
		}
	}
	double DistanceTo(const TransformComponent &t)
	{
		Vector2D v(position.x - t.position.x, position.y - t.position.y);
		return v.Length();
	}
};