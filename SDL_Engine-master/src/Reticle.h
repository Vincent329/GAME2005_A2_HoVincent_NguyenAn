#pragma once

#ifndef ___RETICLE___
#define	___RETICLE___

#include "DisplayObject.h"

class Reticle final : public DisplayObject
{
public:
	Reticle();
	~Reticle();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

};

#endif