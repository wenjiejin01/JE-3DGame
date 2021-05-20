#include "game.h"
#include "input.h"
#include "stage.h"
#include "entity.h"
#include "scene.h"

Stage::Stage() {}

PlayStage::PlayStage() {
	Scene* world = new Scene();
	//CAR
	EntityMesh* car = new EntityMesh();
	car->meshType = EntityMesh::CAR;
	world->entity_list.push_back(car);

	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	car->texture = new Texture();
	car->texture->load("data/assets/color-atlas-new.png");

	// example of loading Mesh from Mesh Manager
	car->mesh = Mesh::Get("data/assets/coches/car-passenger_1.obj");
	car->model.translate(0.0f, 0.0f, 0.0f);
	// example of shader loading using the shaders manager
	car->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	//GRASS
	EntityMesh* grass = new EntityMesh();
	grass->meshType = EntityMesh::GRASS;
	world->entity_list.push_back(grass);
	grass->mesh = new Mesh();
	grass->mesh->createPlane(600.0f);
	grass->texture = new Texture();
	grass->texture->load("data/grass.tga");
	grass->model.translate(0.0f, 0.0f, 0.0f);
	// example of shader loading using the shaders manager
	grass->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	//ROAD
	EntityMesh* road = new EntityMesh();
	road->meshType = EntityMesh::ROAD;
	world->entity_list.push_back(road);
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
	world->entity_list.push_back(sky);

	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	sky->texture = new Texture();
	sky->mesh = Mesh::Get("data/assets/cielo/cielo.ASE");
	sky->texture->load("data/assets/cielo/cielo.tga");
	sky->model.translate(0.0f, 0.0f, 0.0f);
	sky->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

}


void PlayStage::render(Camera* camera){
	EntityMesh* currentMesh = NULL;
	Scene* world = Scene::instance;


	int count = world->entity_list.size();
	Vector3 eye, center, up, padding;
	Vector3 current_pos_world;
	Mesh* usedMesh;
	Texture* usedTex;

	for (size_t i = 0; i < count; i++)
	{
		// Break the game and show error.
		assert(world->entity_list.at(i) != NULL);

		// Check entity type
		if (world->entity_list.at(i)->getType() == ENTITY_TYPE_ID::MESH)
		{			
			//DOWNCAST, BY STATIC_CAST
			currentMesh = static_cast<EntityMesh*>(world->entity_list.at(i));
		}

		// 
		switch (currentMesh->meshType)
		{
		//Cielo
		case EntityMesh::SKY:
			currentMesh->render(camera);
			break;

		//cesped
		case EntityMesh::GRASS:
			//currentMesh->render(camera, 100);
			break;

		//carretera
		case EntityMesh::ROAD:
			currentMesh->moving = false;
			currentMesh->render(camera);
			break;

		//coche
		case EntityMesh::meshType::CAR:
			//fix camera
			if (!world->free_camera)
			{
				eye = currentMesh->model * Vector3(0.0f, 6.0f, 10.0f);
				center = currentMesh->model * Vector3(0.0f, 0.0f, -2.0f);
				up = currentMesh->model.rotateVector(Vector3(0.0f, 1.0f, 0.0f));
				camera->lookAt(eye, center, up);
			}

			currentMesh->render(camera);
			break;


		default:
			break;
		}

	}
}

void PlayStage::update(float seconds_elapsed) {
	Scene* world = Scene::instance;

	EntityMesh* currentMesh = static_cast<EntityMesh*>(world->entity_list.at(0));
	currentMesh->update(seconds_elapsed);
}