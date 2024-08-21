#pragma once
#include "Entity.h"
#include <vector>
#include <unordered_map>
#include <cmath>

enum FishKey {
	small = 0,
	medium = 1,
	large = 2
};

class Enemy
{
	static std::unordered_map<FishKey, Entity> stats;
	static std::unordered_map<FishKey, Sprite> sprites;

	static std::unordered_map<FishKey, Clock> directionRandomizerClocks;
	static std::unordered_map<FishKey, float> directionRandomizerTimes;

	static std::unordered_map<FishKey, Clock> spawnClocks;
	static std::unordered_map<FishKey, float> spawnTimes;
	static std::unordered_map<FishKey, int> spawnQuantities;

public:
	static std::unordered_map<FishKey, std::vector<Enemy>> enemies;

	Entity* stat;
	Sprite sprite;
	Vector2f velocity, direction;

	int currentFrame;
	Clock animationClock;

	bool isEating;
	int eatingCurrentFrame;
	Clock eatingClock;

	int directionRandomizer;
	
	//Constructors
	Enemy(FishKey key);

	//Getters
	static std::unordered_map<FishKey, Entity>& getStats();
	static std::unordered_map<FishKey, Sprite>& getSprites();

	static float getSpawnTime(FishKey key);
	static int getSpawnQuantity(FishKey key);

	//Setters
	static void setStats(FishKey key, Sprite sprite, float maxVel, float accel, float resist, int rowMotion,
		int rowTurn, int maxFrameMotion, int maxFrameTurn, int maxFrameEat, float score);
	static void setStats(FishKey key, Sprite sprite, float maxVel, float accel, float resist, int rowMotion,
		int rowTurn, int maxFrameMotion, int maxFrameTurn, int maxFrameEat, float score,
		float animationTime, float eatingTime);

	static void setSpriteTexture(FishKey key, Texture texture);
	static void setSprite(FishKey key, Sprite sprite);
	static void modifyStats(FishKey key, float maxVel, float accel, float resist, int rowMotion,
		int rowTurn, int maxFrameMotion, int maxFrameTurn, int maxFrameEat, float score);

	static void setDirectionRandomizerTime(FishKey key, float time);
	static void setSpawnTime(FishKey key, float time);
	static void setSpawnQuantity(FishKey key, int quantity);

	//Helper Methods
	static FishKey getEnum(int index);
	static float calcSin(int degree);
	static float calcCos(int degree);
	static void clear();

	//Methods

	static void spawnEnemies();

	static void directionRandomize();
	static void movement(float dt, float movementMultiplier);
	static void behavior(Sprite& playerSprite, float playerScore, float dt, float movementMultiplier);
	static void moveEnemies(float dt, float movementMultiplier);

	static void animation();
	
	void kill(FishKey key);
	static void enemyEnemyCollision();

	static void updateEnemies();
	
	static void drawEnemies(RenderWindow& window);
};