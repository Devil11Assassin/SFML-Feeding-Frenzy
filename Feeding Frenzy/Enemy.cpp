#include "Enemy.h"

const float PI = 3.1415926f;

//Data Structures
std::unordered_map<FishKey, Entity> Enemy::stats;
std::unordered_map<FishKey, Sprite> Enemy::sprites;

std::unordered_map<FishKey, Clock> Enemy::directionRandomizerClocks;
std::unordered_map<FishKey, float> Enemy::directionRandomizerTimes;

std::unordered_map<FishKey, Clock> Enemy::spawnClocks;
std::unordered_map<FishKey, float> Enemy::spawnTimes;
std::unordered_map<FishKey, int> Enemy::spawnQuantities;

std::unordered_map<FishKey, std::vector<Enemy>> Enemy::enemies;

//Constructors
Enemy::Enemy(FishKey key)
{
	stat = &stats[key];
	sprite = sprites[key];

	velocity.x = velocity.y = 0;
	direction.x = direction.y = 0;

	currentFrame = 0;
	animationClock.restart();

	isEating = false;
	eatingCurrentFrame = 0;
	eatingClock.restart();

	directionRandomizer = rand() % 360;

	if ((rand() % 1000) < 500)
		sprite.setPosition(0 - sprite.getGlobalBounds().width, rand() % (WINDOW_HEIGHT - 135) + 135);
	else
		sprite.setPosition(WINDOW_WIDTH, rand() % (WINDOW_HEIGHT - 135) + 135);

	enemies[key].push_back(*this);
}

//Getters
std::unordered_map<FishKey, Entity>& Enemy::getStats() 
{
	return stats;
}
std::unordered_map<FishKey, Sprite>& Enemy::getSprites()
{
	return sprites;
}

float Enemy::getSpawnTime(FishKey key)
{
	return spawnTimes[key];
}
int Enemy::getSpawnQuantity(FishKey key)
{
	return spawnQuantities[key];
}

//Setters
void Enemy::setStats(FishKey key, Sprite sprite, float maxVel, float accel, float resist, int rowMotion,
	int rowTurn, int maxFrameMotion, int maxFrameTurn, int maxFrameEat, float score)
{
	Entity entityStats(maxVel, accel, resist, rowMotion, rowTurn,
		maxFrameMotion, maxFrameTurn, maxFrameEat, score);

	stats[key] = entityStats;
	sprites[key] = sprite;
}

void Enemy::setStats(FishKey key, Sprite sprite, float maxVel, float accel, float resist,
	int rowMotion, int rowTurn, int maxFrameMotion, int maxFrameTurn, int maxFrameEat,
	float score, float animationTime, float eatingTime)
{
	Entity entityStats(maxVel, accel, resist, rowMotion, rowTurn,
		maxFrameMotion, maxFrameTurn, maxFrameEat, score,
		animationTime, eatingTime);

	stats[key] = entityStats;
	sprites[key] = sprite;
}

void Enemy::setSpriteTexture(FishKey key, Texture texture) 
{
	sprites[key].setTexture(texture);
}

void Enemy::setSprite(FishKey key, Sprite sprite) 
{
	sprites[key] = sprite;
}

void Enemy::modifyStats(FishKey key, float maxVel, float accel, float resist, int rowMotion,
	int rowTurn, int maxFrameMotion, int maxFrameTurn, int maxFrameEat, float score) 
{
	Entity newStats(maxVel, accel, resist, rowMotion, rowTurn, maxFrameMotion,
		maxFrameTurn, maxFrameEat, score);

	stats[key] = newStats;
}

void Enemy::setDirectionRandomizerTime(FishKey key, float time) {
	directionRandomizerTimes[key] = time;
}

void Enemy::setSpawnTime(FishKey key, float time) {
	spawnTimes[key] = time;
}

void Enemy::setSpawnQuantity(FishKey key, int quantity) {
	spawnQuantities[key] = quantity;
}

//Helper Methods
FishKey Enemy::getEnum(int index) {
	return static_cast<FishKey>(index);
}

float Enemy::calcSin(int degree) {
	return static_cast<float>(sin(degree * (PI / 180.f)));
}

float Enemy::calcCos(int degree) {
	return static_cast<float>(cos(degree * (PI / 180.f)));
}

void Enemy::clear() 
{
	stats.clear();
	sprites.clear();
	
	directionRandomizerClocks.clear();
	directionRandomizerTimes.clear();

	spawnClocks.clear();
	spawnTimes.clear();
	spawnQuantities.clear();

	enemies.clear();
}

//Methods
void Enemy::updateEnemies() {
	spawnEnemies();
	directionRandomize();
	//movement(dt, movementMultiplier);
	//behavior(playerSprite, playerScore, dt, movementMultiplier);
	//moveEnemies(dt, movementMultiplier)
	animation();
	enemyEnemyCollision();
}

void Enemy::spawnEnemies() 
{
	for (int index = small; index <= large; index++)
	{
		FishKey key = getEnum(index);

		if (enemies[key].size() < spawnQuantities[key])
		{
			if (static_cast<int>(spawnClocks[key].getElapsedTime().asMilliseconds()) >= spawnTimes[key])
			{
				Enemy enemy(key);
				spawnClocks[key].restart();
			}
		}
	}
}

void Enemy::directionRandomize() 
{
	for (int index = small; index <= FishKey::large; index++)
	{
		FishKey key = getEnum(index);

		if (static_cast<float>(directionRandomizerClocks[key].getElapsedTime().asMilliseconds()) >= directionRandomizerTimes[key])
		{
			for (auto& enemy : enemies[key])
				enemy.directionRandomizer = rand() % 360;
			
			directionRandomizerClocks[key].restart();
		}
	}
}

void Enemy::movement(float dt, float movementMultiplier)
{
	for (int index = small; index <= large; index++) 
	{
		FishKey key = getEnum(index);

		for (auto& enemy : enemies[key]) 
		{
			//Preventing enemy fish from leaving the screen on their own
			//South-West Border
			if (enemy.sprite.getPosition().y > WINDOW_HEIGHT - 100 && enemy.sprite.getPosition().x < 50)
				enemy.directionRandomizer = 45;
			//South-East Border
			else if (enemy.sprite.getPosition().y > WINDOW_HEIGHT - 100 && enemy.sprite.getPosition().x > WINDOW_WIDTH - 50)
				enemy.directionRandomizer = 135;
			//North-East Border
			else if (enemy.sprite.getPosition().y < 160 && enemy.sprite.getPosition().x > WINDOW_WIDTH - 50)
				enemy.directionRandomizer = 225;
			//North-West Border
			else if (enemy.sprite.getPosition().y < 160 && enemy.sprite.getPosition().x < 50)
				enemy.directionRandomizer = 315;
			//Northern Border
			else if (enemy.sprite.getPosition().y < 160)
				enemy.directionRandomizer = 270;
			//Southern Border
			else if (enemy.sprite.getPosition().y > WINDOW_HEIGHT - enemy.sprite.getGlobalBounds().height)
				enemy.directionRandomizer = 90;
			//Eastern Border
			else if (enemy.sprite.getPosition().x > WINDOW_WIDTH - enemy.sprite.getGlobalBounds().width)
				enemy.directionRandomizer = 180;
			//Western Border
			else if (enemy.sprite.getPosition().x < 50)
				enemy.directionRandomizer = 0;
			
			float sinDirectionRandomizer = static_cast<float>(sin(enemy.directionRandomizer * (PI / 180.f)));
			float cosDirectionRandomizer = static_cast<float>(cos(enemy.directionRandomizer * (PI / 180.f)));

			//Enemy's movement
			// THIS LITTLE NIGGER IS CAUSING THE BUG THAT DESPAWNS ALL ENEMIES
			if (abs(enemy.velocity.y) < enemy.stat->getMaxVel())
				enemy.velocity.y -= enemy.stat->getAccel() * dt * movementMultiplier * sinDirectionRandomizer;
			if (abs(enemy.velocity.x) < enemy.stat->getMaxVel())
				enemy.velocity.x += enemy.stat->getAccel() * dt * movementMultiplier * cosDirectionRandomizer;

			//Enemy's direction for animations
			//Y-Axis
			if (enemy.velocity.y > 0)
				enemy.direction.y = -1.f;
			else if (enemy.velocity.y < 0)
				enemy.direction.y = 1.f;
			//X-Axis
			if (enemy.velocity.x > 0)
				enemy.direction.x = 1.f;
			else if (enemy.velocity.x < 0)
				enemy.direction.x = -1.f;

			//Enemy's resistance
			//NORTH
			if (enemy.velocity.y > 0.f)
			{
				enemy.velocity.y -= enemy.stat->getResist() * dt * movementMultiplier;

				if (enemy.velocity.y < 0.f)
					enemy.velocity.y = 0;
			}
			//SOUTH
			else if (enemy.velocity.y < 0.f)
			{
				enemy.velocity.y += enemy.stat->getResist() * dt * movementMultiplier;

				if (enemy.velocity.y > 0.f)
					enemy.velocity.y = 0;
			}
			//WEST
			if (enemy.velocity.x < 0.f)
			{
				enemy.velocity.x += enemy.stat->getResist() * dt * movementMultiplier;

				if (enemy.velocity.x > 0.f)
					enemy.velocity.x = 0;
			}
			//EAST
			else if (enemy.velocity.x > 0.f)
			{
				enemy.velocity.x -= enemy.stat->getResist() * dt * movementMultiplier;

				if (enemy.velocity.x < 0.f)
					enemy.velocity.x = 0;
			}
		}
	}
}

void Enemy::behavior(Sprite& playerSprite, float playerScore, float dt, float movementMultiplier)
{
	for (int index = small; index <= large; index++) 
	{
		FishKey key = getEnum(index);

		if (playerScore >= getStats()[key].score)
		{
			for (auto& enemy : enemies[key])
			{
				//Enemy's escape movement
				//UP
				if (playerSprite.getPosition().y - enemy.sprite.getPosition().y < 200 && playerSprite.getPosition().y - enemy.sprite.getPosition().y > 0 &&
					playerSprite.getPosition().x - enemy.sprite.getPosition().x < 250 && playerSprite.getPosition().x - enemy.sprite.getPosition().x > -250)
				{
					enemy.direction.y = -1.f;

					if (enemy.velocity.y > -enemy.stat->getMaxVel())
						enemy.velocity.y += enemy.stat->getAccel() * enemy.direction.y * dt * movementMultiplier * 1.4;
				}
				//DOWN
				if (playerSprite.getPosition().y - enemy.sprite.getPosition().y > -200 && playerSprite.getPosition().y - enemy.sprite.getPosition().y < 0 &&
					(playerSprite.getPosition().x - enemy.sprite.getPosition().x < 250 && playerSprite.getPosition().x - enemy.sprite.getPosition().x > -250))
				{
					enemy.direction.y = 1.f;

					if (enemy.velocity.y < enemy.stat->getMaxVel())
						enemy.velocity.y += enemy.stat->getAccel() * enemy.direction.y * dt * movementMultiplier * 1.4;
				}
				//LEFT
				if (playerSprite.getPosition().x - enemy.sprite.getPosition().x < 250 && playerSprite.getPosition().x - enemy.sprite.getPosition().x > 0 &&
					playerSprite.getPosition().y - enemy.sprite.getPosition().y < 200 && playerSprite.getPosition().y - enemy.sprite.getPosition().y > -200)
				{
					enemy.direction.x = -1.f;

					if (enemy.velocity.x > -enemy.stat->getMaxVel())
						enemy.velocity.x += enemy.stat->getAccel() * enemy.direction.x * dt * movementMultiplier * 1.4;
				}
				//RIGHT
				if (playerSprite.getPosition().x - enemy.sprite.getPosition().x > -250 && playerSprite.getPosition().x - enemy.sprite.getPosition().x < 0 &&
					playerSprite.getPosition().y - enemy.sprite.getPosition().y < 200 && playerSprite.getPosition().y - enemy.sprite.getPosition().y > -200)
				{
					enemy.direction.x = 1.f;

					if (enemy.velocity.x < enemy.stat->getMaxVel())
						enemy.velocity.x += enemy.stat->getAccel() * enemy.direction.x * dt * movementMultiplier * 1.4;
				}
			}
		}
	}
}

void Enemy::moveEnemies(float dt, float movementMultiplier) 
{
	for (int index = small; index <= large; index++)
	{
		FishKey key = getEnum(index);

		for (auto& enemy : enemies[key])
			enemy.sprite.move(enemy.velocity.x * dt * movementMultiplier, enemy.velocity.y * dt * movementMultiplier);
	}
}

void Enemy::animation()
{
	for (int index = small; index <= large; index++)
	{
		FishKey key = getEnum(index);

		for (auto& enemy : enemies[key])
		{
			if (!enemy.isEating)
			{
				if (static_cast<float>(enemy.animationClock.getElapsedTime().asMilliseconds()) >= enemy.stat->getAnimationTime())
				{
					if (enemy.velocity.x > 1)
					{
						enemy.sprite.setTextureRect(IntRect(enemy.currentFrame * enemy.sprite.getLocalBounds().width + enemy.sprite.getLocalBounds().width,
							enemy.stat->getRowMotion() * enemy.sprite.getLocalBounds().height, -enemy.sprite.getLocalBounds().width, enemy.sprite.getLocalBounds().height));

						enemy.currentFrame++;
						enemy.animationClock.restart();
						enemy.currentFrame %= enemy.stat->getMaxFrameMotion();
					}
					else if (enemy.velocity.x < -1)
					{
						enemy.sprite.setTextureRect(IntRect(enemy.currentFrame * enemy.sprite.getLocalBounds().width, enemy.stat->getRowMotion() * enemy.sprite.getLocalBounds().height,
							enemy.sprite.getLocalBounds().width, enemy.sprite.getLocalBounds().height));

						enemy.currentFrame++;
						enemy.animationClock.restart();
						enemy.currentFrame %= enemy.stat->getMaxFrameMotion();
					}
					else if (enemy.velocity.x <= 0.25 && enemy.velocity.x > 0)
					{
						enemy.sprite.setTextureRect(IntRect(enemy.sprite.getLocalBounds().width, enemy.stat->getRowMotion() * enemy.sprite.getLocalBounds().height,
							-enemy.sprite.getLocalBounds().width, enemy.sprite.getLocalBounds().height));

						enemy.currentFrame = 0;
					}
					else if (enemy.velocity.x >= -0.25 && enemy.velocity.x < 0)
					{
						enemy.sprite.setTextureRect(IntRect(enemy.sprite.getLocalBounds().width, enemy.stat->getRowMotion() * enemy.sprite.getLocalBounds().height,
							enemy.sprite.getLocalBounds().width, enemy.sprite.getLocalBounds().height));

						enemy.currentFrame = 0;
					}
				}
			}
			else
			{
				if (static_cast<float>(enemy.eatingClock.getElapsedTime().asMilliseconds()) >= enemy.stat->getEatingTime())
				{
					if (enemy.direction.x == 1)
					{
						enemy.sprite.setTextureRect(IntRect(enemy.eatingCurrentFrame * enemy.sprite.getLocalBounds().width + enemy.sprite.getLocalBounds().width,
							0, -enemy.sprite.getLocalBounds().width, enemy.sprite.getLocalBounds().height));
					}
					else if (enemy.direction.x == -1)
					{
						enemy.sprite.setTextureRect(IntRect(enemy.eatingCurrentFrame * enemy.sprite.getLocalBounds().width,
							0, enemy.sprite.getLocalBounds().width, enemy.sprite.getLocalBounds().height));
					}
					enemy.eatingCurrentFrame++;
					enemy.eatingClock.restart();
					enemy.eatingCurrentFrame %= enemy.stat->getMaxFrameEat();
						if (enemy.eatingCurrentFrame == 0)
							enemy.isEating = false;
				}
			}
		}
	}
}

void Enemy::enemyEnemyCollision()
{
	for (auto& enemySmall : enemies[small]) {
		for (auto& enemyMedium : enemies[medium])
		{
			if (enemySmall.sprite.getGlobalBounds().intersects(enemyMedium.sprite.getGlobalBounds()))
			{
				enemySmall.kill(small);
				enemyMedium.isEating = true;
				enemyMedium.eatingCurrentFrame = 0;
				return;
			}
		}
	}
	for (auto& enemySmall : enemies[small]) {
		for (auto& enemyLarge : enemies[large]) {
			if (enemySmall.sprite.getGlobalBounds().intersects(enemyLarge.sprite.getGlobalBounds()))
			{
				enemySmall.kill(small);
				enemyLarge.isEating = true;
				enemyLarge.eatingCurrentFrame = 0;
				return;
			}
		}
	}
	for (auto& enemyMedium : enemies[medium]) {
		for (auto& enemyLarge : enemies[large]) {
			if (enemyMedium.sprite.getGlobalBounds().intersects(enemyLarge.sprite.getGlobalBounds()))
			{
				enemyMedium.kill(medium);
				enemyLarge.isEating = true;
				enemyLarge.eatingCurrentFrame = 0;
				return;
			}
		}
	}
}

void Enemy::kill(FishKey key) {
	enemies[key].erase(enemies[key].begin() + (this - enemies[key].data()));
}

void Enemy::drawEnemies(RenderWindow& window) {
	for (auto const& index : enemies[small])
		window.draw(index.sprite);
	for (auto const& index : enemies[medium])
		window.draw(index.sprite);
	for (auto const& index : enemies[large])
		window.draw(index.sprite);
}