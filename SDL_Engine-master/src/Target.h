#pragma once
#ifndef __TARGET__
#define __TARGET__

#include "DisplayObject.h"
#include "Util.h"

class Target final : public DisplayObject {
public:
	Target();
	~Target();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;


	
	void m_move();

	// Getter and Setter for Physics Factors
	float getGravityFactor();
	void setGravityFactor(float gFactor);
	float getPixelsPerMeter();
	void setPixelsPerMeter(float ppm);
	void setIsGravityEnabled(bool check);
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
	float m_PPM = 5.0f; // Pixels per meter scale
	float m_Angle = 0.0f; // Launching 

	glm::vec2 initialPosition;
	float m_velocity = 0.0f;
	float m_velocityX = 0.0f;
	float m_velocityY = 0.0f;

	bool m_isGravityEnabled = false;
	bool m_isThrown = false;
	
	float deltaTime = 1.0f / 30.0f;
	float elapsedTime = 1.0f / 30.0f;

	void m_checkBounds();
	void m_reset();
};


#endif /* defined (__TARGET__) */