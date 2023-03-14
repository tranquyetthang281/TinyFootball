#pragma once

#include "Game.h"
#include "ECS.h"
#include "Component.h"

class RonaldoKeyboardController : public Component
{
public:
	TransformComponent *transform;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override
	{
		if (Game::event.type == SDL_KEYDOWN)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				transform->speed = NORMAL_SPEED;
				transform->facing = -90 * FACING_CONVERSION;
				break;
			case SDLK_a:
				transform->speed = NORMAL_SPEED;
				transform->facing = 180 * FACING_CONVERSION;
				break;
			case SDLK_d:
				transform->speed = NORMAL_SPEED;
				transform->facing = 0 * FACING_CONVERSION;
				break;
			case SDLK_s:
				transform->speed = NORMAL_SPEED;
				transform->facing = 90 * FACING_CONVERSION;
				break;
			default:
				break;
			}
			transform->position += transform->VelocityVect();
		}

		if (Game::event.type == SDL_KEYUP)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				transform->speed = 0;
				break;
			case SDLK_a:
				transform->speed = 0;
				break;
			case SDLK_d:
				transform->speed = 0;
				break;
			case SDLK_s:
				transform->speed = 0;
				break;
			case SDLK_ESCAPE:
				Game::isRunning = false;
			default:
				break;
			}
			transform->position += transform->VelocityVect();
		}
	}
};