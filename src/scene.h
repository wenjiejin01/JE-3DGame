#ifndef SCENE_H
#define SCENE_H

#include "entity.h"

class Scene //igual que World
{
public:
	static Scene* instance; //singleton
	bool free_camera = true;

	//standar variable
	float angle = 0;
	Scene();

	//current scene
	std::vector<Entity*> static_list;
	std::vector<Entity*> dynamic_list;
	Entity* selected_entity = NULL;
	EntityMesh* road;
	EntityMesh* grass;


};

#endif 

