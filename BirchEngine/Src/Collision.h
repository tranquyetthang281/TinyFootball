#pragma once
#include <SDL.h>

class ColliderComponent;
class TransformComponent;

enum class ScreenCollision
{
	RIGHT = 0,
	LEFT = 1,
	UP = 2,
	DOWN = 3,
	RIGHTUP = 4,
	RIGHTDOWN = 5,
	LEFTUP = 6,
	LEFTDOWN = 7,
	NOPE = 8
};

class Collision
{
public:
	static bool AABB(const SDL_Rect &recA, const SDL_Rect &recB);
	static bool AABB(const ColliderComponent &colA, const ColliderComponent &colB);
	static ScreenCollision Screen(const ColliderComponent &rec);
	static void PlayerScreenCollision(const ColliderComponent &playerCollider, TransformComponent &playerTransform);
	static void BallScreenCollision(const ColliderComponent &ballCollider, TransformComponent &ballTransform);
	static void PlayerBallCollision(const ColliderComponent &playerCollider, TransformComponent &playerTransform,
																	const ColliderComponent &ballCollider, TransformComponent &ballTransform);
	static void PlayerToPlayerCollision(const ColliderComponent& player1Collider, TransformComponent& player1Transform,
		const ColliderComponent& player2Collider, TransformComponent& player2Transform);
};
