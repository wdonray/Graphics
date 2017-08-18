#include "TestApplication.h"

int main()
{
	Application* app = new TestApplication();
	app->run("Part One", 800, 800, false);
	delete app;
	return  0;
}