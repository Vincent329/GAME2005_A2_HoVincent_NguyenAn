#include "LootBox.h"

LootBox::LootBox()
{
	TextureManager::Instance()->load("../Assets/textures/megaman-idle-0.png", "lootbox");

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
	std::cout << "Enabled working? " << m_isEnabled << std::endl;
	if (!m_isEnabled)
	{	
		
		// Make sure that the box doesn't move at all when you're going down
		getRigidBody()->velocity = glm::vec2(0.0f, 0.0f) * m_PPM;
		getRigidBody()->acceleration = glm::vec2(0.0f, 0.0F) * m_PPM;

		std::cout << "velocity X: " << getRigidBody()->velocity.x << std::endl;
		std::cout << "velocity Y: " << getRigidBody()->velocity.y << std::endl;

		std::cout << "Acceleration X: " << getRigidBody()->acceleration.x << std::endl;
		std::cout << "Acceleration Y: " << getRigidBody()->acceleration.y << std::endl;
	}
	else {
		// implement formulas here
		
		// the ramp will have a frictionless surface

		// So the formula needed here for acceleration will be
		// a = g * sin(theta)
		acceleration = m_gravityFactor * sin(m_Angle);
		m_acceleration = glm::vec2(acceleration * cos(m_Angle), acceleration * sin(m_Angle));
		getRigidBody()->acceleration = m_acceleration;

		std::cout << "Acceleration X: " << getRigidBody()->acceleration.x << std::endl;
		std::cout << "Acceleration Y: " << getRigidBody()->acceleration.y << std::endl;

		// velocity += acceleration * deltaTime * m_PPM
		getRigidBody()->velocity += getRigidBody()->acceleration * deltaTime * m_PPM;
		
		// Pf += velocity * dt (explanation)
		getTransform()->position += getRigidBody()->velocity * deltaTime;

		// ---------- flat surface ------------
		// if the ball reached a certain height
		// could trigger on bool
		// velocity will change from going down the ramp along a flat surface with friction
		// accelaration wilw go the other way because the only net force will be the force of friction
		// acceleration = - (mk) * m_gravityFactor;
		// velocity += acceleration * deltaTime * m_PPM

		// Pf += velocity * dt (explanation)
		// getTransform()->position += getRigidbody()->velocity * deltaTime

		
	}
}

float LootBox::getGravityFactor()
{
	return m_gravityFactor;
}

void LootBox::setGravityFactor(float gFactor)
{
	m_gravityFactor = gFactor;
	gravityVector = glm::vec2(0.0f, m_gravityFactor);
}

float LootBox::getMass()
{
	return m_Mass;
}

void LootBox::setMass(float mass)
{
	m_Mass = mass;
}

float LootBox::getPixelsPerMeter()
{
	return m_PPM;
}

void LootBox::setPixelsPerMeter(float ppm)
{
	m_PPM = ppm;
}

void LootBox::setIsEnabled(bool check)
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

glm::vec2 LootBox::getVelocity()
{
	return m_velocity;
}

void LootBox::setVelocity(glm::vec2 velocity)
{
	m_velocity = velocity;
}


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
