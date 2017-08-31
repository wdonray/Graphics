#include "TestApplication.h"
#include "Camera.h"
#include "CameraApp.h"
#include <iostream>
#include "../LoadFile.h"
using namespace std;

int main()
{
	//Application* app = new TestApplication();
	//app->run("Part One", 1600, 900, false);

	LoadFile * fr = new LoadFile();
	const char * data = fr->ReadFile("test.txt");
	cout << data << endl;

	Application* app = new CameraApp();
	app->run("Part One", 1600, 900, false);
	delete app;

	return  0;
}
