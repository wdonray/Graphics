#include "TextureApplication.h"
void main()
{
	Application* app = new TextureApplication();
	app->run("Texture", 1600, 900, false);
	delete app;
}