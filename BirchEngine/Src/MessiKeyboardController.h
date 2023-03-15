#pragma once

#include "Game.h"
#include "ECS.h"
#include "Component.h"

class MessiKeyboardController : public Component
{
public:
	TransformComponent* transform;
	SpriteComponent* sprite;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override
	{
		if (Game::event.type == SDL_KEYDOWN)
		{
			if (transform->id == Game::controlIdM)
			{
				transform->speed = 3.0f;
				transform->deltaSpeed = 0.0f;
				switch (Game::event.key.keysym.sym)
				{
				case SDLK_UP:
					transform->velocity.y = -1;
					// sprite->Play("Walk");
					break;
				case SDLK_LEFT:
					transform->velocity.x = -1;
					// sprite->Play("Walk");
					// sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
					break;
				case SDLK_RIGHT:
					transform->velocity.x = 1;
					// sprite->Play("Walk");
					break;
				case SDLK_DOWN:
					transform->velocity.y = 1;
					// sprite->Play("Walk");
					break;
				case SDLK_RETURN:
					if (Game::setM == false)
					{
						Game::messis[Game::controlIdM - 1 - NUMPLAYER]->getComponent<SpriteComponent>().SetTex("Imgs/messi.png");
						Game::controlIdM++;
						if (Game::controlIdM > 2 * NUMPLAYER)
						{
							Game::controlIdM = (Game::controlIdM % NUMPLAYER) + NUMPLAYER;
						}
						std::cout << transform->id << " " << Game::controlIdM << '\n';
						Game::setM = true;
						Game::messis[Game::controlIdM - 1 - NUMPLAYER]->getComponent<SpriteComponent>().SetTex("Imgs/messi-active.png");
					}
					break;
				default:
					break;
				}
			}
		}

		if (Game::event.type == SDL_KEYUP)
		{
			if (transform->id == Game::controlIdM)
			{
				switch (Game::event.key.keysym.sym)
				{
				case SDLK_UP:
					transform->velocity.y = 0;
					// sprite->Play("Idle");
					break;
				case SDLK_LEFT:
					transform->velocity.x = 0;
					/*			sprite->Play("Idle");
								sprite->spriteFlip = SDL_FLIP_NONE;*/
					break;
				case SDLK_RIGHT:
					transform->velocity.x = 0;
					// sprite->Play("Idle");
					break;
				case SDLK_DOWN:
					transform->velocity.y = 0;
					// sprite->Play("Idle");
					break;
				case SDLK_ESCAPE:
					Game::isRunning = false;
					break;
				case SDLK_RETURN:
					Game::setM = false;
					break;
				default:
					break;
				}
			}
		}
	}
};