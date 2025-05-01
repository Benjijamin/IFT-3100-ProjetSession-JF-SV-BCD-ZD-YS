#include "Primitives.h"

std::shared_ptr<ofMesh> Primitives::getCubePrimitive(float size)
{
	std::vector<glm::vec3> verts =
	{
		//back
		{  size,  size, -size }, //rtb
		{ -size,  size, -size }, //ltb
		{  size, -size, -size }, //rbb
		{ -size, -size, -size }, //lbb

		//left
		{ -size,  size, -size }, //ltb
		{ -size,  size,  size }, //ltf
		{ -size, -size, -size }, //lbb
		{ -size, -size,  size }, //lbf

		//front
		{ -size,  size,  size }, //ltf
		{  size,  size,  size }, //rtf
		{ -size, -size,  size }, //lbf
		{  size, -size,  size }, //rbf

		//right
		{  size,  size,  size }, //rtf
		{  size,  size, -size }, //rtb
		{  size, -size,  size }, //rbf
		{  size, -size, -size }, //rbb

		//top
		{ -size,  size, -size }, //ltb
		{  size,  size, -size }, //rtb
		{ -size,  size,  size }, //ltf
		{  size,  size,  size }, //rtf

		//bottom
		{ -size, -size,  size }, //lbf
		{  size, -size,  size }, //rbf
		{ -size, -size, -size }, //lbb
		{  size, -size, -size }, //rbb
	};

	std::vector<unsigned int> indices =
	{
		0, 1, 3, 3, 2, 0, //back
		4, 5, 7, 7, 6, 4, //left
		8, 9, 11, 11, 10, 8, //front
		12, 13, 15, 15, 14, 12, //right
		16, 17, 19, 19, 18, 16, //top
		20, 21, 23, 23, 22, 20 //bottom
	};

	std::vector<glm::vec2> texCoords =
	{
		glm::vec2(0, 0.334), glm::vec2(0.25, 0.334), glm::vec2(0, 0.666), glm::vec2(0.25, 0.666), //back
		glm::vec2(0.25, 0.334), glm::vec2(0.5, 0.334), glm::vec2(0.25, 0.666), glm::vec2(0.5, 0.666), //left
		glm::vec2(0.5, 0.334), glm::vec2(0.75, 0.334), glm::vec2(0.5, 0.666), glm::vec2(0.75, 0.666), //front
		glm::vec2(0.75, 0.334), glm::vec2(1, 0.334), glm::vec2(0.75, 0.666), glm::vec2(1, 0.666), //right
		glm::vec2(0.251, 0.333), glm::vec2(0.251, 0), glm::vec2(0.499, 0.333), glm::vec2(0.499, 0), //top
		glm::vec2(0.499, 0.665), glm::vec2(0.499, 1), glm::vec2(0.251, 0.665), glm::vec2(0.251, 1) //bottom
	};

	auto primitive = make_shared<ofMesh>();
	primitive->addVertices(verts);
	primitive->addIndices(indices);
	primitive->addTexCoords(texCoords);
	primitive->addNormals(calculateNormals(primitive));
	primitive->flatNormals();

	return primitive;
}

std::shared_ptr<ofMesh> Primitives::getSkyboxPrimitive(float size) 
{
	std::vector<glm::vec3> verts =
	{
		//back
		{  size,  size, -size }, //rtb
		{ -size,  size, -size }, //ltb
		{  size, -size, -size }, //rbb
		{ -size, -size, -size }, //lbb

		//left
		{ -size,  size, -size }, //ltb
		{ -size,  size,  size }, //ltf
		{ -size, -size, -size }, //lbb
		{ -size, -size,  size }, //lbf

		//front
		{ -size,  size,  size }, //ltf
		{  size,  size,  size }, //rtf
		{ -size, -size,  size }, //lbf
		{  size, -size,  size }, //rbf

		//right
		{  size,  size,  size }, //rtf
		{  size,  size, -size }, //rtb
		{  size, -size,  size }, //rbf
		{  size, -size, -size }, //rbb

		//top
		{ -size,  size, -size }, //ltb
		{  size,  size, -size }, //rtb
		{ -size,  size,  size }, //ltf
		{  size,  size,  size }, //rtf

		//bottom
		{ -size, -size,  size }, //lbf
		{  size, -size,  size }, //rbf
		{ -size, -size, -size }, //lbb
		{  size, -size, -size }, //rbb
	};

	//inverted normals
	std::vector<unsigned int> indices =
	{
		3, 1, 0, 0, 2, 3, //back
		7, 5, 4, 4, 6, 7, //left
		11, 9, 8, 8, 10, 11, //front
		15, 13, 12, 12, 14, 15, //right
		19, 17, 16, 16, 18, 19, //top
		23, 21, 20, 20, 22, 23 //bottom
	};

	std::vector<glm::vec2> texCoords =
	{
		glm::vec2(0, 0.334), glm::vec2(0.25, 0.334), glm::vec2(0, 0.666), glm::vec2(0.25, 0.666), //back
		glm::vec2(0.25, 0.334), glm::vec2(0.5, 0.334), glm::vec2(0.25, 0.666), glm::vec2(0.5, 0.666), //left
		glm::vec2(0.5, 0.334), glm::vec2(0.75, 0.334), glm::vec2(0.5, 0.666), glm::vec2(0.75, 0.666), //front
		glm::vec2(0.75, 0.334), glm::vec2(1, 0.334), glm::vec2(0.75, 0.666), glm::vec2(1, 0.666), //right
		glm::vec2(0.251, 0.333), glm::vec2(0.251, 0), glm::vec2(0.499, 0.333), glm::vec2(0.499, 0), //top
		glm::vec2(0.499, 0.665), glm::vec2(0.499, 1), glm::vec2(0.251, 0.665), glm::vec2(0.251, 1) //bottom
	};

	auto primitive = make_shared<ofMesh>();
	primitive->addVertices(verts);
	primitive->addIndices(indices);
	primitive->addTexCoords(texCoords);
	primitive->addNormals(calculateNormals(primitive));
	primitive->flatNormals();

	return primitive;
}

std::shared_ptr<ofMesh> Primitives::getTetrahedronPrimitive(float size)
{
	std::vector<glm::vec3> verts = 
	{
		{  size,  size,  size },
		{  size, -size, -size },
		{ -size,  size, -size },
		{ -size, -size,  size }
	};

	std::vector<unsigned int> indices = 
	{
		2, 1, 0,
		3, 2, 0,
		1, 3, 0,
		1, 2, 3
	};

	auto primitive = make_shared<ofMesh>();
	primitive->addVertices(verts);
	primitive->addIndices(indices);
	primitive->addNormals(calculateNormals(primitive));
	primitive->flatNormals();

	return primitive;
}

glm::vec3 bezierCurve(float t, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4)
{
	float it = 1.0f - t;

	return (it * it * it * p1) +
		(3 * t * it * it * p2) +
		(3 * t * t * it * p3) +
		(t * t * t * p4);
}

std::shared_ptr<ofMesh> Primitives::getBezierSurface(std::vector<glm::vec3>& controlPoints, int resolution) 
{
	auto mesh = make_shared<ofMesh>();
	mesh->setMode(OF_PRIMITIVE_TRIANGLES);

	//Bezier dans les deux sens
	for (int i = 0; i < resolution; ++i)
	{
		for (int j = 0; j < resolution; ++j)
		{
			float u = (float)i / (resolution - 1);
			float v = (float)j / (resolution - 1);

			glm::vec3 temp[4];

			for (int k = 0; k < 4; ++k)
			{
				glm::vec3 p1 = controlPoints[k * 4 + 0];
				glm::vec3 p2 = controlPoints[k * 4 + 1];
				glm::vec3 p3 = controlPoints[k * 4 + 2];
				glm::vec3 p4 = controlPoints[k * 4 + 3];

				temp[k] = bezierCurve(v, p1, p2, p3, p4);
			}

			glm::vec3 point = bezierCurve(u, temp[0], temp[1], temp[2], temp[3]);

			mesh->addVertex(point);
		}
	}

	for (int y = 0; y < resolution - 1; ++y) 
	{
		for (int x = 0; x < resolution - 1; ++x) 
		{
			int v1 = x + y * resolution;
			int v2 = (x + 1) + y * resolution;
			int v3 = x + (y + 1) * resolution;
			int v4 = (x + 1) + (y + 1) * resolution;

			mesh->addIndex(v1);
			mesh->addIndex(v2);
			mesh->addIndex(v3);

			mesh->addIndex(v2);
			mesh->addIndex(v4);
			mesh->addIndex(v3);
		}
	}

	mesh->addNormals(calculateNormals(mesh));

	return mesh;
}

std::vector<glm::vec3> Primitives::calculateNormals(std::shared_ptr<ofMesh> mesh) {
	std::vector<glm::vec3> normals;

	normals.resize(mesh->getVertices().size(), glm::vec3(0));

	for (int i = 0; i < mesh->getIndices().size(); i += 3)
	{
		unsigned int index1 = mesh->getIndices()[i];
		unsigned int index2 = mesh->getIndices()[i + 1];
		unsigned int index3 = mesh->getIndices()[i + 2];

		glm::vec3 vert1 = mesh->getVertex(index1);
		glm::vec3 vert2 = mesh->getVertex(index2);
		glm::vec3 vert3 = mesh->getVertex(index3);

		glm::vec3 normal =  glm::cross((vert2 - vert1), (vert3 - vert1));
		glm::normalize(normal);

		normals[index1] += normal;
		normals[index2] += normal;
		normals[index3] += normal;
	}

	for (size_t i = 0; i < normals.size(); ++i)
	{
		glm::normalize(normals[i]);
	}

	return normals;
}
