#include "Sphere.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sphere::Sphere(float radius, int sectorCount, int stackCount)
	: radius(radius), sectorCount(sectorCount), stackCount(stackCount)
{
	createSphere();
	normalizeVertices();
	setupMesh();
}
// -------------------------------------------
Sphere::~Sphere()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}
// -------------------------------------------
void Sphere::Render(Shader& shader, const glm::mat4 view, const glm::mat4 projection, const glm::vec3 lightPos)
{
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, -lightPos);
	lightModel = glm::scale(lightModel, glm::vec3(0.05f));

	shader.use();
	shader.setMat4("model", lightModel);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);

	draw(shader);
}
void Sphere::draw(Shader& shader) const
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
}
// -------------------------------------------
void Sphere::createSphere(void)
{
	const float PI = 3.1415926f;

	for (int i = 0; i <= stackCount; ++i)
	{
		float stackAngle = PI / 2 - i * PI / stackCount;
		float xy = radius * cosf(stackAngle);
		float z = radius * sinf(stackAngle);

		for (int j = 0; j <= sectorCount; ++j)
		{
			float sectorAngle = j * 2 * PI / sectorCount;

			float x = xy * cosf(sectorAngle);
			float y = xy * sinf(sectorAngle);

			vertices.push_back(x);

			vertices.push_back(y);
			vertices.push_back(z);
		}
	} // for

	for (int i = 0; i < stackCount; ++i)
	{
		int k1 = i * (sectorCount + 1);
		int k2 = k1 + sectorCount + 1;

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			if (i != (stackCount - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	} // for
}
// -------------------------------------------
void Sphere::normalizeVertices(void)
{
	std::vector<float> res;
	for (size_t i = 0; i < vertices.size(); i += 3) {
		glm::vec3 pos(vertices[i], vertices[i + 1], vertices[i + 2]);
		res.push_back(pos.x);
		res.push_back(pos.y);
		res.push_back(pos.z);

		glm::vec3 normal = glm::normalize(pos);
		res.push_back(normal.x);
		res.push_back(normal.y);
		res.push_back(normal.z);
	}

	vertices.clear();
	vertices = res;
}
// -------------------------------------------
void Sphere::setupMesh(void)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	// 위치 (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}
// -------------------------------------------