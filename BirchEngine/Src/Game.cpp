#include "Game.h"
#include "TextureManager.h"
#include "Vector2D.h"
#include "Component.h"
#include "Collision.h"
#include "string"
#include "Consts.h"
#include <vector>
#include <string>
SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;
SDL_Texture *Game::backGroundTex = nullptr;
Manager manager;
Entity* Game::ronaldos[] = {};
Entity *Game::messis[] = {};
Entity *ball(manager.addEntity());
Entity *players[2 * NUMPLAYER];
bool Game::isRunning = false;

int Game::controlIdM = 1 + NUMPLAYER;
bool Game::setM = false;
int Game::pointM = 0;

int Game::controlIdR = NUMPLAYER;
bool Game::setR = false;
int Game::pointR = 0;



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
		ronaldos[i]->addComponent<TransformComponent>("ronaldo", 100 + 100 * i, 100, 64, 64, 1 + i);
		ronaldos[i]->addComponent<SpriteComponent>("Imgs/ronaldo.png");
		ronaldos[i]->addComponent<RonaldoKeyboardController>();
		ronaldos[i]->addComponent<ColliderComponent>();
		players[i] = ronaldos[i];
	}

	ronaldos[controlIdR - 1]->getComponent<SpriteComponent>().SetTex("Imgs/ronaldo-active.png");

	for (int i = 0; i < NUMPLAYER; ++i)
	{
		messis[i] = manager.addEntity();
		messis[i]->addComponent<TransformComponent>("messi", 650 + 100 * i, 100, 64, 64, i + 1 + NUMPLAYER);
		messis[i]->addComponent<SpriteComponent>("Imgs/messi.png");
		messis[i]->addComponent<MessiKeyboardController>();
		messis[i]->addComponent<ColliderComponent>();
		players[i + NUMPLAYER] = messis[i];
	}

	messis[controlIdM - 1 - NUMPLAYER]->getComponent<SpriteComponent>().SetTex("Imgs/messi-active.png");


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

	SDL_RenderCopy(Game::renderer, Message, NULL, &Message_rect);

	manager.draw();
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}
// TTF_Font *Sans = TTF_OpenFont("Sans.ttf", 24);

// // this is the color in rgb format,
// // maxing out all would give you the color white,
// // and it will be your text's color
// SDL_Color White = {255, 255, 255};

// // as TTF_RenderText_Solid could only be used on
// // SDL_Surface then you have to create the surface first
// SDL_Surface *surfaceMessage =
// 		TTF_RenderText_Solid(Sans, "put your text here", White);

// // now you can convert it into a texture
// SDL_Texture *Message = SDL_CreateTextureFromSurface(Game::renderer, surfaceMessage);

// SDL_Rect Message_rect; // create a rect
// Message_rect.x = 0;		 // controls the rect's x coordinate
// Message_rect.y = 0;		 // controls the rect's y coordinte
// Message_rect.w = 100;	 // controls the width of the rect
// Message_rect.h = 100;	 // controls the height of the rect

// (0,0) is on the top left of the window/screen,
// think a rect as the text's box,
// that way it would be very simple to understand

// Now since it's a texture, you have to put RenderCopy
// in your game loop area, the area where the whole code executes

// you put the renderer's name first, the Message,
// the crop size (you can ignore this if you don't want
// to dabble with cropping), and the rect which is the size
// and coordinate of your texture
// SDL_RenderCopy(Game::renderer, Message, NULL, &Message_rect);

// // Don't forget to free your surface and texture
// SDL_FreeSurface(surfaceMessage);
// SDL_DestroyTexture(Message);