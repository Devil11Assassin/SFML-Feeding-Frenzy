#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>
#include "Menu.h"
#include "Enemy.h"
#include "HUD.h"

using namespace sf;

class Initialization
{
public:
	//Methods
	static void mainMenuInitialization(Texture& backgroundTexture, Sprite& backgroundSprite,
		Menu& mainMenuText, View& view, RenderWindow& window);

	static void instructionsInitialization(Texture& backgroundTexture, Sprite& backgroundSprite,
		Font& bottomInfoFont, Text& bottomInfo, View& view, RenderWindow& window);

	static void creditsInitialization(Texture& backgroundTexture, Sprite& backgroundSprite,
		Font& bottomInfoFont, Text& bottomInfo, View& view, RenderWindow& window);

	static void levelSelectionInitialization(View& view, RenderWindow& window, Font& bottomInfoFont, Text& bottomInfo,
		RectangleShape& textBackground, Menu& text, Text& you, Texture (&backgroundTexture)[3], Sprite& backgroundSprite,
		RectangleShape& darkOverlay, Texture& angelOrange, Texture& anglerBrown, Texture& anglerGreen, Sprite& playerSprite,
		Texture& minnow, Texture& surgeonYellow, Texture& doryGreen, Texture& angelBlue, Texture& surgeonBlue,
		Texture& marlin, Texture& surgeonRed, Texture& doryBlue, Texture& barracuda,
		const int MAX_NUM_SMALL, const int MAX_NUM_MEDIUM, const int MAX_NUM_LARGE);

	static void level_1_Initialization(View& view, RenderWindow& window, Texture& backgroundTexture, Sprite& backgroundSprite, Texture& playerTexture,
		Sprite& playerSprite, Texture& enemySmall, Texture& enemyMedium, Texture& enemyLarge, HUD& hud, Music& levelMusic);

	static void level_2_Initialization(RenderWindow& window, View& view, Texture& backgroundTexture, Sprite& backgroundSprite,
		Texture& playerTexture, Sprite& playerSprite, Texture& enemySmall, HUD& hud, Texture& clockBackgroundTexture, Sprite(&clockBackgroundSprite)[2],
		Texture& clockTexture, Sprite& clockSprite, Font& timerFont, Text& timerText, Font& fishEatenCountFont, Text& fishEatenCountText, Music& levelMusic);
	
	static void level_3_Initialization(View& view, RenderWindow& window, Texture& backgroundTexture, Sprite& backgroundSprite,
		Texture& playerTexture, Sprite& playerSprite, Texture& enemySmall, Texture& enemyMedium, Texture& enemyLarge, HUD& hud, Music& levelMusic);
	
	static void level_4_Initialization(View& view, RenderWindow& window, Texture& backgroundTexture, Sprite& backgroundSprite,
		Texture& playerTexture, Sprite& playerSprite, Texture& enemySmall, Texture& enemyMedium, Texture& enemyLarge, HUD& hud, Music& levelMusic);
};