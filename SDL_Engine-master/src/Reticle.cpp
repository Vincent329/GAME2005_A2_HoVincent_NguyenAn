#pragma once

#include "Reticle.h"
#include "TextureManager.h"

Reticle::Reticle()
{
	TextureManager::Instance()->load("../Assets/textures/crosshair.png", "crosshair");

	const auto size = TextureManager::Instance()->getTextureSize("crosshair");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(500.0f, 400.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	setType(RETICLE);
}

Reticle::~Reticle()
= default;


void Reticle::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("crosshair", x, y, 0, 255, true);
}

void Reticle::update()
{
}

void Reticle::clean()
{
}
