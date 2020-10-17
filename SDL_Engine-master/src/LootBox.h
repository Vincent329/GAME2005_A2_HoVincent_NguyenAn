#pragma once
#ifndef __TARGET__
#define __TARGET__

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
	
	// function to activate the gravity
	void m_move();

	// Getter and Setter for Physics Factors
	float getGravityFactor();
	void setGravityFactor(float gFactor);

	float getPixelsPerMeter();
	void setPixelsPerMeter(float ppm);

	void setIsThrown(bool check);
	float getAngle();
	void setAngle(float angle);

	float getVelocity();
	void setVelocity(float velocity);
	float getVelocityX();
	void setVelocityX(float velocityX);
	float getVelocityY();
	void setVelocityY(float velocityY);

	// Initial Position
	glm::vec2 getInitialPosition();
	void setInitialPosition(glm::vec2 initPos);


	// Resetting purposes
	void resetElapsedTime();

private:
	// Variables for physics on the ball (passed in from PlayScene.cpp)

	float m_gravityFactor = 9.8f; // Gravity Factor
	glm::vec2 gravity = glm::vec2(0.0f, m_gravityFactor);

	float m_PPM = 5.0f; // Pixels per meter scale
	float m_Angle = 0.0f; // Launching 

	glm::vec2 initialPosition;

	// velocity values
	glm::vec2 m_velocity = glm::vec2(0.0f, 0.0f);

	// acceleration values
	glm::vec2 m_acceleration = glm::vec2(0.0f, 0.0f); // afecteed by the value of g*sin(theta)
	float acceleration = 0.0f; // will stay constant and be isolated from the value of g * sin(theta)... 
	float mass = 0.0f; // will determine the force going down wards, check the if statement if 
					   // Fparallel = (mgsin(theta)) < F static friction  (muK * Force Normal)
	
	// Trigger for the physics to work
	bool m_isEnabled = false;
	
	// Timer variables for the elapsed time
	float deltaTime = 1.0f / 30.0f;
	float elapsedTime = 1.0f / 30.0f;

};


#endif /* defined (__TARGET__) */