#pragma once

#include "ofMain.h"

//TODO cache & instanciation
class Primitives 
{
public:
	static std::shared_ptr<ofMesh> getCubePrimitive(float size);
	static std::shared_ptr<ofMesh> getTetrahedronPrimitive(float size);
private:
	static std::vector<glm::vec3> calculateNormals(std::shared_ptr<ofMesh> mesh);
};
