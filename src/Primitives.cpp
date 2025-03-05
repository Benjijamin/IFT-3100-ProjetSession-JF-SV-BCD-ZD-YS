#include "Primitives.h"

std::shared_ptr<ofMesh> Primitives::getCubePrimitive(float size)
{
	std::vector<glm::vec3> verts =
	{
		{ -size, -size, -size }, //0 left bottom back
		{  size, -size, -size }, //1 right bottom back
		{  size,  size, -size }, //2 right top back
		{ -size,  size, -size }, //3 left top back
		{ -size, -size,  size }, //4 left bottom front
		{  size, -size,  size }, //5 right bottom front
		{  size,  size,  size }, //6 right top front
		{ -size,  size,  size }  //7 left top front
	};

	std::vector<unsigned int> indices =
	{
		1, 2, 0, 2, 3, 0, //back
		6, 5, 4, 7, 6, 4, //front
		5, 1, 0, 4, 5, 0, //bottom
		7, 3, 2, 6, 7, 2, //top
		7, 4, 0, 3, 7, 0, //left
		6, 2, 1, 5, 6, 1  //right
	};

	auto primitive = make_shared<ofMesh>();
	primitive->addVertices(verts);
	primitive->addIndices(indices);
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



std::vector<glm::vec3> Primitives::calculateNormals(std::shared_ptr<ofMesh> mesh)
{
	std::vector<glm::vec3> normals;

	normals.resize(mesh->getVertices().size(), glm::vec3(0));

	for (int i = 0; i < mesh->getIndices().size(); i += 3) {
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

	for (size_t i = 0; i < normals.size(); ++i) {
		glm::normalize(normals[i]);
	}

	return normals;
}