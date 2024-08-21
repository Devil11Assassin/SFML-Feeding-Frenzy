#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <fstream>
#include <cmath>
#include "Menu.h"
#include "Initialization.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyExtra.h"
#include "HUD.h"

using namespace sf;

enum WindowType {
	MainMenu,
	LevelSelection,
	Instructions,
	Credits,
	Exit,
	LevelOne,
	LevelTwo,
	LevelThree,
	LevelFour,
	LevelEndScreen,
	GameEndScreen
};

class Game
{
	static const unsigned WINDOW_WIDTH, WINDOW_HEIGHT;
	static const unsigned FRAMERATE_LIMIT;
	
	static WindowType currentWindowType;
	static RenderWindow window;
	static Image icon;
	static Music mainMenuMusic;
	static SoundBuffer scrollBuffer, enterBuffer, introBuffer, stageBuffer;
	static Sound scrollSound, enterSound, introSound, stageSound;

	static Clock dtClock;
	static float dt;
	static int movementMultiplier;
	static Clock changeCurrentWIndowDelay;

	static int levelScore;
	static std::unordered_map<WindowType, int> levelScores;
	static std::unordered_map<WindowType, int> highScores;
	static WindowType currentLevel;

public:
	//Constructor
	Game();

	//Helper Methods
	static WindowType getEnum(int index);
	static void update_dt();
	static void highScores_input();
	static void highScores_output();
	static void highScores_update();
	
	//Menus
	static void mainMenu();
	static void levelSelection();
	static void instructions();
	static void credits();
	static void quit();
	static void level_1();
	static void level_2();
	static void level_3();
	static void level_4();
	static void levelEndScreen();
	static void gameEndScreen();
};

