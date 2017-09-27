#pragma once
#include "Application.h"

class TextureApplication :
	public Application
{
public:
	TextureApplication();
	~TextureApplication();
protected:
	bool startup() override;
	bool shutdown() override;
	bool update(float deltaTime) override;
	bool draw() override;
};

