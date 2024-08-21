#include "Game.h"

const unsigned Game::WINDOW_WIDTH = 1920, Game::WINDOW_HEIGHT = 1080;
const unsigned Game::FRAMERATE_LIMIT = 90;

WindowType Game::currentWindowType;
RenderWindow Game::window;
Image Game::icon;
Music Game::mainMenuMusic;
SoundBuffer Game::scrollBuffer, Game::enterBuffer, Game::introBuffer, Game::stageBuffer;
Sound Game::scrollSound, Game::enterSound, Game::introSound, Game::stageSound;

Clock Game::dtClock;
float Game::dt;
int Game::movementMultiplier = 60;
Clock Game::changeCurrentWIndowDelay;

int Game::levelScore;
std::unordered_map<WindowType, int> Game::levelScores;
std::unordered_map<WindowType, int> Game::highScores;
WindowType Game::currentLevel;

//Game
Game::Game() 
{
	window.create(VideoMode(1920, 1080), "Feeding Frenzy", Style::Fullscreen);
	window.setMouseCursorVisible(false);
	//window.setFramerateLimit(FRAMERATE_LIMIT);
	srand(time(0));
	
	icon.loadFromFile("resources/icon.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	mainMenuMusic.openFromFile("resources/music/main_menu.ogg");
	mainMenuMusic.setLoop(true);
	
	scrollBuffer.loadFromFile("resources/sounds/scroll.ogg");
	enterBuffer.loadFromFile("resources/sounds/enter.ogg");
	introBuffer.loadFromFile("resources/sounds/stage_intro.ogg");
	stageBuffer.loadFromFile("resources/sounds/stage_clear.ogg");

	scrollSound.setBuffer(scrollBuffer);
	enterSound.setBuffer(enterBuffer);
	introSound.setBuffer(introBuffer);
	stageSound.setBuffer(stageBuffer);

	currentWindowType = MainMenu;
	highScores_input();

	while (true)
	{
		switch (currentWindowType)
		{
		case MainMenu: 
			mainMenu(); break;
		case LevelSelection: 
			levelSelection(); break;
		case Instructions: 
			instructions(); break;
		case Credits: 
			credits(); break;
		case Exit: 
			quit(); break;
		case LevelOne: 
			level_1(); break;
		case LevelTwo: 
			level_2(); break;
		case LevelThree: 
			level_3(); break;
		case LevelFour: 
			level_4(); break;
		case LevelEndScreen: 
			levelEndScreen(); break;
		case GameEndScreen: 
			gameEndScreen(); break;
		}
	}
}

//Helper Methods
WindowType Game::getEnum(int index) {
	return static_cast<WindowType>(index);
}

void Game::update_dt() {
	dt = static_cast<float>(dtClock.restart().asSeconds());
}

void Game::highScores_input()
{
	int index = 0;
	std::string line[4];

	std::ifstream highScoresFile;
	highScoresFile.open("resources/high_scores.txt", std::ios::in);
	highScoresFile.seekg(0);

	if (highScoresFile.is_open())
	{
		while (getline(highScoresFile, line[index]))
		{
			if (index == 3)
			{
				index = 0;
				highScores[LevelOne] = stoi(line[0]);
				highScores[LevelTwo] = stoi(line[1]);
				highScores[LevelThree] = stoi(line[2]);
				highScores[LevelFour] = stoi(line[3]);
			}
			else
				index++;
		}
	}

	highScoresFile.close();
}

void Game::highScores_output()
{
	std::ofstream highScoresFile;
	highScoresFile.open("resources/high_scores.txt", std::ios::trunc);

	if (highScoresFile.is_open())
	{
		highScoresFile << highScores[LevelOne] << std::endl;
		highScoresFile << highScores[LevelTwo] << std::endl;
		highScoresFile << highScores[LevelThree] << std::endl;
		highScoresFile << highScores[LevelFour] << std::endl;
	}

	highScoresFile.close();
}

void Game::highScores_update() 
{
	if (levelScore > highScores[currentLevel])
		highScores[currentLevel] = levelScore;
}

//Menus
void Game::mainMenu()
{
	Texture backgroundTexture;
	Sprite backgroundSprite;
	Menu mainMenuText(4);
	View view;

	Initialization::mainMenuInitialization(backgroundTexture,
		backgroundSprite, mainMenuText, view, window);

	while (true)
	{
		if (currentWindowType == MainMenu)
		{
			while (window.isOpen())
			{
				if (mainMenuMusic.getStatus() != SoundSource::Status::Playing)
					mainMenuMusic.play();

				Event event;
				while (window.pollEvent(event))
				{
					if (changeCurrentWIndowDelay.getElapsedTime().asSeconds() < 0.5f)
						break;

					if (event.type == Event::Closed)
					{
						currentWindowType = Exit;
						break;
					}

					if (event.type == Event::KeyReleased)
					{
						if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W)
						{
							scrollSound.play();
							mainMenuText.moveUp();
						}
						if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S)
						{
							scrollSound.play();
							mainMenuText.moveDown();
						}
						if (event.key.code == Keyboard::Return)
						{
							enterSound.play();

							if (mainMenuText.getSelectedItem() == 0)
								currentWindowType = LevelSelection;
							else if (mainMenuText.getSelectedItem() == 1)
								currentWindowType = Instructions;
							else if (mainMenuText.getSelectedItem() == 2)
								currentWindowType = Credits;
							else if (mainMenuText.getSelectedItem() == 3)
								currentWindowType = Exit;
						}
						break;
					}
				}

				if (currentWindowType != MainMenu)
					break;

				window.clear();
				window.draw(backgroundSprite);
				mainMenuText.draw(window);
				window.display();
			}
		}
		else
			break;
	}
}

void Game::levelSelection() 
{
	const int MAX_NUM_SMALL = 10;
	const int MAX_NUM_MEDIUM = 4;
	const int MAX_NUM_LARGE = 2;

	View view;
	Font bottomInfoFont;
	Text bottomInfo;
	RectangleShape textBackground;
	Menu text(4);
	Text you;

	Texture backgroundTexture[3];
	Sprite backgroundSprite;
	RectangleShape darkOverlay;

	//Player Textures
	Sprite playerSprite;
	Texture angelOrangeTexture;
	Texture anglerGreenTexture;
	Texture anglerBrownTexture;

	// Fish Textures
	// Level 1 & 2
	Texture minnowTexture;
	Texture surgeonYellowTexture;
	Texture doryGreenTexture;
	// Level 3
	Texture angelBlueTexture;
	Texture surgeonBlueTexture;
	Texture marlinTexture;
	// Level 4
	Texture surgeonRedTexture;
	Texture doryBlueTexture;
	Texture barracudaTexture;	

	Initialization::levelSelectionInitialization(view, window, bottomInfoFont, bottomInfo, textBackground, text, you,
		backgroundTexture, backgroundSprite, darkOverlay, angelOrangeTexture, anglerBrownTexture, anglerGreenTexture,
		playerSprite, minnowTexture, surgeonYellowTexture, doryGreenTexture, angelBlueTexture, surgeonBlueTexture,
		marlinTexture, surgeonRedTexture, doryBlueTexture, barracudaTexture,
		MAX_NUM_SMALL, MAX_NUM_MEDIUM, MAX_NUM_LARGE);

	bool colorLoopSwitch = false;
	Clock colorLoopClock;
	float colorLoopTimer = 1000;

	Clock freezeClock;

	int selection = 0;

	while (true)
	{
		if (currentWindowType == LevelSelection)
		{
			while (window.isOpen())
			{
				if (mainMenuMusic.getStatus() != Music::Playing)
					mainMenuMusic.play();

				Event event;
				while (window.pollEvent(event))
				{
					if (changeCurrentWIndowDelay.getElapsedTime().asSeconds() < 2.f)
						break;

					if (event.type == Event::Closed)
					{
						currentWindowType = Exit;
						break;
					}

					if (event.type == Event::KeyReleased)
					{
						if (event.key.code == Keyboard::Escape)
						{
							enterSound.play();
							currentWindowType = MainMenu;
						}
						if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W)
						{
							scrollSound.play();
							text.moveUp();
						}
						if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S)
						{
							scrollSound.play();
							text.moveDown();
						}
						if (event.key.code == Keyboard::Return)
						{
							enterSound.play();

							if (text.getSelectedItem() == 0)
								currentWindowType = LevelOne;
							else if (text.getSelectedItem() == 1)
								currentWindowType = LevelTwo;
							else if (text.getSelectedItem() == 2)
								currentWindowType = LevelThree;
							else
								currentWindowType = LevelFour;
						}
						break;
					}
				}

				if (currentWindowType != LevelSelection)
					break;

				//Color loop
				if (static_cast<float>(colorLoopClock.getElapsedTime().asMilliseconds()) >= colorLoopTimer)
				{
					colorLoopClock.restart().asSeconds();
					if (!colorLoopSwitch)
						colorLoopSwitch = true;
					else
						colorLoopSwitch = false;
				}

				//Change background and enemies
				if (selection != text.getSelectedItem())
				{
					selection = text.getSelectedItem();

					if (selection == 1)
					{
						//Player
						playerSprite.setTexture(angelOrangeTexture);
						playerSprite.setTextureRect(IntRect(0, 0, 126, 106));
						playerSprite.setPosition(Vector2f(1585, 510));

						//Small
						Enemy::modifyStats(small, 3.75f, 0.375f, 0.05f, 0, 1, 15, 7, 0, 0);

						Sprite spriteModified;
						spriteModified.setTexture(minnowTexture);
						spriteModified.setTextureRect(IntRect(66, 0, -66, 49));
						spriteModified.setScale(1, 1);
						Enemy::setSprite(small, spriteModified);

						while (Enemy::enemies[small].size() < MAX_NUM_SMALL)
							Enemy fish(small);

						Enemy::enemies[medium].clear();
						Enemy::enemies[large].clear();

						for (auto& smallFish : Enemy::enemies[small])
						{
							smallFish.sprite = Enemy::getSprites()[small];
							smallFish.sprite.setPosition(Vector2f(rand() % 1854, rand() % 1031));
						}
					}
					else if (selection == 2)
					{
						//Player
						playerSprite.setTexture(anglerBrownTexture);
						playerSprite.setTextureRect(IntRect(0, 0, 201, 181));
						playerSprite.setPosition(Vector2f(1560, 455));

						//Small
						Enemy::modifyStats(small, 3.75f, 0.375f, 0.05f, 2, 3, 15, 5, 6, 0);

						Sprite spriteModified;
						spriteModified.setTexture(angelBlueTexture);
						spriteModified.setTextureRect(IntRect(126, 106, -126, 106));
						spriteModified.setScale(0.6, 0.6);
						Enemy::setSprite(small, spriteModified);

						while (Enemy::enemies[small].size() < MAX_NUM_SMALL)
							Enemy fish(small);

						for (auto& smallFish : Enemy::enemies[small])
						{
							smallFish.sprite = Enemy::getSprites()[small];
							smallFish.sprite.setPosition(Vector2f(rand() % 1794, rand() % 974));
						}

						//Medium
						Enemy::modifyStats(medium, 3.f, 0.3f, 0.16875f, 1, 2, 14, 5, 5, 10);
						
						spriteModified.setTexture(surgeonBlueTexture);
						spriteModified.setTextureRect(IntRect(171, 0, -171, 106));
						spriteModified.setScale(1.1, 1.1);
						Enemy::setSprite(medium, spriteModified);

						while (Enemy::enemies[medium].size() < MAX_NUM_MEDIUM)
							Enemy fish(medium);

						for (auto& mediumFish : Enemy::enemies[medium])
						{
							mediumFish.sprite = Enemy::getSprites()[medium];
							mediumFish.sprite.setPosition(Vector2f(rand() % 1749, rand() % 974));
						}

						//Large
						Enemy::modifyStats(large, 2.25f, 0.225f, 0.1875f, 1, 2, 15, 5, 5, 20);

						spriteModified.setTexture(marlinTexture);
						spriteModified.setTextureRect(IntRect(251, 0, -251, 176));
						spriteModified.setScale(1.4, 1.4);
						Enemy::setSprite(large, spriteModified);

						while (Enemy::enemies[large].size() < MAX_NUM_LARGE)
							Enemy fish(large);

						for (auto& largeFish : Enemy::enemies[large])
						{
							largeFish.sprite = Enemy::getSprites()[large];
							largeFish.sprite.setPosition(Vector2f(rand() % 1669, rand() % 904));
						}
					}
					else if (selection == 3)
					{
						//Player
						playerSprite.setTexture(anglerGreenTexture);
						playerSprite.setTextureRect(IntRect(0, 0, 201, 181));
						playerSprite.setPosition(Vector2f(1560, 455));

						//Small
						Enemy::modifyStats(small, 3.75f, 0.375f, 0.05f, 1, 2, 14, 5, 5, 0);

						Sprite spriteModified;
						spriteModified.setTexture(surgeonRedTexture);
						spriteModified.setTextureRect(IntRect(171, 0, -171, 106));
						spriteModified.setScale(0.6, 0.6);
						Enemy::setSprite(small, spriteModified);

						while (Enemy::enemies[small].size() < MAX_NUM_SMALL)
							Enemy fish(small);

						for (auto& smallFish : Enemy::enemies[small])
						{
							smallFish.sprite = Enemy::getSprites()[small];
							smallFish.sprite.setPosition(Vector2f(rand() % 1749, rand() % 974));
						}

						//Medium
						Enemy::modifyStats(medium, 3.f, 0.3f, 0.16875f, 1, 2, 15, 5, 6, 10);

						spriteModified.setTexture(doryBlueTexture);
						spriteModified.setTextureRect(IntRect(181, 0, -181, 181));
						spriteModified.setScale(1.1, 1.1);
						Enemy::setSprite(medium, spriteModified);

						while (Enemy::enemies[medium].size() < MAX_NUM_MEDIUM)
							Enemy fish(medium);

						for (auto& mediumFish : Enemy::enemies[medium])
						{
							mediumFish.sprite = Enemy::getSprites()[medium];
							mediumFish.sprite.setPosition(Vector2f(rand() % 1739, rand() % 899));
						}

						//Large
						Enemy::modifyStats(large, 2.25f, 0.225f, 0.1875f, 1, 2, 14, 5, 6, 20);

						spriteModified.setTexture(barracudaTexture);
						spriteModified.setTextureRect(IntRect(271, 0, -271, 121));
						spriteModified.setScale(1.4, 1.4);
						Enemy::setSprite(large, spriteModified);

						while (Enemy::enemies[large].size() < MAX_NUM_LARGE)
							Enemy fish(large);

						for (auto& largeFish : Enemy::enemies[large])
						{
							largeFish.sprite = Enemy::getSprites()[large];
							largeFish.sprite.setPosition(Vector2f(rand() % 1649, rand() % 959));
						}
					}
					else
					{
						//Player
						playerSprite.setTexture(angelOrangeTexture);
						playerSprite.setTextureRect(IntRect(0, 0, 126, 106));
						playerSprite.setPosition(Vector2f(1585, 510));

						//Small
						Enemy::modifyStats(small, 3.75f, 0.375f, 0.05f, 0, 1, 15, 7, 0, 0);

						Sprite spriteModified;
						spriteModified.setTexture(minnowTexture);
						spriteModified.setTextureRect(IntRect(66, 0, -66, 49));
						spriteModified.setScale(1, 1);
						Enemy::setSprite(small, spriteModified);

						while (Enemy::enemies[small].size() < MAX_NUM_SMALL)
							Enemy fish(small);

						for (auto& smallFish : Enemy::enemies[small])
						{
							smallFish.sprite = Enemy::getSprites()[small];
							smallFish.sprite.setPosition(Vector2f(rand() % 1854, rand() % 1031));
						}

						//Medium
						Enemy::modifyStats(medium, 3.f, 0.3f, 0.16875f, 1, 2, 14, 5, 5, 10);

						spriteModified.setTexture(surgeonYellowTexture);
						spriteModified.setTextureRect(IntRect(171, 0, -171, 106));
						spriteModified.setScale(1, 1);
						Enemy::setSprite(medium, spriteModified);

						while (Enemy::enemies[medium].size() < MAX_NUM_MEDIUM)
							Enemy fish(medium);

						for (auto& mediumFish : Enemy::enemies[medium])
						{
							mediumFish.sprite = Enemy::getSprites()[medium];
							mediumFish.sprite.setPosition(Vector2f(rand() % 1749, rand() % 974));
						}

						//Large
						Enemy::modifyStats(large, 2.25f, 0.225f, 0.1875f, 1, 2, 15, 5, 6, 20);

						spriteModified.setTexture(doryGreenTexture);
						spriteModified.setTextureRect(IntRect(181, 0, -181, 181));
						spriteModified.setScale(1.3, 1.3);
						Enemy::setSprite(large, spriteModified);

						while (Enemy::enemies[large].size() < MAX_NUM_LARGE)
							Enemy fish(large);

						for (auto& largeFish : Enemy::enemies[large])
						{
							largeFish.sprite = Enemy::getSprites()[large];
							largeFish.sprite.setPosition(Vector2f(rand() % 1739, rand() % 899));
						}
					}
				}

				//Entity functions
				update_dt();
				Enemy::spawnEnemies();
				Enemy::directionRandomize();
				Enemy::movement(dt, movementMultiplier);
				Enemy::moveEnemies(dt, movementMultiplier);
				Enemy::animation();
				Enemy::enemyEnemyCollision();

				//Fixes small fish dying to invisible fish in level 2
				if (text.getSelectedItem() == 1)
				{
					Enemy::enemies[medium].clear();
					Enemy::enemies[large].clear();
				}

				//Fixes fish disappearing in the first few seconds
				if (freezeClock.getElapsedTime().asSeconds() < 0.1f)
				{
					for (int i = small; i <= large; i++)
					{
						FishKey key = Enemy::getEnum(i);

						for (auto& enemy : Enemy::enemies[key])
						{
							enemy.velocity.x = enemy.velocity.y = 0;

							if (enemy.sprite.getPosition().x > WINDOW_WIDTH || enemy.sprite.getPosition().x < -enemy.sprite.getGlobalBounds().width ||
								enemy.sprite.getPosition().y > WINDOW_HEIGHT || enemy.sprite.getPosition().y < -enemy.sprite.getGlobalBounds().height)
								enemy.sprite.setPosition(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT);
						}
					}
				}

				//Draw
				window.clear();

				//Draw background & enemies
				if (selection == 0)
				{
					backgroundSprite.setTexture(backgroundTexture[0]);
					window.draw(backgroundSprite);
					for (int index = small; index <= large; index++) {
						FishKey key = Enemy::getEnum(index);

						for (const auto& fish : Enemy::enemies[key])
							window.draw(fish.sprite);
					}
					window.draw(darkOverlay);

					you.setPosition(Vector2f(1580, 420));
					you.setOutlineColor(Color::Black);
					if (!colorLoopSwitch)
						you.setFillColor(Color(255, 165, 0, 255));
					else
						you.setFillColor(Color(216, 52, 235, 255));
				}
				else if (selection == 1)
				{
					backgroundSprite.setTexture(backgroundTexture[0]);
					window.draw(backgroundSprite);
					for (auto& smallFish : Enemy::enemies[small])
						window.draw(smallFish.sprite);
					window.draw(darkOverlay);

					you.setPosition(Vector2f(1580, 420));
					you.setOutlineColor(Color::Black);
					if (!colorLoopSwitch)
						you.setFillColor(Color(255, 165, 0, 255));
					else
						you.setFillColor(Color(216, 52, 235, 255));
				}
				else if (selection == 2)
				{
					backgroundSprite.setTexture(backgroundTexture[1]);
					window.draw(backgroundSprite);
					for (int index = small; index <= large; index++) {
						FishKey key = Enemy::getEnum(index);

						for (const auto& fish : Enemy::enemies[key])
							window.draw(fish.sprite);
					}
					window.draw(darkOverlay);

					you.setPosition(Vector2f(1580, 410));
					you.setOutlineColor(Color::Black);
					if (!colorLoopSwitch)
						you.setFillColor(Color(248, 208, 116, 255));
					else
						you.setFillColor(Color(170, 145, 80, 255));
				}
				else
				{
					backgroundSprite.setTexture(backgroundTexture[2]);
					window.draw(backgroundSprite);
					for (int index = small; index <= large; index++) {
						FishKey key = Enemy::getEnum(index);

						for (const auto& fish : Enemy::enemies[key])
							window.draw(fish.sprite);
					}
					window.draw(darkOverlay);

					you.setPosition(Vector2f(1580, 410));
					you.setOutlineColor(Color::White);
					if (!colorLoopSwitch)
						you.setFillColor(Color(46, 163, 0, 255));
					else
						you.setFillColor(Color(6, 87, 1, 255));
				}

				//Draw rest
				window.draw(playerSprite);
				window.draw(you);
				window.draw(textBackground);
				text.draw(window);
				window.draw(bottomInfo);
				window.display();
			}

			Enemy::clear();
			if (currentWindowType != MainMenu)
			{
				mainMenuMusic.stop();
				introSound.play();
			}
			break;
		}
	}
}

void Game::instructions() 
{
	Texture backgroundTexture;
	Sprite backgroundSprite;
	Font bottomInfoFont;
	Text bottomInfo;
	View view;

	Initialization::instructionsInitialization(backgroundTexture, 
		backgroundSprite, bottomInfoFont, bottomInfo, view, window);

	while (true)
	{
		if (currentWindowType == Instructions)
		{
			while (window.isOpen())
			{
				Event event;
				while (window.pollEvent(event))
				{
					if (event.type == Event::Closed)
						window.close();
					if (event.type == Event::KeyReleased)
						if (event.key.code == Keyboard::Escape)
						{
							enterSound.play();
							currentWindowType = MainMenu;
						}
				}
				if (currentWindowType != Instructions)
					break;

				window.clear();
				window.draw(backgroundSprite);
				window.draw(bottomInfo);
				window.display();
			}
		}
		else
			break;
	}
}

void Game::credits() 
{
	Texture backgroundTexture;
	Sprite backgroundSprite;
	Font bottomInfoFont;
	Text bottomInfo;
	View view;

	Initialization::creditsInitialization(backgroundTexture,
		backgroundSprite, bottomInfoFont, bottomInfo, view, window);

	while (true)
	{
		if (currentWindowType == Credits)
		{
			while (window.isOpen())
			{
				Event event;
				while (window.pollEvent(event))
				{
					if (event.type == Event::Closed)
						window.close();

					if (event.type == Event::KeyPressed)
					{
						if (event.key.code == Keyboard::Escape)
						{
							enterSound.play();
							currentWindowType = MainMenu;
						}
					}
				}
				if (currentWindowType != Credits)
					break;

				window.clear();
				window.draw(backgroundSprite);
				window.draw(bottomInfo);
				window.display();
			}
		}
		else
			break;
	}
}

void Game::quit() 
{
	mainMenuMusic.stop();
	highScores_output();
	window.close();
	exit(0);
}

void Game::level_1() 
{
	View view(FloatRect(0, 0, 1920, 1080));

	//Background
	Texture backgroundTexture;
	Sprite backgroundSprite;

	//Player
	Texture playerTexture;
	Player player(11.25f, 0.6f, 0.2f, 2, 3, 1, 15, 5, 6, 6, 0);
	
	//Enemy textures
	Texture enemySmall;
	Texture enemyMedium;
	Texture enemyLarge;

	//Oyster
	Oyster oyster;

	//Shark
	Shark shark;

	//HUD
	HUD hud;

	//Level's music
	Music levelMusic;

	Initialization::level_1_Initialization(view, window, backgroundTexture, backgroundSprite, 
		playerTexture, player.sprite, enemySmall, enemyMedium, enemyLarge, hud, levelMusic);

	while (true)
	{
		if (currentWindowType == LevelOne)
		{
			if (levelMusic.getStatus() != sf::Music::Playing)
				levelMusic.play();

			while (window.isOpen())
			{
				//Key events
				Event event;
				while (window.pollEvent(event))
				{
					if (event.type == Event::Closed)
						currentWindowType = Exit;
					if (event.type == Event::KeyPressed)
					{
						if (event.key.code == Keyboard::Escape)
						{
							if (!hud.pauseMenuIsOpen)
							{
								hud.pauseMenuIsOpen = true;
								levelMusic.pause();
							}
							else
							{
								hud.pauseMenuIsOpen = false;
								levelMusic.play();
							}
							if (player.deathStatus)
							{
								enterSound.play();
								currentWindowType = MainMenu;
								changeCurrentWIndowDelay.restart();
							}
							break;
						}

						if (player.deathStatus)
						{
							if (event.key.code == Keyboard::Return)
							{
								currentWindowType = MainMenu;
								changeCurrentWIndowDelay.restart();
								break;
							}
						}

						if (hud.pauseMenuIsOpen)
						{
							if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W)
							{
								scrollSound.play();
								hud.pauseMenu.moveUp();
							}
							if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S)
							{
								scrollSound.play();
								hud.pauseMenu.moveDown();
							}
							if (event.key.code == Keyboard::Return)
							{
								enterSound.play();

								if (hud.pauseMenu.getSelectedItem() == 0)
								{
									hud.pauseMenuIsOpen = false;
									levelMusic.play();
								}
								else if (hud.pauseMenu.getSelectedItem() == 1)
								{
									changeCurrentWIndowDelay.restart();
									currentWindowType = LevelSelection;
									break;
								}
								else if (hud.pauseMenu.getSelectedItem() == 2)
									currentWindowType = Exit;
							}
						}
					}
				}

				if (currentWindowType != LevelOne)
					break;

				//Time between frames
				update_dt();

				if (!hud.pauseMenuIsOpen)
				{
					//Player's movement, collision, animation, etc
					player.movement(dt, movementMultiplier);
					player.move(dt, movementMultiplier);
					player.screenCollision();
					player.animation();
					player.grow(hud.isFishMenuLocked[0], hud.isFishMenuLocked[1]);
					player.enemyCollision(hud.leaderboardScore, hud.frenzyMultiplier, hud.frenzyRow,
						hud.frenzyColumn, hud.frenzyGainClockMillisecs, hud.chompSprite);

					//Enemy spawn, movement, behaviour, animation, collision
					Enemy::spawnEnemies();
					Enemy::directionRandomize();
					Enemy::movement(dt, movementMultiplier);
					Enemy::behavior(player.sprite, player.score, dt, movementMultiplier);
					Enemy::moveEnemies(dt, movementMultiplier);
					Enemy::animation();
					Enemy::enemyEnemyCollision();

					//Shark spawn, collision and animation
					shark.spawn();
					shark.movement(dt, movementMultiplier);
					shark.collision(player.sprite, player.deathStatus);
					shark.animation();

					//OYSTER ANIMATION AND SHIT
					oyster.update();
					oyster.animation();
					oyster.collision(player.sprite, player.isEating, player.deathStatus, hud.leaderboardScore, hud.chompSprite);

					//Frenzy and leaderboard score updates
					hud.frenzyUpdate(player.ateInLastSeconds);
					hud.update(player.score);
				}

				//Level completion
				if (hud.progressBarMaxReached)
				{
					stageSound.play();
					levelMusic.stop();

					levelScore = hud.leaderboardScore;

					currentLevel = LevelOne;
					currentWindowType = LevelEndScreen;
				}

				//Rendering
				window.clear();
				window.draw(backgroundSprite);

				//Enemy
				oyster.draw(window);
				Enemy::drawEnemies(window);
				shark.draw(window);

				if (!player.deathStatus)
				{
					//Scorebar, fish menu, chomp, frenzy
					hud.draw(window, player.isEating, "LevelOne");

					//Player
					player.draw(window);
				}
				else //Drawing death screen
				{
					window.draw(hud.darkOverlay);
					window.draw(hud.deathMenuSprite);

					hud.deathTextColorLoop();
					window.draw(hud.deathText);

					if (!player.playerDeathSoundPlayed && player.gameoverSound.getStatus() != Sound::Playing)
					{
						player.gameoverSound.play();
						player.playerDeathSoundPlayed = true;
					}

					levelMusic.stop();
				}

				//when pause menu is open
				if (hud.pauseMenuIsOpen)
				{
					window.draw(hud.darkOverlay);
					window.draw(hud.pauseMenuSprite);
					hud.pauseMenu.draw(window);
				}
				window.display();
			}

			player.clear();
			hud.clear();
			Enemy::clear();
			levelMusic.stop();
			break;
		}
	}
}

void Game::level_2() 
{
	View view(FloatRect(0, 0, 1920, 1080));

	Texture backgroundTexture;
	Sprite backgroundSprite;

	Texture playerTexture;
	Player player(11.25f, 0.6f, 0.2f, 2, 3, 1, 15, 5, 6, 6, 0);

	Texture enemySmall;

	HUD hud;

	Texture clockBackgroundTexture;
	Sprite clockBackgroundSprite[2];
	Texture clockTexture;
	Sprite clockSprite;
	
	Clock levelClock;
	float levelTimer;
	float currentTime;
	float timeBeforePause = 0.f;
	bool isTimerPaused = false;
	
	Font timerFont;
	Text timerText;

	Font fishEatenCountFont;
	Text fishEatenCountText;

	Music levelMusic;

	Initialization::level_2_Initialization(window, view, backgroundTexture, backgroundSprite, playerTexture, player.sprite,
		enemySmall, hud, clockBackgroundTexture, clockBackgroundSprite, clockTexture, clockSprite, timerFont, timerText,
		fishEatenCountFont, fishEatenCountText, levelMusic);

	while (true)
	{
		if (currentWindowType == LevelTwo)
		{
			if (levelMusic.getStatus() != sf::Music::Playing)
				levelMusic.play();

			while (window.isOpen())
			{
				//Key events
				Event event;
				while (window.pollEvent(event))
				{
					if (event.type == Event::Closed)
						currentWindowType = Exit;
					if (event.type == Event::KeyPressed)
					{
						if (event.key.code == Keyboard::Escape)
						{
							if (!hud.pauseMenuIsOpen)
							{
								hud.pauseMenuIsOpen = true;
								levelMusic.pause();
							}
							else
							{
								hud.pauseMenuIsOpen = false;
								levelMusic.play();
							}
							if (player.deathStatus)
							{
								enterSound.play();
								currentWindowType = MainMenu;
								changeCurrentWIndowDelay.restart();
							}
							break;
						}

						if (player.deathStatus)
						{
							if (event.key.code == Keyboard::Return)
							{
								currentWindowType = MainMenu;
								changeCurrentWIndowDelay.restart();
								break;
							}
						}

						if (hud.pauseMenuIsOpen)
						{
							if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W)
							{
								scrollSound.play();
								hud.pauseMenu.moveUp();
							}
							if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S)
							{
								scrollSound.play();
								hud.pauseMenu.moveDown();
							}
							if (event.key.code == Keyboard::Return)
							{
								enterSound.play();

								if (hud.pauseMenu.getSelectedItem() == 0)
								{
									hud.pauseMenuIsOpen = false;
									levelMusic.play();
								}
								else if (hud.pauseMenu.getSelectedItem() == 1)
								{
									changeCurrentWIndowDelay.restart();
									currentWindowType = LevelSelection;
									break;
								}
								else if (hud.pauseMenu.getSelectedItem() == 2)
									currentWindowType = Exit;
							}
						}
					}
				}

				if (currentWindowType != LevelTwo)
					break;

				//Time between frames
				update_dt();

				//timerText update
				levelTimer = levelClock.getElapsedTime().asSeconds();
				currentTime = 30.f - levelTimer - timeBeforePause;

				if (!hud.pauseMenuIsOpen)
				{
					if (static_cast<int>(currentTime) > 9)
						timerText.setString("00:" + std::to_string(static_cast<int>(currentTime)));
					else
						timerText.setString("00:0" + std::to_string(static_cast<int>(currentTime)));
				}
				else
				{
					if (static_cast<int>(30 - timeBeforePause) > 9)
						timerText.setString("00:" + std::to_string(static_cast<int>(30 - timeBeforePause)));
					else
						timerText.setString("00:0" + std::to_string(static_cast<int>(30 - timeBeforePause)));
				}

				//fishEatenCountText update
				if (player.fishEatenCount <= 30)
					fishEatenCountText.setString(std::to_string(player.fishEatenCount) + " of 30 ");

				if (player.fishEatenCount > 9)
					fishEatenCountText.setPosition(1630, 930);

				if (!hud.pauseMenuIsOpen && !isTimerPaused)
				{
					//Player death
					if (levelTimer >= 30.f - timeBeforePause)
						player.deathStatus = true;
					//Level completion
					else if (player.fishEatenCount >= 30)
					{
						stageSound.play();
						levelMusic.stop();

						levelScore = hud.leaderboardScore;

						currentLevel = LevelTwo;
						currentWindowType = LevelEndScreen;
					}
				}
				//Pausing timer
				else if (hud.pauseMenuIsOpen && !isTimerPaused)
				{
					timeBeforePause += levelTimer;
					isTimerPaused = true;
				}
				else if (!hud.pauseMenuIsOpen && isTimerPaused)
				{
					levelClock.restart();
					isTimerPaused = false;
				}

				if (!hud.pauseMenuIsOpen)
				{
					//Player's movement, collision, animation, etc
					player.movement(dt, movementMultiplier);
					player.move(dt, movementMultiplier);
					player.screenCollision();
					player.animation();
					player.grow(hud.isFishMenuLocked[0], hud.isFishMenuLocked[1]);
					player.enemyCollision(hud.leaderboardScore, hud.frenzyMultiplier, hud.frenzyRow,
						hud.frenzyColumn, hud.frenzyGainClockMillisecs, hud.chompSprite);

					//Enemy spawn, movement, behaviour, animation, collision
					Enemy::spawnEnemies();
					Enemy::directionRandomize();
					Enemy::movement(dt, movementMultiplier);
					Enemy::behavior(player.sprite, player.score, dt, movementMultiplier);
					Enemy::moveEnemies(dt, movementMultiplier);
					Enemy::animation();
					Enemy::enemyEnemyCollision();

					//Frenzy and leaderboard score updates
					hud.frenzyUpdate(player.ateInLastSeconds);
					hud.update(player.score);
				}

				//Rendering
				window.clear();
				window.draw(backgroundSprite);

				//Enemy
				Enemy::drawEnemies(window);

				if (!player.deathStatus)
				{
					//Scorebar, fish menu, chomp, frenzy
					hud.draw(window, player.isEating, "LevelTwo");

					window.draw(clockBackgroundSprite[0]);
					window.draw(clockBackgroundSprite[1]);
					window.draw(fishEatenCountText);
					window.draw(clockSprite);
					window.draw(timerText);

					//Player
					player.draw(window);
				}
				else //Drawing death screen
				{
					window.draw(hud.darkOverlay);
					window.draw(hud.deathMenuSprite);

					hud.deathTextColorLoop();
					window.draw(hud.deathText);

					if (!player.playerDeathSoundPlayed && player.gameoverSound.getStatus() != Sound::Playing)
					{
						player.gameoverSound.play();
						player.playerDeathSoundPlayed = true;
					}

					levelMusic.stop();
				}

				//Drawing pauseMenu
				if (hud.pauseMenuIsOpen)
				{
					window.draw(hud.darkOverlay);
					window.draw(hud.pauseMenuSprite);
					hud.pauseMenu.draw(window);
				}
				window.display();
			}

			player.clear();
			hud.clear();
			Enemy::clear();
			levelMusic.stop();
			break;
		}
	}
}

void Game::level_3() 
{
	View view(FloatRect(0, 0, 1920, 1080));

	//Background
	Texture backgroundTexture;
	Sprite backgroundSprite;

	//Player
	Texture playerTexture;
	Player player(11.25f, 0.6f, 0.2f, 2, 3, 1, 15, 5, 6, 7, 0);

	//Enemy textures
	Texture enemySmall;
	Texture enemyMedium;
	Texture enemyLarge;

	//HUD
	HUD hud;

	//Level's music
	Music levelMusic;

	Initialization::level_3_Initialization(view, window, backgroundTexture, backgroundSprite,
		playerTexture, player.sprite, enemySmall, enemyMedium, enemyLarge, hud, levelMusic);

	while (true)
	{
		if (currentWindowType == LevelThree)
		{
			if (levelMusic.getStatus() != sf::Music::Playing)
				levelMusic.play();

			while (window.isOpen())
			{
				//Key events
				Event event;
				while (window.pollEvent(event))
				{
					if (event.type == Event::Closed)
						currentWindowType = Exit;
					if (event.type == Event::KeyPressed)
					{
						if (event.key.code == Keyboard::Escape)
						{
							if (!hud.pauseMenuIsOpen)
							{
								hud.pauseMenuIsOpen = true;
								levelMusic.pause();
							}
							else
							{
								hud.pauseMenuIsOpen = false;
								levelMusic.play();
							}
							if (player.deathStatus)
							{
								enterSound.play();
								currentWindowType = MainMenu;
								changeCurrentWIndowDelay.restart();
							}
							break;
						}

						if (player.deathStatus)
						{
							if (event.key.code == Keyboard::Return)
							{
								currentWindowType = MainMenu;
								changeCurrentWIndowDelay.restart();
								break;
							}
						}

						if (hud.pauseMenuIsOpen)
						{
							if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W)
							{
								scrollSound.play();
								hud.pauseMenu.moveUp();
							}
							if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S)
							{
								scrollSound.play();
								hud.pauseMenu.moveDown();
							}
							if (event.key.code == Keyboard::Return)
							{
								enterSound.play();

								if (hud.pauseMenu.getSelectedItem() == 0)
								{
									hud.pauseMenuIsOpen = false;
									levelMusic.play();
								}
								else if (hud.pauseMenu.getSelectedItem() == 1)
								{
									changeCurrentWIndowDelay.restart();
									currentWindowType = LevelSelection;
									break;
								}
								else if (hud.pauseMenu.getSelectedItem() == 2)
									currentWindowType = Exit;
							}
						}
					}
				}

				if (currentWindowType != LevelThree)
					break;

				//Time between frames
				update_dt();

				if (!hud.pauseMenuIsOpen)
				{
					//Player's movement, collision, animation, etc
					player.movement(dt, movementMultiplier);
					player.move(dt, movementMultiplier);
					player.screenCollision();
					player.animation();
					player.grow(hud.isFishMenuLocked[0], hud.isFishMenuLocked[1]);
					player.enemyCollision(hud.leaderboardScore, hud.frenzyMultiplier, hud.frenzyRow,
						hud.frenzyColumn, hud.frenzyGainClockMillisecs, hud.chompSprite);

					//Enemy spawn, movement, behaviour, animation, collision
					Enemy::spawnEnemies();
					Enemy::directionRandomize();
					Enemy::movement(dt, movementMultiplier);
					Enemy::behavior(player.sprite, player.score, dt, movementMultiplier);
					Enemy::moveEnemies(dt, movementMultiplier);
					Enemy::animation();
					Enemy::enemyEnemyCollision();

					//Frenzy and leaderboard score updates
					hud.frenzyUpdate(player.ateInLastSeconds);
					hud.update(player.score);
				}

				//Level completion
				if (hud.progressBarMaxReached)
				{
					stageSound.play();
					levelMusic.stop();

					levelScore = hud.leaderboardScore;

					currentLevel = LevelThree;
					currentWindowType = LevelEndScreen;
				}

				//Rendering
				window.clear();
				window.draw(backgroundSprite);

				//Enemy
				Enemy::drawEnemies(window);

				if (!player.deathStatus)
				{
					//Scorebar, fish menu, chomp, frenzy
					hud.draw(window, player.isEating, "LevelThree");

					//Player
					player.draw(window);
				}
				else //Drawing death screen
				{
					window.draw(hud.darkOverlay);
					window.draw(hud.deathMenuSprite);

					hud.deathTextColorLoop();
					window.draw(hud.deathText);

					if (!player.playerDeathSoundPlayed && player.gameoverSound.getStatus() != Sound::Playing)
					{
						player.gameoverSound.play();
						player.playerDeathSoundPlayed = true;
					}

					levelMusic.stop();
				}

				//when pause menu is open
				if (hud.pauseMenuIsOpen)
				{
					window.draw(hud.darkOverlay);
					window.draw(hud.pauseMenuSprite);
					hud.pauseMenu.draw(window);
				}
				window.display();
			}

			player.clear();
			hud.clear();
			Enemy::clear();
			levelMusic.stop();
			break;
		}
	}
}

void Game::level_4() 
{
	View view(FloatRect(0, 0, 1920, 1080));

	//Background
	Texture backgroundTexture;
	Sprite backgroundSprite;

	//Player
	Texture playerTexture;
	Player player(11.25f, 0.6f, 0.2f, 2, 3, 1, 15, 5, 6, 7, 0);

	//Enemy textures
	Texture enemySmall;
	Texture enemyMedium;
	Texture enemyLarge;

	//Oyster
	Oyster oyster;

	//HUD
	HUD hud;

	//Level's music
	Music levelMusic;

	Initialization::level_4_Initialization(view, window, backgroundTexture, backgroundSprite,
		playerTexture, player.sprite, enemySmall, enemyMedium, enemyLarge, hud, levelMusic);

	while (true)
	{
		if (currentWindowType == LevelFour)
		{
			if (levelMusic.getStatus() != sf::Music::Playing)
				levelMusic.play();

			while (window.isOpen())
			{
				//Key events
				Event event;
				while (window.pollEvent(event))
				{
					if (event.type == Event::Closed)
						currentWindowType = Exit;
					if (event.type == Event::KeyPressed)
					{
						if (event.key.code == Keyboard::Escape)
						{
							if (!hud.pauseMenuIsOpen)
							{
								hud.pauseMenuIsOpen = true;
								levelMusic.pause();
							}
							else
							{
								hud.pauseMenuIsOpen = false;
								levelMusic.play();
							}
							if (player.deathStatus)
							{
								enterSound.play();
								currentWindowType = MainMenu;
								changeCurrentWIndowDelay.restart();
							}
							break;
						}

						if (player.deathStatus)
						{
							if (event.key.code == Keyboard::Return)
							{
								currentWindowType = MainMenu;
								changeCurrentWIndowDelay.restart();
								break;
							}
						}

						if (hud.pauseMenuIsOpen)
						{
							if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W)
							{
								scrollSound.play();
								hud.pauseMenu.moveUp();
							}
							if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S)
							{
								scrollSound.play();
								hud.pauseMenu.moveDown();
							}
							if (event.key.code == Keyboard::Return)
							{
								enterSound.play();

								if (hud.pauseMenu.getSelectedItem() == 0)
								{
									hud.pauseMenuIsOpen = false;
									levelMusic.play();
								}
								else if (hud.pauseMenu.getSelectedItem() == 1)
								{
									changeCurrentWIndowDelay.restart();
									currentWindowType = LevelSelection;
									break;
								}
								else if (hud.pauseMenu.getSelectedItem() == 2)
									currentWindowType = Exit;
							}
						}
					}
				}

				if (currentWindowType != LevelFour)
					break;

				//Time between frames
				update_dt();

				if (!hud.pauseMenuIsOpen)
				{
					//Player's movement, collision, animation, etc
					player.movement(dt, movementMultiplier);
					player.move(dt, movementMultiplier);
					player.screenCollision();
					player.animation();
					player.grow(hud.isFishMenuLocked[0], hud.isFishMenuLocked[1]);
					player.enemyCollision(hud.leaderboardScore, hud.frenzyMultiplier, hud.frenzyRow,
						hud.frenzyColumn, hud.frenzyGainClockMillisecs, hud.chompSprite);

					//Enemy spawn, movement, behaviour, animation, collision
					Enemy::spawnEnemies();
					Enemy::directionRandomize();
					Enemy::movement(dt, movementMultiplier);
					Enemy::behavior(player.sprite, player.score, dt, movementMultiplier);
					Enemy::moveEnemies(dt, movementMultiplier);
					Enemy::animation();
					Enemy::enemyEnemyCollision();

					//OYSTER ANIMATION AND SHIT
					oyster.update();
					oyster.animation();
					oyster.collision(player.sprite, player.isEating, player.deathStatus, hud.leaderboardScore, hud.chompSprite);

					//Frenzy and leaderboard score updates
					hud.frenzyUpdate(player.ateInLastSeconds);
					hud.update(player.score);
				}

				//Level completion
				if (hud.progressBarMaxReached)
				{
					stageSound.play();
					levelMusic.stop();

					levelScore = hud.leaderboardScore;

					currentLevel = LevelFour;
					currentWindowType = LevelEndScreen;
				}

				//Rendering
				window.clear();
				window.draw(backgroundSprite);

				//Enemy
				oyster.draw(window);
				Enemy::drawEnemies(window);

				if (!player.deathStatus)
				{
					//Scorebar, fish menu, chomp, frenzy
					hud.draw(window, player.isEating, "LevelFour");

					//Player
					player.draw(window);
				}
				else //Drawing death screen
				{
					window.draw(hud.darkOverlay);
					window.draw(hud.deathMenuSprite);

					hud.deathTextColorLoop();
					window.draw(hud.deathText);

					if (!player.playerDeathSoundPlayed && player.gameoverSound.getStatus() != Sound::Playing)
					{
						player.gameoverSound.play();
						player.playerDeathSoundPlayed = true;
					}

					levelMusic.stop();
				}

				//when pause menu is open
				if (hud.pauseMenuIsOpen)
				{
					window.draw(hud.darkOverlay);
					window.draw(hud.pauseMenuSprite);
					hud.pauseMenu.draw(window);
				}
				window.display();
			}

			player.clear();
			hud.clear();
			Enemy::clear();
			levelMusic.stop();
			break;
		}
	}
}

void Game::levelEndScreen()
{
	View view(FloatRect(0, 0, 1920, 1080));
	window.setView(view);

	stageSound.play();

	Texture backgroundTexture;
	backgroundTexture.loadFromFile("resources/backgrounds/level_endscreen.png");
	Sprite backgroundSprite;
	backgroundSprite.setTexture(backgroundTexture);

	Font scoreFont;
	scoreFont.loadFromFile("resources/fonts/five.otf");

	Text highScoreText;
	Text levelScoreText;

	highScoreText.setFont(scoreFont);
	if (levelScore < highScores[currentLevel])
		highScoreText.setString("- high score: " + std::to_string(highScores[currentLevel]));
	else
		highScoreText.setString("- new high score");
	highScoreText.setCharacterSize(30);
	highScoreText.setFillColor(Color(194, 232, 255, 150));
	highScoreText.setPosition(Vector2f(800, 325));

	levelScoreText.setFont(scoreFont);
	levelScoreText.setString(std::to_string(levelScore));
	levelScoreText.setCharacterSize(75);
	levelScoreText.setFillColor(Color(194, 232, 255));
	levelScoreText.setOrigin(levelScoreText.getGlobalBounds().width, 0);
	levelScoreText.setPosition(Vector2f(1525, 295));

	Menu endScreenMenu(2);

	endScreenMenu.lines[0].setString("NEXT");
	endScreenMenu.lines[0].setCharacterSize(75);
	endScreenMenu.lines[0].setPosition(Vector2f(1140, 595));

	endScreenMenu.lines[1].setString("HOME");
	endScreenMenu.lines[1].setCharacterSize(75);
	endScreenMenu.lines[1].setPosition(Vector2f(615, 595));

	while (true)
	{
		while (window.isOpen())
		{
			Event event;
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
				{
					currentWindowType = Exit;
					break;
				}

				if (event.type == Event::KeyReleased)
				{
					if (event.key.code == Keyboard::Right)
					{
						scrollSound.play();
						endScreenMenu.moveDown();
						break;
					}
					if (event.key.code == Keyboard::Left)
					{
						scrollSound.play();
						endScreenMenu.moveUp();
						break;
					}
					if (event.key.code == Keyboard::Return)
					{
						enterSound.play();
						
						if (endScreenMenu.getSelectedItem() == 0)
						{
							switch (currentLevel)
							{
							case LevelOne:
								currentWindowType = LevelTwo;
								break;
							case LevelTwo:
								currentWindowType = LevelThree;
								break;
							case LevelThree:
								currentWindowType = LevelFour;
								break;
							case LevelFour:
								currentWindowType = GameEndScreen;
							}
						}
						else if (endScreenMenu.getSelectedItem() == 1)
							currentWindowType = MainMenu;

						break;
					}
				}
			}

			if (currentWindowType != LevelEndScreen)
			{
				levelScores[currentLevel] = levelScore;
				highScores_update();
				break;
			}

			window.clear();

			window.draw(backgroundSprite);
			window.draw(highScoreText);
			window.draw(levelScoreText);
			endScreenMenu.draw(window);

			window.display();
		}
		break;
	}
}

void Game::gameEndScreen() 
{
	View view(FloatRect(0, 0, 1920, 1080));
	window.setView(view);

	Texture backgroundTexture;
	Sprite backgroundSprite;
	backgroundTexture.loadFromFile("resources/backgrounds/game_endscreen.png");
	backgroundSprite.setTexture(backgroundTexture);

	Font bottomInfoFont;
	bottomInfoFont.loadFromFile("resources/fonts/scrubland_regular.ttf");
	
	Text bottomInfo;
	bottomInfo.setFont(bottomInfoFont);
	bottomInfo.setCharacterSize(30);
	bottomInfo.setFillColor(Color::Yellow);
	bottomInfo.setString("Press Escape to return to main menu");
	bottomInfo.setPosition(Vector2f(700, 1040));
	bottomInfo.setOutlineColor(Color::Black);
	bottomInfo.setOutlineThickness(1.5);

	Font font;
	font.loadFromFile("resources/fonts/five.otf");

	Text highScore[4], levelScore[4];

	for (int index = 0; index < 4; index++)
	{
		highScore[index].setFont(font);
		levelScore[index].setFont(font);

		highScore[index].setString(std::to_string(highScores[getEnum(index + 5)]));
		levelScore[index].setString(std::to_string(levelScores[getEnum(index + 5)]));

		highScore[index].setCharacterSize(100);
		levelScore[index].setCharacterSize(100);

		highScore[index].setOrigin(highScore[index].getGlobalBounds().width, 0);
		levelScore[index].setOrigin(levelScore[index].getGlobalBounds().width, 0);

		highScore[index].setFillColor(Color(194, 232, 255));
		levelScore[index].setFillColor(Color(194, 232, 255, 200));
	}

	highScore[0].setPosition(1610, 362);
	highScore[1].setPosition(1610, 528);
	highScore[2].setPosition(1610, 694);
	highScore[3].setPosition(1610, 860);

	levelScore[0].setPosition(1075, 362);
	levelScore[1].setPosition(1075, 528);
	levelScore[2].setPosition(1075, 694);
	levelScore[3].setPosition(1075, 860);

	while (true)
	{
		while (window.isOpen())
		{
			if (mainMenuMusic.getStatus() != Music::Playing)
				mainMenuMusic.play();

			Event event;
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
					currentWindowType = Exit;
				if (event.type == Event::KeyReleased && (event.key.code == Keyboard::Return || event.key.code == Keyboard::Escape))
				{
					enterSound.play();
					currentWindowType = MainMenu;
				}
			}

			if (currentWindowType != GameEndScreen)
				break;

			window.clear();
			window.draw(backgroundSprite);
			window.draw(bottomInfo);
			
			for (int index = 0; index < 4; index++)
			{
				window.draw(highScore[index]);
				window.draw(levelScore[index]);
			}

			window.display();
		}
		break;
	}
}