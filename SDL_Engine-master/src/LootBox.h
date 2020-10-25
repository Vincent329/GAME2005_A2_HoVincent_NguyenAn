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

		// Set the crate into motion
	bool getIsStopped();
	void setIsStopped(bool check);

	// Kinetic Friction
	float getKineticFriction();
	void setKineticFriction(float kFriction);

	// setting the angle
	float getAngle();
	void setAngle(float angle);

private:
	// Variables for physics on the crate (passed in from PlayScene.cpp)

	float m_gravityFactor = 9.8f; // Gravity Factor
	float m_PPM = 5.0f; // Pixels per meter scale
	float m_Angle = 0.0f; // Launching 

	glm::vec2 finalPosition;

	// acceleration values
	glm::vec2 m_acceleration = glm::vec2(0.0f, 0.0f); // afecteed by the value of g*sin(theta)
	float acceleration = 0.0f; // use this to get a value and split it a
	float m_Mass = 0.0f; // will determine the force going down wards, check the if statement if 
					   // Fparallel = (mgsin(theta)) < F static friction  (muK * Force Normal)

	// Coefficient of Kinetic Friction
	float kineticFriction = 0.0f; 
	
	// bool triggers for events
	// Trigger for the physics to work
	bool m_isEnabled = false;

	// once the crate goes into a stop
	bool m_isStopped = false;
	
	// Timer variables for the elapsed time
	float deltaTime = 1.0f / 30.0f;

};


#endif /* defined (__TARGET__) */