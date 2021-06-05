#include "entity.h"
#include "game.h"
#include "scene.h"
#include <string>
Entity::Entity(){

}
Entity::~Entity() {

}

void Entity::render(Camera* camera) {
	std::cout << "this is entity render" << std::endl;
}

void Entity::update(float elapsed_time) {
	std::cout << "this is entity update" << std::endl;
}

Matrix44 Entity::getGlobalMatrix()
{
	if (parent) //if I have a parent, ask his global and concatenate
		return model * parent->getGlobalMatrix();
	return model; //otherwise just return my model as global
}

bool Entity::isCollision(Entity* entity) {
	Scene* world = Scene::instance;
	
	Vector3 coll, collnorm;
	//sacar posicion de entity que recibimos
	Matrix44 global_matrix = entity->getGlobalMatrix();
	Vector3 position = global_matrix.getTranslation();

	//calculamos el centro de la esfera de colisi��n del player elevandola hasta la cintura
	Vector3 character_center = position + Vector3(0, 1, 0);

	//comprobamos si colisiona el objeto con la esfera (radio 3)
	bool iscollision = mesh->testSphereCollision(model, character_center, 2.1, coll, collnorm);

	// False = no hay colision
	// True = hay colision
	return iscollision;
}

void Entity::onCollision(float seconds_elapsed) {
	//model.getTranslation();
	//model.rotate(90.0f * DEG2RAD, Vector3(0.0f, 1.0f, 0.0f));
	
}

void EntityMesh::render(Camera* camera, float tiling)
{
	Game* game = Game::instance;
	if (this->meshType == EntityMesh::GRASS) tiling = 100.0f;

	// frustum check
	BoundingBox box = transformBoundingBox(model, mesh->box);
	if (!camera->testBoxInFrustum(box.center, box.halfsize)) return;


	if (shader)
	{
		//enable shader
		shader->enable();

		//upload uniforms
		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		if (texture != NULL) shader->setUniform("u_texture", texture, 0);
		shader->setUniform("u_model", model);
		shader->setUniform("u_time", time);
		shader->setUniform("u_texture_tiling", tiling);

		//do the draw call
		mesh->render(GL_TRIANGLES);

		//disable shader
		shader->disable();
	}

	//mesh->renderBounding(model);
}

void EntityMesh::update(float seconds_elapsed) {
	Game* game = Game::instance;
	Camera* camera = game->camera;
	Scene* world = Scene::instance;

	float speed = seconds_elapsed * game->mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant
	//example
	world->angle += (float)seconds_elapsed * 10.0f;

	////mouse input to rotate the cam
	if (world->free_camera)
	{
		if ((Input::mouse_state & SDL_BUTTON_LEFT) || game->mouse_locked) //is left button pressed?
		{
			camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
			camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
		}

		//////async input to move the camera around
		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
	}
}

void EntityCar::onCollision(float seconds_elapsed) {
	vel = vel * -0.1;
	pos = pos + (vel * seconds_elapsed);
}

void EntityCar::render(Mesh* mesh, Matrix44 model, Camera* camera, Texture* texture, float tiling)
{
	Game* game = Game::instance;
	// frustum check
	BoundingBox box = transformBoundingBox(model, mesh->box);
	if (!camera->testBoxInFrustum(box.center, box.halfsize)) return;


	if (shader)
	{
		//enable shader
		shader->enable();

		//upload uniforms
		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		if (texture != NULL) shader->setUniform("u_texture", texture, 0);
		shader->setUniform("u_model", model);
		shader->setUniform("u_time", time);
		shader->setUniform("u_texture_tiling", tiling);

		//do the draw call
		mesh->render(GL_TRIANGLES);

		//disable shader
		shader->disable();
	}

	mesh->renderBounding(model);
	//render the FPS, Draw Calls, etc
	std::string text = "velocidad: " + std::to_string(std::abs(vel.z));
	drawText(2, Game::instance->window_height - 20, text, Vector3(1, 1, 1), 2);
}

void EntityCar::update(float seconds_elapsed) {
	EntityMesh::update(seconds_elapsed);
	Game* game = Game::instance;
	Scene* world = Scene::instance;

	this->moving = false;
	vel_mod = vel.length();
	vel_mod = clamp(vel_mod, 0, 2.0f);
	float angular_acc = car_rot_speed * seconds_elapsed * vel_mod;
	//// fuera de carretera
	//for (size_t i = 0; i < world->dynamic_list.size(); i++)
	//{
	//	EntityCar* dynamic_entity = static_cast<EntityCar*>(world->dynamic_list.at(i));

	//	if (world->road->isCollision(dynamic_entity))
	//	{
	//		dynamic_entity->vel = dynamic_entity->vel * 0;
	//	}
	//}

	// Colision Event
	//{
	//	for (size_t i = 0; i < world->static_list.size(); i++)
	//	{
	//		for (size_t j = 0; j < world->dynamic_list.size(); j++)
	//		{
	//			Entity* dynamic_entity = world->dynamic_list.at(j);
	//			Entity* static_entity = world->static_list.at(i);

	//			// check colision
	//			if (static_entity->isCollision(dynamic_entity))
	//			{
 //					dynamic_entity->onCollision(seconds_elapsed);
	//			}
	//		}
	//	}
	//}


	////mouse input to rotate the cam
	if (!world->free_camera){

		Vector3 target;


		//Movimiento fisica
		{
			// Coordenada  local
			Vector3 carForward = Vector3(0.0f, 0.0f, -1.0f);
			// Convertir cordenada mundo
			carForward = model.rotateVector(carForward);

			if (Input::isKeyPressed(SDL_SCANCODE_W)) { vel = vel + (carForward * seconds_elapsed * acc_front); moving = true; }
			else if (Input::isKeyPressed(SDL_SCANCODE_S)) { vel = vel - (carForward * seconds_elapsed * acc_back); moving = true; }
			else {
				vel = vel - (vel * seconds_elapsed * 2.0f);
			}

			target = pos + (vel * seconds_elapsed);
			
			// rotate only when is moving 
			if (Input::isKeyPressed(SDL_SCANCODE_D) && moving) angular_vel += angular_acc;
			else if (Input::isKeyPressed(SDL_SCANCODE_A) && moving) angular_vel -= angular_acc;
			else {
				angular_vel = angular_vel - (angular_vel * seconds_elapsed * 10.0f);

			}
			angular_vel = clamp(angular_vel, -max_angular_acc, max_angular_acc);
			yaw = yaw + (angular_vel * seconds_elapsed);

		}

		/******************* check colision  *****************************/
		{
			Vector3 checkTarget = target;
			Entity* currentMesh = NULL;
			//Variable para chequear colision
			Vector3 coll;
			Vector3 collNorm;
			Vector3 characterTargetCenter = checkTarget + Vector3(0, 1, 0);

			//check for any static entity if there is a collision
			for (size_t i = 0; i < world->static_list.size(); i++)
			{
				// Break the game and show error.
				assert(world->static_list.at(i) != NULL);

				// Check entity type
				if (world->static_list.at(i)->getType() == ENTITY_TYPE_ID::MESH)
				{
					//DOWNCAST, BY STATIC_CAST
					currentMesh = world->static_list.at(i);
				}

				bool result = currentMesh->mesh->testSphereCollision(currentMesh->model, characterTargetCenter, 2.0, coll, collNorm);
				if (result)
				{
					//si la esfera est�� colisionando muevela a su posicion anterior alejandola del objeto
					Vector3 push_away = normalize(coll - characterTargetCenter) * seconds_elapsed;
					//move to previous pos but a little bit further
					checkTarget = pos - push_away * 10.0f;
					checkTarget.y = pos.y;
				}
			}	
			pos = checkTarget;
		}
	}

	//to navigate with the mouse fixed in the middle
	if (game->mouse_locked)
		Input::centerMouse();

	//switch entre vista coche o mundo
	if (Input::wasKeyPressed(SDL_SCANCODE_TAB))
	{
		world->free_camera = !(world->free_camera);
	}
}

Matrix44 EntityCar::get_CarModel() {
	model.setTranslation(pos.x, pos.y, pos.z);
	model.rotate(yaw * DEG2RAD, Vector3(0.0f, 1.0f, 0.0f));
	return model;
}