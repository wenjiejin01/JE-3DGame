#pragma once
#pragma once
#ifndef STAGE_H
#define STAGE_H

#include "includes.h"
#include "camera.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"

class Stage {
public:
	float angle = 0;

	Stage();

	virtual void render(Camera* camera) {}; //empty body
	virtual void update(float elapsed_time) {}; //empty body

};

//class IntroStage : public Stage {
//public:
//
//	IntroStage();
//	virtual void render(Camera* camera);
//	virtual void update(float elapsed_time);
//};
//
//class TutorialStage : public Stage {
//public:
//
//	TutorialStage();
//	virtual void render(Camera* camera);
//	virtual void update(float elapsed_time);
//};

class PlayStage : public Stage {
public:
	Mesh* mesh = NULL;
	Texture* texture;
	Shader* shader = NULL;
	Animation* anim = NULL;

	FBO* fbo = NULL;


	PlayStage();
	virtual void render(Camera* camera);
	virtual void update(float elapsed_time);
	/*bool isValid(Vector2 target);
	void posUpdate(Vector2 target, Player* player);
	bool isStepOn();
	bool isFinished();
	void restart();*/
};

//class EndStage : public Stage {
//public:
//
//	EndStage(Camera* camera);
//	virtual void render();
//	virtual void update(float elapsed_time);
//};

#endif



