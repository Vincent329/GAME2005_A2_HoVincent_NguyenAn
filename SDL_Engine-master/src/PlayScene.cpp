#include "PlayScene.h"
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

	// Scale factor 1represented as white line on bottom left
	m_PPMdisplay->setText("PPM: " + std::to_string(m_PPM));
	Util::DrawLine(glm::vec2(50.0f, 550.0f), glm::vec2(50.0f + m_PPM, 550.0f), glm::vec4(1.0f));
	Util::DrawLine(glm::vec2(50.0f, 551.0f), glm::vec2(50.0f + m_PPM, 551.0f), glm::vec4(1.0f));
	Util::DrawLine(glm::vec2(50.0f, 552.0f), glm::vec2(50.0f + m_PPM, 552.0f), glm::vec4(1.0f));
	Util::DrawLine(glm::vec2(50.0f, 553.0f), glm::vec2(50.0f + m_PPM, 553.0f), glm::vec4(1.0f));
	Util::DrawLine(glm::vec2(50.0f, 554.0f), glm::vec2(50.0f + m_PPM, 554.0f), glm::vec4(1.0f));
	Util::DrawLine(glm::vec2(50.0f, 555.0f), glm::vec2(50.0f + m_PPM, 555.0f), glm::vec4(1.0f));

	// Drawing a vector line to Simulate aiming vector
	if (!isMoving)
	{
		Util::DrawLine(glm::vec2(m_pBall->getTransform()->position),
			glm::vec2(m_pBall->getTransform()->position.x + 30 * cos(glm::radians(m_Angle)),
				m_pBall->getTransform()->position.y + 30 * -sin(glm::radians(m_Angle))),
			glm::vec4(1.0f));
		Util::DrawLine(glm::vec2(m_pBall->getTransform()->position.x, m_pBall->getTransform()->position.y + 1),
			glm::vec2(m_pBall->getTransform()->position.x + 30 * cos(glm::radians(m_Angle)),
				m_pBall->getTransform()->position.y + 30 * -sin(glm::radians(m_Angle))),
			glm::vec4(1.0f));
		Util::DrawLine(glm::vec2(m_pBall->getTransform()->position.x, m_pBall->getTransform()->position.y + 2),
			glm::vec2(m_pBall->getTransform()->position.x + 30 * cos(glm::radians(m_Angle)),
				m_pBall->getTransform()->position.y + 30 * -sin(glm::radians(m_Angle))),
			glm::vec4(1.0f));
	}

	// Draw the ramp
	Util::DrawLine(glm::vec2(400.0f, 400.0f), glm::vec2(400.0f + xRamp, 400.0f), glm::vec4(1.0f));
	Util::DrawLine(glm::vec2(400.0f, 400.0f), glm::vec2(400.0f, 400.0f - yRamp), glm::vec4(1.0f));
	Util::DrawLine(glm::vec2(400.0f + xRamp, 400.0f), glm::vec2(400.0f, 400.0f - yRamp), glm::vec4(1.0f));


	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	if (isMoving)
	{
		m_pBall->m_move();
	}	
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
	TextureManager::Instance()->load("../Assets/textures/Background.png", "background");
	const SDL_Color white = { 255, 255, 255, 255 };

	// Set GUI Title
	m_guiTitle = "Play Scene";	

	// Player Sprite
	m_pPlayer = new Player();
	m_pPlayer->getTransform()->position = glm::vec2(100.0f, 400.0f);
	addChild(m_pPlayer);
	m_playerFacingRight = true;

	// Ball sprite
	m_pBall = new Target();
	m_pBall->getTransform()->position = m_pPlayer->getTransform()->position;
	m_pBall->getTransform()->position.x += m_pBall->getWidth();
	m_pBall->setInitialPosition(m_pBall->getTransform()->position);
	addChild(m_pBall);

	// ship sprite for testing purposes
	m_pEnemy = new Enemy();
	m_pEnemy->getTransform()->position = glm::vec2(700.f, m_pPlayer->getTransform()->position.y);
	addChild(m_pEnemy);

	// import reticle
	m_pReticle = new Reticle();
	m_pReticle->getTransform()->position.x = reticleDistance(m_velocity, m_Angle);
	addChild(m_pReticle);

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas", 20, white);
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 520.0f);
	addChild(m_pInstructionsLabel);

	// Pixels Per Meter Label
	m_PPMdisplay = new Label("PPM: " + std::to_string(m_PPM), "Consolas", 10, white, glm::vec2(50.0f, 540.0f));
	addChild(m_PPMdisplay);
}

// Reset Values
void PlayScene::resetValues()
{
	m_gravityFactor = 9.8f;
	m_PPM = 5.0f;
	m_Angle = 0.0f;
	m_velocity = 0.0f;
}

float PlayScene::reticleDistance(float velocity, float angle)
{
	// R = (v^2)sin(2(theta))/g
	// remember to multiply by the scale function
	float range = ((((velocity * m_PPM) * (velocity*m_PPM)) * sin(glm::radians(2 * angle))) / (m_gravityFactor*m_PPM));
	std::cout << "Range: " << range << std::endl;
	return m_pBall->getTransform()->position.x + range;
}

float PlayScene::angleChange(float distance, float velocity)
{
	// theta = asin((g*deltaDistance)/velocity^2)/2

	float calculation = ((m_gravityFactor * m_PPM) * distance) / ((velocity * m_PPM) * (velocity * m_PPM));
	if (calculation < -1 || calculation > 1)
	{
		return 45.0f;
	}
	else {
		float angleInRads = asin(calculation) / 2.0f;
		std::cout << "Angle in Rads: " << angleInRads << std::endl;
		return glm::degrees(angleInRads);
	}
}

// velocity lock
// passed in delta distance, as well as the angle of kick, adjust velocity accordingly
float PlayScene::velocityAdjust(float distance, float angle)
{
	// v = square root (g*d/sin2(theta))
	// divide by PPM because velocity will be multiplied by that when move calls
	return sqrt((((m_gravityFactor*m_PPM)*distance))/sin(glm::radians(2*angle)))/m_PPM;
}

float PlayScene::maxDistanceLock(float distance)
{
	// since this will just use the maximum distance (sin(2(45)) = 1
	// v = sqrt(g*d)
	return sqrt(((m_gravityFactor * m_PPM) * distance)) / m_PPM;
}


void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	// ImGui::ShowDemoWindow();
	
	ImGui::Begin("Physics Controls", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBringToFrontOnFocus);

	// The Throw Function
	if(ImGui::Button("Throw"))
	{
		isMoving = (isMoving) ? false : true;
		m_pBall->setIsThrown(isMoving);
		std::cout << "Is moving: " << isMoving << std::endl;
	}

	// Resets the position of the ball as well as the elapsed time so that the acceleration resets
	ImGui::SameLine();
	if (ImGui::Button("Reset Ball Position"))
	{
		// resets position of ball
		m_pBall->getTransform()->position = m_pPlayer->getTransform()->position;
		m_pBall->getTransform()->position.x += m_pBall->getWidth();
		isMoving = false; // Makes sure that throw is reset
		m_pBall->resetElapsedTime(); // reset the elapsed time for acceleration purposes
		m_pBall->setIsThrown(isMoving); // send boolean to ball class
	}

	ImGui::Separator();

	// Gravity Toggle, If user triggers Throw, ball should just move in a straight line
	static bool isGravityEnabled = false;
	if (ImGui::Checkbox("Gravity Enabled", &isGravityEnabled)) // toggling gravity with a checkbox
	{
		m_pBall->setIsGravityEnabled(isGravityEnabled);
	}

	ImGui::SameLine();

	// Resetting values for elements on screen
	if (ImGui::Button("Reset All"))
	{
		isGravityEnabled = false;
		isMoving = false;
		m_pBall->setIsGravityEnabled(isGravityEnabled);
		resetValues();

		// resetting position values
		m_pPlayer->getTransform()->position = glm::vec2(100.0f, 400.0f);
		m_pBall->getTransform()->position = m_pPlayer->getTransform()->position;
		m_pBall->getTransform()->position.x += m_pBall->getWidth();
		m_pBall->setGravityFactor(m_gravityFactor);
		m_pBall->setPixelsPerMeter(m_PPM);
		m_pBall->setAngle(m_Angle);
		m_pBall->setInitialPosition(m_pBall->getTransform()->position);
		m_pBall->resetElapsedTime();
		m_pReticle->getTransform()->position.x = reticleDistance(m_velocity, m_Angle);
	}

	// Pixels per meter scale
	if (ImGui::SliderFloat("Pixels Per Meter", &m_PPM, 0.1f, 30.0f, "%.1f"))
	{
		m_pBall->setPixelsPerMeter(m_PPM);
		std::cout << "Pixels Per Meter: " << m_pBall->getPixelsPerMeter() << std::endl;
		m_pReticle->getTransform()->position.x = reticleDistance(m_velocity, m_Angle);
	}
	
	// Modifiable Gravity Coefficient
	if (ImGui::SliderFloat("Gravity", &m_gravityFactor, 0.1f, 30.0f, "%.1f"))
	{
		m_pBall->setGravityFactor(m_gravityFactor);
		std::cout << "Gravity Factor: " << m_pBall->getGravityFactor() << std::endl;
		m_pReticle->getTransform()->position.x = reticleDistance(m_velocity, m_Angle);
	}

	// Angle for the ball to be kicked at
	if (ImGui::SliderFloat("Launch Angle", &m_Angle, 0.0f, 90.0f, "%.1f"))
	{
		m_pBall->setAngle(m_Angle);
		std::cout << "Angle value: " << m_pBall->getAngle() << std::endl;
		m_pReticle->getTransform()->position.x = reticleDistance(m_velocity, m_Angle);
	}

	// Change for high angle and low angle launch
	if (ImGui::Button("Switch Angle"))
	{
		m_Angle = 90 - m_Angle;
		m_pBall->setAngle(m_Angle);
		std::cout << "Angle value: " << m_pBall->getAngle() << std::endl;
	}
	
	// Velocity for the ball to be kicked
	if (ImGui::SliderFloat("Initial Velocity: ", &m_velocity, 0.0f, 75.0f))
	{
		m_pBall->setVelocity(m_velocity);
		std::cout << "Initial Velocity: " << m_pBall->getVelocity() << std::endl;
		m_pReticle->getTransform()->position.x = reticleDistance(m_velocity, m_Angle);
	}
	
	// 3 lock functions
	// Target lock (Angle set to 45, and adjust velocity accordingly given delta distance)
	// Velocity Lock (given delta distance abd angle, adjust velocity accordingly)
	// Angle Lock (given delta distance and velocity, adjust angle accordingly, otherwise default to 45)
	if (ImGui::Button("Target Lock"))
	{
		deltaDistance = m_pEnemy->getTransform()->position.x - m_pBall->getTransform()->position.x;
		m_Angle = 45.0f;
		m_velocity = maxDistanceLock(deltaDistance);
		m_pBall->setVelocity(m_velocity);
		m_pBall->setAngle(m_Angle);
		m_pReticle->getTransform()->position.x = reticleDistance(m_velocity, m_Angle);
		std::cout << "New Velocity: " << m_pBall->getVelocity() << std::endl;
	}

	ImGui::SameLine();
	if (ImGui::Button("Velocity Lock"))
	{
		deltaDistance = m_pEnemy->getTransform()->position.x - m_pBall->getTransform()->position.x;
		std::cout << "Distance between ball and target: " << deltaDistance << std::endl;
		m_velocity = velocityAdjust(deltaDistance, m_Angle);
		m_pBall->setVelocity(m_velocity);
		m_pReticle->getTransform()->position.x = reticleDistance(m_velocity, m_Angle);
		std::cout << "New Velocity: " << m_pBall->getVelocity() << std::endl;
	}

	ImGui::SameLine();
	if (ImGui::Button("Angle Lock"))
	{
		deltaDistance = m_pEnemy->getTransform()->position.x - m_pBall->getTransform()->position.x;
		std::cout << "Distance between ball and target: " << deltaDistance << std::endl;
		m_Angle = angleChange(deltaDistance, m_velocity);
		m_pBall->setAngle(m_Angle);
		m_pReticle->getTransform()->position.x = reticleDistance(m_velocity, m_Angle);
		std::cout << "New Angle: " << m_pBall->getAngle() << std::endl;
	}
	
	ImGui::Separator();

	// display the player's position in with regards to the corresponding Pixels Per Meter
	ImGui::Text("Player Distance in Meters: %f", m_pPlayer->getTransform()->position.x * m_PPM);
	
	// Position Slider for Player  
	static int xPlayerPos = 100;
	static int xEnemyPos = 700;
	if (ImGui::SliderInt("Player Position X", &xPlayerPos, 0, 800)) {
		m_pPlayer->getTransform()->position.x = xPlayerPos;
			// Ball moves along with player
		m_pBall->getTransform()->position = glm::vec2(xPlayerPos+m_pBall->getWidth(), 400);
		m_pBall->setInitialPosition(m_pBall->getTransform()->position);
		std::cout << "Initial Position = X: " << m_pBall->getTransform()->position.x << " Y: " << m_pBall->getTransform()->position.y << std::endl;
		m_pReticle->getTransform()->position.x = reticleDistance(m_velocity, m_Angle);

		// check so that the bomb does not pass the enemy position
		if (m_pBall->getTransform()->position.x >= xEnemyPos)
		{
			xEnemyPos = m_pBall->getTransform()->position.x;
			m_pEnemy->getTransform()->position.x = m_pBall->getTransform()->position.x;
		}
	}

	// Slider for Enemy Position
	if (ImGui::SliderInt("Enemy Position X", &xEnemyPos, 0, 800)) {
		m_pEnemy->getTransform()->position.x = xEnemyPos;

		// Checking so that the enemy does not pass the bomb
		if (m_pBall->getTransform()->position.x >= xEnemyPos)
		{
			xEnemyPos = m_pBall->getTransform()->position.x;
			m_pEnemy->getTransform()->position.x = m_pBall->getTransform()->position.x;
		}
	}

	
	// Lab purposes
	if (ImGui::SliderFloat("Ramp Length", &xRamp, 0, 300)) {
	}

	if (ImGui::SliderFloat("Ramp Height", &yRamp, 0, 300)) {
	}


	ImGui::End();
	ImGui::EndFrame();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsClassic();
}
