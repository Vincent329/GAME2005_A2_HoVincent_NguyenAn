#include "Target.h"
#include "TextureManager.h"


Target::Target()
{
	TextureManager::Instance()->load("../Assets/textures/detonator.png","detonator");

	const auto size = TextureManager::Instance()->getTextureSize("detonator");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(100.0f, 100.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	setType(TARGET);
}

Target::~Target()
= default;

void Target::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("detonator", x, y, 0, 255, true);
}

void Target::update()
{
	m_checkBounds();
}

void Target::clean()
{
}

float Target::getGravityFactor()
{
	return m_gravityFactor;
}

void Target::setGravityFactor(float gFactor)
{
	m_gravityFactor = gFactor;
}

float Target::getPixelsPerMeter()
{
	return m_PPM;
}

void Target::setPixelsPerMeter(float ppm)
{
	m_PPM = ppm;
}

void Target::setIsGravityEnabled(bool check)
{
	m_isGravityEnabled = check;
}

void Target::setIsThrown(bool check)
{
	m_isThrown = check;
}

float Target::getAngle()
{
	return m_Angle;
}

void Target::setAngle(float angle)
{
	m_Angle = angle;
}

float Target::getVelocity()
{
	return m_velocity;
}

void Target::setVelocity(float velocity)
{
	m_velocity = velocity;
}

float Target::getVelocityX()
{
	return m_velocityX;
}

void Target::setVelocityX(float velocityX)
{
	m_velocityX = velocityX;
}

float Target::getVelocityY()
{
	return m_velocityY;
}

void Target::setVelocityY(float velocityY)
{
	m_velocityY = velocityY;
}

glm::vec2 Target::getInitialPosition()
{
	return initialPosition;
}

void Target::setInitialPosition(glm::vec2 initPos)
{
	initialPosition = initPos;
}

void Target::resetElapsedTime()
{
	elapsedTime = 1.0f / 60.0f;
}

void Target::m_move()
{
	// multiplying velocity and acceleration in the meters
	std::cout << "Calling Move" << std::endl;

	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f) * m_PPM;
	getRigidBody()->acceleration = glm::vec2(0.0f, m_gravityFactor) * m_PPM;

	if (!m_isGravityEnabled && m_isThrown)
	{
		m_velocityX = (m_velocity * m_PPM) * cos(glm::radians(m_Angle));
		m_velocityY = (m_velocity * m_PPM) * -sin(glm::radians(m_Angle));

		getRigidBody()->velocity = glm::vec2(m_velocityX, m_velocityY);

		getTransform()->position = getTransform()->position
			+ (getRigidBody()->velocity * elapsedTime);

		std::cout << "Velocity X: " << getRigidBody()->velocity.x << " Y:" << getRigidBody()->velocity.y << std::endl;
		std::cout << "Acceleration X: " << getRigidBody()->acceleration.x << " Y:" << getRigidBody()->acceleration.y << std::endl;
	}
	else if (m_isGravityEnabled && m_isThrown) {

		// new logic
		// velocity components
		// Pfx = Pix + Vix(cosΘ)*t
		// Pfy = Piy + Vix(sinΘ)*t + 1/2a*t^2

		// PPM calculations here so that we're not accidentally multiplying the velocity vector every frame
		m_velocityX = (m_velocity * m_PPM) * cos(glm::radians(m_Angle));
		m_velocityY = (m_velocity * m_PPM) * -sin(glm::radians(m_Angle)); // make this negative because SDL up vector needs to be negative to go up

		std::cout << "Velocity X: " << m_velocityX << std::endl;
		std::cout << "Velocity Y: " << m_velocityY << std::endl;

		getRigidBody()->velocity = glm::vec2(m_velocityX, m_velocityY);

		// Formula for position
		// Pf = Pi + v*t + 1/2a*t^2

		getTransform()->position = initialPosition
			+ (getRigidBody()->velocity * elapsedTime)
			+ ((0.5f * getRigidBody()->acceleration) * (elapsedTime * elapsedTime));

		//std::cout << "Velocity X: " << getRigidBody()->velocity.x << " Y:" << getRigidBody()->velocity.y << std::endl;
		//std::cout << "Acceleration X: " << getRigidBody()->acceleration.x << " Y:" << getRigidBody()->acceleration.y << std::endl;

		elapsedTime += deltaTime;
		std::cout << elapsedTime << std::endl;
	}
}


void Target::m_checkBounds()
{
}

void Target::m_reset()
{
}

