#include "TestApplication.h"
#include "Camera.h"
#include "CameraApp.h"


int main()
{
	//Application* app = new TestApplication();
	//app->run("Part One", 1600, 900, false);
	Application* app = new CameraApp();
	app->run("Part One", 1600, 900, false);
	delete app;
	return  0;
}
