#include "entity.h"
#include "game.h"
#include "scene.h"

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

	mesh->renderBounding(model);
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

bool EntityMesh::isCollision(Vector3 targetPos) {
	Scene* world = Scene::instance;


	return false;
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
}

void EntityCar::update(float seconds_elapsed) {
	EntityMesh::update(seconds_elapsed);
	Game* game = Game::instance;
	Scene* world = Scene::instance;

	this->moving = false;

	////mouse input to rotate the cam
	if (!world->free_camera){

		// Mesh movimente.
		float model_speed = seconds_elapsed * 30.0f;
		target = Vector3(0, 0, 0);
		// Coordenada  local
		Vector3 goFront = Vector3(0.0f, 0.0f, 1.0f);
		// Convertir cordenada mundo
		goFront = model.rotateVector(goFront);

		if (Input::isKeyPressed(SDL_SCANCODE_W)) {
			moving = true;
			target = goFront * -model_speed;
			//model.translate(0.0f, 0.0f, -1.0f * model_speed);
		}
		if (Input::isKeyPressed(SDL_SCANCODE_S)) {
			moving = true;
			target = goFront * model_speed;
			//model.translate(0.0f, 0.0f, 1.0f * model_speed);
		}
		// rotate only when is moving 
		if (Input::isKeyPressed(SDL_SCANCODE_D) && moving) yaw += rot_speed * seconds_elapsed;//model.rotate(-90.0f * seconds_elapsed * DEG2RAD, Vector3(0.0f, -1.0f, 0.0f));
		if (Input::isKeyPressed(SDL_SCANCODE_A) && moving) yaw -= rot_speed * seconds_elapsed; //model.rotate(-90.0f * seconds_elapsed * DEG2RAD, Vector3(0.0f, 1.0f, 0.0f));
	
		//check colision
		Vector3 checkTarget = target + pos;
		EntityMesh* currentMesh = NULL;
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
				currentMesh = static_cast<EntityMesh*>(world->static_list.at(i));
			}

			bool result = currentMesh->mesh->testSphereCollision(currentMesh->model, characterTargetCenter, 1.0, coll, collNorm);
			if (result)
			{
				Vector3 push_away = normalize(coll - characterTargetCenter) * seconds_elapsed;
				checkTarget = pos - push_away;
				checkTarget.y = pos.y;
			}
		}

		pos = checkTarget;
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