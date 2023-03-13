#include "Game.h"
#include "TextureManager.h"
#include "Vector2D.h"
#include "Component.h"
#include "Collision.h"
#include "string"

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
Manager manager;
auto& ronaldo(manager.addEntity());
auto& messi(manager.addEntity());
auto& ball(manager.addEntity());
bool Game::isRunning = false;

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int width, int height, bool fullscreen)
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
		Game::isRunning = true;
	}

	ronaldo.addComponent<TransformComponent>("ronaldo", 100, 100, 64, 64);
	ronaldo.addComponent<SpriteComponent>("Imgs/ronaldo.png");
	ronaldo.addComponent<RonaldoKeyboardController>();
	ronaldo.addComponent<ColliderComponent>();

	messi.addComponent<TransformComponent>("messi", 500, 100, 64, 64);
	messi.addComponent<SpriteComponent>("Imgs/messi.png");
	messi.addComponent<MessiKeyboardController>();
	messi.addComponent<ColliderComponent>();

	ball.addComponent<TransformComponent>("ball", 300.0f, 300.0f);
	ball.addComponent<SpriteComponent>("Imgs/soccer-ball.png");
	ball.addComponent<ColliderComponent>();
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

	Collision::PlayerScreenCollision(ronaldo.getComponent<ColliderComponent>(), 
		ronaldo.getComponent<TransformComponent>());

	Collision::PlayerScreenCollision(messi.getComponent<ColliderComponent>(),
		messi.getComponent<TransformComponent>());

	Collision::BallScreenCollision(ball.getComponent<ColliderComponent>(),
		ball.getComponent<TransformComponent>());

	Collision::PlayerBallCollision(ronaldo.getComponent<ColliderComponent>(), ronaldo.getComponent<TransformComponent>(),
		ball.getComponent<ColliderComponent>(), ball.getComponent<TransformComponent>());

	Collision::PlayerBallCollision(messi.getComponent<ColliderComponent>(), messi.getComponent<TransformComponent>(),
		ball.getComponent<ColliderComponent>(), ball.getComponent<TransformComponent>());

	manager.update();
}

void Game::render()
{
	//SDL_RenderClear(renderer);
	auto background = TextureManager::LoadTexture("Imgs/pitches.png");
	SDL_RenderCopy(Game::renderer, background, NULL, NULL);
	manager.draw();
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}