#include <raylib.h>
#include <raymath.h>
#include <string>
#include <iostream>
#include <vector>

#include "PlayerStateMachine.cpp"
#include "EnemyStateMachine.cpp"
#include "entity.hpp"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const float FPS = 60;

int main(){

    //full entity parameter: 
    //Entity(Vector2 pos = {0,0}, float radius = 0.0f, float size = 0.0f, float speed = 0, float healthPoints = 0);
    // note that the equal signs are default values
    Player player({WINDOW_WIDTH/2, WINDOW_HEIGHT/2}, 50.0f, 200.0f, 5); // for player Player(Vector2 pos, float rad, float spd, int hp);
    Enemy enemy1({WINDOW_WIDTH/8, WINDOW_HEIGHT/8}, 100.0f, 100.0f, 2); // for enemy Vector2 pos, float sz, float spd, int hp
    Enemy enemy2({7*WINDOW_WIDTH/8, 7*WINDOW_HEIGHT/8}, 100.0f, 100.0f, 2); 


    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "miss ko na siya");
    SetTargetFPS(FPS);

    Camera2D camera_view = {0};
    float EDGE_X[2], EDGE_Y[2];
    camera_view.offset = {WINDOW_WIDTH/2, WINDOW_HEIGHT/2};
    camera_view.zoom = 1.0f;

    Texture joshHutcherson = LoadTexture("josh_hutcherson.png");

    while(!WindowShouldClose()){
        float delta_time = GetFrameTime();

        camera_view.target = player._position;

        player.Update(delta_time);
        if(enemy1._healthPoints > 0){
            player.HandleCollision(&enemy1);
            enemy1.Update(delta_time);
            enemy1.HandleCollision(&player);
            enemy2.HandleEnemyCollision(&enemy1);
        }
        if(enemy2._healthPoints > 0){
            player.HandleCollision(&enemy2);
            enemy2.Update(delta_time);
            enemy2.HandleCollision(&player);
            enemy1.HandleEnemyCollision(&enemy2);
        }

        BeginDrawing();
        ClearBackground(WHITE);
        BeginMode2D(camera_view);
        DrawTexture(joshHutcherson, 0, 0, WHITE);
        if(enemy1._healthPoints > 0){
            enemy1.Draw();
        }
        if(enemy2._healthPoints  > 0){
            enemy2.Draw();
        }
        player.Draw();
        EndMode2D();
        char buffer[10];
        sprintf_s(buffer, "%.0i", player._healthPoints);
        DrawText(buffer, 10, 10, 100, BLACK);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}