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
#include "soundmanager.h"

class Stage {
public:
	float angle = 0;
	int number_level = 1;
	SoundManager* sound;

	Stage();

	virtual void render(Camera* camera) {}; //empty body
	virtual void update(float elapsed_time) {}; //empty body
	void getKeyDownEvent(Camera* camera, int key_num);
	void AddObjectInFont(Camera* camera, const char* mesh, const char* texture);
	void LoadFile(std::stringstream filename, char* word);
	void SelectEntity(Camera* camera);
	void LoadFile();
	void restartGame();
};

class IntroStage : public Stage {
public:
	float angle = 0;

	IntroStage();
	EntityMesh* startButton;
	EntityMesh* TutorialButton;
	virtual void render(Camera* camera);
	virtual void update(float elapsed_time);
};

class TutorialStage : public Stage {
public:
	EntityMesh* goInit;

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
	void renderMiniMap();
	virtual void update(float elapsed_time);
};

class EndStage : public Stage {
public:

	EntityMesh* gameOver;
	EntityMesh* restart;
	EntityMesh* goInit;
	EndStage();
	virtual void render(Camera* camera);
	virtual void update(float elapsed_time);
};

#endif



