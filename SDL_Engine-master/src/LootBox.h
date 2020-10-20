#pragma once
#ifndef __LOOTBOX__
#define __LOOTBOX__

#include "DisplayObject.h"
#include "TextureManager.h"
#include "Util.h"

class LootBox final : public DisplayObject {
public:
	LootBox();
	~LootBox();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	
	// function to activate the motion
	void m_move();

	// Getter and Setter for Physics Factors
	float getGravityFactor();
	void setGravityFactor(float gFactor);

	float getMass();
	void setMass(float mass);

	// Change pixels per meter
	float getPixelsPerMeter();
	void setPixelsPerMeter(float ppm);

	// Set the crate into motion
	bool getIsEnabled();
	void setIsEnabled(bool check);

	// Kinetic Friction
	float getKineticFriction();
	void setKineticFriction(float kFriction);

	// setting the angle
	float getAngle();
	void setAngle(float angle);

	glm::vec2 getVelocity();
	void setVelocity(glm::vec2 velocity);

	// Resetting purposes
	void resetElapsedTime();

private:
	// Variables for physics on the ball (passed in from PlayScene.cpp)

	float m_gravityFactor = 9.8f; // Gravity Factor
	glm::vec2 gravityVector = glm::vec2(0.0f, m_gravityFactor);

	float m_PPM = 5.0f; // Pixels per meter scale
	float m_Angle = 0.0f; // Launching 

	glm::vec2 finalPosition;

	// velocity values
	glm::vec2 m_velocity = glm::vec2(0.0f, 0.0f);

	// acceleration values
	glm::vec2 m_acceleration = glm::vec2(0.0f, 0.0f); // afecteed by the value of g*sin(theta)
	float acceleration = 0.0f; // use this to get a value and split it a
	float m_Mass = 0.0f; // will determine the force going down wards, check the if statement if 
					   // Fparallel = (mgsin(theta)) < F static friction  (muK * Force Normal)

	float kineticFriction = 0.0f;
	
	// Trigger for the physics to work
	bool m_isEnabled = false;

	// once the ball goes into a stop
	bool m_isStopped = false;
	
	// Timer variables for the elapsed time
	float deltaTime = 1.0f / 30.0f;
	float elapsedTime = 1.0f / 30.0f;

};


#endif /* defined (__TARGET__) */