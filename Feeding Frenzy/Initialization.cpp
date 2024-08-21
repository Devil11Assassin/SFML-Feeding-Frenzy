#include "Initialization.h"

//Methods
void Initialization::mainMenuInitialization(Texture& backgroundTexture, Sprite& backgroundSprite,
	Menu& mainMenuText, View& view, RenderWindow& window) 
{
	backgroundTexture.loadFromFile("resources/backgrounds/main_menu.jpg");
	backgroundSprite.setTexture(backgroundTexture);

	mainMenuText.lines[0].setString("Play");
	mainMenuText.lines[0].setPosition(Vector2f(890, 435));
	mainMenuText.lines[0].setCharacterSize(70);

	mainMenuText.lines[1].setString("Instructions");
	mainMenuText.lines[1].setPosition(Vector2f(760, 600));
	mainMenuText.lines[1].setCharacterSize(70);

	mainMenuText.lines[2].setString("Credits");
	mainMenuText.lines[2].setPosition(Vector2f(830, 765));
	mainMenuText.lines[2].setCharacterSize(70);

	mainMenuText.lines[3].setString("Exit");
	mainMenuText.lines[3].setPosition(Vector2f(880, 930));
	mainMenuText.lines[3].setCharacterSize(70);

	view.reset(FloatRect(0, 0, 1920, 1080));
	window.setView(view);
}

void Initialization::levelSelectionInitialization(View& view, RenderWindow& window, Font& bottomInfoFont, Text& bottomInfo,
	RectangleShape& textBackground, Menu& text, Text& you, Texture (&backgroundTexture)[3], Sprite& backgroundSprite,
	RectangleShape& darkOverlay, Texture& angelOrange, Texture& anglerBrown, Texture& anglerGreen, Sprite& playerSprite,
	Texture& minnow, Texture& surgeonYellow, Texture& doryGreen, Texture& angelBlue, Texture& surgeonBlue,
	Texture& marlin, Texture& surgeonRed, Texture& doryBlue, Texture& barracuda,
	const int MAX_NUM_SMALL, const int MAX_NUM_MEDIUM, const int MAX_NUM_LARGE)
{
	view.reset(FloatRect(0, 0, 1920, 1080));
	window.setView(view);

	bottomInfoFont.loadFromFile("resources/fonts/scrubland_regular.ttf");
	bottomInfo.setFont(bottomInfoFont);
	bottomInfo.setCharacterSize(30);
	bottomInfo.setFillColor(Color::Yellow);
	bottomInfo.setString("Press Escape to return to main menu");
	bottomInfo.setPosition(Vector2f(700, 1040));
	bottomInfo.setOutlineColor(Color::Black);
	bottomInfo.setOutlineThickness(1.5);

	textBackground.setSize(Vector2f(610, 470));
	textBackground.setFillColor(Color(0, 0, 0, 100));
	textBackground.setPosition(Vector2f(310, 315));
	textBackground.setOutlineColor(Color::White);
	textBackground.setOutlineThickness(2);

	text.lines[0].setString("1 | Humble Beginnings");
	text.lines[0].setPosition(Vector2f(335, 320));
	text.lines[0].setCharacterSize(55);
	text.lines[1].setString("2 | Time Bounded");
	text.lines[1].setPosition(Vector2f(335, 450));
	text.lines[1].setCharacterSize(55);
	text.lines[2].setString("3 | Wasteland");
	text.lines[2].setPosition(Vector2f(335, 580));
	text.lines[2].setCharacterSize(55);
	text.lines[3].setString("4 | Into the Abyss");
	text.lines[3].setPosition(Vector2f(335, 710));
	text.lines[3].setCharacterSize(55);

	you.setFont(bottomInfoFont);
	you.setCharacterSize(60);
	you.setFillColor(Color(255, 165, 0, 255));
	you.setString("YOU");
	you.setPosition(Vector2f(1580, 420));
	you.setOutlineColor(Color::Black);
	you.setOutlineThickness(2);

	backgroundTexture[0].loadFromFile("resources/backgrounds/level_1_2.jpg");
	backgroundTexture[1].loadFromFile("resources/backgrounds/level_3.jpg");
	backgroundTexture[2].loadFromFile("resources/backgrounds/level_4.jpg");
	backgroundSprite.setTexture(backgroundTexture[0]);

	darkOverlay.setSize(Vector2f(1920, 1080));
	darkOverlay.setFillColor(Color(0, 0, 0, 100));

	angelOrange.loadFromFile("resources/sprites/angelfish_orange.png");
	anglerGreen.loadFromFile("resources/sprites/anglerfish_green.png");
	anglerBrown.loadFromFile("resources/sprites/anglerfish_brown.png");
	playerSprite.setTexture(angelOrange);
	playerSprite.setTextureRect(IntRect(0, 0, 126, 106));
	playerSprite.setPosition(Vector2f(1585, 510));

	minnow.loadFromFile("resources/sprites/minnow.png");
	surgeonYellow.loadFromFile("resources/sprites/surgeonfish_yellow.png");
	doryGreen.loadFromFile("resources/sprites/johndory_green.png");
	angelBlue.loadFromFile("resources/sprites/angelfish_blue.png");
	surgeonBlue.loadFromFile("resources/sprites/surgeonfish_blue.png");
	marlin.loadFromFile("resources/sprites/marlin.png");
	surgeonRed.loadFromFile("resources/sprites/surgeonfish_red.png");
	doryBlue.loadFromFile("resources/sprites/johndory_blue.png");
	barracuda.loadFromFile("resources/sprites/barracuda.png");

	Sprite smallFishSprite;
	smallFishSprite.setTexture(minnow);
	smallFishSprite.setTextureRect(IntRect(66, 0, -66, 49));
	
	Sprite mediumFishSprite;
	mediumFishSprite.setTexture(surgeonYellow);
	mediumFishSprite.setTextureRect(IntRect(171, 0, -171, 106));
	Sprite largeFishSprite;
	largeFishSprite.setTexture(doryGreen);
	largeFishSprite.setTextureRect(IntRect(181, 0, -181, 181));
	largeFishSprite.setScale(1.3f, 1.3f);

	Enemy::setStats(small, smallFishSprite, 3.75f, 0.375f, 0.05f, 0, 1, 15, 7, 0, 0);
	Enemy::setStats(medium, mediumFishSprite, 3.f, 0.3f, 0.16875f, 1, 2, 14, 5, 5, 10);
	Enemy::setStats(large, largeFishSprite, 2.25f, 0.225f, 0.1875f, 1, 2, 15, 5, 6, 20);

	Enemy::setDirectionRandomizerTime(small, 1500.f);
	Enemy::setDirectionRandomizerTime(medium, 1500.f);
	Enemy::setDirectionRandomizerTime(large, 1500.f);

	Enemy::setSpawnTime(small, 1000.f);
	Enemy::setSpawnTime(medium, 5000.f);
	Enemy::setSpawnTime(large, 10000.f);

	Enemy::setSpawnQuantity(small, MAX_NUM_SMALL);
	Enemy::setSpawnQuantity(medium, MAX_NUM_MEDIUM);
	Enemy::setSpawnQuantity(large, MAX_NUM_LARGE);

	for (int i = 0; i < MAX_NUM_SMALL; i++)
	{
		Enemy fish(small);
		Enemy::enemies[small].at(i).sprite.setPosition(Vector2f(rand() % 1854, rand() % 1031));
	}
	for (int i = 0; i < MAX_NUM_MEDIUM; i++)
	{
		Enemy fish(medium);
		Enemy::enemies[medium].at(i).sprite.setPosition(Vector2f(rand() % 1749, rand() % 974));
	}
	for (int i = 0; i < MAX_NUM_LARGE; i++)
	{
		Enemy fish(large);
		Enemy::enemies[large].at(i).sprite.setPosition(Vector2f(rand() % 1739, rand() % 899));
	}
}

void Initialization::instructionsInitialization(Texture& backgroundTexture, Sprite& backgroundSprite,
	Font& bottomInfoFont, Text& bottomInfo, View& view, RenderWindow& window)
{
	backgroundTexture.loadFromFile("resources/backgrounds/instructions.png");
	backgroundSprite.setTexture(backgroundTexture);

	bottomInfoFont.loadFromFile("resources/fonts/scrubland_regular.ttf");
	bottomInfo.setFont(bottomInfoFont);
	bottomInfo.setCharacterSize(30);
	bottomInfo.setFillColor(Color::Yellow);
	bottomInfo.setString("Press Escape to return to main menu");
	bottomInfo.setPosition(Vector2f(700, 1040));
	bottomInfo.setOutlineColor(Color::Black);
	bottomInfo.setOutlineThickness(1.5);

	view.reset(FloatRect(0, 0, 1920, 1080));
	window.setView(view);
}

void Initialization::creditsInitialization(Texture& backgroundTexture, Sprite& backgroundSprite,
	Font& bottomInfoFont, Text& bottomInfo, View& view, RenderWindow& window)
{
	backgroundTexture.loadFromFile("resources/backgrounds/credits.png");
	backgroundSprite.setTexture(backgroundTexture);

	bottomInfoFont.loadFromFile("resources/fonts/scrubland_regular.ttf");
	bottomInfo.setFont(bottomInfoFont);
	bottomInfo.setCharacterSize(30);
	bottomInfo.setFillColor(Color::Yellow);
	bottomInfo.setString("Press Escape to return to main menu");
	bottomInfo.setPosition(Vector2f(700, 1040));
	bottomInfo.setOutlineColor(Color::Black);
	bottomInfo.setOutlineThickness(1.5);

	view.reset(FloatRect(0, 0, 1920, 1080));
	window.setView(view);
}

void Initialization::level_1_Initialization(View& view, RenderWindow& window, Texture& backgroundTexture, Sprite& backgroundSprite,
	Texture& playerTexture, Sprite& playerSprite, Texture& enemySmall, Texture& enemyMedium, Texture& enemyLarge, HUD& hud, Music& levelMusic)
{
	window.setView(view);
	
	backgroundTexture.loadFromFile("resources/backgrounds/level_1_2.jpg");
	Vector2u sizeBG = backgroundTexture.getSize();
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setOrigin(sizeBG.x / 2, sizeBG.y / 2);
	backgroundSprite.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	playerTexture.loadFromFile("resources/sprites/angelfish_orange.png");
	playerSprite.setTexture(playerTexture);
	playerSprite.setTextureRect(IntRect(126, 106, -126, 106));
	playerSprite.setScale(Vector2f(0.8, 0.8));
	playerSprite.setPosition(static_cast<int>((WINDOW_WIDTH / 2.f) - (playerSprite.getGlobalBounds().width / 2.f)), 
		static_cast<int>((WINDOW_HEIGHT / 2.f) - (playerSprite.getGlobalBounds().height / 2.f)));

	enemySmall.loadFromFile("resources/sprites/minnow.png");
	enemyMedium.loadFromFile("resources/sprites/surgeonfish_yellow.png");
	enemyLarge.loadFromFile("resources/sprites/johndory_green.png");

	Sprite enemySmallSprite;
	enemySmallSprite.setTexture(enemySmall);
	enemySmallSprite.setTextureRect(IntRect(66, 0, -66, 49));

	Sprite enemyMediumSprite;
	enemyMediumSprite.setTexture(enemyMedium);
	enemyMediumSprite.setTextureRect(IntRect(171, 0, -171, 106));

	Sprite enemyLargeSprite;
	enemyLargeSprite.setTexture(enemyLarge);
	enemyLargeSprite.setTextureRect(IntRect(181, 0, -181, 181));
	enemyLargeSprite.setScale(1.3f, 1.3f);

	Enemy::setStats(small, enemySmallSprite, 3.75f, 0.375f, 0.05f, 0, 1, 15, 7, 0, 0);
	Enemy::setStats(medium, enemyMediumSprite, 3.f, 0.3f, 0.16875f, 1, 2, 14, 5, 5, 10);
	Enemy::setStats(large, enemyLargeSprite, 2.25f, 0.225f, 0.1875f, 1, 2, 15, 5, 6, 20);

	Enemy::setDirectionRandomizerTime(small, 1000.f);
	Enemy::setDirectionRandomizerTime(medium, 1500.f);
	Enemy::setDirectionRandomizerTime(large, 2000.f);

	Enemy::setSpawnTime(small, 500.f);
	Enemy::setSpawnTime(medium, 8000.f);
	Enemy::setSpawnTime(large, 15000.f);

	Enemy::setSpawnQuantity(small, 16);
	Enemy::setSpawnQuantity(medium, 6);
	Enemy::setSpawnQuantity(large, 2);

	hud.fishMenu[small] = Enemy::getSprites()[small];
	hud.fishMenu[small].setPosition(430, 30);
	hud.fishMenu[small].setScale(1.2, 1.2);
	hud.fishMenu[medium] = Enemy::getSprites()[medium];
	hud.fishMenu[medium].setPosition(600, 5);
	hud.fishMenu[medium].setScale(0.8, 0.8);
	hud.fishMenu[large] = Enemy::getSprites()[large];
	hud.fishMenu[large].setPosition(860, -27);
	hud.fishMenu[large].setScale(0.7, 0.7);

	hud.fishMenuLockedSprite[0].setScale(0.9, 0.9);
	hud.fishMenuLockedSprite[0].setPosition(580, -10);
	hud.fishMenuLockedSprite[1].setPosition(817, -18);

	levelMusic.openFromFile("resources/music/level_1.ogg");
	levelMusic.setLoop(true);
}

void Initialization::level_2_Initialization(RenderWindow& window, View& view, Texture& backgroundTexture, Sprite& backgroundSprite,
	Texture& playerTexture, Sprite& playerSprite, Texture& enemySmall, HUD& hud, Texture& clockBackgroundTexture, Sprite (& clockBackgroundSprite)[2],
	Texture& clockTexture, Sprite& clockSprite, Font& timerFont, Text& timerText, Font& fishEatenCountFont, Text& fishEatenCountText, Music& levelMusic)
{
	window.setView(view);

	backgroundTexture.loadFromFile("resources/backgrounds/level_1_2.jpg");
	Vector2u sizeBG = backgroundTexture.getSize();
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setOrigin(sizeBG.x / 2, sizeBG.y / 2);
	backgroundSprite.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	playerTexture.loadFromFile("resources/sprites/angelfish_orange.png");
	playerSprite.setTexture(playerTexture);
	playerSprite.setPosition(static_cast<float>(WINDOW_WIDTH) / 2, static_cast<float>(WINDOW_HEIGHT) / 2);
	playerSprite.setTextureRect(IntRect(126, 106, -126, 106));
	playerSprite.setScale(Vector2f(0.8, 0.8));

	enemySmall.loadFromFile("resources/sprites/minnow.png");
	
	Sprite enemySmallSprite;
	enemySmallSprite.setTexture(enemySmall);
	enemySmallSprite.setTextureRect(IntRect(66, 0, -66, 49));
	
	Enemy::setStats(small, enemySmallSprite, 3.75f, 0.375f, 0.05f, 0, 1, 15, 7, 0, 0);
	Enemy::setDirectionRandomizerTime(small, 1000.f);
	Enemy::setSpawnTime(small, 500.f);

	Enemy::setSpawnQuantity(small, 16);
	Enemy::setSpawnQuantity(medium, 0);
	Enemy::setSpawnQuantity(large, 0);

	hud.fishMenu[small] = Enemy::getSprites()[small];
	hud.fishMenu[small].setPosition(430, 30);
	hud.fishMenu[small].setScale(1.2, 1.2);

	hud.isFishMenuLocked[0] = false;
	hud.isFishMenuLocked[1] = false;

	clockBackgroundTexture.loadFromFile("resources/sprites/hud_rounded_rectangle.png");
	clockBackgroundSprite[0].setTexture(clockBackgroundTexture);
	clockBackgroundSprite[0].setPosition(1610, 915);
	clockBackgroundSprite[0].setScale(0.38, 0.4);
	clockBackgroundSprite[1].setTexture(clockBackgroundTexture);
	clockBackgroundSprite[1].setPosition(785, 890);
	clockBackgroundSprite[1].setScale(0.55, 0.63);

	clockTexture.loadFromFile("resources/sprites/hud_clock.png");
	clockSprite.setTexture(clockTexture);
	clockSprite.setScale(0.025, 0.025);
	clockSprite.setPosition(820, 915);

	timerFont.loadFromFile("resources/fonts/roboto_black.ttf");
	timerText.setFont(timerFont);
	timerText.setCharacterSize(50);
	timerText.setFillColor(Color(194, 232, 255));
	timerText.setPosition(955, 930);

	fishEatenCountFont.loadFromFile("resources/fonts/roboto_black.ttf");
	fishEatenCountText.setFont(fishEatenCountFont);
	fishEatenCountText.setCharacterSize(50);
	fishEatenCountText.setFillColor(Color(194, 232, 255));
	fishEatenCountText.setPosition(1650, 930);

	levelMusic.openFromFile("resources/music/level_2.ogg");
	levelMusic.setLoop(true);
}

void Initialization::level_3_Initialization(View& view, RenderWindow& window, Texture& backgroundTexture, Sprite& backgroundSprite,
	Texture& playerTexture, Sprite& playerSprite, Texture& enemySmall, Texture& enemyMedium, Texture& enemyLarge, HUD& hud, Music& levelMusic)
{
	window.setView(view);

	backgroundTexture.loadFromFile("resources/backgrounds/level_3.jpg");
	Vector2u sizeBG = backgroundTexture.getSize();
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setOrigin(sizeBG.x / 2, sizeBG.y / 2);
	backgroundSprite.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	playerTexture.loadFromFile("resources/sprites/anglerfish_brown.png");
	playerSprite.setTexture(playerTexture);
	playerSprite.setTextureRect(IntRect(0, 0, 201, 181));
	playerSprite.setScale(Vector2f(0.8f, 0.8f));
	playerSprite.setPosition(static_cast<int>((WINDOW_WIDTH / 2.f) - (playerSprite.getGlobalBounds().width / 2.f)),
		static_cast<int>((WINDOW_HEIGHT / 2.f) - (playerSprite.getGlobalBounds().height / 2.f)));

	enemySmall.loadFromFile("resources/sprites/angelfish_blue.png");
	enemyMedium.loadFromFile("resources/sprites/surgeonfish_blue.png");
	enemyLarge.loadFromFile("resources/sprites/marlin.png");

	Sprite enemySmallSprite;
	enemySmallSprite.setTexture(enemySmall);
	enemySmallSprite.setTextureRect(IntRect(126, 106, -126, 106));
	enemySmallSprite.setScale(0.6f, 0.6f);

	Sprite enemyMediumSprite;
	enemyMediumSprite.setTexture(enemyMedium);
	enemyMediumSprite.setTextureRect(IntRect(171, 0, -171, 106));
	enemyMediumSprite.setScale(1.1f, 1.1f);

	Sprite enemyLargeSprite;
	enemyLargeSprite.setTexture(enemyLarge);
	enemyLargeSprite.setTextureRect(IntRect(251, 0, -251, 176));
	enemyLargeSprite.setScale(1.4f, 1.4f);

	Enemy::setStats(small, enemySmallSprite, 3.75f, 0.375f, 0.05f, 2, 3, 15, 5, 6, 0);
	Enemy::setStats(medium, enemyMediumSprite, 3.f, 0.3f, 0.16875f, 1, 2, 14, 5, 5, 10);
	Enemy::setStats(large, enemyLargeSprite, 2.25f, 0.225f, 0.1875f, 1, 2, 15, 5, 5, 20);

	Enemy::setDirectionRandomizerTime(small, 1000.f);
	Enemy::setDirectionRandomizerTime(medium, 1500.f);
	Enemy::setDirectionRandomizerTime(large, 2000.f);

	Enemy::setSpawnTime(small, 500.f);
	Enemy::setSpawnTime(medium, 8000.f);
	Enemy::setSpawnTime(large, 15000.f);

	Enemy::setSpawnQuantity(small, 16);
	Enemy::setSpawnQuantity(medium, 6);
	Enemy::setSpawnQuantity(large, 0);

	hud.fishMenu[small] = Enemy::getSprites()[small];
	hud.fishMenu[small].setPosition(445, 20);
	hud.fishMenu[small].setScale(0.55, 0.55);
	hud.fishMenu[medium] = Enemy::getSprites()[medium];
	hud.fishMenu[medium].setPosition(600, 5);
	hud.fishMenu[medium].setScale(0.8, 0.8);
	hud.fishMenu[large] = Enemy::getSprites()[large];
	hud.fishMenu[large].setPosition(850, 0);
	hud.fishMenu[large].setScale(0.7, 0.7);

	hud.fishMenuLockedSprite[0].setScale(0.9, 0.9);
	hud.fishMenuLockedSprite[0].setPosition(580, -10);
	hud.fishMenuLockedSprite[1].setPosition(817, -18);

	levelMusic.openFromFile("resources/music/level_3.ogg");
	levelMusic.setLoop(true);
}

void Initialization::level_4_Initialization(View& view, RenderWindow& window, Texture& backgroundTexture, Sprite& backgroundSprite,
	Texture& playerTexture, Sprite& playerSprite, Texture& enemySmall, Texture& enemyMedium, Texture& enemyLarge, HUD& hud, Music& levelMusic)
{
	window.setView(view);

	backgroundTexture.loadFromFile("resources/backgrounds/level_4.jpg");
	Vector2u sizeBG = backgroundTexture.getSize();
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setOrigin(sizeBG.x / 2, sizeBG.y / 2);
	backgroundSprite.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	playerTexture.loadFromFile("resources/sprites/anglerfish_green.png");
	playerSprite.setTexture(playerTexture);
	playerSprite.setTextureRect(IntRect(0, 0, 201, 181));
	playerSprite.setScale(0.8, 0.8);
	playerSprite.setPosition(static_cast<int>((WINDOW_WIDTH / 2.f) - (playerSprite.getGlobalBounds().width / 2.f)),
		static_cast<int>((WINDOW_HEIGHT / 2.f) - (playerSprite.getGlobalBounds().height / 2.f)));

	enemySmall.loadFromFile("resources/sprites/surgeonfish_red.png");
	enemyMedium.loadFromFile("resources/sprites/johndory_blue.png");
	enemyLarge.loadFromFile("resources/sprites/barracuda.png");

	Sprite enemySmallSprite;
	enemySmallSprite.setTexture(enemySmall);
	enemySmallSprite.setTextureRect(IntRect(171, 0, -171, 106));
	enemySmallSprite.setScale(0.6f, 0.6f);

	Sprite enemyMediumSprite;
	enemyMediumSprite.setTexture(enemyMedium);
	enemyMediumSprite.setTextureRect(IntRect(181, 0, -181, 181));
	enemyMediumSprite.setScale(1.1f, 1.1f);

	Sprite enemyLargeSprite;
	enemyLargeSprite.setTexture(enemyLarge);
	enemyLargeSprite.setTextureRect(IntRect(271, 0, -271, 121));
	enemyLargeSprite.setScale(1.4f, 1.4f);

	Enemy::setStats(small, enemySmallSprite, 3.75f, 0.375f, 0.05f, 1, 2, 14, 5, 5, 0);
	Enemy::setStats(medium, enemyMediumSprite, 3.f, 0.3f, 0.16875f, 1, 2, 15, 5, 6, 10);
	Enemy::setStats(large, enemyLargeSprite, 2.25f, 0.225f, 0.1875f, 1, 2, 14, 5, 6, 20);

	Enemy::setDirectionRandomizerTime(small, 1000.f);
	Enemy::setDirectionRandomizerTime(medium, 1500.f);
	Enemy::setDirectionRandomizerTime(large, 2000.f);

	Enemy::setSpawnTime(small, 500.f);
	Enemy::setSpawnTime(medium, 8000.f);
	Enemy::setSpawnTime(large, 15000.f);

	Enemy::setSpawnQuantity(small, 16);
	Enemy::setSpawnQuantity(medium, 6);
	Enemy::setSpawnQuantity(large, 2);

	hud.fishMenu[small] = Enemy::getSprites()[small];
	hud.fishMenu[small].setPosition(430, 30);
	hud.fishMenu[small].setScale(0.6, 0.6);
	hud.fishMenu[medium] = Enemy::getSprites()[medium];
	hud.fishMenu[medium].setPosition(620, -25);
	hud.fishMenu[medium].setScale(0.7, 0.7);
	hud.fishMenu[large] = Enemy::getSprites()[large];
	hud.fishMenu[large].setPosition(840, 5);
	hud.fishMenu[large].setScale(0.6, 0.6);

	hud.fishMenuLockedSprite[0].setScale(0.9, 0.9);
	hud.fishMenuLockedSprite[0].setPosition(580, -10);
	hud.fishMenuLockedSprite[1].setPosition(817, -18);

	levelMusic.openFromFile("resources/music/level_4.ogg");
	levelMusic.setLoop(true);
}