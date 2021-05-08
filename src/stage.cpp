#include "game.h"
#include "input.h"
#include "stage.h"
#include "entity.h"

Stage::Stage() {}

PlayStage::PlayStage() {
	//Load Entity
	EntityMesh* box = new EntityMesh();
	box->name = "BOX";
	mesh_List.push_back(box);

	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	box->texture = new Texture();
	box->texture->load("data/texture.tga");

	// example of loading Mesh from Mesh Manager
	box->mesh = Mesh::Get("data/box.ASE");

	// example of shader loading using the shaders manager
	box->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
}


void PlayStage::render(Camera* camera){
	EntityMesh* currentMesh = mesh_List.at(0);
	currentMesh->render(camera);
}

void PlayStage::update(float seconds_elapsed) {
	EntityMesh* currentMesh = mesh_List.at(0);
	currentMesh->update(seconds_elapsed);
}