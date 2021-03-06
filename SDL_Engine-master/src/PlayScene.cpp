﻿#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "Util.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	// draw the background
	TextureManager::Instance()->draw("background", 400.0f, 300.0f, 0, 255, true, SDL_FLIP_NONE);

	drawDisplayList();
	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	// Scale factor 1represented as white line on bottom left, need for Assignment 2
	m_PPMdisplay->setText("PPM: " + std::to_string(m_PPM));
	Util::DrawLine(glm::vec2(50.0f, 550.0f), glm::vec2(50.0f + m_PPM, 550.0f), glm::vec4(1.0f));
	Util::DrawLine(glm::vec2(50.0f, 551.0f), glm::vec2(50.0f + m_PPM, 551.0f), glm::vec4(1.0f));
	Util::DrawLine(glm::vec2(50.0f, 552.0f), glm::vec2(50.0f + m_PPM, 552.0f), glm::vec4(1.0f));
	Util::DrawLine(glm::vec2(50.0f, 553.0f), glm::vec2(50.0f + m_PPM, 553.0f), glm::vec4(1.0f));
	Util::DrawLine(glm::vec2(50.0f, 554.0f), glm::vec2(50.0f + m_PPM, 554.0f), glm::vec4(1.0f));
	Util::DrawLine(glm::vec2(50.0f, 555.0f), glm::vec2(50.0f + m_PPM, 555.0f), glm::vec4(1.0f));

	// Draw the ramp FOR ASSIGNMENT 2
	Util::DrawLine(glm::vec2(150.0f, 400.0f), glm::vec2(150.0f + xRamp, 400.0f), glm::vec4(1.0f)); // flat line
	Util::DrawLine(glm::vec2(150.0f, 400.0f), glm::vec2(150.0f, 400.0f - yRamp), glm::vec4(1.0f));
	Util::DrawLine(glm::vec2(150.0f + xRamp, 400.0f), glm::vec2(150.0F, 400.0f - yRamp), glm::vec4(1.0f));

	// draw Vectors for force
	gravityForce = calculateGravityForce(mass, m_gravityFactor);
	frictionForce = calculateFrictionForce(mass, m_gravityFactor, m_kineticFriction);
	accelerationForce = calculateAccelerationForce(mass, m_gravityFactor, m_Angle);
	normalForce = calculateNormalForce(mass, m_gravityFactor, m_Angle);

	m_FgravityDisplay->setText("Fg: " + std::to_string(gravityForce));
	m_FgravityDisplay->getTransform()->position = m_pLootbox->getTransform()->position + glm::vec2(0.0, gravityForce);
	Util::DrawLine(m_pLootbox->getTransform()->position, m_pLootbox->getTransform()->position + glm::vec2(0.0, gravityForce), glm::vec4(1.0f));
	

	if (m_pLootbox->getTransform()->position.y < 400 - (m_pLootbox->getHeight() / 2.0f)) {
		m_FnormalDisplay->setText("FN = " + std::to_string(normalForce));
		m_FnormalDisplay->getTransform()->position = m_pLootbox->getTransform()->position - glm::vec2(normalForce * cos(glm::radians(m_Angle + 90)), normalForce * sin(glm::radians(m_Angle + 90)) + 1.0f);
		m_FaccelDisplay->setText("Fa = " + std::to_string(accelerationForce));
		m_FaccelDisplay->getTransform()->position = m_pLootbox->getTransform()->position + glm::vec2(accelerationForce * cos(glm::radians(m_Angle)), accelerationForce * sin(glm::radians(m_Angle)));
		m_FfrictionDisplay->setText(" ");
		Util::DrawLine(m_pLootbox->getTransform()->position, m_pLootbox->getTransform()->position - glm::vec2(normalForce * cos(glm::radians(m_Angle + 90)), normalForce * sin(glm::radians(m_Angle + 90))), glm::vec4(1.0f));
		Util::DrawLine(m_pLootbox->getTransform()->position, m_pLootbox->getTransform()->position + glm::vec2(accelerationForce * cos(glm::radians(m_Angle)), accelerationForce * sin(glm::radians(m_Angle))), glm::vec4(1.0f));
		
	}
	else if (m_pLootbox->getTransform()->position.y >= 400 - (m_pLootbox->getHeight() / 2.0f))
	{
		normalForce = calculateNormalForce(mass, m_gravityFactor, 0.0f);
		m_FnormalDisplay->setText("FN = " + std::to_string(normalForce));
		m_FnormalDisplay->getTransform()->position = m_pLootbox->getTransform()->position - glm::vec2(0.0f, normalForce);
		m_FfrictionDisplay->setText("Fk = " + std::to_string(frictionForce));
		m_FfrictionDisplay->getTransform()->position = m_pLootbox->getTransform()->position - glm::vec2(frictionForce, 0.0f);
		m_FaccelDisplay->setText(" ");
		Util::DrawLine(m_pLootbox->getTransform()->position, m_pLootbox->getTransform()->position - glm::vec2(0.0f, normalForce), glm::vec4(1.0f));
		Util::DrawLine(m_pLootbox->getTransform()->position, m_pLootbox->getTransform()->position - glm::vec2(frictionForce, 0.0f), glm::vec4(1.0f));
		
	}
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	// once Physics enabled is pressed, the lootbox function will activate
	
	m_pLootbox->m_move();

	updateDisplayList();
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController
	if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
				m_playerFacingRight = true;
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
				m_playerFacingRight = false;
			}
			else
			{
				if (m_playerFacingRight)
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
				}
				else
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
				}
			}
		}
	}


	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
			m_playerFacingRight = false;
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;
		}
		else
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
			}
		}
	}
	

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}

void PlayScene::start()
{
	// set the background
	TextureManager::Instance()->load("../Assets/textures/Background.png", "background");
	const SDL_Color white = { 255, 255, 255, 255 };
	const SDL_Color blue = { 0, 0, 255, 255 };

	// Set GUI Title
	m_guiTitle = "Physics Simulator";	

	// Player Sprite
	m_pPlayer = new Player();
	m_pPlayer->getTransform()->position = glm::vec2(100.0f, 400.0f);
	addChild(m_pPlayer);
	m_playerFacingRight = true;
	
	// Box Sprite
	m_pLootbox = new LootBox();
	m_pLootbox->getTransform()->position = glm::vec2(150.0f, yRamp);
	m_Angle = calculateAngle(xRamp, yRamp); // returns in radians first
	m_pLootbox->setAngle(m_Angle);
	m_pLootbox->getTransform()->position += glm::vec2(m_pLootbox->getWidth() / 2.0f * cos(m_Angle), -(m_pLootbox->getHeight() / 2.0f));
	m_pLootbox->setGravityFactor(m_gravityFactor);
	m_pLootbox->setPixelsPerMeter(m_PPM);
	m_pLootbox->setKineticFriction(m_kineticFriction);
	addChild(m_pLootbox);
	
	std::cout << "Gravity Factor: " << m_pLootbox->getGravityFactor() << std::endl;
	std::cout << "Mass: " << m_pLootbox->getMass() << std::endl;
	std::cout << "Pixels Per Meter: " << m_pLootbox->getPixelsPerMeter() << std::endl;
	m_Angle = glm::degrees(m_Angle);
	std::cout << "Angle: " << m_pLootbox->getAngle() << std::endl;
	std::cout << "Angle in degrees: " << m_Angle << std::endl;
	std::cout << "Frictional Coefficient" << m_pLootbox->getKineticFriction() << std::endl;

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas", 20, white);
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 520.0f);
	addChild(m_pInstructionsLabel);

	// Pixels Per Meter Label
	m_PPMdisplay = new Label("PPM: " + std::to_string(m_PPM), "Consolas", 10, white, glm::vec2(50.0f, 540.0f));
	addChild(m_PPMdisplay);

	m_FnormalDisplay = new Label(" ", "Consolas", 15, white, m_pLootbox->getTransform()->position);
	addChild(m_FnormalDisplay);

	m_FgravityDisplay = new Label(" ", "Consolas", 15, white, m_pLootbox->getTransform()->position);
	addChild(m_FgravityDisplay);

	m_FaccelDisplay = new Label(" ", "Consolas", 15, white, m_pLootbox->getTransform()->position);
	addChild(m_FaccelDisplay);

	m_FfrictionDisplay = new Label(" ", "Consolas", 15, white, m_pLootbox->getTransform()->position);
	addChild(m_FfrictionDisplay);
}

float PlayScene::calculateAngle(float x, float y)
{
	// arctan(ramp rise/ramp run)
	return atan(y/x); 
}

float PlayScene::calculateHeight(float angle, float x)
{
	// height = run * tan(angle)
	return x * tan(glm::radians(angle));
}

// Calculate the values of force for drawing the vectors
float PlayScene::calculateNormalForce(float m, float g, float angle)
{
	// FN = mgcos(theta)
	return m*g*cos(glm::radians(angle));
}

float PlayScene::calculateFrictionForce(float m, float g, float mk)
{
	// Fk = mk * mass * gravity
	return mk*m*g;
}

float PlayScene::calculateAccelerationForce(float m, float g, float angle)
{
	// Fa = mass * gravity * sin(theta)
	return m * g * sin(glm::radians(angle));
}

float PlayScene::calculateGravityForce(float m, float g)
{
	// Fg = mass * gravity
	return m * g;
}

// Reset Values
void PlayScene::resetValues()
{
	xRamp = 200.0f;
	yRamp = 200.0f;
	m_gravityFactor = 9.8f;
	m_PPM = 5.0f;
	m_Angle = calculateAngle(xRamp,yRamp);
	m_kineticFriction = 0.7;
	mass = 10.0f;
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	// ImGui::ShowDemoWindow();
	
	ImGui::Begin("Physics Controls", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBringToFrontOnFocus);

	// The Throw Function
	if(ImGui::Button("Activate Physics"))
	{
		isEnabled = (isEnabled) ? false : true;
		std::cout << "Is moving: " << isEnabled << std::endl;
		m_pLootbox->setIsEnabled(isEnabled);
	}

	ImGui::SameLine();

	// Resetting values for elements on screen
	if (ImGui::Button("Reset All"))
	{
		isEnabled = false;
		resetValues();

		// resetting position values
		m_pLootbox->setIsEnabled(isEnabled);
		m_pLootbox->getTransform()->position = glm::vec2(150.0f, 400 - yRamp);
		m_pLootbox->setAngle(m_Angle);
		m_pLootbox->getTransform()->position += glm::vec2(m_pLootbox->getWidth()/2.0f * cos(m_Angle), -(m_pLootbox->getHeight() / 2.0f));
		m_pLootbox->setGravityFactor(m_gravityFactor);
		m_pLootbox->setPixelsPerMeter(m_PPM);		
		m_pLootbox->setMass(mass);
		m_pLootbox->setKineticFriction(m_kineticFriction);
		m_pLootbox->getTransform()->rotation = glm::vec2(cos(m_Angle), sin(m_Angle));
		m_pLootbox->setIsStopped(false);

		m_Angle = glm::degrees(m_Angle);
		std::cout << "Gravity Factor: " << m_pLootbox->getGravityFactor() << std::endl;
		std::cout << "Pixels Per Meter: " << m_pLootbox->getPixelsPerMeter() << std::endl;
		std::cout << "Angle: " << m_pLootbox->getAngle() << std::endl;
		std::cout << "Mass: " << m_pLootbox->getMass() << std::endl;
		std::cout << "Frictional Coefficient" << m_pLootbox->getKineticFriction() << std::endl;
	}

	// ----------------------- PARAMETER CHANGES -----------------------

	// Pixels per meter scale
	if (ImGui::SliderFloat("Pixels Per Meter", &m_PPM, 0.1f, 30.0f, "%.1f"))
	{
		m_pLootbox->setPixelsPerMeter(m_PPM);
		std::cout << "Pixels Per Meter: " << m_pLootbox->getPixelsPerMeter() << std::endl;
	}
	
	if (ImGui::SliderFloat("Mass", &mass, 0.1f, 50.0f))
	{
		m_pLootbox->setMass(mass);
		std::cout << "Mass: " << m_pLootbox->getMass() << std::endl;
	}

	// Modifiable Gravity Coefficient
	if (ImGui::SliderFloat("Gravity", &m_gravityFactor, 0.1f, 30.0f, "%.1f"))
	{
		m_pLootbox->setGravityFactor(m_gravityFactor);
		std::cout << "Gravity Factor: " << m_pLootbox->getGravityFactor() << std::endl;
	}
	
	if (ImGui::SliderFloat("Kinetic Friction", &m_kineticFriction, 0.1f, 2.0f))
	{
		m_pLootbox->setKineticFriction(m_kineticFriction);
		std::cout << "Kinetic Friction: " << m_pLootbox->getKineticFriction() << std::endl;
	}

	// Lab purposes
	if (ImGui::SliderFloat("Ramp Length", &xRamp, 0, 800)) {
		if (!isEnabled)
		{
			m_Angle = calculateAngle(xRamp, yRamp);
			m_pLootbox->setAngle(m_Angle); // sending the value in radians
			m_pLootbox->getTransform()->position.y = 400.0f - (yRamp);
			m_pLootbox->getTransform()->position += glm::vec2(0.0f, -(m_pLootbox->getHeight() / 2.0f));

		}

		m_Angle = glm::degrees(m_Angle); // this makes sure that the imgui slider is ok
		std::cout << "Player Transform: " << m_pLootbox->getTransform()->position.y << std::endl;
		std::cout << "Angle: " << m_pLootbox->getAngle() << std::endl;
	}

	if (ImGui::SliderFloat("Ramp Height", &yRamp, 0, 600)) {
		if (!isEnabled)
		{
			m_Angle = calculateAngle(xRamp, yRamp);
			m_pLootbox->setAngle(m_Angle); // sending the value in radians
			m_pLootbox->getTransform()->position.y = 400.0f - (yRamp);
			m_pLootbox->getTransform()->position += glm::vec2(0.0f, -(m_pLootbox->getHeight() / 2.0f));

		}
		

		m_Angle = glm::degrees(m_Angle); // this makes sure that the imgui slider is ok
		std::cout << "Player Transform: " << m_pLootbox->getTransform()->position.y << std::endl;
		std::cout << "Angle: " << m_pLootbox->getAngle() << std::endl;

	}

	if (ImGui::SliderFloat("Angle", &m_Angle, 0.0f, 89.0f))
	{

		if (!isEnabled)
		{
			m_pLootbox->setAngle(glm::radians(m_Angle));
			yRamp = calculateHeight(m_Angle, xRamp);
			m_pLootbox->getTransform()->position.y = 400.0f - (yRamp);
			m_pLootbox->getTransform()->position += glm::vec2(0.0f, -(m_pLootbox->getHeight() / 2.0f));

		}

		std::cout << "Angle: " << m_pLootbox->getAngle() << std::endl;
		std::cout << "Height: " << yRamp << std::endl;

	}	
	
	ImGui::Separator();
	ImGui::End();
	ImGui::EndFrame();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsClassic();
}
