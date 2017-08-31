#include "RenderingApp.h"
void main()
{
	Application* app = new RenderingApp();
	app->run("RenderingGeo", 1600, 900, false);
	delete app;
}
