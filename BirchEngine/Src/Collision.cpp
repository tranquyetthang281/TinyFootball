#include "Collision.h"
#include "ColliderComponent.h"
#include "Consts.h"

bool Collision::AABB(const SDL_Rect& recA, const SDL_Rect& recB)
{
	if (
		recA.x + recA.w >= recB.x &&
		recB.x + recB.w >= recA.x &&
		recA.y + recA.h >= recB.y &&
		recB.y + recB.h >= recA.y
		)
	{
		return true;
	}

	return false;
}

bool Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB)
{
	if (AABB(colA.collider, colB.collider))
	{
		//std::cout << colA.tag << " hit: " << colB.tag << std::endl;
		return true;
	}
	else
	{
		return false;
	}
}

ScreenCollision Collision::Screen(const ColliderComponent& rec)
{
	if (rec.collider.x <= 0)
	{
		if (rec.collider.y <= 0)
			return ScreenCollision::LEFTUP;

		if (rec.collider.y + rec.collider.h >= HEIGHT)
			return ScreenCollision::LEFTDOWN;

		return ScreenCollision::LEFT;
	}

	if (rec.collider.x + rec.collider.w >= WIDTH)
	{
		if (rec.collider.y <= 0)
			return ScreenCollision::RIGHTUP;

		if (rec.collider.y + rec.collider.h >= HEIGHT)
			return ScreenCollision::RIGHTDOWN;

		return ScreenCollision::RIGHT;
	}

	if (rec.collider.y <= 0)
	{
		return ScreenCollision::UP;
	}

	if (rec.collider.y + rec.collider.h >= HEIGHT)
		return ScreenCollision::DOWN;

	return ScreenCollision::NOPE;
}

void Collision::PlayerScreenCollision(const ColliderComponent& playerCollider, TransformComponent& playerTransform)
{
	switch (Screen(playerCollider))
	{
	case ScreenCollision::RIGHT:
		if (playerTransform.velocity.x > 0)
			playerTransform.velocity.x = 0;
		break;
	case ScreenCollision::LEFT:
		if (playerTransform.velocity.x < 0)
			playerTransform.velocity.x = 0;
		break;
	case ScreenCollision::UP:
		if (playerTransform.velocity.y < 0)
			playerTransform.velocity.y = 0;
		break;
	case ScreenCollision::DOWN:
		if (playerTransform.velocity.y > 0)
			playerTransform.velocity.y = 0;
		break;
	case ScreenCollision::RIGHTUP:
		if (playerTransform.velocity.x > 0)
			playerTransform.velocity.x = 0;
		if (playerTransform.velocity.y < 0)
			playerTransform.velocity.y = 0;
		break;
	case ScreenCollision::RIGHTDOWN:
		if (playerTransform.velocity.x > 0)
			playerTransform.velocity.x = 0;
		if (playerTransform.velocity.y > 0)
			playerTransform.velocity.y = 0;
		break;
	case ScreenCollision::LEFTUP:
		if (playerTransform.velocity.x < 0)
			playerTransform.velocity.x = 0;
		if (playerTransform.velocity.y < 0)
			playerTransform.velocity.y = 0;
		break;
	case ScreenCollision::LEFTDOWN:
		if (playerTransform.velocity.x < 0)
			playerTransform.velocity.x = 0;
		if (playerTransform.velocity.y > 0)
			playerTransform.velocity.y = 0;
		break;
	default:
		break;
	}
}

void Collision::BallScreenCollision(const ColliderComponent& ballCollider, TransformComponent& ballTransform)
{
	if (Screen(ballCollider) != ScreenCollision::NOPE)
	{
		ballTransform.velocity * -1;
	}
}

void Collision::PlayerBallCollision(const ColliderComponent& playerCollider, const TransformComponent& playerTransform,
	const ColliderComponent& ballCollider, TransformComponent& ballTransform)
{
	if (AABB(playerCollider, ballCollider))
	{
		ballTransform.velocity = playerTransform.velocity;
		ballTransform.speed = 10;
		ballTransform.deltaSpeed = 10.0f / DELTAFRAME;
	}
}