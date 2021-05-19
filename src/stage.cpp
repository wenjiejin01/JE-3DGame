#include "game.h"
#include "input.h"
#include "stage.h"
#include "entity.h"
#include "scene.h"

Stage::Stage() {}

PlayStage::PlayStage() {
	Scene* world = new Scene();
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
	road->texture->load("data/biglib/GiantGeneralPack/color-atlas-new.png");
	// example of loading Mesh from Mesh Manager
	//road->mesh = Mesh::Get("data/map_road.obj");
	road->mesh = Mesh::Get("data/mapa_carretera.obj");
	road->model.translate(0.0f, 0.0f, 0.0f);
	// example of shader loading using the shaders manager
	road->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	//CAR
	EntityMesh* car = new EntityMesh();
	car->meshType = EntityMesh::CAR;
	world->entity_list.push_back(car);

	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	car->texture = new Texture();
	car->texture->load("data/biglib/GiantGeneralPack/color-atlas-new.png");

	// example of loading Mesh from Mesh Manager
	car->mesh = Mesh::Get("data/biglib/GiantGeneralPack/Cars_T/car-passenger_1.obj");
	car->model.translate(0.0f, 0.0f, 0.0f);
	// example of shader loading using the shaders manager
	car->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	//SKY
	EntityMesh* sky = new EntityMesh();
	sky->meshType = EntityMesh::SKY;
	world->entity_list.push_back(sky);

	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	sky->texture = new Texture();
	sky->mesh = Mesh::Get("data/cielo.ASE");
	sky->texture->load("data/cielo.tga");
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
		

		/*case EntityMesh::meshType::HOUSE:
			padding = currentMesh->mesh->box.halfsize * 2;

			for (size_t i = 0; i < 40; i++)
			{
				for (size_t j = 0; j < 40; j++)
				{
					currentMesh->model.setTranslation(i * padding.x + 10.0f, 0.0f, j * padding.y + 10.0f);
					current_pos_world = currentMesh->model * Vector3(0.0f, 0.0f, 0.0f);
					usedMesh = currentMesh->mesh;
					usedTex = currentMesh->texture;
					if (!camera->testSphereInFrustum(current_pos_world, usedMesh->radius)) continue;
					currentMesh->render(camera);
				}

			}
			break;*/
		case EntityMesh::SKY:
			//currentMesh->model.translate(camera->eye.x, camera->eye.y, camera->eye.z);
			eye = currentMesh->model * Vector3(camera->eye.x, camera->eye.y, camera->eye.z);
			center = currentMesh->model * Vector3(camera->center.x, camera->center.y, camera->center.z);
			up = currentMesh->model * Vector3(camera->up.x, camera->up.y, camera->up.z);
			camera->lookAt(eye, center, up);
			currentMesh->render(camera);
			break;

		case EntityMesh::GRASS:
			currentMesh->render(camera, 100);
			break;

		case EntityMesh::ROAD:
			currentMesh->moving = false;
			currentMesh->render(camera);
			break;

		case EntityMesh::meshType::CAR:
			//fix camera
			eye = currentMesh->model * Vector3(0.0f, 6.0f, 10.0f);
			center = currentMesh->model * Vector3(0.0f, 0.0f, -2.0f);
			up = currentMesh->model.rotateVector(Vector3(0.0f, 1.0f, 0.0f));
			camera->lookAt(eye, center, up);

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