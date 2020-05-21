#pragma once
#include <SDL.h>
#include "SDL_mixer.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Constants.h"
#include "GameScreenManager.h"
#include "SoundEffect.h"
#include <string>

class Window
{
public:
	Window(const std::string& title, int width, int height);
	~Window();

	void pollEvents();
	void Clear() const;
	void LoadMusic(string path);

	bool GetTextOnePlayer();
	bool GetTextTwoPlayer();

	inline bool isClosed() const { return _closed; };

	static SDL_Renderer* gRenderer;

private:
	bool init();

	std::string _title;
	int _width = 512;
	int _height = 416;

	bool _closed = false;

	SDL_Window* gWindow = nullptr;
	
	Mix_Music* gMusic = nullptr;
	Mix_Chunk* gEffect = NULL;
	Uint32 gOldTime;
};

