#ifndef SCENE_H
#define SCENE_H

#include "entity.h"

class Scene //igual que World
{
public:
	static Scene* instance; //singleton
	bool free_camera = false;

	//standar variable
	float angle = 0;
	Scene();

	//current scene
	std::vector<Entity*> entity_list;

};

#endif 

