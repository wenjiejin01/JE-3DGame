#include "game.h"
#include "input.h"
#include "stage.h"
#include "entity.h"

Stage::Stage() {}

PlayStage::PlayStage() {
	//Load Entity
	EntityMesh* truck = new EntityMesh();
	truck->meshType = EntityMesh::meshType::CAR;
	mesh_List.push_back(truck);

	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	truck->texture = new Texture();
	truck->texture->load("data/truck/truck.tga");

	// example of loading Mesh from Mesh Manager
	truck->mesh = Mesh::Get("data/truck/truck.ASE");
	truck->model.translate(0.0f, 0.0f, 0.0f);
	// example of shader loading using the shaders manager
	truck->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	// House entity
	EntityMesh* house = new EntityMesh();
	house->meshType = EntityMesh::meshType::HOUSE;
	mesh_List.push_back(house);

	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	house->texture = new Texture();
	house->texture->load("data/truck/houses_and_windows.tga");

	// example of loading Mesh from Mesh Manager
	house->mesh = Mesh::Get("data/truck/house2.ASE");
	house->model.translate(50.0f, 0.0f, 0.0f);
	// example of shader loading using the shaders manager
	house->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

}


void PlayStage::render(Camera* camera){
	EntityMesh* currentMesh = NULL;
	int count = mesh_List.size();
	Vector3 eye, center, up, padding;

	for (size_t i = 0; i < count; i++)
	{
		currentMesh = mesh_List.at(i);

		switch (currentMesh->meshType)
		{
		case EntityMesh::meshType::CAR:
			//fix camera
			eye = currentMesh->model * Vector3(0.0f, 6.0f, 10.0f);
			center = currentMesh->model * Vector3(0.0f, 0.0f, -2.0f);
			up = currentMesh->model.rotateVector(Vector3(0.0f, 1.0f, 0.0f));
			camera->lookAt(eye, center, up);

			currentMesh->render(camera);
			break;

		case EntityMesh::meshType::HOUSE:
			padding = currentMesh->mesh->box.halfsize * 2;

			for (size_t i = 0; i < 40; i++)
			{
				for (size_t j = 0; j < 40; j++)
				{
					currentMesh->model.setTranslation(i * padding.x + 50.0f, 0.0f, j * padding.y + 50.0f);
					currentMesh->render(camera);
				}

			}
			break;

		default:
			break;
		}

	}
}

void PlayStage::update(float seconds_elapsed) {
	EntityMesh* currentMesh = mesh_List.at(0);
	currentMesh->update(seconds_elapsed);
}