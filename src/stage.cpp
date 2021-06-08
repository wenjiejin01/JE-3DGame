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

/********************************************************* Introstage *********************************************************/
IntroStage::IntroStage() {

}

void IntroStage::render(Camera* camera) {

}

void IntroStage::update(float elapse_time) {

}


/********************************************************* PlayStagte *********************************************************/
PlayStage::PlayStage() {
	Scene* world = Scene::instance;
	world->global_Shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	world->global_texture = Texture::Get("data/assets/color-atlas-new.png");
	//GRASS
	world->grass = new EntityMesh();
	EntityMesh* grass = world->grass;
	grass->meshType = EntityMesh::GRASS;
	grass->mesh = new Mesh();
	grass->mesh->createPlane(600.0f);
	grass->texture = new Texture();
	grass->texture->load("data/grass.tga");
	grass->model.translate(0.0f, -0.5f, 0.0f);
	grass->shader = world->global_Shader;

	//ROAD
	world->road = new EntityMesh();
	EntityMesh* road = world->road;
	road->meshType = EntityMesh::ROAD;
	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	road->texture = world->global_texture;
	// example of loading Mesh from Mesh Manager
	road->mesh = Mesh::Get("data/assets/carretera/mapa_carretera.obj");
	road->model.translate(0.0f, 0.0f, 0.0f);
	// example of shader loading using the shaders manager
	road->shader = world->global_Shader;

	//SKY
	EntityMesh* sky = new EntityMesh();
	sky->meshType = EntityMesh::SKY;
	world->static_list.push_back(sky);

	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	sky->texture = new Texture();
	sky->mesh = Mesh::Get("data/assets/cielo/cielo.ASE");
	sky->texture->load("data/assets/cielo/cielo.tga");
	sky->model.translate(0.0f, 0.0f, 0.0f);
	sky->model.scale(0.63f, 0.7f, 0.6f);
	sky->shader = world->global_Shader;


	//CAR
	world->player_car = new EntityCar();
	world->player_car->meshType = EntityMesh::CAR;
	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	world->player_car->texture = world->global_texture;
	// example of loading Mesh from Mesh Manager
	world->player_car->mesh = Mesh::Get("data/assets/coches/car-passenger_1.obj");
	world->player_car->model.translate(0.0f, 0.0f, 0.0f);
	// example of shader loading using the shaders manager
	world->player_car->shader = world->global_Shader;

	//CAR2
	world->enemy_car = new EntityCar();
	world->enemy_car->meshType = EntityMesh::CAR;
	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	world->enemy_car->texture = world->global_texture;
	// example of loading Mesh from Mesh Manager
	world->enemy_car->mesh = Mesh::Get("data/assets/coches/car-passenger_1.obj");
	world->enemy_car->pos = Vector3(0.0f, 0.0f, 20.0f);
	world->enemy_car->model.translate(world->enemy_car->pos.x, world->enemy_car->pos.y, world->enemy_car->pos.z);
	// example of shader loading using the shaders manager
	world->enemy_car->shader = world->global_Shader;

	/* Minimap */
	world->minimap = new EntityMesh();
	world->minimap->meshType = EntityMesh::MINIMAP;
	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	world->minimap->texture = world->global_texture;
	// example of loading Mesh from Mesh Manager
	world->minimap->mesh = Mesh::Get("data/sphere.ASE");
	// example of shader loading using the shaders manager
	world->minimap->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");

	//OBJECTS
	LoadFile();
}

void PlayStage::render(Camera* camera){
	EntityMesh* currentMesh = NULL;
	Scene* world = Scene::instance;

	//Declare temporal variable
	int count = world->static_list.size();
	Vector3 eye, center, up, padding;
	Vector3 current_pos_world;
	Mesh* usedMesh;
	Texture* usedTex;

	world->grass->render(camera);
	world->road->render(camera);
	//renderMiniMap();

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

	EntityCar* currentCar = world->player_car;

	// camara se sigue al coche
	if (!world->free_camera)
	{
		eye = currentCar->model * Vector3(0.0f, 3.0f, 6.0f);
		center = currentCar->model * Vector3(0.0f, 0.0f, -2.0f);
		up = currentCar->model.rotateVector(Vector3(0.0f, 1.0f, 0.0f));
		camera->lookAt(eye, center, up);
	}
	// render player car
	currentCar->getModel(currentCar->pos, currentCar->yaw); // actualizar model
	currentCar->render(currentCar->mesh, currentCar->model, camera, currentCar->texture);

	// render enemy car
	currentCar = world->enemy_car;
	currentCar->getModel(currentCar->pos, currentCar->yaw); // actualizar model
	currentCar->render(currentCar->mesh, currentCar->model, camera, currentCar->texture);
}

void Stage::getKeyDownEvent(Camera* camera, int key_num) {
	Scene* world = Scene::instance;

	switch (key_num)
	{
		case 1: AddObjectInFont(camera, "data/assets/edificios/building-office-big_13.obj", "data/assets/color-atlas-new.png"); break;
		case 2: AddObjectInFont(camera, "data/assets/arboles/tree-birch_42.obj", "data/assets/color-atlas-new.png"); break;
		case 3: AddObjectInFont(camera, "data/assets/edificios/building-house-middle_7.obj", "data/assets/color-atlas-new.png"); break;
		case 4: AddObjectInFont(camera, "data/assets/checkpoints/finish_check.obj", "data/assets/checkpoints/finish_check.mtl"); break;
		case 5: AddObjectInFont(camera, "data/assets/checkpoints/check.obj", "data/assets/checkpoints/check.mtl"); break;
		/*case 6: AddObjectInFont(camera, "data/assets/edificios/building-house-middle_7.obj", "data/assets/color-atlas-new.png"); break;
		case 7: AddObjectInFont(camera, "data/assets/checkpoints/finish_check.obj", "data/assets/checkpoints/finish_check.mtl"); break;
		case 8: AddObjectInFont(camera, "data/assets/checkpoints/check.obj", "data/assets/checkpoints/check.mtl"); break;*/

		case 6: SelectEntity(camera); break;
		case 7: {
			EntityMesh* mesh = static_cast<EntityMesh*>(world->selected_entity);
			mesh->model.rotate(10.0f * DEG2RAD, Vector3(0, 1, 0));		}
		break;
		case 8: {
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

	myfile << "\t -entity: " << pos.x << " " << pos.y << "       " << pos.z << "     " << mesh << "     " << texture << "  " << "\n\n";
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
	//std::cout << "debug de pos" << 0 << " " << 0 << " " << 0 << "\n\n ";

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

		//std::cout << "debug de pos " << pos.x << " " << pos.y << " " << pos.z << " " << mesh << " " << texture << "\n";
	
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

void PlayStage::update(float seconds_elapsed) {
	Scene* world = Scene::instance;

	world->player_car->update(seconds_elapsed);
}

void PlayStage::renderMiniMap() {
	Scene* world = Scene::instance;
	EntityMesh* minimap = world->minimap;
	Game* game = Game::instance;

	glViewport(game->window_width - 200, game->window_height - 200, 200, 200);

	Camera minimapCam;
	minimapCam.setPerspective(60, 1, 0, 1000);

	EntityCar* currentCar = world->player_car;
	Matrix44 carmodel = currentCar->getModel(currentCar->pos, currentCar->yaw);
	Vector3 pos = carmodel.getTranslation();
	Vector3 forward = carmodel.rotateVector(Vector3(0, 0, -1));
	minimapCam.lookAt(pos + Vector3(0, 100, 0), pos, forward);
	minimapCam.enable();

	//render suelo de mapa
	Mesh floor;
	floor.createPlane(100);
	minimap->Entity::render(&floor, Matrix44(), &minimapCam, Vector4(1, 1, 1, 1), minimap->texture, world->global_Shader, 4, 1.0f);

	// render player posicion en minimap
	carmodel.scale(0.2, 0.2, 0.2);
	minimap->Entity::render(Mesh::Get("data/sphere.ASE"), carmodel, &minimapCam, Vector4(1, 1, 1, 1),NULL, minimap->shader, 4, 1.0f);

	//// render enemy posicion en minimap
	carmodel = world->enemy_car->getModel(world->enemy_car->pos, world->enemy_car->yaw);
	minimap->model.scale(0.02, 0.02, 0.02);
	minimap->Entity::render(minimap->mesh, carmodel, &minimapCam, Vector4(1, 0, 0, 1), NULL, minimap->shader, 4, 1.0f);
	
	//elimina
	//EntityMesh* entity = new EntityMesh();
	//entity->Entity::renderGUI(100, 100, 100, 30, true);

	glViewport(0, 0, game->window_width, game->window_height);
}


/********************************************************* End Stage *********************************************************/
TutorialStage::TutorialStage() {

}

void TutorialStage::render(Camera* camera) {

}

void TutorialStage::update(float elapse_time) {

}


/********************************************************* End Stage *********************************************************/
EndStage::EndStage() {

}

void EndStage::render(Camera* camera) {

}

void EndStage::update(float elapse_time) {

}