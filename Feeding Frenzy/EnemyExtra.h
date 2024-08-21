#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

class Shark
{
	Texture texture;
	Sprite sprite;

	Clock spawnClockSecs, animationClockMillisecs, eatingClockMillisecs;
	float spawnTimeSecs, animationTimeMillisecs, eatingTimeMillisecs;
	
	int spawnSide;
	int currentFrame;
	bool isEating;

public:
	Shark(float spawnTimeSecs = 13.f, float animationTimeMillisecs = 100.f, float eatingTimeMillisecs = 40.f);

	void spawn();

	void movement(float dt, float movementMultiplier);

	void animation();

	void collision(Sprite& playerSprite, bool& playerDeathStatus);

	void draw(RenderWindow& window);
};

class Oyster
{
	Texture texture, pearlTexture;
	Sprite sprite, pearlSprite;

	Clock openClockSecs, stayOpenClockSecs, animationClockMillisecs;
	float openTimeSecs, stayOpenTimeSecs, animationTimeMillisecs;

	int currentFrame;
	bool isClosing;
	bool isPearlVisible;

	SoundBuffer pearlEatBuffer;
	Sound pearlEat;

	SoundBuffer killPlayerBuffer;
	Sound killPlayer;

public:
	Oyster(float openTimeSecs = 12.f, float stayOpenTimeSecs = 8.f, float animationTimeMillisecs = 40.f);

	void update();

	void animation();

	void collision(Sprite& playerSprite, bool& isPlayerEating, bool& deathStatus, int& hudLeaderboardScore, Sprite& chomp);

	void draw(RenderWindow& window);
};