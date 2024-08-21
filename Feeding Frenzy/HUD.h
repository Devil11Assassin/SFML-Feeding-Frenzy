#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "Enemy.h"
#include "Menu.h"

using namespace sf;

class HUD
{
public:
	Texture scorebarTexture;
	Sprite scorebarSprite;

	std::unordered_map<FishKey, Sprite> fishMenu;
	Texture fishMenuLockedTexture;
	Sprite fishMenuLockedSprite[2];
	bool isFishMenuLocked[2];

	RectangleShape progressBarBackground;
	RectangleShape progressBar;
	CircleShape progressBarCheckpoint[2];
	bool progressBarMaxReached;

	Font leaderboardScoreFont;
	Text leaderboardScoreText;
	int leaderboardScore;

	Texture frenzyTexture;
	Sprite frenzySprite;
	int frenzyRow, frenzyColumn;

	Clock frenzyGainClockMillisecs, frenzyLossClockMillisecs;
	float frenzyGainTimeLimitMillisecs, frenzyLossTimeLimitMillisecs;

	Font frenzyMultiplierFont;
	Text frenzyMultiplierText;
	int frenzyMultiplier;

	Texture chompTexture;
	Sprite chompSprite;

	Texture pauseMenuTexture;
	Sprite pauseMenuSprite;
	Menu pauseMenu;
	bool pauseMenuIsOpen;

	Texture deathMenuTexture;
	Sprite deathMenuSprite;
	Font deathFont;
	Text deathText;

	bool colorLoopSwitch;
	Clock colorLoopClock;
	float colorLoopTimer;

	RectangleShape darkOverlay;

	//Constructor
	HUD(float frenzyGainTimeLimitMillisecs = 2000.f, float frenzyLossTimeLimitMillisecs = 350.f);

	//Methods
	void frenzyUpdate(bool& ateInLastSeconds);

	void deathTextColorLoop();

	void update(float playerScore);

	void draw(RenderWindow& window, bool isPlayerEating, std::string currentWindowType);

	void clear();
};

