#ifndef ENTITY_H
#define ENTITY_H
#include "camera.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"

enum ENTITY_TYPE_ID {
    ENTITY,
    MESH,
    CAR,
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
    virtual void render(Camera* camera , float tiling = 1.0f);
    virtual void update(float dt);
    bool isCollision(Vector3 target);
    ENTITY_TYPE_ID getType() { return ENTITY_TYPE_ID::MESH;};
};

class EntityCar : public EntityMesh
{
public:
    Vector3 pos;
    Vector3 target;
    Vector3 vel;
    float yaw;
    float acc_front = 15.0f;
    float acc_back = 10.0f;
    float car_rot_speed = 10.0f;
    float max_speed = 40.0f;
    float max_angular_acc = 100.0f;
    float vel_mod;
    float angular_vel;


    //methods overwritten 
    void render(Mesh* mesh, Matrix44 model, Camera* camera, Texture* texture, float tiling = 1.0f);
    void update(float dt);
    ENTITY_TYPE_ID getType() { return ENTITY_TYPE_ID::CAR; };
    Matrix44 get_CarModel();
};

#endif // ENTITY_H
