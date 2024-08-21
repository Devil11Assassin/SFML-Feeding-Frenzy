#include "EnemyExtra.h"
#include "Enemy.h"

Shark::Shark(float spawnTimeSecs, float animationTimeMillisecs, float eatingTimeMillisecs)
{
	texture.loadFromFile("resources/sprites/barracuda.png");
	sprite.setTexture(texture);
	sprite.setScale(2, 2);
	sprite.setPosition(10000, 5000);
	sprite.setTextureRect(IntRect(0, 0, 271, 121));

	spawnClockSecs.restart().asSeconds();
	animationClockMillisecs.restart().asMilliseconds();
	eatingClockMillisecs.restart().asMilliseconds();

	this->spawnTimeSecs = spawnTimeSecs;
	this->animationTimeMillisecs = animationTimeMillisecs;
	this->eatingTimeMillisecs = eatingTimeMillisecs;

	spawnSide = 1;
	currentFrame = 0;
	isEating = false;
}

void Shark::spawn()
{
	if (static_cast<float>(spawnClockSecs.getElapsedTime().asSeconds()) >= spawnTimeSecs)
	{
		spawnSide = rand() % 2;
		if (spawnSide == 0)
		{
			sprite.setTextureRect(IntRect(0, 121, 271, 121));
			sprite.setPosition(WINDOW_WIDTH, rand() % (WINDOW_HEIGHT - 121 - 135) + 135);
		}
		else
		{
			sprite.setTextureRect(IntRect(271, 121, -271, 121));
			sprite.setPosition(-271, rand() % (WINDOW_HEIGHT - 121 - 135) + 135);
		}
		spawnClockSecs.restart().asSeconds();
	}
}

void Shark::movement(float dt, float movementMultiplier)
{
	if (spawnSide == 0)
		sprite.move(-10 * dt * movementMultiplier, 0);
	else
		sprite.move(10 * dt * movementMultiplier, 0);
}

void Shark::animation()
{
	if (spawnSide == 0)
	{
		if (!isEating)
		{
			if (static_cast<float>(animationClockMillisecs.getElapsedTime().asMilliseconds()) >= animationTimeMillisecs)
			{
				sprite.setTextureRect(IntRect((++currentFrame) * 271, 121, 271, 121));
				animationClockMillisecs.restart().asMilliseconds();
				if (currentFrame == 13)
					currentFrame = 0;
			}
		}
		else
		{
			if (static_cast<float>(eatingClockMillisecs.getElapsedTime().asMilliseconds()) >= eatingTimeMillisecs)
			{
				sprite.setTextureRect(IntRect((++currentFrame) * 271, 0, 271, 121));
				eatingClockMillisecs.restart().asMilliseconds();
				if (currentFrame == 5)
				{
					currentFrame = 0;
					isEating = false;
				}
			}
		}
	}
	else if (spawnSide == 1)
	{
		if (!isEating)
		{
			if (static_cast<float>(animationClockMillisecs.getElapsedTime().asMilliseconds()) >= animationTimeMillisecs)
			{
				sprite.setTextureRect(IntRect((++currentFrame) * 271, 121, -271, 121));
				animationClockMillisecs.restart().asMilliseconds();
				if (currentFrame == 13)
					currentFrame = 0;
			}
		}
		else
		{
			if (static_cast<float>(eatingClockMillisecs.getElapsedTime().asMilliseconds()) >= eatingTimeMillisecs)
			{
				sprite.setTextureRect(IntRect((++currentFrame) * 271, 0, -271, 121));
				eatingClockMillisecs.restart().asMilliseconds();
				if (currentFrame == 5)
				{
					currentFrame = 0;
					isEating = false;
				}
			}
		}
	}
}

void Shark::collision(Sprite& playerSprite, bool& playerDeathStatus)
{
	if (sprite.getGlobalBounds().intersects(playerSprite.getGlobalBounds()))
	{
		playerDeathStatus = true;
		this->isEating = true;
		this->currentFrame = 0;
	}

	for (int index = small; index <= large; index++)
	{
		FishKey key = Enemy::getEnum(index);

		for (auto& fish : Enemy::enemies[key]) {
			if (sprite.getGlobalBounds().intersects(fish.sprite.getGlobalBounds()))
			{
				this->isEating = true;
				this->currentFrame = 0;

				fish.kill(key);
				break;
			}
		}
	}
}

void Shark::draw(RenderWindow& window) 
{
	window.draw(sprite);
}


Oyster::Oyster(float openTimeSecs, float stayOpenTimeSecs, float animationTimeMillisecs)
{
	texture.loadFromFile("resources/sprites/oyster.png");
	sprite.setTexture(texture);
	sprite.setTextureRect(IntRect(0, 0, 101, 101));
	sprite.setPosition(860, 880);
	sprite.setScale(2, 2);

	pearlTexture.loadFromFile("resources/sprites/pearl.png");
	pearlSprite.setTexture(pearlTexture);
	pearlSprite.setTextureRect(IntRect(0, 51, 52, 51));
	pearlSprite.setPosition(910, 930);
	pearlSprite.setScale(2, 2);

	openClockSecs.restart().asSeconds();
	stayOpenClockSecs.restart().asSeconds();
	animationClockMillisecs.restart().asMilliseconds();

	this->openTimeSecs = openTimeSecs;
	this->stayOpenTimeSecs = stayOpenTimeSecs;
	this->animationTimeMillisecs = animationTimeMillisecs;

	currentFrame = 0;
	isClosing = false;
	isPearlVisible = false;

	pearlEatBuffer.loadFromFile("resources/sounds/oyster_pearl_eat.ogg");
	pearlEat.setBuffer(pearlEatBuffer);

	killPlayerBuffer.loadFromFile("resources/sounds/oyster_kill_player.ogg");
	killPlayer.setBuffer(killPlayerBuffer);
}

void Oyster::update() {
	if (!isPearlVisible && openClockSecs.getElapsedTime().asSeconds() >= openTimeSecs)
	{
		stayOpenClockSecs.restart().asSeconds();
		animationClockMillisecs.restart().asMilliseconds();
		isPearlVisible = true;
	}
	else if (openClockSecs.getElapsedTime().asSeconds() > openTimeSecs
		&& stayOpenClockSecs.getElapsedTime().asSeconds() >= stayOpenTimeSecs)
	{
		openClockSecs.restart().asSeconds();
		animationClockMillisecs.restart().asMilliseconds();
		isClosing = true;
	}
}

void Oyster::animation() {
	if (openClockSecs.getElapsedTime().asSeconds() >= openTimeSecs)
	{
		if (currentFrame < 4)
		{
			if (static_cast<float>(animationClockMillisecs.getElapsedTime().asMilliseconds()) >= animationTimeMillisecs)
			{
				animationClockMillisecs.restart().asMilliseconds();
				sprite.setTextureRect(IntRect((++currentFrame) * 101, 0, 101, 101));
			}
		}
	}
	else if (openClockSecs.getElapsedTime().asSeconds() < openTimeSecs &&
		stayOpenClockSecs.getElapsedTime().asSeconds() >= stayOpenTimeSecs)
	{
		if (currentFrame > 0 && isClosing == true)
		{
			if (static_cast<float>(animationClockMillisecs.getElapsedTime().asMilliseconds()) >= animationTimeMillisecs)
				sprite.setTextureRect(IntRect((--currentFrame) * 101, 0, 101, 101));
		}
		else if (currentFrame == 0)
		{
			isClosing = false;
			isPearlVisible = false;
			pearlSprite.setScale(2, 2);
		}
	}
}

void Oyster::collision(Sprite& playerSprite, bool& isPlayerEating, bool& deathStatus, int& hudLeaderboardScore, Sprite& chomp)
{
	if (currentFrame >= 3 && playerSprite.getGlobalBounds().intersects(pearlSprite.getGlobalBounds()))
	{
		hudLeaderboardScore += 100;
		pearlSprite.setScale(0, 0);
		
		chomp.setTextureRect(IntRect(228, 61, 114, 93));
		chomp.setPosition(pearlSprite.getPosition().x, pearlSprite.getPosition().y);
		isPlayerEating = true;
		pearlEat.play();
	}
	if (isClosing == true && currentFrame == 1 &&
		playerSprite.getGlobalBounds().intersects(sprite.getGlobalBounds()))
	{
		deathStatus = true;
		killPlayer.play();
	}
}

void Oyster::draw(RenderWindow& window) 
{
	window.draw(sprite);

	if (isPearlVisible && currentFrame >= 3)
		window.draw(pearlSprite);
}