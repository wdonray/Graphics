#pragma once
#include "Application.h"
#include "gl_core_4_4.h"

//Derives from application
class TestApplication : public Application
{
public:
	TestApplication();
	virtual ~TestApplication();
	bool startup()override;
	bool shutdown()override;
	bool update(float deltaTime)override;
	bool draw()override;
	/**
	  runs our application
	  @parm title name of window
	  @parm width the width of window
	  @parm height the height of the window
	  @parm fullscreen is it fullscreen or not
	 */
	//bool run(const char* title, unsigned int width, unsigned int height, bool fullscreen);
	//bool createWindow(const char* title, unsigned int width, unsigned int height, bool fullscreen);
	//GLuint vao = 0;
	//GLuint shader_programme;
};

