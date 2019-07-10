#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "CommonValues.h"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "Model.h"

#include <assimp/Importer.hpp>

#include "CameraLight.h"


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Shader directionalShadowShader;
Camera camera;

Texture plainTexture;

Material shinyMaterial;
Material dullMaterial;

Model House;

DirectionalLight mainLight;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
uniformSpecularIntensity = 0, uniformShininess = 0;
GLfloat sumaX = 0.0f, sumaY = 0.0f;
std::vector<GLfloat> xChanges, yChanges;


// Vertex Shaders
static const char* vShader = "core.vert"; 
static const char* dlvShader = "directional_shadow_map.vert";

// Fragment Shaders
static const char* fShader = "core.frag"; 
static const char* dlfShader = "directional_shadow_map.frag";

void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	directionalShadowShader = Shader();
	directionalShadowShader.CreateFromFiles(dlvShader, dlfShader);
}


void RenderScene() {
	glm::mat4 model;
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, 10.0f, 10.0f));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	model = glm::rotate(model, TO_RADIANS * 45, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	House.RenderModel();
}

void DirectionalShadowMapPass(DirectionalLight* light) {
	directionalShadowShader.UseShader();

	glViewport(0, 0, light->getShadowMap()->GetShadowWidth(), light->getShadowMap()->GetShadowHeight());

	light->getShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader.GetModelLocation();
	directionalShadowShader.SetDirectionalLightTransform(&light->CalculateLightTransform());

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	shaderList[0].UseShader();

	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformModel = shaderList[0].GetModelLocation();
	uniformEyePosition = shaderList[0].GetEyePositionLocation();
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	uniformShininess = shaderList[0].GetShininessLocation();

	glViewport(0, 0, 1280, 720);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

	shaderList[0].SetDirectionalLight(&mainLight);
	shaderList[0].SetDirectionalLightTransform(&mainLight.CalculateLightTransform());

	mainLight.getShadowMap()->Read(GL_TEXTURE1);
	shaderList[0].SetTexture(0);
	shaderList[0].SetDirectionalShadowMap(1);
	RenderScene();
}


int main()
{
	mainWindow = Window(1280, 720); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateShaders();

	CameraLight lightCords;
	lightCords.Start();

	camera = Camera(glm::vec3(0.0f, 50.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, -20.0f, 50.0f, 0.5f);

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();

	shinyMaterial = Material(4.0f, 256);
	dullMaterial = Material(0.3f, 4);

	House = Model();

	if (House.LoadModel("Models/city.obj"))
	{
		std::cout << "loaded";
	}
	xChanges.resize(10); yChanges.resize(10);

	mainLight = DirectionalLight(8192, 8192,
		1.0f, 1.0f, 1.0f,
		0.2f, 0.3f,
		26.0f, 5.0f, -20.0f);

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 250.0f);
	float curx, cury, curz;
	curx = 15.0f; cury = -15.0f;

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;
		lightCords.Calculate();

		glfwPollEvents();

		bool *keys = mainWindow.getKeys();
		camera.keyControl(keys, deltaTime);
		xChanges.erase(xChanges.begin(), xChanges.begin() + 1);
		yChanges.erase(yChanges.begin(), yChanges.begin() + 1);
		xChanges.push_back(-lightCords.xLight / 75.0f);
		yChanges.push_back(lightCords.yLight / 75.0f);
		sumaX = sumaY = 0;
		for (int i = 0; i < xChanges.size(); i++) {
			sumaX += xChanges[i];
			sumaY += yChanges[i];
		}
		sumaX /= xChanges.size();
		sumaY /= yChanges.size();
		mainLight.SetLightDirection(sumaX/1.75f, -40.0f, sumaY/1.75f);
		mainLight.SetLightColor(lightCords.red, lightCords.green, lightCords.blue);
		mainLight.SetLightIntense(lightCords.brightnessAv/750.0f, lightCords.brightnessLight/750.0f);

		DirectionalShadowMapPass(&mainLight);
		RenderPass(projection, camera.calculateViewMatrix());

		mainWindow.swapBuffers();
	}
	return 0;
}