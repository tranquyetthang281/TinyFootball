#include "Collision.h"
#include "ColliderComponent.h"
#include "Consts.h"
#include <algorithm>

// two match each other
const double DECISION_COLISION = 0.7;
const int OBJECT_SIZE = 21;
bool Collision::AABB(const SDL_Rect &recA, const SDL_Rect &recB)
{
	if (
			recA.x + recA.w * DECISION_COLISION >= recB.x &&
			recB.x + recB.w * DECISION_COLISION >= recA.x &&
			recA.y + recA.h * DECISION_COLISION >= recB.y &&
			recB.y + recB.h * DECISION_COLISION >= recA.y)
	{
		std::cout << recA.x << " " << recB.x << '\n';
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
	if (Screen(ballCollider) != ScreenCollision::NOPE)
	{
		ballTransform.velocity * -1;
	}
}

void Collision::PlayerBallCollision(const ColliderComponent &playerCollider, const TransformComponent &playerTransform,
																		const ColliderComponent &ballCollider, TransformComponent &ballTransform)
{
	if (AABB(playerCollider, ballCollider))
	{

		Vector2D vObjOther(playerTransform.position.x - ballTransform.position.x,
											 playerTransform.position.y - ballTransform.position.y);

		vObjOther.Normalize();
		// playerTransform.facing *= vObjOther.Rotate(PI).Length();
		double f = playerTransform.facing;
		if (f < EPSILON)
		{
			f = PI / 2;
		}
		ballTransform.facing = playerTransform.facing * vObjOther.Rotate(PI).Length();

		ballTransform.velocity = playerTransform.velocity;
		ballTransform.speed = playerTransform.speed * 3;
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
		double s = OBJECT_SIZE - dist / 2; // big enough for not to stick to each other
		player1Transform.position += (Vector2D(vObjOther.x * s, vObjOther.y * s));
		player2Transform.position += (Vector2D(vObjOther.x * s, vObjOther.y * s)).Rotate(PI);

		std::cout << vObjOther << " " << s << " " << player1Transform.deltaSpeed << " " << player1Transform.speed << " collide " << player2Transform.deltaSpeed << " " << player1Transform.speed << '\n';
	}
}
void Collision::PlayerWithWallCollision(const ColliderComponent &playerC, TransformComponent &playerT)
{
	if (playerT.OnBorder())
	{
		playerT.facing += PI;
		playerT.position += playerT.VelocityVect();
		playerT.position += playerT.VelocityVect();
		return;
		std::cout << "on border\n";
		int xWall = int(playerT.position.x / CELL_SIZE) * CELL_SIZE;
		int yWall = int(playerT.position.y / CELL_SIZE) * CELL_SIZE;

		double nearestPointX = std::max(float(xWall), std::min(playerT.position.x, float(xWall + CELL_SIZE)));
		double nearestPointY = std::max(float(yWall), std::min(playerT.position.y, float(yWall + CELL_SIZE)));
		Vector2D vObsToPlayer(nearestPointX - playerT.position.x, nearestPointY - playerT.position.y);
		double overlap = playerT.height / 2 - vObsToPlayer.Length();
		if (vObsToPlayer.Length() < EPSILON)
		{
			return;
		}
		if (overlap > 0)
		{
			std::cout << "overlap:" << overlap << '\n';
			vObsToPlayer.Normalize();
			playerT.position += Vector2D(vObsToPlayer.x * -abs(overlap), vObsToPlayer.y * -abs(overlap)).Normalize();
		}
	}
}