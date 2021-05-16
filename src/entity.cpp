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



void EntityMesh::render(Camera* camera)
{
	Game* game = Game::instance;
	
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

		//do the draw call
		mesh->render(GL_TRIANGLES);

		//disable shader
		shader->disable();
	}
}

void EntityMesh::update(float seconds_elapsed) {
	Game* game = Game::instance;
	Camera* camera = game->camera;
	Scene* world = Scene::instance;

	float speed = seconds_elapsed * game->mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant
	this->moving = false;
		//example
	world->angle += (float)seconds_elapsed * 10.0f;

	//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT) || game->mouse_locked) //is left button pressed?
	{
		camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
		camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
	}

	////async input to move the camera around
	//if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
	//if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
	//if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
	//if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
	//if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
	// Mesh movimente.
	float model_speed = seconds_elapsed * 200.0f;
	if (Input::isKeyPressed(SDL_SCANCODE_W)) {
		moving = true;
		this->model.translate(0.0f, 0.0f, -1.0f * model_speed);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_S)) {
		moving = true;
		this->model.translate(0.0f, 0.0f, 1.0f * model_speed);
	}
	// rotate only when is moving 
	if (Input::isKeyPressed(SDL_SCANCODE_D) && moving) model.rotate(90.0f * seconds_elapsed * DEG2RAD, Vector3(0.0f, 1.0f, 0.0f));
	if (Input::isKeyPressed(SDL_SCANCODE_A) && moving) model.rotate(-90.0f * seconds_elapsed * DEG2RAD, Vector3(0.0f, 1.0f, 0.0f));


	//to navigate with the mouse fixed in the middle
	if (game->mouse_locked)
		Input::centerMouse();

	if (Input::wasKeyPressed(SDL_SCANCODE_TAB))
	{
		world->free_camera = !(world->free_camera);
	}
}