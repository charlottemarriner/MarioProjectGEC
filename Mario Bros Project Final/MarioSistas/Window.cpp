#include "Window.h"
#include <iostream>

SDL_Renderer* Window::gRenderer = nullptr;

Window::Window(const std::string& title, int width, int height) : _title(title), _width(width), _height(height)
{
	_closed = !init();
}

Window::~Window()
{
	//Release the window
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;

	//Quit SDL Subsystems
	IMG_Quit();
	SDL_Quit();

	//Release the renderer
	SDL_DestroyRenderer(gRenderer);
	gRenderer = nullptr;

	//Destroy GameScreenManager
	delete GameScreenManager::Instance;
	GameScreenManager::Instance = nullptr;

	//Release Music
	Mix_FreeMusic(gMusic);
	Mix_Quit();
	gMusic = nullptr;

	//Release TTF
	TTF_Quit();
}

bool Window::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		cout << "SDL did not initialise. Error: " << SDL_GetError();
		return 0;
	}
	
	//All good, so attempt to create the window
	gWindow = SDL_CreateWindow("Games Engine Creation",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		_width,
		_height,
		SDL_WINDOW_SHOWN);

	//Did the window get created?
	if (gWindow == nullptr)
	{
		//Nope.
		cout << "Window was not created. Error: " << SDL_GetError();
		return 0;
	}

	//Create renderer
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

	if (gRenderer == nullptr)
	{
		cout << "Renderer could not initialise. Error: " << SDL_GetError();
		return 0;
	}

	//Load PNG images
	int imageFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imageFlags) & imageFlags))
	{
		cout << "SDL_Image could not initialise. Error: " << IMG_GetError();
		return 0;
	}

	if (TTF_Init() == -1)
	{
		cout << "Failed to initialise SDL_ttf. Error: " << TTF_GetError();
		return 0;
	}

	//Initialise the mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		cout << "Mixer could not initialise. Error: " << Mix_GetError();
		return 0;
	}

	GameScreenManager::Instance = new GameScreenManager(gRenderer, SCREEN_MENU);
	gOldTime = SDL_GetTicks();

	LoadMusic("Music/Mario.mp3");
		if (Mix_PlayingMusic() == 0)
		{
			Mix_PlayMusic(gMusic, -1);
		}

	return true;
}

void Window::pollEvents()
{
	Uint32 newTime = SDL_GetTicks();

	SDL_Event event;

	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			_closed = true;
			break;

		default:
			break;
		}
	}

	GameScreenManager::Instance->Update((float)(newTime - gOldTime) / 1000.0f, event);

	//Set current time to be the old time
	gOldTime = newTime;
}

void Window::Clear() const
{
	//Update the screen
	SDL_RenderPresent(gRenderer);

	//Clear the screen
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(gRenderer);

	//Set where to render the texture
	SDL_Rect renderLocation = { 0, 0, _width, _height };

	GameScreenManager::Instance->Render();
}

void Window::LoadMusic(string path)
{
	gMusic = Mix_LoadMUS(path.c_str());

	if (gMusic == nullptr)
	{
		cout << "Failed to load background music! Error: " << Mix_GetError() << endl;
	}
}

bool Window::GetTextOnePlayer()
{
	if ((GameScreenManager::Instance->gameStart == true) && (GameScreenManager::Instance->twoPlayers==false))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Window::GetTextTwoPlayer()
{
	if ((GameScreenManager::Instance->gameStart == true) && (GameScreenManager::Instance->twoPlayers == true))
	{
		return true;
	}
	else
	{
		return false;
	}
}
