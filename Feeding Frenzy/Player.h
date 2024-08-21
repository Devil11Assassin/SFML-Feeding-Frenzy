#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include "Entity.h"
#include "Enemy.h"

class Player : public Entity
{
protected:
	int rowIdle;
	int maxFrameIdle;
	float idleTime;

public:
	Sprite sprite;
	Vector2f velocity, direction;

	int currentFrame;
	Clock animationClock;
	
	bool isEating;
	bool ateInLastSeconds;
	int eatingCurrentFrame;
	Clock eatingClock;
	int fishEatenCount;

	int currentFrameIdle;
	Clock idleClock;

	bool deathStatus;
	bool playerDeathSoundPlayed;

	std::unordered_map<FishKey, SoundBuffer> eatingBuffer;
	std::unordered_map<FishKey, Sound> eatingSound;
	SoundBuffer gulpBuffer;
	Sound gulpSound;

	SoundBuffer feedingFrenzyBuffer, doubleFrenzyBuffer;
	Sound feedingFrenzySound, doubleFrenzySound;

	SoundBuffer growthBuffer;
	Sound growthSound;
	bool isGrowing;

	SoundBuffer gameoverBuffer;
	Sound gameoverSound;

	//Constructor
	Player(float maxVel, float accel, float resist, int rowMotion, int rowTurn, int rowIdle,
		int maxFrameMotion, int maxFrameTurn, int maxFrameEat, int maxFrameIdle, float score,
		float animationTime = 60, float eatingTime = 80, float idleTime = 120);

	//Methods
	void movement(float dt, float movementMultiplier);
	void move(float dt, float movementMultiplier);
	void screenCollision();
	void animation();

	void enemyCollision(int& leaderboardScore, int& frenzyMultiplier, int& frenzyRow,
		int& frenzyColumn, Clock& frenzyGainClockMillisecs, Sprite& chomp);

	void grow(bool& isFishMenuLocked_0, bool& isFishMenuLocked_1);

	void draw(RenderWindow& window);
	void clear();
};