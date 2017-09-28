#include "LightingApp.h"
#include "CameraApp.h"
#include <gl_core_4_4.h>
#include "Camera.h"
#include <GLFW/glfw3.h>
#include <Shader.h>
#include <glm/ext.hpp>
#include <Mesh.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define PI 3.14159265359
static char fbuffer[5000];
LightingApp::LightingApp() : runTime(0), m_rows(0), m_cols(0), segments(0), rings(0), m_VAO(0), m_VBO(0), m_IBO(0), index_count(0), imageWidth(0), imageHeight(0), imageFormat(0), m_textureID(0), data(nullptr), m_modelMatrix(1)
{
	cam = new Camera();
	camapp = new CameraApp();
	shader = new Shader();
	gridMesh = new Mesh();
}

LightingApp::~LightingApp()
{
	delete camapp;
	delete cam;
	delete shader;
	delete gridMesh;
}

void LightingApp::generateSphere(unsigned segments, unsigned rings, unsigned& vao, unsigned& vbo, unsigned& ibo, unsigned& indexCount) const
{
	unsigned int vertCount = (segments + 1) * (rings + 2);
	indexCount = segments * (rings + 1) * 6;

	// using AIEVertex for now, but could be any struct as long as it has the correct elements
	Vertex* vertices = new Vertex[vertCount];
	unsigned int* indices = new unsigned int[indexCount];

	float ringAngle = pi<float>() / (rings + 1);
	float segmentAngle = 2.0f * pi<float>() / segments;

	Vertex* vertex = vertices;

	for (unsigned int ring = 0; ring < (rings + 2); ++ring)
	{
		float r0 = sin(ring * ringAngle);
		float y0 = cos(ring * ringAngle);

		for (unsigned int segment = 0; segment < (segments + 1); ++segment, ++vertex)
		{
			float x0 = r0 * sin(segment * segmentAngle);
			float z0 = r0 * cos(segment * segmentAngle);

			vertex->position = vec4(x0 * 0.5f, y0 * 0.5f, z0 * 0.5f, 1);
			vertex->normal = vec4(x0, y0, z0, 0);

			vertex->tangent = vec4(sin(segment * segmentAngle + half_pi<float>()), 0, cos(segment * segmentAngle + half_pi<float>()), 0);

			// not a part of the AIEVertex, but this is how w generate bitangents
			vertex->bitangent = vec4(cross(vec3(vertex->normal), vec3(vertex->tangent)), 0);

			vertex->texcoord = vec2(segment / (float)segments, ring / (float)(rings + 1));
		}
	}

	unsigned int index = 0;
	for (unsigned i = 0; i < (rings + 1); ++i)
	{
		for (unsigned j = 0; j < segments; ++j)
		{
			indices[index++] = i * (segments + 1) + j;
			indices[index++] = (i + 1) * (segments + 1) + j;
			indices[index++] = i * (segments + 1) + (j + 1);

			indices[index++] = (i + 1) * (segments + 1) + (j + 1);
			indices[index++] = i * (segments + 1) + (j + 1);
			indices[index++] = (i + 1) * (segments + 1) + j;
		}
	}

	// generate buffers
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	// generate vertex array object (descriptors)
	glGenVertexArrays(1, &vao);

	// all changes will apply to this handle
	glBindVertexArray(vao);

	// set vertex buffer data
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

	// colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));
	// normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(vec4) * 2));

	// texcoords
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4) * 3));

	// tangents
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(vec4) * 3 + sizeof(vec2)));

	// safety
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indices;
	delete[] vertices;
}

int LightingApp::TextEditCallBackStub(ImGuiTextEditCallbackData* data)
{
	auto app = static_cast<LightingApp*>(data->UserData);
	return app->TextEditCallback(data);
}

int LightingApp::TextEditCallback(ImGuiTextEditCallbackData* data)
{
	auto sd = static_cast<ShaderData*>(data->UserData);
	auto shader = sd->shader;
	shader->load(sd->source, sd->type, sd->isFile);
	shader->attach();
	return 1;
}

void LightingApp::loadTexture()
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);
}
void LightingApp::onGUI()
{
	auto fps_window = true;
	ImGui_ImplGlfwGL3_NewFrame();
#pragma region Fps_Window
	ImGui::Begin("FPS", &fps_window);
	ImGui::SetWindowPos(ImVec2(0, 130));
	ImGui::Text("Application FPS (%.1f FPS)", ImGui::GetIO().Framerate);
	ImGui::End();
#pragma endregion

#pragma region Color
	ImGui::Begin("Color");
	ImGui::SetWindowPos(ImVec2(0, 180));
	ImGui::ColorEdit3("Clear color", reinterpret_cast<float*>(&clear_color));
	ImGui::ColorEdit3("Light color", reinterpret_cast<float*>(&ball_color));
	m_ambientLight = vec3(ball_color.x, ball_color.y, ball_color.z);
	setBackgroundColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	ImGui::End();
#pragma endregion

#pragma region Rotate

	ImGui::Begin("Light Position");
	ImGui::SetWindowPos(ImVec2(0, 265));
	ImGui::BeginGroup();
	if (ImGui::Button("Rotate Light"))
	{
		rotate = true;
		rotateSpeed -= .1;
		m_directLight.direction = normalize(vec3(sinf(runTime / rotateSpeed), 0, cosf(runTime / rotateSpeed)));
	}
	ImGui::SameLine();
	ImGui::Text("Runtime / Speed value (%f)", rotateSpeed);
	if (ImGui::Button("Pause Rotation"))
	{
		rotate = false;
		m_directLight.direction = normalize(vec3(0, 1, 0));
	}
	//ImGui::BeginGroup();
	//if (ImGui::Button("Light at Bottom"))
	//{ 
	//	rotate = false;
	//	m_directLight.direction = normalize(vec3(0, 10, 0));
	//}
	//ImGui::SameLine();
	//if (ImGui::Button("Light at Right"))
	//{
	//	rotate = false;
	//	m_directLight.direction = normalize(vec3(0, 0, 10));
	//}
	//ImGui::BeginGroup();
	//ImGui::EndGroup();
	//if (ImGui::Button("Light at Left"))
	//{
	//	rotate = false;
	//	m_directLight.direction = normalize(vec3(10, 0, 0));
	//}
	//ImGui::SameLine();
	//if (ImGui::Button("Light at Top"))
	//{
	//	rotate = false;
	//	m_directLight.direction = normalize(vec3(0, -10, 0));
	//}
	ImGui::DragFloat3("Light Direction", &m_directLight.direction[0], 0.01f, -1, 1);
	ImGui::DragFloat("Specular Power", &m_material.specularPower, 1, 0, 99999);
	ImGui::EndGroup();
	ImGui::End();
	//ImGui::ShowTestWindow();
#pragma endregion 

#pragma region Fill/ Line
	ImGui::Begin("Fill / Line");
	ImGui::SetWindowPos(ImVec2(0, 390));
	if (ImGui::Checkbox("Fill", &fill))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//if (ImGui::DragInt("Rows", &m_rows, 1, 0, 100) || ImGui::DragInt("Colm", &m_cols, 1, 0, 100))
	//{
	//	gridChanged = true;
	//	if (gridChanged == true)
	//	{
	//		gridMesh = generateGrid(m_rows, m_cols);
	//		gridMesh->Create_Buffers();
	//	}
	//}

	ImGui::End();
#pragma endregion

#pragma region Menu
	if (ImGui::BeginMainMenuBar())
	{
		float tex_w = static_cast<float>(ImGui::GetIO().Fonts->TexWidth);
		float tex_h = static_cast<float>(ImGui::GetIO().Fonts->TexHeight);
		ImTextureID tex_id = ImGui::GetIO().Fonts->TexID;
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::BeginMenu("Load Frag"))
			{
				if (ImGui::MenuItem("Hemi Frag"))
				{
					shader->load("hemi.frag", GL_FRAGMENT_SHADER, true);
					shader->attach();
				}
				if (ImGui::MenuItem("Phong Frag"))
				{
					shader->load("phong.frag", GL_FRAGMENT_SHADER, true);
					shader->attach();
				}
				if (ImGui::MenuItem("Blinn Phong Frag"))
				{
					shader->load("blinnphong.frag", GL_FRAGMENT_SHADER, true);
					shader->attach();
				}
				ImGui::EndMenu();
			}

			//if (ImGui::MenuItem("Restart")) { startup(); }
			//if (ImGui::IsItemHovered())
			//	ImGui::SetTooltip("Restart if you are deep");

			if (ImGui::MenuItem("Quit", "Alt+F4")) { glfwSetWindowShouldClose(m_window, true); }
			static auto n = 2;
			ImGui::Combo("Are you amazing?", &n, "Yes\0No\0");
			if (n == 1)
				glfwSetWindowShouldClose(m_window, true);
			else if (n == 0)
			{
				ImGui::Text("%.0fx%.0f", tex_w, tex_h);
				ImGui::Image(tex_id, ImVec2(tex_w, tex_h), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
			}
			ImGui::EndMenu();
		}
		ImGui::SetNextWindowSize(ImVec2(500, 500));
		if (ImGui::BeginMenu("Phong Frag"))
		{
			auto sd = ShaderData{ shader, fbuffer, GL_FRAGMENT_SHADER, false };
			ImGui::InputTextMultiline("Frag Shader", fbuffer, sizeof fbuffer, ImGui::GetWindowSize(),
				ImGuiInputTextFlags_CallbackAlways, TextEditCallBackStub, static_cast<void*>(&sd));
			ImGui::EndMenu();
		}
		ImGui::SetNextWindowSize(ImVec2(200, 200));
		if (ImGui::BeginMenu("Image to Load"))
		{
			if (ImGui::MenuItem("Blue"))
			{
				data = stbi_load("./textures/blue.png", &imageWidth, &imageHeight, &imageFormat, STBI_default);
				loadTexture();
			}
			if (ImGui::MenuItem("Earth"))
			{
				data = stbi_load("./textures/test.png", &imageWidth, &imageHeight, &imageFormat, STBI_default);
				loadTexture();
			}
			if (ImGui::MenuItem("Another Earth"))
			{
				data = stbi_load("./textures/world.png", &imageWidth, &imageHeight, &imageFormat, STBI_default);
				loadTexture();
			}
			if (ImGui::MenuItem("Brick"))
			{
				data = stbi_load("./textures/brick.png", &imageWidth, &imageHeight, &imageFormat, STBI_default);
				loadTexture();
			}
			if (ImGui::MenuItem("Wood"))
			{
				data = stbi_load("./textures/woodd.png", &imageWidth, &imageHeight, &imageFormat, STBI_default);
				loadTexture();
			}
			if (ImGui::MenuItem("Class"))
			{
				data = stbi_load("./textures/class.png", &imageWidth, &imageHeight, &imageFormat, STBI_default);
				loadTexture();
			}
			if (ImGui::MenuItem("Room"))
			{
				data = stbi_load("./textures/room.png", &imageWidth, &imageHeight, &imageFormat, STBI_default);
				loadTexture();
			}
			if (ImGui::MenuItem("Mountain"))
			{
				data = stbi_load("./textures/mountain.png", &imageWidth, &imageHeight, &imageFormat, STBI_default);
				loadTexture();
			}
			if (ImGui::MenuItem("River"))
			{
				data = stbi_load("./textures/river.png", &imageWidth, &imageHeight, &imageFormat, STBI_default);
				loadTexture();
			}
			if (ImGui::MenuItem("Sunset at Pier"))
			{
				data = stbi_load("./textures/sunset.png", &imageWidth, &imageHeight, &imageFormat, STBI_default);
				loadTexture();
			}
			if (ImGui::MenuItem("Game"))
			{
				data = stbi_load("./textures/game.png", &imageWidth, &imageHeight, &imageFormat, STBI_default);
				loadTexture();
			}
			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();
#pragma endregion
}
Mesh* LightingApp::generateGrid(unsigned int rows, unsigned int cols)
{
	auto aoVertices = new Vertex[rows * cols];
	for (unsigned int r = 0; r < rows; ++r)
	{
		for (unsigned int c = 0; c < cols; ++c)
		{
			aoVertices[r * cols + c].position = vec4((float)c, 0, (float)r, 1);
			//Create some arbitrary color based off something
			//that might not be related to tiling a tecture
			vec3 colour = vec3(sinf((c / (float)(cols - 1)) * (r / (float)(rows - 1))));
			aoVertices[r * cols + c].color = vec4(colour, 1);
		}
	}

	vector<Vertex> verts = vector<Vertex>();
	vector<unsigned int> indices = vector<unsigned int>();

	//Defining index count based off quad count (2 triangles per quad)
	unsigned int* auiIndices = new unsigned int[(rows - 1) * (cols - 1) * 6];
	unsigned int index = 0;
	for (unsigned int r = 0; r < (rows - 1); ++r)
	{
		for (unsigned int c = 0; c < (cols - 1); ++c)
		{
			//Triangle 1
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			//Triangle 2
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			auiIndices[index++] = r * cols + (c + 1);
		}
	}
	//Create and bind buffers to a vertex array object
	m_rows = rows;
	m_cols = cols;

	for (unsigned int i = 0; i < (rows * cols); i++)
		verts.push_back(aoVertices[i]);
	for (unsigned int i = 0; i < index; i++)
		indices.push_back(auiIndices[i]);

	gridMesh->initialize(verts, indices);
	gridMesh->bind();

	delete[] aoVertices;
	delete[] auiIndices;
	return gridMesh;
}


bool LightingApp::startup()
{
	setBackgroundColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	cam->setLookAt(cam->m_posvec3, vec3(0), vec3(0, 1, 0));

	m_directLight.diffuse = vec3(0, 0, 0);
	m_directLight.specular = vec3(1);
	m_ambientLight = vec3(ball_color.x = 1, ball_color.y = 1, ball_color.z = 1);

	m_material.diffuse = vec3(1);
	m_material.ambient = vec3(1);
	m_material.specular = vec3(1);

	m_material.specularPower = 30;
	segments = 100;
	rings = 100;
	generateSphere(segments, rings, m_VAO, m_VBO, m_IBO, index_count);

	m_rows = 10;
	m_cols = 10;
	gridMesh = generateGrid(m_rows, m_cols);
	gridMesh->Create_Buffers();

	shader->load("phong.vert", GL_VERTEX_SHADER, true);
	shader->load("phong.frag", GL_FRAGMENT_SHADER, true);
	shader->attach();

	memmove(fbuffer, shader->fsSource, 5000);

	m_modelMatrix = scale(vec3(5));

	data = stbi_load("./textures/test.png", &imageWidth, &imageHeight, &imageFormat, STBI_default);
	loadTexture();

	ImGui_ImplGlfwGL3_Init(m_window, true);
	auto& io = ImGui::GetIO();
	io.DisplaySize.x = 1600;
	io.DisplaySize.y = 900;
	return false;
}

bool LightingApp::shutdown()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) || m_GameOver)
		glfwSetWindowShouldClose(m_window, true);
	return false;
}

bool LightingApp::update(float deltaTime)
{
	runTime += deltaTime;

	if (ImGui::IsAnyItemActive() != true || ImGui::IsAnyItemHovered() != true)
	{
		camapp->Keyboard_Movement(cam, m_window);
		camapp->Mouse_Movement(cam, m_window);
	}

	if (rotate == true)
		m_directLight.direction = normalize(vec3(sinf(runTime / rotateSpeed), 0, cosf(runTime / rotateSpeed)));

	onGUI();

	return false;
}

bool LightingApp::draw()
{
	int matUniform = shader->getUniform("ProjectionViewModel");
	int lightUniform = shader->getUniform("direction");
	unsigned int crate = shader->getUniform("image");

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	if (fill)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	shader->bind();
	mat4 pvm = cam->getProjectionView() * m_modelMatrix;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glUniform1i(crate, 0);

	glUniformMatrix4fv(matUniform, 1, GL_FALSE, &pvm[0][0]);

	glUniform3fv(lightUniform, 1, value_ptr(m_directLight.direction));

	lightUniform = shader->getUniform("Id");
	glUniform3fv(lightUniform, 1, value_ptr(m_directLight.diffuse));

	lightUniform = shader->getUniform("Ia");
	glUniform3fv(lightUniform, 1, value_ptr(m_ambientLight));

	lightUniform = shader->getUniform("Is");
	glUniform3fv(lightUniform, 1, value_ptr(m_directLight.specular));

	lightUniform = shader->getUniform("Ka");
	glUniform3fv(lightUniform, 1, value_ptr(m_material.ambient));

	lightUniform = shader->getUniform("Kd");
	glUniform3fv(lightUniform, 1, value_ptr(m_material.diffuse));

	lightUniform = shader->getUniform("Ks");
	glUniform3fv(lightUniform, 1, value_ptr(m_material.specular));

	lightUniform = shader->getUniform("a");
	glUniform1f(lightUniform, m_material.specularPower);

	lightUniform = shader->getUniform("camPos");
	glUniform3fv(lightUniform, 1, value_ptr(cam->m_posvec3));

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr);


	gridMesh->bind();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUniformMatrix4fv(matUniform, 1, false, value_ptr(cam->getProjectionView() * translate(vec3(-4.5f, -2, -4.5f))));
	gridMesh->draw(GL_TRIANGLES);
	gridMesh->unbind();

	shader->unbind();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	ImGui::Render();
	return false;
}
