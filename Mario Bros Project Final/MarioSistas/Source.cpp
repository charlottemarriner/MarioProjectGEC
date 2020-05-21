#include "Window.h"
#include "Text.h"
#include "GameScreenManager.h"

using namespace::std;

int main(int argc, char* args[])
{
	Window window("Mario Bros.", 512, 416);
	std::string y;
	y = std::to_string(GameScreenManager::Instance->score);
	
	Text Text7(Window::gRenderer, "Images/OCRAEXT.ttf", 24, "LEVEL 5", { 255,255,255,255 });
	Text Text6(Window::gRenderer, "Images/OCRAEXT.ttf", 24, "LEVEL 4", { 255,255,255,255 });
	Text Text5(Window::gRenderer, "Images/OCRAEXT.ttf", 24, "LEVEL 3", { 255,255,255,255 });
	Text Text4(Window::gRenderer, "Images/OCRAEXT.ttf", 24, "LEVEL 2", { 255,255,255,255 });
	Text Text3(Window::gRenderer, "Images/OCRAEXT.ttf", 24, "LEVEL 1", { 255,255,255,255 });
	Text Text2(Window::gRenderer, "Images/OCRAEXT.ttf", 16, "Luigi Lives: ", { 255,255,255,255 });
	Text Text(Window::gRenderer, "Images/OCRAEXT.ttf", 16, "Mario Lives: ", { 255,255,255,255 });

	while (!window.isClosed())
	{
		window.pollEvents();
		window.Clear();
		if(window.GetTextOnePlayer())
		{
			Text.display(70, 8, Window::gRenderer);
			GameScreenManager::Instance->mTexthandler->display(15, 100, Window::gRenderer);

			if (GameScreenManager::Instance->level1)
			{
				Text3.display(20, 390, Window::gRenderer);
			}
			else if (GameScreenManager::Instance->level2)
			{
				Text4.display(20, 390, Window::gRenderer);
			}
			else if (GameScreenManager::Instance->level3)
			{
				Text5.display(20, 390, Window::gRenderer);
			}
			else if (GameScreenManager::Instance->level4)
			{
				Text6.display(20, 390, Window::gRenderer);
			}
			else if (GameScreenManager::Instance->level5)
			{
				Text7.display(20, 390, Window::gRenderer);
			}
		}
		else if (window.GetTextTwoPlayer())
		{
			Text.display(70, 8, Window::gRenderer);
			Text2.display(260, 8, Window::gRenderer);
			GameScreenManager::Instance->mTexthandler->display(15, 100, Window::gRenderer);

			if (GameScreenManager::Instance->level1)
			{
				Text3.display(20, 390, Window::gRenderer);
			}
			else if (GameScreenManager::Instance->level2)
			{
				Text4.display(20, 390, Window::gRenderer);
			}
			else if (GameScreenManager::Instance->level3)
			{
				Text5.display(20, 390, Window::gRenderer);
			}
			else if (GameScreenManager::Instance->level4)
			{
				Text6.display(20, 390, Window::gRenderer);
			}
			else if (GameScreenManager::Instance->level5)
			{
				Text7.display(20, 390, Window::gRenderer);
			}
		}
	}

	return 0;
}