#include "LootBox.h"

LootBox::LootBox()
{
	TextureManager::Instance()->load("../Assets/textures/crate.png", "lootbox");

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
	if (getTransform()->position.y >= 400.0f - (getHeight() / 2.0))
	{
		TextureManager::Instance()->draw("lootbox", x, y, 0, 255, true);
	}
	else
	{
		TextureManager::Instance()->draw("lootbox", x, y, glm::degrees(m_Angle), 255, true);
	}
}

void LootBox::update()
{
}

void LootBox::clean()
{
}

void LootBox::m_move()
{
	/*std::cout << "Calling Move" << std::endl;
	std::cout << "Enabled working? " << m_isEnabled << std::endl;*/
	if (!m_isEnabled)
	{	
		// Make sure that the box doesn't move at all when you're going down
		getRigidBody()->velocity = glm::vec2(0.0f, 0.0f) * m_PPM;
		getRigidBody()->acceleration = glm::vec2(0.0f, 0.0F) * m_PPM;


	}
	else {
		if (getTransform()->position.y < 400.0f - (getHeight() / 2.0)) {
			// implement formulas here

			// the ramp will have a frictionless surface
			// So the formula needed here for acceleration will be
			// a = g * sin(theta)
			acceleration = m_gravityFactor * sin(m_Angle);
			m_acceleration = glm::vec2(acceleration * cos(m_Angle), acceleration * sin(m_Angle));
			getRigidBody()->acceleration = m_acceleration;

			// velocity += acceleration * deltaTime * m_PPM
			getRigidBody()->velocity += getRigidBody()->acceleration * deltaTime * m_PPM;

			// Pf += velocity * dt (explanation)
			getTransform()->position += getRigidBody()->velocity * deltaTime;

			// ---------- flat surface ------------
			// if the crate reached a certain  say like 400 input value
			// could trigger on bool
			// velocity will change from going down the ramp along a flat surface with friction
			// acceleration will go the other way because the only net force will be the force of friction
			// acceleration = - (mk) * m_gravityFactor;
			// velocity += acceleration * deltaTime * m_PPM

			// Pf += velocity * dt (explanation)
			// getTransform()->position += getRigidbody()->velocity * deltaTime
		}
		else if (getTransform()->position.y >= 400.0f - (getHeight() / 2.0))
		{
			if (!m_isStopped) {
				getRigidBody()->velocity = glm::vec2(Util::magnitude(getRigidBody()->velocity), 0.0f);
				acceleration = kineticFriction * m_gravityFactor;
				m_acceleration = glm::vec2(-acceleration, 0.0f);

				getRigidBody()->acceleration = m_acceleration;
				std::cout << "position X: " << getTransform()->position.x << std::endl;
				std::cout << "position Y: " << getTransform()->position.y << std::endl;

				// velocity += acceleration * deltaTime * m_PPM
				if (Util::magnitude(getRigidBody()->velocity) < Util::magnitude(getRigidBody()->acceleration) && Util::magnitude(getRigidBody()->velocity) > 0) {
					m_isStopped = !m_isStopped;
				}
				std::cout << "Velocity magnitude: " << Util::magnitude(getRigidBody()->velocity) << std::endl;
				std::cout << "Acceleration magnitude: " << Util::magnitude(getRigidBody()->acceleration) << std::endl;

				glm::vec2 pos = getTransform()->position;
				getRigidBody()->velocity += getRigidBody()->acceleration * deltaTime * m_PPM;

				pos.x += getRigidBody()->velocity.x * deltaTime;
				pos.y += getRigidBody()->velocity.y * deltaTime;
				// Pf += velocity * dt 
				getTransform()->position = pos;
				finalPosition = getTransform()->position;
			}
			else
			{
				getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
				getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
				getTransform()->position = finalPosition;
				std::cout << "Velocity magnitude: " << Util::magnitude(getRigidBody()->velocity) << std::endl;
				std::cout << "Acceleration magnitude: " << Util::magnitude(getRigidBody()->acceleration) << std::endl;
			}
		}
		
	}
}

float LootBox::getGravityFactor()
{
	return m_gravityFactor;
}

void LootBox::setGravityFactor(float gFactor)
{
	m_gravityFactor = gFactor;
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

bool LootBox::getIsEnabled()
{
	return isEnabled();
}

void LootBox::setIsEnabled(bool check)
{
	m_isEnabled = check;
}

bool LootBox::getIsStopped()
{
	return m_isStopped;
}

void LootBox::setIsStopped(bool check)
{
	m_isStopped = check;
}

float LootBox::getKineticFriction()
{
	return kineticFriction;
}

void LootBox::setKineticFriction(float kFriction)
{
	kineticFriction = kFriction;
}

float LootBox::getAngle()
{
	return m_Angle;
}

void LootBox::setAngle(float angle)
{
	m_Angle = angle;
}


//
//glm::vec2 LootBox::getInitialPosition()
//{
//	return initialPosition;
//}
//
//void LootBox::setInitialPosition(glm::vec2 initPos)
//{
//	initialPosition = initPos;
//}
