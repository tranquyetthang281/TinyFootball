#pragma once

#include "Component.h"
#include "SDL.h"
#include "TransformComponent.h"
#include "TextureManager.h"
 
class SpriteComponent : public Component
{
private:
	TransformComponent* transform;

	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;


public:


	SpriteComponent() = default;
	SpriteComponent(const char* path)
	{
		texture = TextureManager::LoadTexture(path);
	}

	~SpriteComponent()
	{
		SDL_DestroyTexture(texture);
	}

	void SetTex(const char* path)
	{
		texture = TextureManager::LoadTexture(path);
	}

	void setTex(std::string id)
	{
	}

	void init() override
	{

		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}

	void update() override
	{
		destRect.x = static_cast<int>(transform->position.x);
		destRect.y = static_cast<int>(transform->position.y);
		destRect.w = transform->width;
		destRect.h = transform->height;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect);
	}
};