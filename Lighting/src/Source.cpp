#include "LightingApp.h"
void main()
{
	Application* app = new LightingApp();
	app->run("Lighting", 1600, 900, false);
	delete app;
}