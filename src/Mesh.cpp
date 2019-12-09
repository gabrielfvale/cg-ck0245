#include "Mesh.h"

#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>
#include <cstdio>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

using namespace std;

Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	EBO = 0;
	indexCount = 0;
}

void Mesh::CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
	indexCount = numOfIndices;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Mesh::CreateMesh(const char* objPath)
{
  this->loadObj(objPath);

  this->indexCount = indices.size();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

  // Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  glEnableVertexAttribArray(0);
  // UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
  glEnableVertexAttribArray(1);
  // Normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
  glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Mesh::RenderMesh()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::ClearMesh()
{
	if (EBO != 0)
	{
		glDeleteBuffers(1, &EBO);
		EBO = 0;
	}

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	indexCount = 0;
}

void Mesh::loadObj(const char* objPath)
{
  vector<unsigned int> vertexIndices, uvIndices, normalIndices;
  vector<glm::vec3> vertices;
  vector<glm::vec2> uvs;
  vector<glm::vec3> normals;

  FILE* file = fopen(objPath, "r");
  if(!file)
  {
    cerr << "Cannot open OBJ: \n" << objPath << endl;
    exit(1);
  }

  while(true)
  {
    char header[128];
    int res = fscanf(file, "%s", header);
    if(res == EOF) break;

    if(strcmp(header, "v") == 0) // Vertices
    {
      glm::vec3 vertex;
      fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
      vertices.push_back(vertex);
    } else if(strcmp(header, "vt") == 0) // UVs
    {
      glm::vec2 uv;
      fscanf(file, "%f %f\n", &uv.x, &uv.y);
      uvs.push_back(uv);
    } else if(strcmp(header, "vn") == 0) // Normals
    {
      glm::vec3 normal;
      fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
      normal.x *= -1; normal.y *= -1; normal.z *= -1;
      normals.push_back(normal);
    } else if(strcmp(header, "f") == 0) // Faces
    {
      string vertex1, vertex2, vertex3;
      unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
      int matches = fscanf(file, 
        "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
        &vertexIndex[0], &uvIndex[0], &normalIndex[0], 
        &vertexIndex[1], &uvIndex[1], &normalIndex[1], 
        &vertexIndex[2], &uvIndex[2], &normalIndex[2]
      );
      if(matches != 9)
      {
        cerr << "Invalid OBJ format" << endl;
        exit(1);
      }
      for(int i = 0; i < 3; i++)
      {
        vertexIndices.push_back(vertexIndex[i]);
        uvIndices.push_back(uvIndex[i]);
        normalIndices.push_back(normalIndex[i]);
      }
    }
  }
  for(unsigned int i = 0; i < vertexIndices.size(); i++)
  {
    unsigned int vertexIndex = vertexIndices[i] - 1;
    unsigned int uvIndex = uvIndices[i] - 1;
    unsigned int normalIndex = normalIndices[i] - 1;
    Vertex vertex;
    vertex.pos = vertices[vertexIndex];
    vertex.uv = uvs[uvIndex];
    vertex.normal = normals[normalIndex];
    this->vertices.push_back(vertex);
    this->indices.push_back(i);
  }
}

Mesh::~Mesh()
{
	ClearMesh();
}
