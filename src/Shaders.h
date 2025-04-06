#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ofxAssimpModelLoader.h"


enum ShaderType {
	SHADER_NONE = 0,
	SHADER_LAMBERT,
	SHADER_PHONG,
	SHADER_GOURAUD,
	SHADER_BLINN_PHONG,
	SHADER_TOON
};

class Shaders {
public:
	void setup();
	void update();
	void draw();
	void reset();

	void drawShadersMenu();

	ShaderType activeShader;

	void drawLambertShader();
	void drawPhongShader();
	void drawGouraudShader();
	void drawBlinn_phongShader();
	void drawToonShader();

	void listShaderFiles();
	void logShaderStatus();

	void drawSphere();

	// ref pour loader shader
	ofShader lambertShader, phongShader, gouraudShader, blinn_phongShader, toonShader;

	ofSpherePrimitive sphere;   // Objet à illuminer

	ofLight dirLight;

	ofxAssimpModelLoader teapot;

	// Material
	ofMaterial material;        // Matériau appliqué

	// Direction lumiere
	ofVec3f lightDirection;     // Direction de la lumière
};

