#pragma once

#include "ofMain.h"

class Primitives 
{
public:
	static std::shared_ptr<ofMesh> getCubePrimitive(float size);
	static std::shared_ptr<ofMesh> getSkyboxPrimitive(float size);
	static std::shared_ptr<ofMesh> getTetrahedronPrimitive(float size);
	//4x4 points de controle
	static std::shared_ptr<ofMesh> getBezierSurface(std::vector<glm::vec3>& controlPoints, int resolution);
private:
	static std::vector<glm::vec3> calculateNormals(std::shared_ptr<ofMesh> mesh);
};
