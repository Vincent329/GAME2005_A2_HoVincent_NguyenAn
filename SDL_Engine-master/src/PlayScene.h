#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Enemy.h"
#include "Button.h"
#include "Target.h"
#include "LootBox.h"
#include "Label.h"
#include "Util.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

	// Getter and Setter
	//float getGravityFactor();
	//void setGravityFactor(float gFactor);
	//float getPixelserMeter();
	//void setPixelsPerMeter(float ppm);

	// Calculate Physics functions
	float calculateAngle(float x, float y); // takes in xramp and yramp and gets angle through atan(x/y);
	float calculateHeight(float angle, float x); // if player wants to change angle specifically, 
											     // then call this function where y = x * tan(angle)
	float calculateNormalForce(float m, float g, float angle);
	float calculateFrictionForce(float m, float g, float mk);
	float calculateAccelerationForce(float m, float g, float angle);
	float calculateGravityForce(float m, float g);


	// this will toggle the physics simulation
	bool isEnabled = false;

private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	LootBox* m_pLootbox; // the sprite we're going to add in
	Enemy* m_pEnemy; // for testing purposes
	Plane* m_pPlaneSprite;
	Player* m_pPlayer;
	Target* m_pBall;
	bool m_playerFacingRight;

	// UI Items
	Label* m_pInstructionsLabel;
	Label* m_PPMdisplay;
	Label* m_FnormalDisplay;
	Label* m_FgravityDisplay;
	Label* m_FaccelDisplay;
	Label* m_FfrictionDisplay;

	// Physics variables to pass into the Physics simulator
	float mass = 10.0f;
	float m_gravityFactor = 9.8f; // earth gravity, positive because going downwards is positive, can be modifiable
	float m_PPM = 5.0f; // Pixels per meter scale
	float m_kineticFriction = 0.7f;

	float m_velocity = 0.0f; // initial velocity of the ball
	float m_acceleration = 0.0f; //

	// for display purposes, must calculate
	float gravityForce;
	float frictionForce;
	float accelerationForce;
	float normalForce;
	
	// set ramp params
	float xRamp = 200.0f;
	float yRamp = 200.0f;
	float m_Angle = 0.0f; // Angle of the ramp as designated by tan(theta) = opposite/adjacent (yRamp/xRamp)4
					      // can also change to user specifics

	// timing variables
	void resetValues();
};

#endif /* defined (__PLAY_SCENE__) */