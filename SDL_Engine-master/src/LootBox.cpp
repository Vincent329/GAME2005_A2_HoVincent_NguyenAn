#include "LootBox.h"

LootBox::LootBox()
{
	TextureManager::Instance()->load("../Assets/textures/circle.png", "lootbox");

	const auto size = TextureManager::Instance()->getTextureSize("lootbox");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(100.0f, 100.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	setType(LOOTBOX);
}

LootBox::~LootBox()
= default;

void LootBox::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("lootbox", x, y, 0, 255, true);
}

void LootBox::update()
{
}

void LootBox::clean()
{
}

void LootBox::m_move()
{
	std::cout << "Calling Move" << std::endl;
	if (!m_isEnabled)
	{	
		
		// Make sure that the box doesn't move at all when you're going down
		getRigidBody()->velocity = glm::vec2(0.0f, 0.0f) * m_PPM;
		getRigidBody()->acceleration = glm::vec2(0.0f, 0.0F) * m_PPM;
	}
	else {
		// implement formulas here

		// if Static Friction is greater than mg sin (angle)
		// do not move
		// Fs = (mS)*(mass * gravityFactor * cos(angle)
		// Fparallel = mgsin(theta), the force going down

		// if  Fs > Fparallel
		// object does not move

		// else if Fparallel >= Fs

		// Apply the isolated acceleration so that 
		// F parallel = mass*gravity*sin(angle) - (kinetic coefficient)*mass*gravity*cos(angle)
		// MAKE SURE THAT KINETIC FRICTION DOES NOT SURPASS STATIC FRICTION
			// SET THAT IN THE PLAYSCENE.CPP
		// isolate mass to get the acceleration
		// Acceleration = gravity * sin(angle) - (kinetic co
	}
}

float LootBox::getGravityFactor()
{
	return 0.0f;
}

void LootBox::setGravityFactor(float gFactor)
{
	m_gravityFactor = gFactor;
}

float LootBox::getPixelsPerMeter()
{
	return m_PPM;
}

void LootBox::setPixelsPerMeter(float ppm)
{
	m_PPM = ppm;
}

void LootBox::setIsThrown(bool check)
{
	m_isEnabled = check;
}

float LootBox::getAngle()
{
	return m_Angle;
}

void LootBox::setAngle(float angle)
{
	m_Angle = angle;
}

//float LootBox::getVelocity()
//{
//	return m_velocity;
//}
//
//void LootBox::setVelocity(float velocity)
//{
//	m_velocity = velocity;
//}
//
//float LootBox::getVelocityX()
//{
//	return m_velocityX;
//}
//
//void LootBox::setVelocityX(float velocityX)
//{
//	m_velocityX = velocityX;
//}
//
//float LootBox::getVelocityY()
//{
//	return m_velocityY;
//}
//
//void LootBox::setVelocityY(float velocityY)
//{
//	m_velocityY = velocityY;
//}

glm::vec2 LootBox::getInitialPosition()
{
	return initialPosition;
}

void LootBox::setInitialPosition(glm::vec2 initPos)
{
	initialPosition = initPos;
}

void LootBox::resetElapsedTime()
{
	elapsedTime = 1.0f / 30.0f;
}
