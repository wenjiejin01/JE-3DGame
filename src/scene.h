#ifndef SCENE_H
#define SCENE_H

#include "entity.h"

class Scene //igual que World
{
protected:
	Scene* scene; //singleton
	Scene();

public:
	Scene* getScene();

	//current scene
	std::vector<Entity*> entities;

};

#endif 

