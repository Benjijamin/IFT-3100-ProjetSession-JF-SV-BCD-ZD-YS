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

	//cubemap ex:de6.jpg
	std::vector<glm::vec2> texCoords =
	{
		glm::vec2(0, 0.333), glm::vec2(0.25, 0.333), glm::vec2(0, 0.666), glm::vec2(0.25, 0.666), //back
		glm::vec2(0.25, 0.333), glm::vec2(0.5, 0.333), glm::vec2(0.25, 0.666), glm::vec2(0.5, 0.666), //left
		glm::vec2(0.5, 0.333), glm::vec2(0.75, 0.333), glm::vec2(0.5, 0.666), glm::vec2(0.75, 0.666), //front
		glm::vec2(0.75, 0.333), glm::vec2(1, 0.333), glm::vec2(0.75, 0.666), glm::vec2(1, 0.666), //right
		glm::vec2(0.5, 0), glm::vec2(0.75, 0), glm::vec2(0.5, 0.333), glm::vec2(0.75, 0.333), //top
		glm::vec2(0.5, 0.666), glm::vec2(0.75, 0.666), glm::vec2(0.5, 1), glm::vec2(0.75, 1) //bottom
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

std::vector<glm::vec3> Primitives::calculateNormals(std::shared_ptr<ofMesh> mesh)
{
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
