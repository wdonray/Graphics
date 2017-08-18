#include <glm/glm.hpp>
#include <iostream>
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include <Gizmos.h>
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtc/constants.inl>

int main()
{
	glfwInit();
	auto * window = glfwCreateWindow(1600, 900, "Cuck", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		//ImGui::Shutdown();
		//exit(EXIT_SUCCESS);
		return false;
	}
	assert(window != nullptr);

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();

	printf("GL: %i.%i\n", major, minor);
	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	auto clearColor = glm::vec4(0.1f, 0.1f, 0.5f, 0.5f);
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

	//static auto f = 0.0f;
	//auto show_first_window = true;
	//auto show_second_window = false;
	//auto show_third_window = true;
	//ImGui_ImplGlfwGL3_Init(window, true);
	//auto view = lookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
	//auto projection = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.f);
	//Gizmos::create();
	glEnable(GL_DEPTH_TEST);
	while (true)
	{
		glfwPollEvents();
		//ImGui_ImplGlfwGL3_NewFrame();
		//if (show_first_window)
		//{
		//	ImGui::Begin("Window One", &show_first_window);
		//	auto Show = "Show Second Window";
		//	auto Hide = "Hide Second Window";
		//	if (show_second_window == false)
		//	{
		//		if (ImGui::Button(Show)) show_second_window ^= 1;
		//	}
		//	else if (show_second_window == true)
		//	{
		//		if (ImGui::Button(Hide)) show_second_window ^= 1;
		//	}
		//	ImGui::Text("Application FPS (%.1f FPS)", ImGui::GetIO().Framerate);
		//	ImGui::End();
		//}
		//if (show_second_window)
		//{
		//	ImGui::Begin("Window Two", &show_second_window);
		//	ImGui::Text("Hello, world!");
		//	ImGui::SliderFloat("float test slider", &f, 0.0f, 1.0f);
		//	ImGui::Text("%f", &f);
		//	ImGui::End();
		//}
		//if (show_third_window)
		//{
		//	ImGui::Begin("Window Three", &show_third_window);
		//	ImGui::SetNextWindowSize(ImVec2(200, 100));
		//	//ImGui::SetWindowPos(ImVec2(450, 20));
		//	ImGui::End();
		//}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (glfwWindowShouldClose(window))
			break;
		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, true);
		/*if (glfwGetKey(window, GLFW_KEY_SPACE))
		{
			clearColor.r -= .005f;
			clearColor.g -= .005f;
			clearColor.b -= .005f;
			printf("Color Vec4 => %f\n", clearColor.r);
			glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		}*/
		/*Gizmos::clear();
		Gizmos::addTransform(glm::mat4(1));
		glm::vec4 white(1);
		glm::vec4 black(0, 0, 0, 1);

		for (auto i = 0; i < 21; ++i)
		{
			Gizmos::addLine(
				glm::vec3(-10 + i, 0, 10),
				glm::vec3(-10 + i, 0, -10),
				i == 10 ? white : black);
			Gizmos::addLine(
				glm::vec3(10, 0, -10 + i),
				glm::vec3(-10, 0, -10 + i),
				i == 10 ? white : black);
		}
		Gizmos::addSphere(glm::vec3(0, 0, 0), 5.f, 25, 25, black);
		Gizmos::draw(projection * view);
		ImGui::Render();*/
		glfwSwapBuffers(window);
	}
	return 0;
}
