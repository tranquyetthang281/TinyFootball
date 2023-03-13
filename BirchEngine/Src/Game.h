#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>

class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	void render();
	void clean();

	static SDL_Renderer* renderer;
	static SDL_Event event;
	static bool isRunning;

	bool running() { return Game::isRunning; }
private:
	int cnt = 0;
	SDL_Window *window;
};