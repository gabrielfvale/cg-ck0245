#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Vertex {
  glm::vec3 pos;
  glm::vec2 uv;
  glm::vec3 normal;
};

class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
  void CreateMesh(const char* objPath);
	void RenderMesh();
	void ClearMesh();

	~Mesh();

private:
	GLuint VAO, VBO, EBO;
	GLsizei indexCount;
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  void loadObj(const char* objPath);
};
