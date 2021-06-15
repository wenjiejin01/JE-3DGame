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


	//AI
	int map_width;
	int map_height;
	int current_map_steps = -1;
	//the map info should be an array W*H of bytes where 0 means block, 1 means walkable
	uint8* map;
	//when we want to find the shortest path, this array contains the shortest path, every value is the Nth position in the map, 100 steps max
	int output[100];


	// Global variable
	Vector3 startPoint;
	std::vector<Vector3> TargetPoint;
	Shader* global_Shader;
	Texture* global_texture;

	//gameplay
	float live_time; // time to live
	int target_num = 0; // # target
	int target_visited = 0; // # target visited


};

#endif 

