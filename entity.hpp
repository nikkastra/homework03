#ifndef ENTITY
#define ENTITY

#include <raylib.h>
#include <raymath.h>

//TO DO 
// considering doing optional parameters
// player parameters needed: (Vector2 pos, float rad, float spd, int hp)
// enemy parameters needed: (Vector2 pos, float sz, float spd, int hp)
// item parameters needed: (Vector2 pos, float size)

/*
class Entity; 


class EntityState {
public:
    virtual ~EntityState() {}
    virtual void Enter(Entity& entity) = 0;
    virtual void Update(Entity& entity, float delta_time) = 0;
};

class EntityCheckCollision: public EntityState{
    virtual void Enter(Entity& entity);
    virtual void Update(Entity& entity, float delta_time);
};*/

class Entity{
public: 

    Vector2 _position; 
    float _radius;
    float _size;
    float _speed;
    int _healthPoints;


    float _damage;
    bool _damaged;
    
    //Entity(Vector2 pos = {0,0}, float radius = 0.0f, float size = 0.0f, float speed = 0, float healthPoints = 0);

    //EntityCheckCollision eCollision;

   
    //void Update(float delta_time);
    
    //void SetState(EntityState* new_state);

private:
    
    //EntityState* current_state;

};



#endif