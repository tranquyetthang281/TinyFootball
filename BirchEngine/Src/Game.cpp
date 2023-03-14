#include "Game.h"
#include "TextureManager.h"
#include "Vector2D.h"
#include "Component.h"
#include "Collision.h"
#include "string"
#include "Consts.h"
#include <vector>

SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;
SDL_Texture *Game::backGroundTex = nullptr;
Manager manager;
Entity *ronaldos[NUMPLAYER];
Entity *messis[NUMPLAYER];
Entity *ball(manager.addEntity());
Entity *players[2 * NUMPLAYER];
bool Game::isRunning = false;

Game::Game()
{
}

Game::~Game()
{
}

void Game::init(const char *title, int width, int height, bool fullscreen)
{
	int flags = 0;

	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		backGroundTex = TextureManager::LoadTexture("Imgs/pitches.png");
		Game::isRunning = true;
	}

	for (int i = 0; i < NUMPLAYER; ++i)
	{
		ronaldos[i] = manager.addEntity();
		ronaldos[i]->addComponent<TransformComponent>("ronaldo", 100 + 100 * i, 100, 64, 64, i + 1);
		ronaldos[i]->addComponent<SpriteComponent>("Imgs/ronaldo.png");
		//ronaldos[i]->addComponent<RonaldoKeyboardController>();
		ronaldos[i]->addComponent<ColliderComponent>();
		players[i] = ronaldos[i];
	}

	ronaldos[0]->addComponent<RonaldoKeyboardController>();

	for (int i = 0; i < NUMPLAYER; ++i)
	{
		messis[i] = manager.addEntity();
		messis[i]->addComponent<TransformComponent>("messi", 100 + 100 * i, 500, 64, 64, i + 1 + NUMPLAYER);
		messis[i]->addComponent<SpriteComponent>("Imgs/messi.png");
		messis[i]->addComponent<MessiKeyboardController>();
		messis[i]->addComponent<ColliderComponent>();
		players[i + NUMPLAYER] = messis[i];
	}

	ball->addComponent<TransformComponent>("ball", 300.0f, 300.0f);
	ball->addComponent<SpriteComponent>("Imgs/soccer-ball.png");
	ball->addComponent<ColliderComponent>();
}

void Game::handleEvents()
{

	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		Game::isRunning = false;
		break;
	default:
		break;
	}
}

void Game::update()
{
	manager.refresh();

	for (auto& p : players)
	{
		for (auto& op : players)
		{
			if (p->getComponent<TransformComponent>().id != op->getComponent<TransformComponent>().id)
				Collision::PlayerToPlayerCollision(p->getComponent<ColliderComponent>(), p->getComponent<TransformComponent>(),
					op->getComponent<ColliderComponent>(), op->getComponent<TransformComponent>());
		}
	}

	for (auto ronaldo : ronaldos)
	{
		Collision::PlayerBallCollision(ronaldo->getComponent<ColliderComponent>(), ronaldo->getComponent<TransformComponent>(),
																	 ball->getComponent<ColliderComponent>(), ball->getComponent<TransformComponent>());

		Collision::PlayerScreenCollision(ronaldo->getComponent<ColliderComponent>(),
																		 ronaldo->getComponent<TransformComponent>());
	}

	for (auto &messi : messis)
	{
		Collision::PlayerBallCollision(messi->getComponent<ColliderComponent>(), messi->getComponent<TransformComponent>(),
																	 ball->getComponent<ColliderComponent>(), ball->getComponent<TransformComponent>());

		Collision::PlayerScreenCollision(messi->getComponent<ColliderComponent>(),
																		 messi->getComponent<TransformComponent>());
	}

	Collision::BallScreenCollision(ball->getComponent<ColliderComponent>(),
																 ball->getComponent<TransformComponent>());

	manager.update();
}

void Game::render()
{
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, backGroundTex, NULL, NULL);
	manager.draw();
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}