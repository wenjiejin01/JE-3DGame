#include "game.h"
#include "input.h"
#include "stage.h"
#include "entity.h"
#include "scene.h"

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
		case 1: AddObjectInFont(camera, Mesh::Get("data/assets/town/minihouse.obj"), Texture::Get("data/texture.tga")); break;
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

void Stage::AddObjectInFont(Camera* camera, Mesh* mesh, Texture* texture) {
	Scene* world = Scene::instance;
	Game* game = Game::instance;

	Vector3 origin = camera->eye;
	Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, game->window_width, game->window_height);
	Vector3 pos = RayPlaneCollision(Vector3(), Vector3(0, 1, 0), origin, dir);

	//create new entitymesh, set model, mesh, texture, and push to static list
	EntityMesh* entity = new EntityMesh();
	entity->model.setTranslation(pos.x, pos.y, pos.z);
	entity->mesh = mesh;
	entity->texture = texture;
	entity->shader =  Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	entity->meshType = EntityMesh::HOUSE;
	world->static_list.push_back(entity);
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
