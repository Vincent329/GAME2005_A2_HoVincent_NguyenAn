#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Enemy.h"
#include "Button.h"
#include "Target.h"
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

	// this will toggle the physics simulation
	bool isMoving = false;

private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	Enemy* m_pEnemy; // for testing purposes
	Plane* m_pPlaneSprite;
	Player* m_pPlayer;
	Target* m_pBall;
	bool m_playerFacingRight;

	// UI Items
	Label* m_pInstructionsLabel;
	Label* m_PPMdisplay;

	// Physics variables to pass into the Physics simulator
	float m_gravityFactor = 9.8f; // earth gravity, positive because going downwards is positive, can be modifiable
	float m_PPM = 5.0f; // Pixels per meter scale

	float m_Angle = 0.0f; // Angle of the ramp as designated by tan(theta) = opposite/adjacent (yRamp/xRamp)
	float m_velocity = 0.0f; // initial velocity of the ball
	float m_acceleration = 0.0f; //

	float netForce = 0.0f;
	glm::vec2 staticFrictionForce; // the amount of force acting upon the object so that it doesn't move
	glm::vec2 kineticFrictionForce; // the amount of friction acting upon the object as it is moving along the ramp

	// set ramp params
	float xRamp = 400.0f;
	float yRamp = 200.0f;

	// distance between bomb and the target
	float deltaDistance = 0;
	/*float m_velocityX = 0.0f;
	float m_velocityY = 0.0f;*/

	// timing variables
	void resetValues();
};

#endif /* defined (__PLAY_SCENE__) */