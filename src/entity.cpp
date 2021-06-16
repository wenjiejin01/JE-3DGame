#include "entity.h"
#include "game.h"
#include "scene.h"
#include <string>
Entity::Entity(){
	havesound = false;
}

Entity::~Entity() {

}

void Entity::render(Mesh* sentmesh, Matrix44 sentmodel, Camera* camera, Vector4 color, Texture* senttexture = NULL, Shader* sentshader = NULL, int primitive = 4, float tiling = 1.0f) {
	Game* game = Game::instance;

	// frustum check
	BoundingBox box = transformBoundingBox(sentmodel, sentmesh->box);
	if (!camera->testBoxInFrustum(box.center, box.halfsize)) return;

	if (sentshader)
	{
		//enable shader
		sentshader->enable();

		//upload uniforms
		sentshader->setUniform("u_color", color);
		sentshader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		if (senttexture != NULL) sentshader->setUniform("u_texture", senttexture, 0);
		sentshader->setUniform("u_model", sentmodel);
		sentshader->setUniform("u_time", time);
		sentshader->setUniform("u_texture_tiling", tiling);

		//do the draw call
		sentmesh->render(primitive);

		//disable shader
		sentshader->disable();
	}
}

void Entity::update(float elapsed_time) {
	std::cout << "this is entity update" << std::endl;
}

bool Entity::renderButton(float x, float y, float w, float h, bool flipuvs) {
	Camera cam2D;
	cam2D.setOrthographic(0, Game::instance->window_width, Game::instance->window_height, 0, -1, 1);
	cam2D.enable();

	Mesh quad;
	quad.createQuad(x, y, w, h, flipuvs);

	//hover effect
	Vector2 mouse = Input::mouse_position;
	float halfWidth = w * 0.5;
	float halfhight = h * 0.5;
	float min_x = x - halfWidth;
	float max_x = x + halfWidth;
	float min_y = y - halfhight;
	float max_y = y + halfhight;

	bool hover = mouse.x > min_x && mouse.y > min_y && mouse.x < max_x&& mouse.y < max_y;
	bool pressed = Input::wasMouseButtonDown; 

	Vector4 normalColor = Vector4(1, 1, 1, 0.6);
	Vector4 hoverColor = Vector4(1, 1, 1, 1);


	// utiliza texture, model y shader de entity
	shader->enable();
	shader->setUniform("u_color", hover ? hoverColor : normalColor);
	shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
	shader->setUniform("u_texture", texture, 0);
	shader->setUniform("u_model", model);
	shader->setUniform("u_texture_tiling", 1.0f);
	quad.render(GL_TRIANGLES);
	shader->disable();

	return hover && pressed;
}

Matrix44 Entity::getGlobalMatrix()
{
	if (parent) //if I have a parent, ask his global and concatenate
		return model * parent->getGlobalMatrix();
	return model; //otherwise just return my model as global
}

bool Entity::isCollision(Entity* entity, Matrix44 sentModel, float radio) {
	Scene* world = Scene::instance;
	
	Vector3 coll, collnorm;
	//sacar posicion de entity que recibimos
	//Matrix44 global_matrix = entity->getGlobalMatrix();
	Vector3 position = sentModel.getTranslation();
	
	//calculamos el centro de la esfera de colisi��n del player elevandola hasta la cintura
	Vector3 character_center = position + Vector3(0, 1, 0);

	//comprobamos si colisiona el objeto con la esfera (radio 3)
	bool iscollision = mesh->testSphereCollision(model, character_center, radio, coll, collnorm);

	// False = no hay colision
	// True = hay colision
	return iscollision;
}

void Entity::onCollision(float seconds_elapsed) {
	//model.getTranslation();
	//model.rotate(90.0f * DEG2RAD, Vector3(0.0f, 1.0f, 0.0f));
	
}

void Entity::pathfinder(int startx, int starty, int targety, int targetx) {
	Scene* world = Scene::instance;

	world->current_map_steps = AStarFindPathNoTieDiag(
		startx, starty, //origin (tienen que ser enteros)
		targetx, targety, //target (tienen que ser enteros)
		world-> map, //pointer to map data
		world->map_width, world->map_height, //map width and height
		world->output, //pointer where the final path will be stored
		100);
}

void EntityMesh::render(Camera* camera, Vector4 color, float tiling)
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
		shader->setUniform("u_color", color);
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

	//Render Sound
	if (havesound == false) {
		sound = new SoundManager();
		sound->playSound("RALENTI", true);
		sound->SetVolume("RALENTI", 0.5);
		havesound = true;
	}

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

	//Render Sound
	//if (Input::isKeyPressed(SDL_SCANCODE_W)) {
	//	sound->SetVolume("RALENTI", 0.1);
	//	/*sound2 = new SoundManager();*/
	///*	if (sound2->IsStarted("CAR_ACC") == false) {
	//		sound2->playSound("CAR_ACC", false);
	//		havesound = true;
	//	}*/
	//}
	//else if (Input::isKeyPressed(SDL_SCANCODE_S)) {}
	//else {
	//}
	//// freno
	//if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT))
	//{
	//	//sonido giro
	//	sound->SetVolume("RALENTI", 0.25);
	//	/*			sound2 = new SoundManager();
	//				sound2->playSound("FRENO_MANO", false);
	//				sound2->SetVolume("FRENO_MANO", 0.6);*/
	//}
	//if (Input::isKeyPressed(SDL_SCANCODE_D)) {
	//	sound->SetVolume("RALENTI", 0.25);
	//	sound2->StopSound("CAR_ACC");
	//	/*sound2 = new SoundManager();
	//	if (sound2->IsStarted("GIRO") == false) {
	//		sound2->playSound("GIRO", false);
	//		sound2->SetVolume("GIRO", 0.6);
	//	}*/
	//}
	//else if (Input::isKeyPressed(SDL_SCANCODE_A)) {
	//	sound->SetVolume("RALENTI", 0.25);
	//	sound2->StopSound("CAR_ACC");
	//	sound2 = new SoundManager();
	//	/*if (sound2->IsStarted("GIRO") == false) {
	//		sound2->playSound("GIRO", false);
	//		sound2->SetVolume("GIRO", 0.6);
	//	}*/
	//}
	//else {
	//	sound->SetVolume("RALENTI", 0.5);

	//}

}

void EntityCar::update(float seconds_elapsed) {
	EntityMesh::update(seconds_elapsed);
	Game* game = Game::instance;
	Scene* world = Scene::instance;

	this->moving = false;
	vel_mod = vel.length();
	vel_mod = clamp(vel_mod, 0, 2.0f);
	float angular_acc = car_rot_speed * seconds_elapsed * vel_mod;
	// fuera de carretera

	EntityCar* car_entity = world->player_car;

	// colision contra fuera de carretera
	if (!world->road->isCollision(car_entity, car_entity->model))
	{
		car_entity->vel = car_entity->vel * 0.99;
	}

	// comprobar targets.
	int size = world->static_list.size();
	for (size_t i = 0; i < size; i++)
	{
		Entity* static_entity = world->static_list.at(i);
		if (static_entity->name == "target" && !static_entity->isBroken)
		{
			Matrix44 hight_model = car_entity->model;
			hight_model.translate(0, 2, 0);
			if (static_entity->isCollision(car_entity, hight_model, 3))
			{
				world->live_time += 5.0f;
				world->target_visited++;
				static_entity->isBroken = true;

				int targetx = clamp(world->player_car->pos.x, 0, world->map_width);
				int targety = clamp(world->player_car->pos.z, 0, world->map_height);
				int startx = clamp(world->enemy_car->pos.x, 0, world->map_height);
				int starty = clamp(world->enemy_car->pos.z, 0, world->map_width);
				
				//Target sound
				sound2 = new SoundManager();
				sound2->playSound("TARGET", false);

				world->enemy_car->pathfinder(startx, starty, targety, targetx);
			}
		}
	}

	////mouse input to rotate the cam
	if (!world->free_camera){

		Vector3 target;

		//Movimiento fisica
		{
			// Coordenada  local
			Vector3 carForward = Vector3(0.0f, 0.0f, -1.0f);
			// Convertir cordenada mundo
			carForward = model.rotateVector(carForward);

			if (Input::isKeyPressed(SDL_SCANCODE_W)) { 
				vel = vel + (carForward * seconds_elapsed * acc_front); 
				moving = true;
			}
			else if (Input::isKeyPressed(SDL_SCANCODE_S)) { vel = vel - (carForward * seconds_elapsed * acc_back); moving = true; }
			else  {
				vel = vel - (vel * seconds_elapsed * 5.0f);
			}
			// freno
			if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT))
			{
				vel = vel * 0;
			}

			target = pos + (vel * seconds_elapsed);
			// rotate only when is moving 
			if (Input::isKeyPressed(SDL_SCANCODE_D) && moving) {
				angular_vel += angular_acc;
			}
			else if (Input::isKeyPressed(SDL_SCANCODE_A) && moving) { 
				angular_vel -= angular_acc;
			}
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

void EntityCar::ResetCar() {
	pos = pos * 0;
	vel = vel * 0;
	yaw = 0;
}

Matrix44 Entity::getModel(Vector3 pos, float yaw) {
	model.setTranslation(pos.x, pos.y, pos.z);
	model.rotate(yaw * DEG2RAD, Vector3(0.0f, 1.0f, 0.0f));
	return model;
}

void EntityAnimation::render(Mesh* mesh, Matrix44 model, Camera* camera, Texture* texture, Skeleton* sk, float tiling)
{
	Game* game = Game::instance;
	if (this->meshType == EntityMesh::PERSONA) tiling = 100.0f;

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
		mesh->renderAnimated(GL_TRIANGLES, sk);

		//disable shader
		shader->disable();
	}
}

void EntityAnimation::update(float dt)
{
	Scene* world = Scene::instance;

	EntityMesh::update(dt);
}

Matrix44 EntityAnimation::get_AnimationModel()
{
	model.setTranslation(pos.x, pos.y, pos.z);
	model.rotate(yaw * DEG2RAD, Vector3(0.0f, 1.0f, 0.0f));
	return model;
}
