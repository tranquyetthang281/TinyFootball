#include "Collision.h"
#include "ColliderComponent.h"
#include "Consts.h"

bool Collision::AABB(const SDL_Rect &recA, const SDL_Rect &recB)
{
	if (
			recA.x + recA.w >= recB.x &&
			recB.x + recB.w >= recA.x &&
			recA.y + recA.h >= recB.y &&
			recB.y + recB.h >= recA.y)
	{
		return true;
	}

	return false;
}

bool Collision::AABB(const ColliderComponent &colA, const ColliderComponent &colB)
{
	if (AABB(colA.collider, colB.collider))
	{
		// std::cout << colA.tag << " hit: " << colB.tag << std::endl;
		return true;
	}
	else
	{
		return false;
	}
}

ScreenCollision Collision::Screen(const ColliderComponent &rec)
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

void Collision::PlayerScreenCollision(const ColliderComponent &playerCollider, TransformComponent &playerTransform)
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

void Collision::BallScreenCollision(const ColliderComponent &ballCollider, TransformComponent &ballTransform)
{
	switch (Screen(ballCollider))
	{
	case ScreenCollision::LEFT:
	case ScreenCollision::RIGHT:
	{
		Vector2D normalX(1, 0);
		ballTransform.velocity - (normalX * (2 * ballTransform.velocity.Dot(normalX)));
	}
	break;
	case ScreenCollision::UP:
	case ScreenCollision::DOWN:
	{
		Vector2D normalY(0, 1);
		ballTransform.velocity - (normalY * (2 * ballTransform.velocity.Dot(normalY)));
	}
	break;
	case ScreenCollision::LEFTDOWN:
	case ScreenCollision::LEFTUP:
	case ScreenCollision::RIGHTDOWN:
	case ScreenCollision::RIGHTUP:
		ballTransform.velocity * -1;
		break;

	default:
		break;
	};
}

void Collision::PlayerBallCollision(const ColliderComponent &playerCollider, const TransformComponent &playerTransform,
																		const ColliderComponent &ballCollider, TransformComponent &ballTransform)
{
	if (AABB(playerCollider, ballCollider))
	{
		ballTransform.velocity = playerTransform.velocity;
		ballTransform.speed = 10;
		ballTransform.deltaSpeed = 10.0f / DELTAFRAME;
	}
}
void Collision::PlayerToPlayerCollision(const ColliderComponent &player1Collider, TransformComponent &player1Transform,
																				const ColliderComponent &player2Collider, TransformComponent &player2Transform)
{
	if (AABB(player1Collider, player2Collider))
	{
		Vector2D vObjOther(player1Transform.position.x - player2Transform.position.x,
											 player1Transform.position.y - player2Transform.position.y);
		vObjOther.Normalize();
		double dist = player1Transform.DistanceTo(player2Transform);
		double s = player1Transform.height - dist / 2; // big enough for not to stick to each other
		// player1Transform.position += (Vector2D(vObjOther.x * s, vObjOther.y * s));
		// player2Transform.position += (Vector2D(vObjOther.x * s, vObjOther.y * s)).Rotate(PI);
		player1Transform.velocity += (vObjOther);
		player2Transform.velocity += (vObjOther).Rotate(PI);
		
		std::cout << vObjOther << " " << s << " " << player1Transform.deltaSpeed << " " << player1Transform.speed << " collide " << player2Transform.deltaSpeed << " " << player1Transform.speed << '\n';
	}
}