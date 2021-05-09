#ifndef ENTITY_H
#define ENTITY_H
#include "camera.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"

class Entity
{
    public:
        Entity(); //constructor
        virtual ~Entity(); //destructor

        //some attributes 
        std::string name;
        Matrix44 model;

        //methods overwritten by derived classes 
        virtual void render(Camera* camera);
        virtual void update(float elapsed_time);

        //some useful methods...
        Vector3 getPosition();
};

class EntityMesh : public Entity
{
    public:
        enum meshType
        {
            CAR,
            HOUSE
        };
        //Attributes of this class 
        meshType meshType;
        bool moving = false;

        Mesh* mesh;
        Texture* texture;
        Shader* shader;
        Matrix44 model;
        Vector4 color;

        bool free_camera = false;

        //standar variable
        float angle = 0;

        //methods overwritten 
        void render(Camera* camera);
        void update(float dt);
};

#endif // ENTITY_H