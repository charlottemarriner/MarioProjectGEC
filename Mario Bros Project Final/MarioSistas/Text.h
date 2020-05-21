#pragma once
#include <SDL_ttf.h>
#include <SDL.h>
#include <string>

class Text
{
public: 
	Text(SDL_Renderer* gRenderer, const std::string& font_path, int font_size, const std::string& message_text, const SDL_Color& color);

	void display(int x, int y, SDL_Renderer *gRenderer) const;

	static SDL_Texture* loadFont(SDL_Renderer* gRenderer, const std::string& font_path, int font_size, const std::string& message_text, const SDL_Color& color);

private:
	SDL_Texture* _text_texture = nullptr;
	mutable SDL_Rect _text_rect;
};

