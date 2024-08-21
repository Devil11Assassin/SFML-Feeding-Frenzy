#pragma once
#include "sfml/Graphics.hpp"
using namespace sf;

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

class Entity
{
protected:
	float maxVel, accel, resist;
	int rowMotion, rowTurn, maxFrameMotion, maxFrameTurn, maxFrameEat;
	float animationTime, eatingTime;

public:
	Texture texture;
	float score;

	//Constructor
	Entity();
	Entity(float maxVel, float accel, float resist, int rowMotion, int rowTurn,
		int maxFrameMotion, int maxFrameTurn, int maxFrameEat, float score,
		float animationTime = 60, float eatingTime = 80);

	//Getters
	float getMaxVel();
	float getAccel();
	float getResist();

	int getRowMotion();
	int getRowTurn();
	int getMaxFrameMotion();
	int getMaxFrameTurn();
	int getMaxFrameEat();

	float getAnimationTime();
	float getEatingTime();
};