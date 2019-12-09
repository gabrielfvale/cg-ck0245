#define STB_IMAGE_IMPLEMENTATION

#include <cstdio>
#include <string>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Shader directionalShadowShader;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture beigePaint;
Texture whitePlastic;

Material shinyMaterial;
Material dullMaterial;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,uniformSpecularIntensity = 0, uniformShininess = 0;

// Vertex shader
static const char* vShader = "/home/gabrielfv/Projects/cg-ck0245/src/Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "/home/gabrielfv/Projects/cg-ck0245/src/Shaders/shader.frag";

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

void CreateObjects() 
{
	unsigned int indices[] = {		
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
	//	x      y      z			u	  v			nx	  ny    nz
		-1.0f, -1.0f, -0.6f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	/*
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	*/
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh("/home/gabrielfv/Projects/cg-ck0245/src/Models/RoomWindow.obj");
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh("/home/gabrielfv/Projects/cg-ck0245/src/Models/lampFlat.obj");
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh("/home/gabrielfv/Projects/cg-ck0245/src/Models/AC.obj");
	meshList.push_back(obj3);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

  directionalShadowShader = Shader();
  directionalShadowShader.CreateFromFiles(
    "/home/gabrielfv/Projects/cg-ck0245/src/Shaders/directional_shadowmap.vert",
    "/home/gabrielfv/Projects/cg-ck0245/src/Shaders/directional_shadowmap.frag"
  );
}

void RenderScene(GLfloat deltaTime)
{
  glm::mat4 model = glm::mat4(1.0);	

  model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
  //model = glm::scale(model, glm::vec3(1.0f, 1.0f, 2.0f));
  glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
  
  beigePaint.UseTexture();
  dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
  meshList[0]->RenderMesh();

  model = glm::translate(model, glm::vec3(-1.5f, 3.0f, -1.0f));
  model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
  glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
  
  plainTexture.UseTexture();
  dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
  meshList[1]->RenderMesh();

  model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
  glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
  
  plainTexture.UseTexture();
  dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
  meshList[1]->RenderMesh();

  model = glm::translate(model, glm::vec3(0.0f, 0.0f, 4.0f));
  glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
  
  plainTexture.UseTexture();
  dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
  meshList[1]->RenderMesh();

  model = glm::translate(model, glm::vec3(-6.0f, 0.0f, 0.0f));
  glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
  
  plainTexture.UseTexture();
  dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
  meshList[1]->RenderMesh();

  model = glm::mat4(1.0);	

  model = glm::translate(model, glm::vec3(-1.5f, 2.5f, -1.5f));
  glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
  
  whitePlastic.UseTexture();
  dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
  meshList[2]->RenderMesh();

  /*
  model = glm::mat4(1.0);	
  model = glm::translate(model, glm::vec3(0.0f, 3.0f, 2.5f));
  //model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.8f));
  glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
  dirtTexture.UseTexture();
  dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
  meshList[1]->RenderMesh();

  model = glm::mat4(1.0);	
  //model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.8f));
  glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
  plainTexture.UseTexture();
  shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
  meshList[2]->RenderMesh();

  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, now, glm::vec3(0.0f, 1.0f, 0.0f));
  glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
  plainTexture.UseTexture();
  dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
  meshList[3]->RenderMesh();
  */
}

void DirectionalShadowMapPass(DirectionalLight* light, GLfloat deltaTime)
{
  ShadowMap* map = light->GetShadowMap();
  directionalShadowShader.UseShader();
  glViewport(0, 0, map->GetShadowWidth(), map->GetShadowHeight());
  light->GetShadowMap()->Write();
  glClear(GL_DEPTH_BUFFER_BIT);
  uniformModel = directionalShadowShader.GetModelLocation();
  glm::mat4 lightTransform = light->CalcLightTransform();
  directionalShadowShader.SetDirectionalLightTransform(&lightTransform);
  
  RenderScene(deltaTime);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, GLfloat deltaTime)
{
  shaderList[0].UseShader();
  uniformModel = shaderList[0].GetModelLocation();
  uniformProjection = shaderList[0].GetProjectionLocation();
  uniformView = shaderList[0].GetViewLocation();
  uniformEyePosition = shaderList[0].GetEyePositionLocation();
  uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
  uniformShininess = shaderList[0].GetShininessLocation();

  glViewport(0, 0, mainWindow.getBufferWidth(), mainWindow.getBufferHeight());
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
  glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
  glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

  glm::mat4 lightTransform = mainLight.CalcLightTransform();
  shaderList[0].SetDirectionalLight(&mainLight);
  shaderList[0].SetPointLights(pointLights, pointLightCount);
  shaderList[0].SetSpotLights(spotLights, spotLightCount);
  shaderList[0].SetDirectionalLightTransform(&lightTransform);

  mainLight.GetShadowMap()->Read(GL_TEXTURE1);
  shaderList[0].SetTexture(0);
  shaderList[0].SetDirectionalShadowMap(1);

  RenderScene(deltaTime);
}

int main() 
{
	/*
	if (glewGetExtension("GL_ARB_multitexture") != GL_TRUE) {
		std::cerr << "Driver does not support GL_ARB_multitexture" << std::endl;
		exit(1);
	}*/	

	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f, 5.0f, 0.5f);

	//brickTexture = Texture("/home/ceos/Documents/Batista/LearningOpenGL/1-Introduction/Models/src/Textures/brick.png");
	brickTexture = Texture("/home/gabrielfv/Projects/cg-ck0245/src/Textures/brick.png");
	brickTexture.LoadTextureA();
	//dirtTexture = Texture("/home/ceos/Documents/Batista/LearningOpenGL/1-Introduction/Models/src/Textures/dirt.png");
	dirtTexture = Texture("/home/gabrielfv/Projects/cg-ck0245/src/Textures/dirt.png");
	dirtTexture.LoadTextureA();

	//plainTexture = Texture("/home/ceos/Documents/Batista/LearningOpenGL/1-Introduction/Models/src/Textures/plain.png");
	plainTexture = Texture("/home/gabrielfv/Projects/cg-ck0245/src/Textures/plain.png");
	plainTexture.LoadTextureA();

	beigePaint = Texture("/home/gabrielfv/Projects/cg-ck0245/src/Textures/beigePaint.PNG");
	beigePaint.LoadTexture();

	whitePlastic = Texture("/home/gabrielfv/Projects/cg-ck0245/src/Textures/whitePlastic.PNG");
	whitePlastic.LoadTextureA();

	shinyMaterial = Material(4.0f, 256);
	dullMaterial = Material(0.3f, 4);

	mainLight = DirectionalLight(
    1024, 1024,
    1.0f, 1.0f, 1.0f, 
    0.2f, 0.3f, 
    -4.0f, -4.0f, 0.0f);

	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f, 
								0.1f, 0.1f,
								-1.5f, 3.0f, -1.0f,
								0.2f, 0.1f, 0.1f);
	pointLightCount++;


	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f, 
								0.1f, 0.1f,
								1.5f, 3.0f, -1.0f,
								0.2f, 0.1f, 0.1f);
	pointLightCount++;

	pointLights[2] = PointLight(1.0f, 1.0f, 1.0f, 
								0.1f, 0.1f,
								1.5f, 3.0f, 1.0f,
								0.2f, 0.1f, 0.1f);
	pointLightCount++;

	pointLights[3] = PointLight(1.0f, 1.0f, 1.0f, 
								0.1f, 0.1f,
								-1.5f, 3.0f, 1.0f,
								0.2f, 0.1f, 0.1f);
	pointLightCount++;

	/*
	pointLights[1] = PointLight(0.0f, 1.0f, 0.0f, 
								0.1f, 1.0f,
								4.0f, 0.0f, 0.0f,
								0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[2] = PointLight(1.0f, 0.0f, 0.0f, 
								0.1f, 1.0f,
								0.0f, 2.5f, 0.0f,
								0.3f, 0.2f, 0.1f);
	pointLightCount++;
	*/

	spotLights[0] = SpotLight(1.0f, 0.0f, 0.0f,
						0.0f, 0.0f,
						0.0f, 3.0f, 0.0f,
						0.0f, -1.0f, 0.0f,
						0.3f, 0.2f, 0.1f,
						20.0f);
	spotLightCount++;

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		// Gets the current time and updates DeltaTime and LastTime
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime; 
		lastTime = now;

		// Get + Handle User Input
		glfwPollEvents();

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

    DirectionalShadowMapPass(&mainLight, now);
    RenderPass(projection, camera.calculateViewMatrix(), now);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

  glfwTerminate();

	return 0;
}
