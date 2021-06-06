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
#include "entity.h"

class Stage {
public:
	float angle = 0;
	int number_level = 1;

	Stage();

	virtual void render(Camera* camera) {}; //empty body
	virtual void update(float elapsed_time) {}; //empty body
	void getKeyDownEvent(Camera* camera, int key_num);
	void AddObjectInFont(Camera* camera, const char* mesh, const char* texture);
	void SelectEntity(Camera* camera);
	void LoadFile();
};

class IntroStage : public Stage {
public:

	IntroStage();
	virtual void render(Camera* camera);
	virtual void update(float elapsed_time);
};

class TutorialStage : public Stage {
public:

	TutorialStage();
	virtual void render(Camera* camera);
	virtual void update(float elapsed_time);
};

class PlayStage : public Stage {
public:
	FBO* fbo = NULL;
	std::vector<Entity*> mesh_List;
	

	PlayStage();
	virtual void render(Camera* camera);
	virtual void update(float elapsed_time);
};

class EndStage : public Stage {
public:

	EndStage();
	virtual void render(Camera* camera);
	virtual void update(float elapsed_time);
};

#endif



