#pragma once
#include <string>
#include <SDL.h>
#include "Component.h"
#include "TextureManager.h"

class ColliderComponent : public Component
{
public:
	SDL_Rect collider;


	TransformComponent* transform;

	ColliderComponent() {}

	ColliderComponent(std::string t) : Component(t)
	{
	}

	void init() override
	{
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}

		transform = &entity->getComponent<TransformComponent>();

	}

	void update() override
	{

		collider.x = static_cast<int>(transform->position.x);
		collider.y = static_cast<int>(transform->position.y);
		collider.w = transform->width;
		collider.h = transform->height;
	}

	void draw() override
	{
		//TextureManager::Draw(tex, srcR, destR, SDL_FLIP_NONE);
	}

private:

};