#ifndef ENEMY
#define ENEMY

#include <raylib.h>
#include <raymath.h>

class Player;

class Enemy;

class EnemyState{
public:
    virtual ~EnemyState(){}
    virtual void Enter(Enemy& enemy) = 0;
    virtual void Update(Enemy& enemy, float delta_time) = 0;
};

class EnemyIdle : public EnemyState{
public:
    void Enter(Enemy& enemy);
    void Update(Enemy& enemy, float delta_time);
};

class EnemyWandering : public EnemyState{
public:
    void Enter(Enemy& enemy);
    void Update(Enemy& enemy, float delta_time);
};

class EnemyChasing : public EnemyState{
public:
    void Enter(Enemy& enemy);
    void Update(Enemy& enemy, float delta_time);
};

class EnemyReadyingAttack : public EnemyState{
public:
    void Enter(Enemy& enemy);
    void Update(Enemy& enemy, float delta_time);
};

class EnemyAttacking : public EnemyState{
public:
    void Enter(Enemy& enemy);
    void Update(Enemy& enemy, float delta_time);
};

class Enemy{
public:
    Player* player;

    Vector2 position;
    float size;
    Color color;

    Vector2 velocity;
    float speed;
    float rotation;

    Vector2 bodyCenter;
    float detectionRange;
    float aggroRange;
    float attackRange;

    bool nearOtherEnemy;

    float timer;
    int HP;
    bool damaged;

    //target player
    Vector2 targetPos;
    float damage;

    EnemyWandering wandering;
    EnemyChasing chasing;
    EnemyReadyingAttack readyingAttack;
    EnemyAttacking attacking;

    Enemy(Vector2 pos, float sz, float spd, int hp);

    void Update(float delta_time);
    
    void Draw();

    void SetState(EnemyState* new_state);

    void HandleCollision(Player* player);

    void HandleEnemyCollision(Enemy* enemy);

private:
    EnemyState* current_state;
};

#endif