#include "Player.h"
#include "Enemy.h"
#include "HUD.h"

//Constructor
Player::Player(float maxVel, float accel, float resist, int rowMotion, int rowTurn, int rowIdle, int maxFrameMotion,
	int maxFrameTurn, int maxFrameEat, int maxFrameIdle, float score, float animationTime, float eatingTime, float idleTime)
	: Entity(maxVel, accel, resist, rowMotion, rowTurn, maxFrameMotion,
		maxFrameTurn, maxFrameEat, score, animationTime, eatingTime)
{
	velocity.x = velocity.y = 0;
	direction.x = direction.y = 0;

	currentFrame = 0;
	animationClock.restart();

	isEating = false;
	ateInLastSeconds = false;
	eatingCurrentFrame = 0;
	eatingClock.restart();
	fishEatenCount = 0;

	this->rowIdle = rowIdle;
	this->maxFrameIdle = maxFrameIdle;
	currentFrameIdle = 0;
	idleClock.restart();
	this->idleTime = idleTime;
	
	deathStatus = false;
	playerDeathSoundPlayed = false;

	SoundBuffer eatingSmallBuffer;
	SoundBuffer eatingMediumBuffer;
	SoundBuffer eatingLargeBuffer;
	eatingSmallBuffer.loadFromFile("resources/sounds/bite1.ogg");
	eatingMediumBuffer.loadFromFile("resources/sounds/bite2.ogg");
	eatingLargeBuffer.loadFromFile("resources/sounds/bite3.ogg");

	eatingBuffer[small] = eatingSmallBuffer;
	eatingBuffer[medium] = eatingMediumBuffer;
	eatingBuffer[large] = eatingLargeBuffer;

	Sound eatingSmallSound;
	Sound eatingMediumSound;
	Sound eatingLargeSound;
	eatingSmallSound.setBuffer(eatingBuffer[small]);
	eatingMediumSound.setBuffer(eatingBuffer[medium]);
	eatingLargeSound.setBuffer(eatingBuffer[large]);

	eatingSound[small] = eatingSmallSound;
	eatingSound[medium] = eatingMediumSound;
	eatingSound[large] = eatingLargeSound;
	
	gulpBuffer.loadFromFile("resources/sounds/gulp.ogg");
	gulpSound.setBuffer(gulpBuffer);

	feedingFrenzyBuffer.loadFromFile("resources/sounds/feeding_frenzy.ogg");
	doubleFrenzyBuffer.loadFromFile("resources/sounds/double_frenzy.ogg");
	feedingFrenzySound.setBuffer(feedingFrenzyBuffer);
	doubleFrenzySound.setBuffer(doubleFrenzyBuffer);

	growthBuffer.loadFromFile("resources/sounds/player_grow.ogg");
	growthSound.setBuffer(growthBuffer);
	isGrowing = false;

	gameoverBuffer.loadFromFile("resources/sounds/game_over.ogg");
	gameoverSound.setBuffer(gameoverBuffer);
}

//Methods
void Player::enemyCollision(int& leaderboardScore, int& frenzyMultiplier, int& frenzyRow,
	int& frenzyColumn, Clock& frenzyGainClockMillisecs, Sprite& chomp)
{
	for (int index = small; index <= large; index++)
	{
		FishKey key = Enemy::getEnum(index);

		for (auto& enemy : Enemy::enemies[key])
		{
			if (sprite.getGlobalBounds().intersects(enemy.sprite.getGlobalBounds()) && !deathStatus)
			{
				int enemyScore = enemy.stat->score;
				if (score >= enemyScore)
				{
					//ENEMY DESPAWN
					enemy.kill(key);
					int chompCol = rand() % 3;

					if (key == small)
					{
						score += 0.3;
						leaderboardScore += (10 * frenzyMultiplier);

						chomp.setTextureRect(IntRect(74 * chompCol, 0, 74, 61));
						chomp.setPosition(sprite.getPosition().x, sprite.getPosition().y);
					}
					else if (key == medium)
					{
						score += 0.6;
						leaderboardScore += (20 * frenzyMultiplier);

						chomp.setTextureRect(IntRect(114 * chompCol, 61, 114, 93));
						chomp.setPosition(sprite.getPosition().x, sprite.getPosition().y);
					}
					else
					{
						score += 0.9;
						leaderboardScore += (30 * frenzyMultiplier);

						chomp.setTextureRect(IntRect(171 * chompCol, 154, 171, 139));
						chomp.setPosition(sprite.getPosition().x, sprite.getPosition().y);
					}

					gulpSound.play();
					eatingSound[key].play();
					isEating = true;
					eatingCurrentFrame = 0;
					fishEatenCount++;

					//FRENZY ANIMATION
					frenzyGainClockMillisecs.restart();
					ateInLastSeconds = true;

					if (frenzyColumn == 0)
					{
						frenzyColumn++;
						frenzyRow = 0;
					}
					else if (frenzyColumn == 1) {
						frenzyRow++;
						if (frenzyRow == 6)
							feedingFrenzySound.play();
					}
					else if (frenzyColumn == 2) {
						frenzyRow++;
						if (frenzyRow == 6)
							doubleFrenzySound.play();
					}

					if (frenzyRow > 6)
					{
						frenzyColumn++;
						frenzyRow = 0;
						
						if (frenzyColumn > 2)
						{
							frenzyColumn = 2;
							frenzyRow = 6;
						}
					}

					break;
				}
				else if (score < enemyScore)
				{
					deathStatus = true;
					enemy.isEating = true;
					enemy.eatingCurrentFrame = 0;
					gulpSound.play();

					if (score < Enemy::getStats()[medium].score)
						eatingSound[small].play();
					else if (score < Enemy::getStats()[large].score)
						eatingSound[medium].play();
					else
						eatingSound[large].play();

					score = 0;
					return;
				}
			}
		}
	}
}

void Player::movement(float dt, float movementMultiplier)
{
	//Player's movement
	//UP
	if (Keyboard::isKeyPressed(Keyboard::Key::W) || Keyboard::isKeyPressed(Keyboard::Key::Up))
	{
		direction.y = -1.f;

		if (velocity.y > -maxVel)
			velocity.y += accel * direction.y * dt * movementMultiplier;
	}
	//DOWN
	if (Keyboard::isKeyPressed(Keyboard::Key::S) || Keyboard::isKeyPressed(Keyboard::Key::Down))
	{
		direction.y = 1.f;

		if (velocity.y < maxVel)
			velocity.y += accel * direction.y * dt * movementMultiplier;
	}
	//LEFT
	if (Keyboard::isKeyPressed(Keyboard::Key::A) || Keyboard::isKeyPressed(Keyboard::Key::Left))
	{
		direction.x = -1.f;

		if (velocity.x > -maxVel)
			velocity.x += accel * direction.x * dt * movementMultiplier;
	}
	//RIGHT
	if (Keyboard::isKeyPressed(Keyboard::Key::D) || Keyboard::isKeyPressed(Keyboard::Key::Right))
	{
		direction.x = 1.f;

		if (velocity.x < maxVel)
			velocity.x += accel * direction.x * dt * movementMultiplier;
	}
	//
	//Player's resistance
	//UP
	if (velocity.y > 0.f)
	{
		velocity.y -= resist * dt * movementMultiplier;

		if (velocity.y < 0.f)
			velocity.y = 0;
	}
	//DOWN
	else if (velocity.y < 0.f)
	{
		velocity.y += resist * dt * movementMultiplier;

		if (velocity.y > 0.f)
			velocity.y = 0;
	}
	//LEFT
	else if (velocity.x < 0.f)
	{
		velocity.x += resist * dt * movementMultiplier;

		if (velocity.x > 0.f)
			velocity.x = 0;
	}
	//RIGHT
	if (velocity.x > 0.f)
	{
		velocity.x -= resist * dt * movementMultiplier;

		if (velocity.x < 0.f)
			velocity.x = 0;
	}
}

void Player::move(float dt, float movementMultiplier) {
	sprite.move(velocity.x * dt * movementMultiplier, velocity.y * dt * movementMultiplier);
}

void Player::screenCollision()
{
	//Top
	if (sprite.getPosition().y < 135)
		sprite.setPosition(sprite.getPosition().x, 135);
	//Down
	if (sprite.getPosition().y + sprite.getGlobalBounds().height > WINDOW_HEIGHT)
		sprite.setPosition(sprite.getPosition().x, WINDOW_HEIGHT - sprite.getGlobalBounds().height);
	//Left
	if (sprite.getPosition().x < 0)
		sprite.setPosition(0, sprite.getPosition().y);
	//Right
	if (sprite.getPosition().x + sprite.getGlobalBounds().width > WINDOW_WIDTH)
		sprite.setPosition(WINDOW_WIDTH - sprite.getGlobalBounds().width, sprite.getPosition().y);
}

void Player::animation()
{
	if (!isEating)
	{
		if (velocity.x != 0 || velocity.y != 0)
		{
			if (static_cast<float>(animationClock.getElapsedTime().asMilliseconds()) >= animationTime)
			{
				if (velocity.x > 1)
				{
					sprite.setTextureRect(IntRect(currentFrame * sprite.getLocalBounds().width + sprite.getLocalBounds().width,
						rowMotion * sprite.getLocalBounds().height, -sprite.getLocalBounds().width, sprite.getLocalBounds().height));

					currentFrame++;
					animationClock.restart();
					currentFrame %= maxFrameMotion;
				}
				else if (velocity.x < -1)
				{
					sprite.setTextureRect(IntRect(currentFrame * sprite.getLocalBounds().width,
						rowMotion * sprite.getLocalBounds().height, sprite.getLocalBounds().width, sprite.getLocalBounds().height));

					currentFrame++;
					animationClock.restart();
					currentFrame %= maxFrameMotion;

				}
				else if (velocity.x <= 0.25 && velocity.x > 0)
				{
					sprite.setTextureRect(IntRect(sprite.getLocalBounds().width,
						rowMotion * sprite.getLocalBounds().height, -sprite.getLocalBounds().width, sprite.getLocalBounds().height));

					currentFrame = 0;
				}
				else if (velocity.x >= -0.25 && velocity.x < 0)
				{
					sprite.setTextureRect(IntRect(sprite.getLocalBounds().width,
						rowMotion * sprite.getLocalBounds().height, sprite.getLocalBounds().width, sprite.getLocalBounds().height));

					currentFrame = 0;
				}
			}
		}
		else
		{
			if (static_cast<float>(idleClock.getElapsedTime().asMilliseconds()) >= idleTime)//idleTime = 120
			{
				if (direction.x >= 0)
				{
					sprite.setTextureRect(IntRect(currentFrameIdle * sprite.getLocalBounds().width + sprite.getLocalBounds().width,
						sprite.getLocalBounds().height, -sprite.getLocalBounds().width, sprite.getLocalBounds().height));
				}
				else if (direction.x < 0)
				{
					sprite.setTextureRect(IntRect(currentFrameIdle * sprite.getLocalBounds().width,
						sprite.getLocalBounds().height, sprite.getLocalBounds().width, sprite.getLocalBounds().height));
				}
				currentFrameIdle++;
				idleClock.restart();
				currentFrameIdle %= maxFrameIdle;
			}
		}
	}
	else
	{
		if (static_cast<float>(eatingClock.getElapsedTime().asMilliseconds()) >= eatingTime)// eatingTime = 80
		{
			if (direction.x == 1)
			{
				sprite.setTextureRect(IntRect(eatingCurrentFrame * sprite.getLocalBounds().width + sprite.getLocalBounds().width,
					0, -sprite.getLocalBounds().width, sprite.getLocalBounds().height));
			}
			else if (direction.x == -1)
			{
				sprite.setTextureRect(IntRect(eatingCurrentFrame * sprite.getLocalBounds().width,
					0, sprite.getLocalBounds().width, sprite.getLocalBounds().height));
			}
			eatingCurrentFrame++;
			eatingClock.restart();
			eatingCurrentFrame %= maxFrameEat;
			if (eatingCurrentFrame == 0)
				isEating = false;
		}
	}
}

void Player::grow(bool& isFishMenuLocked_0, bool& isFishMenuLocked_1)
{
	if (score >= Enemy::getStats()[medium].score && score < Enemy::getStats()[large].score)
	{
		if (!isGrowing)
		{
			isGrowing = true;

			sprite.setScale(1.1, 1.1);
			growthSound.play();
			isFishMenuLocked_0 = false;

			Enemy::setSpawnTime(medium, Enemy::getSpawnTime(medium) - 3000.f);
			Enemy::setSpawnTime(large, Enemy::getSpawnTime(large) - 3000.f);
			Enemy::setSpawnQuantity(medium, Enemy::getSpawnQuantity(medium) + 4);
			Enemy::setSpawnQuantity(large, Enemy::getSpawnQuantity(large) + 1);
		}
	}
	else if (score >= Enemy::getStats()[large].score)
	{
		if (isGrowing) 
		{
			isGrowing = false;

			sprite.setScale(1.6, 1.6);
			growthSound.play();
			isFishMenuLocked_1 = false;

			Enemy::setSpawnTime(large, Enemy::getSpawnTime(large) - 4000.f);
			Enemy::setSpawnQuantity(large, Enemy::getSpawnQuantity(large) + 1);
		}
	}
}

void Player::draw(RenderWindow& window) 
{
	window.draw(sprite);
}

void Player::clear() {
	eatingBuffer.clear();
	eatingSound.clear();
}