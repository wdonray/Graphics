#include "TestApplication.h"
#include "Camera.h"

int main()
{
	Application* app = new TestApplication();
	app->run("Part One", 1600, 900, false);
	delete app;
	return  0;
}
