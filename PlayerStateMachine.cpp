#include <raylib.h>
#include <raymath.h>
#include <string>
#include <iostream>

#include "Player.hpp"
#include "entity.hpp"

class Entity;

void Player::Update(float delta_time){
    if(damaged){
        invulnerabilityTimer += delta_time;
        if(invulnerabilityTimer > 2.0f){
            damaged = false;
            invulnerabilityTimer = 0;
        }
    }
    current_state->Update(*this, delta_time);
}

void Player::Draw(){
    DrawCircleV(position, radius, color);
    if(current_state == &attacking){
        DrawCircleV(hitboxPos, hitboxRad, color);
    }
}

void Player::SetState(PlayerState* new_state){
    current_state = new_state;
    current_state->Enter(*this);
}

void Player::HandleCollision(Entity* entity){
    Vector2 q;

    if(current_state == &attacking){
        if(hitboxPos.x < entity->position.x){
            q.x = entity->position.x;
        } else if (hitboxPos.x > entity->position.x + entity->size){
            q.x = entity->position.x + entity->size;
        } else {
            q.x = hitboxPos.x;
        }

        if(hitboxPos.y < entity->position.y){
            q.y = entity->position.y;
        } else if (hitboxPos.y > entity->position.y + entity->size){
            q.y = entity->position.y + entity->size;
        } else {
            q.y = hitboxPos.y;
        }

        if(Vector2Distance(hitboxPos, q) <= radius && !entity->damaged){
            entity->healthPoints -= 1;
            entity->damaged = true;
        }
    } else {
        entity->damaged = false;

        if(position.x < entity->position.x){
            q.x = entity->position.x;
        } else if (position.x > entity->position.x + entity->size/2){
            q.x = entity->position.x + entity->size/2;
        } else {
            q.x = position.x;
        }

        if(position.y < entity->position.y){
            q.y = entity->position.y;
        } else if (position.y > entity->position.y + entity->size/2){
            q.y = entity->position.y + entity->size/2;
        } else {
            q.y = position.y;
        }

        if(Vector2Distance(position, q) <= radius && !damaged){
            if(current_state == &dodging){
                HP -= 0;
            } else if (current_state == &blocking){
                HP -= (int) entity->damage/2;
            } else {
                HP -= (int) entity->damage;
            }
            damaged = true;
        }
    }
}

Player::Player(Vector2 pos, float rad, float spd, int hp): Entity(position, radius, /*size */ 0.0f,speed, healthPoints){
    position = pos;
    radius = rad;
    speed = spd;
    SetState(&idle);
    timer = 0;
    invulnerabilityTimer = 0;
    HP = hp;
    hitboxPos = {pos.x + rad +25.0f, pos.y};
    hitboxRad = rad/2;
    damaged = false;
}


void PlayerIdle::Enter(Player& player){
    player.color = PINK;
}

void PlayerMoving::Enter(Player& player){
    player.color = GREEN;
}

void PlayerDodging::Enter(Player& player){
    player.color = ORANGE;
}

void PlayerAttacking::Enter(Player& player){
    player.color = BROWN;
}

void PlayerBlocking::Enter(Player& player){
    player.color = BLUE;
}

void PlayerIdle::Update(Player& player, float delta_time){
    if(IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D)){
        player.SetState(&player.moving);
    }
    if(IsMouseButtonDown(0)) {
        player.SetState(&player.attacking);
    }
    if(IsMouseButtonDown(1)) {
        player.SetState(&player.blocking);
    }
}

void PlayerMoving::Update(Player& player, float delta_time){
    player.velocity = Vector2Zero();
    if(IsKeyDown(KEY_W)){
        player.velocity = {0.0f, -1*player.speed};
    }
    if(IsKeyDown(KEY_A)){
        player.velocity = { -1*player.speed, 0.0f};
    }
    if(IsKeyDown(KEY_S)){
        player.velocity = {0.0f, player.speed};
    }
    if(IsKeyDown(KEY_D)){
        player.velocity = {player.speed, 0.0f};
    }
    if(IsKeyPressed(KEY_SPACE)){
        player.SetState(&player.dodging);
    }
    player.position = Vector2Add(player.position, Vector2Scale(player.velocity, delta_time));

    if(IsMouseButtonDown(0)) {
        player.SetState(&player.attacking);
    }

    if(Vector2Length(player.velocity) == 0) {
        player.SetState(&player.idle);
    } else {
        player.hitboxPos = {player.position.x + player.velocity.x/player.speed*(player.radius + 25.0f), player.position.y + player.velocity.y/player.speed*(player.radius + 25.0f)};
    }
}

void PlayerDodging::Update(Player& player, float delta_time){
    player.position = Vector2Add(player.position, Vector2Scale(Vector2Scale(player.velocity, delta_time), 10.0f));
    player.timer += delta_time;
    if(player.timer > 0.125){
        player.timer = 0;
        player.SetState(&player.idle);
    }
}

void PlayerAttacking::Update(Player& player, float delta_time){
    player.timer += delta_time;
    if(player.timer > 0.25){
        player.timer = 0;
        player.SetState(&player.idle);
    }
}

void PlayerBlocking::Update(Player& player, float delta_time){
    if(IsMouseButtonReleased(1)){
        player.SetState(&player.idle);
    }
}