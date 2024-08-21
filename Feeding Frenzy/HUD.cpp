#include "HUD.h"

//Constructor
HUD::HUD(float frenzyGainTimeLimitMillisecs, float frenzyLossTimeLimitMillisecs) : pauseMenu(3)
{
	scorebarTexture.loadFromFile("resources/backgrounds/score_bar.png");
	scorebarSprite.setTexture(scorebarTexture);
	scorebarSprite.setTextureRect(IntRect(0, 0, 1920, 137));

	for (int index = small; index <= large; index++)
	{
		FishKey key = Enemy::getEnum(index);
		fishMenu[key] = Enemy::getSprites()[key];
	}

	fishMenuLockedTexture.loadFromFile("resources/sprites/hud_ellipse.png");
	fishMenuLockedSprite[0].setTexture(fishMenuLockedTexture);
	fishMenuLockedSprite[1].setTexture(fishMenuLockedTexture);
	isFishMenuLocked[0] = isFishMenuLocked[1] = true;

	progressBarBackground.setSize(Vector2f(700, 15));
	progressBarBackground.setPosition(390, 93);
	progressBarBackground.setFillColor(Color::Black);

	progressBar.setSize(Vector2f(94, 11));
	progressBar.setPosition(393, 95);
	progressBar.setFillColor(Color::Red);

	progressBarCheckpoint[0].setRadius(18);
	progressBarCheckpoint[0].setPointCount(3);
	progressBarCheckpoint[0].setPosition(575, 99);
	progressBarCheckpoint[0].setFillColor(Color(194, 232, 255));
	progressBarCheckpoint[0].setOutlineColor(Color::Black);
	progressBarCheckpoint[0].setOutlineThickness(1);

	progressBarCheckpoint[1].setRadius(18);
	progressBarCheckpoint[1].setPointCount(3);
	progressBarCheckpoint[1].setPosition(773, 99);
	progressBarCheckpoint[1].setFillColor(Color(194, 232, 255));
	progressBarCheckpoint[1].setOutlineColor(Color::Black);
	progressBarCheckpoint[1].setOutlineThickness(1);

	progressBarMaxReached = false;

	leaderboardScoreFont.loadFromFile("resources/fonts/gumdrop.ttf");
	leaderboardScoreText.setFont(leaderboardScoreFont);
	leaderboardScoreText.setCharacterSize(40);
	leaderboardScoreText.setFillColor(Color::White);
	leaderboardScoreText.setPosition(1560, 20);
	leaderboardScore = 0;

	frenzyTexture.loadFromFile("resources/sprites/hud_frenzy.png");
	frenzySprite.setTexture(frenzyTexture);
	frenzySprite.setTextureRect(IntRect(0, 0, 350, 120));
	frenzySprite.setPosition(WINDOW_WIDTH / 2 - 175, WINDOW_HEIGHT / 2 - 360);
	frenzyRow = 0;
	frenzyColumn = 0;

	frenzyGainClockMillisecs.restart().asMilliseconds();
	frenzyLossClockMillisecs.restart().asMilliseconds();
	this->frenzyGainTimeLimitMillisecs = frenzyGainTimeLimitMillisecs;
	this->frenzyLossTimeLimitMillisecs = frenzyLossTimeLimitMillisecs;

	frenzyMultiplierFont.loadFromFile("resources/fonts/roboto_black.ttf");
	frenzyMultiplierText.setFont(frenzyMultiplierFont);
	frenzyMultiplierText.setFillColor(Color::White);
	frenzyMultiplierText.setOutlineColor(Color::Black);
	frenzyMultiplierText.setOutlineThickness(3);
	frenzyMultiplierText.setPosition(1760, 5);
	frenzyMultiplierText.setCharacterSize(70);
	frenzyMultiplier = 1;

	chompTexture.loadFromFile("resources/sprites/hud_chomp.png");
	chompSprite.setTexture(chompTexture);
	chompSprite.setTextureRect(IntRect(0, 0, 74, 61));
	chompSprite.setPosition(920, 920);

	pauseMenuTexture.loadFromFile("resources/sprites/pause_menu.png");
	pauseMenuSprite.setTexture(pauseMenuTexture);

	pauseMenu.lines[0].setString("RESUME");
	pauseMenu.lines[0].setCharacterSize(70);
	pauseMenu.lines[0].setPosition(Vector2f(static_cast<int>((WINDOW_WIDTH / 2.f) - (pauseMenu.lines[0].getGlobalBounds().width / 2.f)), 430));
	pauseMenu.lines[1].setString("LEVELS");
	pauseMenu.lines[1].setCharacterSize(70);
	pauseMenu.lines[1].setPosition(Vector2f(static_cast<int>((WINDOW_WIDTH / 2.f) - (pauseMenu.lines[1].getGlobalBounds().width / 2.f)), 550));
	pauseMenu.lines[2].setString("EXIT");
	pauseMenu.lines[2].setCharacterSize(70);
	pauseMenu.lines[2].setPosition(Vector2f(static_cast<int>((WINDOW_WIDTH / 2.f) - (pauseMenu.lines[2].getGlobalBounds().width / 2.f)), 670));

	pauseMenuIsOpen = false;

	deathMenuTexture.loadFromFile("resources/sprites/game_over.png");
	deathMenuSprite.setTexture(deathMenuTexture);

	deathFont.loadFromFile("resources/fonts/scrubland_regular.ttf");
	deathText.setFont(deathFont);

	deathText.setString("Press ENTER to return to Home");
	deathText.setFillColor(Color(190, 0, 0));
	deathText.setCharacterSize(60);
	deathText.setOutlineColor(Color::Black);
	deathText.setOutlineThickness(2);
	deathText.setPosition(Vector2f(static_cast<int>((WINDOW_WIDTH / 2.f) - (deathText.getGlobalBounds().width / 2.f)), 580));

	colorLoopSwitch = false;
	colorLoopTimer = 1000;

	darkOverlay.setSize(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
	darkOverlay.setFillColor(Color(0, 0, 0, 100));
}

//Methods
void HUD::frenzyUpdate(bool& ateInLastSeconds)
{
	//Reset ateInLastSeconds to false after frenzyGainTimeLimit passes
	if (static_cast<float>(frenzyGainClockMillisecs.getElapsedTime().asMilliseconds()) > frenzyGainTimeLimitMillisecs)
		ateInLastSeconds = false;

	//Lower frenzy if player hasn't eaten for a while
	if (!ateInLastSeconds)
	{
		if (static_cast<float>(frenzyLossClockMillisecs.getElapsedTime().asMilliseconds()) >= frenzyLossTimeLimitMillisecs)
		{
			frenzyLossClockMillisecs.restart();
			if (frenzyRow > 0)
				frenzyRow--;

			if (frenzyRow == 0 && frenzyColumn != 0)
			{
				frenzyColumn--;
				if (frenzyColumn > 0)
					frenzyRow = 6;
				else
					frenzyRow = 0;
			}
			if (frenzyRow == 0 && frenzyColumn == 0)
			{
				frenzyColumn = 0;
				frenzyRow = 0;
			}
		}
	}
	//Update frenzy sprite
	frenzySprite.setTextureRect(IntRect(frenzyColumn * 353, frenzyRow * 121, 353, 121));

	//Update frenzy multiplier
	if (frenzyColumn < 2 && frenzyRow < 6)
	{
		frenzyMultiplierText.setString("1X");
		frenzyMultiplierText.setFillColor(Color::White);
		frenzyMultiplier = 1;
	}
	else if ((frenzyColumn == 1 && frenzyRow == 6) || (frenzyColumn == 2 && frenzyRow < 6))
	{
		frenzyMultiplierText.setString("2X");
		frenzyMultiplierText.setFillColor(Color::Blue);
		frenzyMultiplier = 2;
	}
	else if (frenzyColumn == 2 && frenzyRow == 6)
	{
		frenzyMultiplierText.setString("3X");
		frenzyMultiplierText.setFillColor(Color::Red);
		frenzyMultiplier = 3;
	}
}

void HUD::deathTextColorLoop()
{
	if (static_cast<float>(colorLoopClock.getElapsedTime().asMilliseconds()) >= colorLoopTimer)
	{
		colorLoopClock.restart().asSeconds();
		if (!colorLoopSwitch)
		{
			deathText.setFillColor(Color(140, 0, 0));
			colorLoopSwitch = true;
		}
		else
		{
			deathText.setFillColor(Color(190, 0, 0));
			colorLoopSwitch = false;
		}
	}
}

void HUD::update(float playerScore)
{
	leaderboardScoreText.setString(std::to_string(leaderboardScore));

	if (progressBar.getSize().x < 694)
	{
		progressBar.setSize(Vector2f(playerScore * 20, 11));
	}
	else
	{
		progressBar.setSize(Vector2f(694, 11));
		progressBarMaxReached = true;
	}
}

void HUD::draw(RenderWindow& window, bool isPlayerEating, std::string currentWindowType)
{
	//Scorebar
	window.draw(scorebarSprite);
	window.draw(progressBarBackground);
	if (currentWindowType != "LevelTwo")
	{
		window.draw(progressBar);
		window.draw(progressBarCheckpoint[0]);
		window.draw(progressBarCheckpoint[1]);
	}

	//Fish menu
	window.draw(fishMenu[small]);
	if (currentWindowType != "LevelTwo")
	{
		window.draw(fishMenu[medium]);
		window.draw(fishMenu[large]);
		if (isFishMenuLocked[0])
			window.draw(fishMenuLockedSprite[0]);
		if (isFishMenuLocked[1])
			window.draw(fishMenuLockedSprite[1]);
	}

	//Chomp
	if (isPlayerEating)
		window.draw(chompSprite);

	//Frenzy, its multiplier and score
	window.draw(leaderboardScoreText);
	window.draw(frenzyMultiplierText);
	window.draw(frenzySprite);
}

void HUD::clear() 
{
	fishMenu.clear();
}