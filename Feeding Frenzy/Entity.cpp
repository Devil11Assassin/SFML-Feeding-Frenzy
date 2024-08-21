#include "Entity.h"

//Constructor
Entity::Entity() 
{
	maxVel = 0;
	accel = 0;
	resist = 0;

	rowMotion = 0;
	rowTurn = 0;
	maxFrameMotion = 0;
	maxFrameTurn = 0;
	maxFrameEat = 0;
	animationTime = 0;
	eatingTime = 0;

	score = 0;
}

Entity::Entity(float maxVel, float accel, float resist, int rowMotion, int rowTurn,
	int maxFrameMotion, int maxFrameTurn, int maxFrameEat, float score, float animationTime, float eatingTime)
{
	this->maxVel = maxVel;
	this->accel = accel;
	this->resist = resist;

	this->rowMotion = rowMotion;
	this->rowTurn = rowTurn;
	this->maxFrameMotion = maxFrameMotion;
	this->maxFrameTurn = maxFrameTurn;
	this->maxFrameEat = maxFrameEat;
	this->animationTime = animationTime;
	this->eatingTime = eatingTime;
	
	this->score = score;
}

//Getters
float Entity::getMaxVel() {	return maxVel; }
float Entity::getAccel() { return accel; }
float Entity::getResist() { return resist; }

int Entity::getRowMotion() { return rowMotion; }
int Entity::getRowTurn() { return rowTurn; }
int Entity::getMaxFrameMotion() { return maxFrameMotion; }
int Entity::getMaxFrameTurn() { return maxFrameTurn; }
int Entity::getMaxFrameEat() { return maxFrameEat; }

float Entity::getAnimationTime() { return animationTime; }
float Entity::getEatingTime() { return eatingTime; }