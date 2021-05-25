#include "game.h"
#include "input.h"
#include "stage.h"
#include "entity.h"
#include "scene.h"
#include "extra/textparser.h"

#include <iostream>
#include <fstream>
#include <cmath>


Stage::Stage() {}


PlayStage::PlayStage() {
	Scene* world = Scene::instance;

	//GRASS
	EntityMesh* grass = new EntityMesh();
	grass->meshType = EntityMesh::GRASS;
	world->static_list.push_back(grass);
	grass->mesh = new Mesh();
	grass->mesh->createPlane(600.0f);
	grass->texture = new Texture();
	grass->texture->load("data/grass.tga");
	grass->model.translate(0.0f, -0.5f, 0.0f);
	// example of shader loading using the shaders manager
	grass->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	//ROAD
	EntityMesh* road = new EntityMesh();
	road->meshType = EntityMesh::ROAD;
	world->static_list.push_back(road);
	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	road->texture = new Texture();
	road->texture->load("data/assets/color-atlas-new.png");
	// example of loading Mesh from Mesh Manager
	//road->mesh = Mesh::Get("data/map_road.obj");
	road->mesh = Mesh::Get("data/assets/carretera/mapa_carretera.obj");
	road->model.translate(0.0f, 0.0f, 0.0f);
	// example of shader loading using the shaders manager
	road->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	//SKY
	EntityMesh* sky = new EntityMesh();
	sky->meshType = EntityMesh::SKY;
	world->static_list.push_back(sky);

	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	sky->texture = new Texture();
	sky->mesh = Mesh::Get("data/assets/cielo/cielo.ASE");
	sky->texture->load("data/assets/cielo/cielo.tga");
	sky->model.translate(0.0f, 0.0f, 0.0f);
	sky->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");


	//CAR
	EntityCar* car = new EntityCar();
	car->meshType = EntityMesh::CAR;
	world->dynamic_list.push_back(car);
	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	car->texture = new Texture();
	car->texture->load("data/assets/color-atlas-new.png");
	// example of loading Mesh from Mesh Manager
	car->mesh = Mesh::Get("data/assets/coches/car-passenger_1.obj");
	car->model.translate(0.0f, 0.0f, 0.0f);
	// example of shader loading using the shaders manager
	car->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	//OBJECTS
	LoadFile();
}

void PlayStage::render(Camera* camera){
	EntityMesh* currentMesh = NULL;
	Scene* world = Scene::instance;

	int count = world->static_list.size();
	Vector3 eye, center, up, padding;
	Vector3 current_pos_world;
	Mesh* usedMesh;
	Texture* usedTex;

	for (size_t i = 0; i < count; i++)
	{
		// Break the game and show error.
		assert(world->static_list.at(i) != NULL);

		// Check entity type
		if (world->static_list.at(i)->getType() == ENTITY_TYPE_ID::MESH)
		{			
			//DOWNCAST, BY STATIC_CAST
			currentMesh = static_cast<EntityMesh*>(world->static_list.at(i));
		}

		currentMesh->render(camera);
	}

	for (size_t i = 0; i < world->dynamic_list.size(); i++)	{
		// Break the game and show error.
		assert(world->dynamic_list.at(i) != NULL);

		// Check entity type
		if (world->dynamic_list.at(i)->getType() == ENTITY_TYPE_ID::MESH)
		{
			//DOWNCAST, BY STATIC_CAST
			currentMesh = static_cast<EntityMesh*>(world->dynamic_list.at(i));
		}

		if (!world->free_camera)
		{
			eye = currentMesh->model * Vector3(0.0f, 6.0f, 10.0f);
			center = currentMesh->model * Vector3(0.0f, 0.0f, -2.0f);
			up = currentMesh->model.rotateVector(Vector3(0.0f, 1.0f, 0.0f));
			camera->lookAt(eye, center, up);
		}
		currentMesh->EntityMesh::render(camera);
	}
}

void PlayStage::update(float seconds_elapsed) {
	Scene* world = Scene::instance;

	EntityMesh* currentMesh = static_cast<EntityMesh*>(world->dynamic_list.at(0));
	currentMesh->EntityMesh::update(seconds_elapsed);
}

void Stage::getKeyDownEvent(Camera* camera, int key_num) {
	Scene* world = Scene::instance;

	switch (key_num)
	{
		case 1: AddObjectInFont(camera, "data/assets/edificios/building-office-big_13.obj", "data/assets/color-atlas-new.png"); break;
		//case 1: AddObjectInFont(camera, "data/assets/arboles/tree-birch_42.obj", "data/assets/color-atlas-new.png"); break;
		//case 1: AddObjectInFont(camera, "data/assets/edificios/building-house-middle_7.obj", "data/assets/color-atlas-new.png"); break;
		//case 1: AddObjectInFont(camera, "data/assets/edificios/building-office-tall_15.obj", "data/assets/color-atlas-new.png"); break;

		case 2: SelectEntity(camera); break;
		case 3: {
			EntityMesh* mesh = static_cast<EntityMesh*>(world->selected_entity);
			mesh->model.rotate(10.0f * DEG2RAD, Vector3(0, 1, 0));		}
		break;
		case 4: {
			EntityMesh* mesh = static_cast<EntityMesh*>(world->selected_entity);
			mesh->model.rotate(-10.0f * DEG2RAD, Vector3(0, 1, 0));
		}
		break;

		
	}
}

void Stage::AddObjectInFont(Camera* camera, const char* mesh, const char* texture) {
	Scene* world = Scene::instance;
	Game* game = Game::instance;

	Vector3 origin = camera->eye;
	Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, game->window_width, game->window_height);
	Vector3 pos = RayPlaneCollision(Vector3(), Vector3(0, 1, 0), origin, dir);

	//create new entitymesh, set model, mesh, texture, and push to static list
	EntityMesh* entity = new EntityMesh();
	entity->model.setTranslation(pos.x, pos.y, pos.z);
	entity->mesh = Mesh::Get(mesh);
	/*entity->texture = new Texture();;
	entity->texture->load(texture);*/
	entity->texture = Texture::Get(texture);
	entity->shader =  Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	entity->meshType = EntityMesh::HOUSE;
	world->static_list.push_back(entity);

	//write pos, mesh and text in file
	std::stringstream filename;
	filename << "data/levels/level" << number_level << ".txt";
	std::fstream myfile;
	myfile.open(filename.str(), myfile.out | myfile.app);
	
	if (!myfile.is_open())
	{
		std::cout << "no file to write levels" << std::endl;
		exit(0);
	}

	myfile << "\t -ENTITY: " << pos.x << " " << pos.y << "       " << pos.z << "     " << mesh << "     " << texture << "  " << "\n\n";
	myfile.close();
}
void Stage::LoadFile()
{
	Scene* world = Scene::instance;
	Vector3 pos = Vector3();
	const char* mesh;
	const char* texture;
	
	//load file of level with mesh , text and pos 
	
	std::stringstream filename;
	filename << "data/levels/level" << number_level << ".txt";
	TextParser t = TextParser(filename.str().c_str());
	std::fstream myfile;
	//FILE* file = fopen(filename.str().c_str(), "rb");
	if (!t.create(filename.str().c_str())) {
		std::cout << "File not found " << filename.str() << std::endl;
		exit(0);
	}

	int count = t.countword("-ENTITY:");
	std::cout << "debug de pos" << 0 << " " << 0 << " " << 0 << "\n\n ";

	for (int i = 0; i < count; i++) {
		t.seek("-ENTITY:");
		pos.x = t.getfloat();
		pos.y = t.getfloat();
		pos.z = t.getfloat();
		mesh = t.getword();
		//texture = t.getword();
		texture = "data/assets/color-atlas-new.png";
		strlwr((char*)mesh);
		strlwr((char*)texture);

		std::cout << "debug de pos " << pos.x << " " << pos.y << " " << pos.z << " " << mesh << " " << texture << "\n";
	
		EntityMesh* entity = new EntityMesh();
		entity->model.setTranslation(pos.x, pos.y, pos.z);
		entity->mesh = Mesh::Get((const char*)mesh);
		entity->texture = Texture::Get((const char*)texture);
		entity->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
		entity->meshType = EntityMesh::HOUSE;
		world->static_list.push_back(entity);
	}
	myfile.close();

}

void Stage::SelectEntity(Camera* camera) {
	Scene* world = Scene::instance;
	Game* game = Game::instance;

	Vector3 origin = camera->eye;
	Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, game->window_width, game->window_height);
	Vector3 pos = RayPlaneCollision(Vector3(), Vector3(0, 1, 0), origin, dir);

	//Select entity object
	int count = world->static_list.size();
	EntityMesh* current;
	Vector3 col, normal;
	for (size_t i = 0; i < count; i++)
	{
		assert(world->static_list.at(i) != NULL);

		if (world->static_list.at(i)->getType() == ENTITY_TYPE_ID::MESH)
		{
			//DOWNCAST, BY STATIC_CAST
			current = static_cast<EntityMesh*>(world->static_list.at(i));
		}
		else {
			continue;
		}

		if (!current->mesh->testRayCollision(current->model, origin, dir, col, normal)) continue;

		world->selected_entity = current;
		break;
	}
}
