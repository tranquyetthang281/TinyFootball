#pragma once

#include "Game.h"
#include "ECS.h"
#include "Component.h"
#include <ctime>

class RonaldoKeyboardController : public Component
{
public:
	TransformComponent* transform;
	// SpriteComponent* sprite;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		// sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override
	{
		//if (transform->id != Game::controlIdR)
		//{
		//	transform->speed = 3.0f;
		//	transform->deltaSpeed = 0.0f;
		//	srand(time(NULL));
		//	int res = rand();
		//	if (res % 2 == 0)
		//	{

		//	}
		//}

		if (Game::event.type == SDL_KEYDOWN)
		{
			// std::cout << transform->id << " " << Game::controlIdR << '\n';
			if (transform->id == Game::controlIdR)
			{
				transform->speed = 3.0f;
				transform->deltaSpeed = 0.0f;
				switch (Game::event.key.keysym.sym)
				{
				case SDLK_w:
					transform->velocity.y = -1;
					// sprite->Play("Walk");
					break;
				case SDLK_a:
					transform->velocity.x = -1;
					// sprite->Play("Walk");
					// sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
					break;
				case SDLK_d:
					transform->velocity.x = 1;
					// sprite->Play("Walk");
					break;
				case SDLK_s:
					transform->velocity.y = 1;
					// sprite->Play("Walk");
					break;
				case SDLK_SPACE:
					if (Game::setR == false)
					{
						Game::ronaldos[Game::controlIdR - 1]->getComponent<SpriteComponent>().SetTex("Imgs/ronaldo.png");

						Game::controlIdR++;
						if (Game::controlIdR > NUMPLAYER)
						{
							Game::controlIdR %= NUMPLAYER;
						}
						std::cout << transform->id << " " << Game::controlIdR << '\n';
						Game::setR = true;
						Game::ronaldos[Game::controlIdR - 1]->getComponent<SpriteComponent>().SetTex("Imgs/ronaldo-active.png");

					}
					break;
				default:
					break;
				}
			}
		}

		if (Game::event.type == SDL_KEYUP)
		{
			if (transform->id == Game::controlIdR)
			{
				switch (Game::event.key.keysym.sym)
				{
				case SDLK_w:
					transform->velocity.y = 0;
					// sprite->Play("Idle");
					break;
				case SDLK_a:
					transform->velocity.x = 0;
					/*			sprite->Play("Idle");
								sprite->spriteFlip = SDL_FLIP_NONE;*/
					break;
				case SDLK_d:
					transform->velocity.x = 0;
					// sprite->Play("Idle");
					break;
				case SDLK_s:
					transform->velocity.y = 0;
					// sprite->Play("Idle");
					break;
				case SDLK_ESCAPE:
					Game::isRunning = false;
					break;
				case SDLK_SPACE:
					Game::setR = false;
					break;

				default:
					break;
				}
			}
		}
	}
};