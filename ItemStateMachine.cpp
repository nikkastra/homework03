#include <raylib.h>
#include <raymath.h>
#include <string>
#include <iostream>

#include "item.hpp"
#include "entity.hpp"

class Entity; 
float originalSpeed;
Entity* collided;


void Item::Update(float delta_time){
    current_state->Update(*this, delta_time);
}

void Item::Draw(){
    Rectangle e = {_position.x, _position.y, _size, _size};
    DrawRectanglePro(e, {_size/4, _size/4}, 20.0f, color);

}

void Item::SetState(ItemState* new_state){
    current_state = new_state;
    current_state->Enter(*this);
}

void Item::HandleCollision(Entity* entity,  float delta_time){ // need delta_time for a timer
    Vector2 q;
    originalSpeed = entity -> _speed;

    // for player - circle shaped
    if(_position.x < entity->_position.x){
        q.x = entity->_position.x;
    } else if (_position.x > entity->_position.x + entity->_size/2){
        q.x = entity->_position.x + entity->_size/2;
    } else {
        q.x = _position.x;
    }

    if(_position.y < entity->_position.y){
        q.y = entity->_position.y;
    } else if (_position.y > entity->_position.y + entity->_size/2){
        q.y = entity->_position.y + entity->_size/2;
    } else {
        q.y = _position.y;
    }

    if(Vector2Distance(_position, q) <= entity -> _radius){
        entity -> _speed = originalSpeed * speedBuff;
        _healthPoints -= 1;
        collided = entity;
        SetState(&buffDecay);
    }

    //for enemy - square shaped
}


Item::Item(Vector2 pos, float sz, float spdBuff, int hp){
    _position = pos;
    _size = sz;
    _healthPoints = hp;
    speedBuff = spdBuff;
    color = BLUE;
    SetState(&idle);
    
}


void ItemIdle::Enter(Item& item){
    item.color = BLUE;
}

void ItemIdle::Update(Item& item, float delta_time){
    //lmao do nothing
}

void ItemBuffDecay::Enter(Item& item){
    
}

void ItemBuffDecay::Update(Item& item, float delta_time){
    item.timer += delta_time;
    if(item.timer > 0.5f)
    {
        item.timer = 0;
        collided -> _speed = originalSpeed;
        item.SetState(&item.idle);
    }
}