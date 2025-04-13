/**
 * \file renderer.h
 * \brief Classe regroupant toutes les instantes d'objets modifiant le rendu
 * \author Yohan
 * \version 0.1
 * \date Avril 2025
 *
 *  IFT-3100 Equipe 2 TP2
 *
 */

#pragma once

#include "ofMain.h"
#include "light.h"
#include "shaders.h"
#include "ofxAssimpModelLoader.h"


class Renderer {
public:
	void setup();
	void update();
	void draw();
};
