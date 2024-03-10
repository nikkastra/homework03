#include <raylib.h>
#include <raymath.h>
#include <string>
#include <iostream>

#include "Player.hpp"

void Player::Update(float delta_time){
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

void Player::HandleCollision(Enemy* enemy){
    if(current_state == &attacking){
        Vector2 q;
        float leastDistance = 99999999999999999999.0f;
        if(Vector2Distance(hitboxPos, enemy->position) < leastDistance){
            leastDistance = Vector2Distance(hitboxPos, enemy->position);
            q = enemy->position;
        }
        if(Vector2Distance(hitboxPos, {enemy->position.x + enemy->size, enemy->position.y}) < leastDistance){
            leastDistance = Vector2Distance(hitboxPos, {enemy->position.x + enemy->size, enemy->position.y});
            q = {enemy->position.x + enemy->size, enemy->position.y};
        }
        if(Vector2Distance(hitboxPos, {enemy->position.x, enemy->position.y + enemy->size}) < leastDistance){
            leastDistance = Vector2Distance(hitboxPos, {enemy->position.x, enemy->position.y + enemy->size});
            q = {enemy->position.x, enemy->position.y + enemy->size};
        }
        if(Vector2Distance(hitboxPos, {enemy->position.x + enemy->size, enemy->position.y + enemy->size}) < leastDistance){
            leastDistance = Vector2Distance(hitboxPos, {enemy->position.x + enemy->size, enemy->position.y + enemy->size});
            q = {enemy->position.x + enemy->size, enemy->position.y + enemy->size};
        }

        if(Vector2Distance(hitboxPos, q) <= radius && !enemy->damaged){
            enemy->HP -= 1;
            enemy->damaged = true;
        }
    } else {
        enemy->damaged = false;
    }
}

Player::Player(Vector2 pos, float rad, float spd, int hp){
    position = pos;
    radius = rad;
    speed = spd;
    SetState(&idle);
    timer = 0;
    HP = hp;
    hitboxPos = {pos.x + rad +25.0f, pos.y};
    hitboxRad = rad/2;
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