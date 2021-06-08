#ifndef SCENE_H
#define SCENE_H

#include "entity.h"
#include "animation.h"
#include "mesh.h"

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
	EntityCar* player_car;
	EntityCar* enemy_car;
	std::vector<Entity*> dynamic_list;

	Entity* selected_entity = NULL;
	EntityMesh* minimap;
	EntityMesh* road;
	EntityMesh* grass;

	// Global variable
	Vector3 startPoint;
	std::vector<Vector3> TargetPoint;
	Shader* global_Shader;
	Texture* global_texture;


};

#endif 

