#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include "Consts.h"

class Entity;

class Game
{
public:
	Game();
	~Game();

	void init(const char *title, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	void render();
	void clean();

	static SDL_Renderer *renderer;
	static SDL_Event event;
	static SDL_Texture *backGroundTex;
	static bool isRunning;

	static int controlIdM;
	static bool setM;
	
	static int controlIdR;
	static bool setR;

	bool running() { return Game::isRunning; }

	static Entity* ronaldos[NUMPLAYER];
	static Entity* messis[NUMPLAYER];

private:
	int cnt = 0;
	SDL_Window *window;
};