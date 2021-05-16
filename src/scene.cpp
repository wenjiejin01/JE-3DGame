#include "scene.h"

Scene* Scene::instance = NULL;

Scene::Scene() {
	assert(instance == NULL);

	instance = this;
}
