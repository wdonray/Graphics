#include "TextureApplication.h"
#include "CameraApp.h"
#include <gl_core_4_4.h>
#include "Camera.h"
#include "Shader.h"
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <Mesh.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <time.h>

#define maxPrimeIndex 10
int primeIndex = 0;

int primes[maxPrimeIndex][3] = {
	{995615039, 600173719, 701464987},
	{831731269, 162318869, 136250887},
	{174329291, 946737083, 245679977},
	{362489573, 795918041, 350777237},
	{457025711, 880830799, 909678923},
	{787070341, 177340217, 593320781},
	{405493717, 291031019, 391950901},
	{458904767, 676625681, 424452397},
	{531736441, 939683957, 810651871},
	{997169939, 842027887, 423882827}
};

TextureApplication::TextureApplication() : m_rows(0), m_cols(0), imageWidth(0), imageHeight(0), imageFormat(0), data(nullptr), runTime(0), m_textureID(0), textureLoad(new char), m_perlinTexture(0)
{
	cam = new Camera();
	camapp = new CameraApp();
	shader = new Shader();
	plane = new Mesh();
}


TextureApplication::~TextureApplication()
{
	delete camapp;
	delete cam;
	delete shader;
	delete plane;
}

Mesh* TextureApplication::generateGrid(unsigned int rows, unsigned int cols)
{
	auto aoVertices = new Vertex[rows * cols];
	for (unsigned int r = 0; r < rows; ++r)
	{
		for (unsigned int c = 0; c < cols; ++c)
		{
			Vertex verts = {
				vec4(float(c), 0, float(r), 1),
				vec4(sin(r), cos(c), 0, 1),
				vec4(0, 1, 0, 0),
				vec2(float(c) / float(cols - 1),
				     float(r) / float(rows - 1))
			};
			aoVertices[r * cols + c] = verts;
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

	plane->initialize(verts, indices);
	plane->bind();

	delete[] aoVertices;
	delete[] auiIndices;
	return plane;
}

bool TextureApplication::startup()
{
	srand(time(nullptr));
	setBackgroundColor(0, 0, 1.0f, 1);
	cam->setLookAt(vec3(100, 100, 100), vec3(0), vec3(0, 1, 0));

	shader->load("perlin.vert", GL_VERTEX_SHADER, true);
	shader->load("perlin.frag", GL_FRAGMENT_SHADER, true);
	shader->attach();

	m_rows = 64 , m_cols = 64;
	plane = generateGrid(m_rows, m_cols);
	plane->Create_Buffers();

	PerlinTest();
	////data = stbi_load("./textures/donray.png", &imageWidth, &imageHeight, &imageFormat, STBI_default);
	//glGenTextures(1, &m_textureID);
	//glBindTexture(GL_TEXTURE_2D, m_textureID);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);

	ImGui_ImplGlfwGL3_Init(m_window, true);
	auto& io = ImGui::GetIO();
	io.DisplaySize.x = 1600;
	io.DisplaySize.y = 900;
	return false;
}

bool TextureApplication::shutdown()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) || m_GameOver)
		glfwSetWindowShouldClose(m_window, true);
	return false;
}


void TextureApplication::PerlinTest()
{
	float* perlinData = new float[m_rows * m_cols];
	float scale = (1.0f / m_rows) * 3;
	int octaves = 6;
	for (int x = 0; x < m_rows; ++x)
	{
		for (int y = 0; y < m_cols; ++y)
		{
			float amplitude = 1.f, persistence = 0.3f;
			perlinData[y * m_rows + x] = 0;
			for (int o = 0; o < octaves; ++o)
			{
				float freq = powf(2, float(o));
				//float perlinSample = DonrayNoise(vec2(float(x), float(y)) * scale * freq , 0)  * 0.5f + 0.5f;
				//float perlinSample = perlin(vec2(float(x), float(y)) * scale * freq)  * 0.5f + 0.5f;
				float perlinSample = InterpolatedNoise(vec3(float(x), float(y), 1) * scale * freq, 350) * 0.5f + 0.5f;
				perlinData[y * m_rows + x] += perlinSample * amplitude;
				amplitude *= persistence;
			}
		}
	}
	glGenTextures(1, &m_perlinTexture);
	glBindTexture(GL_TEXTURE_2D, m_perlinTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, m_rows, m_cols, 0, GL_RED, GL_FLOAT, perlinData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

double TextureApplication::DonrayNoise(vec2 pos, int i)
{
	//Get random prime number
	auto a = primes[i][rand() % 10], b = primes[i][rand() % 10], c = primes[i][rand() % 10];
	// From Canvas Slides
	int n = pos.x + pos.y * 57;
	n = (n << 13) ^ n;
	auto nn = (n * (n * n * a + b) + c) & 0x7fffffff;
	return 1.0 - (double(nn) / 1073741824.0);
}

double TextureApplication::Interpolate(vec3 pos)
{
	double ft = pos.z * glm::length(pos);
	double f = (2.0 - cos(ft)) * 0.5f;
	return pos.x * (1.0 - f) + pos.y * f;
}

double TextureApplication::InterpolatedNoise(vec2 pos, int value)
{
	//Get the surrounding pixels to calculate the transition.
	double v1 = DonrayNoise(vec3(pos.x, pos.y, 0), 0),
		v2 = DonrayNoise(vec3(pos.x + value, pos.y, 0), 0),
		v3 = DonrayNoise(vec3(pos.x, pos.y + value, 0), 0),
		v4 = DonrayNoise(vec3(pos.x + value, pos.y + value, 0), 0);
	//Interpolate between the values.
	double int1 = Interpolate(vec3(v1, v2, pos.x - pos.y)),
		int2 = Interpolate(vec3(v3, v4, pos.x - pos.y));

	return Interpolate(vec3(int1, int2, 0));
}

void TextureApplication::OnGUI() const
{
	auto fps_window = true;
	ImGui_ImplGlfwGL3_NewFrame();
	ImGui::Begin("FPS", &fps_window);
	ImGui::SetWindowPos(ImVec2(0, 130));
	ImGui::Text("Application FPS (%.1f FPS)", ImGui::GetIO().Framerate);
	ImGui::End();

	//ImGui::Begin("Load Image");
	//ImGui::SetWindowPos(ImVec2(0, 190));
	//ImGui::InputText("Texture", textureLoad, 50);
	//ImGui::End();

	//if (glfwGetKey(m_window, GLFW_KEY_SPACE))
	//{
	//	data = stbi_load(textureLoad, &imageWidth, &imageHeight, &imageFormat, STBI_default);
	//	glGenTextures(1, &m_textureID);
	//	glBindTexture(GL_TEXTURE_2D, m_textureID);
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//	stbi_image_free(data);
	//}
}

bool TextureApplication::update(float deltaTime)
{
	runTime += deltaTime;
	camapp->Keyboard_Movement(cam, m_window);
	camapp->Mouse_Movement(cam, m_window);
	//if (glfwGetKey(m_window, '9') == GLFW_PRESS)
	//{
	//	data = stbi_load("./textures/dkbutt.png", &imageWidth, &imageHeight, &imageFormat, STBI_default);
	//	glGenTextures(1, &m_textureID);
	//	glBindTexture(GL_TEXTURE_2D, m_textureID);
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//	stbi_image_free(data);
	//}
	//if (glfwGetKey(m_window, '0') == GLFW_PRESS)
	//{
	//	data = stbi_load("./textures/donray.png", &imageWidth, &imageHeight, &imageFormat, STBI_default);
	//	glGenTextures(1, &m_textureID);
	//	glBindTexture(GL_TEXTURE_2D, m_textureID);
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//	stbi_image_free(data);
	//}
	OnGUI();
	return false;
}

bool TextureApplication::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	unsigned int projectionViewUniform = shader->getUniform("ProjectionViewModel");
	unsigned int time = shader->getUniform("time");
	unsigned int crate = shader->getUniform("textcrate");
	unsigned int perlin = shader->getUniform("perlinTexture");

	shader->bind();

	mat4 pvm = cam->getProjectionView();
	glUniformMatrix4fv(projectionViewUniform, 1, GL_FALSE, &pvm[0][0]);
	glUniform1f(time, glfwGetTime());
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_textureID);

	//draw
	plane->bind();
	glUniform1i(perlin, 0);
	plane->draw(GL_TRIANGLES);
	plane->unbind();

	shader->unbind();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	ImGui::Render();
	return false;
}
