#pragma once
struct GLFWwindow;
//Pure virtual base class
class Application
{
public:
	Application();
	virtual ~Application();
	// ReSharper disable once CppHiddenFunction
	bool run(const char* title, unsigned int width, unsigned int height, bool fullscreen);
protected:
	virtual bool startup() = 0;
	virtual bool shutdown() = 0;
	virtual bool update(float deltaTime) = 0;
	virtual bool draw() = 0;
	void setBackgroundColor(float r, float g, float b, float a = 1.0f);
	float getTime() const;
	// ReSharper disable once CppHiddenFunction
	bool createWindow(const char* title, unsigned int width, unsigned int height, bool fullscreen);

	GLFWwindow * m_window;
	bool m_GameOver;
};

