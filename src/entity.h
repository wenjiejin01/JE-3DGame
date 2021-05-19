#ifndef ENTITY_H
#define ENTITY_H
#include "camera.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"

enum ENTITY_TYPE_ID {
    MESH,
    SOUND,
};

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
        virtual ENTITY_TYPE_ID getType() = 0;

        ////some useful methods...
        //Vector3 getPosition();
};

class EntityMesh : public Entity
{
public:
    enum meshType
    {
        CAR,
        HOUSE,
        ROAD,
        GRASS,
        SKY

    };
    //Attributes of this class 
    meshType meshType;
    bool moving = false;

    Mesh* mesh;
    Texture* texture;
    Shader* shader;
    Matrix44 model;
    Vector4 color;
   

    //methods overwritten 
    void render(Camera* camera , float tiling = 1.0f);
    void update(float dt);
    ENTITY_TYPE_ID getType() { return ENTITY_TYPE_ID::MESH;};
};

#endif // ENTITY_H
