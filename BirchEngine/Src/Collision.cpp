#include "Collision.h"
#include "ColliderComponent.h"
#include "Consts.h"

bool Collision::lockedBallScreen = false;
bool Collision::lockedBallPlayer = false;
int Collision::playerId = -1;
std::string Collision::playerTag = "";

bool Collision::AABB(const SDL_Rect& recA, const SDL_Rect& recB)
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

bool Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB)
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

	switch (Screen(ballCollider))
	{
	case ScreenCollision::LEFT:
	case ScreenCollision::RIGHT:
		if (!lockedBallScreen)
		{
			Vector2D normalX(1, 0);
			ballTransform.velocity - (normalX * (2 * ballTransform.velocity.Dot(normalX)));
			lockedBallScreen = false;
		}
		break;
	case ScreenCollision::UP:
	case ScreenCollision::DOWN:
		if (!lockedBallScreen)
		{
			Vector2D normalY(0, 1);
			ballTransform.velocity - (normalY * (2 * ballTransform.velocity.Dot(normalY)));
			lockedBallScreen = false;
		}
		break;
	case ScreenCollision::LEFTDOWN:
	case ScreenCollision::LEFTUP:
	case ScreenCollision::RIGHTDOWN:
	case ScreenCollision::RIGHTUP:
		if (!lockedBallScreen)
		{
			ballTransform.velocity * -1;
			lockedBallScreen = false;
		}
		break;

	default:
		lockedBallScreen = false;
		break;
	};
}

void Collision::PlayerBallCollision(const ColliderComponent& playerCollider, TransformComponent& playerTransform,
	const ColliderComponent& ballCollider, TransformComponent& ballTransform)
{
	if (AABB(playerCollider, ballCollider))
	{
		//Vector2D vObjOther(playerTransform.position.x - ballTransform.position.x,
		//	playerTransform.position.y - ballTransform.position.y);
		//vObjOther.Normalize();
		////player1Transform.velocity + (vObjOther);
		//ballTransform.velocity + vObjOther.Rotate(PI);
		//ballTransform.velocity.Normalize();
		if (!lockedBallPlayer)
		{
			if (playerTransform.velocity.Length() < EPSILON)
			{
				ballTransform.velocity * -1;

			}
			else if (ballTransform.velocity.Dot(playerTransform.velocity) > 0)
			{
				ballTransform.velocity * -1;
			}
			else
			{
				ballTransform.velocity = playerTransform.velocity;
				ballTransform.speed = 12.0f;
				ballTransform.deltaSpeed = 12.0f / DELTAFRAME;
			}
			lockedBallPlayer = true;
			playerId = playerTransform.id;
			playerTag = playerTransform.tag;
		}
	}
	else
	{
		if (playerId == playerTransform.id && playerTag == playerTransform.tag)
			lockedBallPlayer = false;
	}
}

void Collision::PlayerToPlayerCollision(const ColliderComponent& player1Collider, TransformComponent& player1Transform,
	const ColliderComponent& player2Collider, TransformComponent& player2Transform)
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
		player1Transform.velocity + (vObjOther);
		player1Transform.velocity.Normalize();
		player1Transform.speed = 3.0f;
		player1Transform.deltaSpeed = 3.0f / DELTAFRAME;
		player2Transform.velocity + (vObjOther).Rotate(PI);
		player2Transform.velocity.Normalize();
		player2Transform.speed = 3.0f;
		player2Transform.deltaSpeed = 3.0f / DELTAFRAME;
	}
}