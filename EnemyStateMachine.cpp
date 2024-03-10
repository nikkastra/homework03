#include <raylib.h>
#include <raymath.h>
#include <string>

#include "enemy.hpp"
#include "player.hpp"

void Enemy::Update(float delta_time){
    current_state->Update(*this, delta_time);
    float cosTheta = Vector2DotProduct({1,0}, Vector2Normalize(velocity));
    rotation = acos(cosTheta)*(180/3.14);
    if(velocity.y < 0){
        rotation *= -1;
    }
}

void Enemy::Draw(){
    Rectangle e = {position.x, position.y, size, size};
    DrawRectanglePro(e, {size/2, size/2}, rotation, color);
    DrawCircleLines(bodyCenter.x, bodyCenter.y, aggroRange, YELLOW);
    DrawCircleLines(bodyCenter.x, bodyCenter.y, detectionRange, ORANGE); 
    DrawCircleLines(bodyCenter.x, bodyCenter.y, attackRange, RED);
    DrawLineV(position, Vector2Add(position, Vector2Scale(Vector2Normalize(velocity), 50.0f)), BLACK);
}

void Enemy::SetState(EnemyState* new_state){
    current_state = new_state;
    current_state->Enter(*this);
}

void Enemy::HandleCollision(Player* player){
    if(detectionRange + player->radius > Vector2Distance(bodyCenter, player->position)){
        targetPos = player->position;
        SetState(&chasing);
    } else if (aggroRange + player->radius < Vector2Distance(bodyCenter, player->position)) {
        if(current_state != &wandering){
            SetState(&wandering);
        }
    }
}

Enemy::Enemy(Vector2 pos, float sz, float spd, int hp){
    position = pos;
    size = sz;
    speed = spd;
    SetState(&wandering);
    timer = 0;
    HP = hp;
    bodyCenter = {pos.x, pos.y};
    aggroRange = 3*sz + 50;
    detectionRange = 2*sz + 50;
    attackRange = sz + 50;
    damaged = false;
    damage = 2;
}

void EnemyWandering::Enter(Enemy& enemy){
    enemy.color = PINK;
    enemy.velocity = {enemy.speed*GetRandomValue(-100, 100)/100, enemy.speed*GetRandomValue(-100, 100)/100};
}

void EnemyWandering::Update(Enemy& enemy, float delta_time){
    enemy.position = Vector2Add(enemy.position, Vector2Scale(enemy.velocity, delta_time));
    enemy.bodyCenter = Vector2Add(enemy.bodyCenter, Vector2Scale(enemy.velocity, delta_time));
    enemy.timer += delta_time;
    if(enemy.timer > 1){
        enemy.timer = 0;
        enemy.velocity = {enemy.speed*GetRandomValue(-100, 100)/100, enemy.speed*GetRandomValue(-100, 100)/100};
    }
}

void EnemyChasing::Enter(Enemy& enemy){
    enemy.color = RED;
    enemy.velocity = Vector2Scale(Vector2Normalize(Vector2Subtract(enemy.targetPos, enemy.bodyCenter)), enemy.speed);
}

void EnemyChasing::Update(Enemy& enemy, float delta_time){
    enemy.position = Vector2Add(enemy.position, Vector2Scale(enemy.velocity, delta_time));
    enemy.bodyCenter = Vector2Add(enemy.bodyCenter, Vector2Scale(enemy.velocity, delta_time));
}