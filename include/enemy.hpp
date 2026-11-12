#pragma once
#include<raylib.h>

class Enemy{
    Rectangle m_DeathPos={};
    float m_FrameTime;
    float m_UpdatedFrameTime;    
    int m_CurrFrame;
    int  m_MaxFrames;
    bool m_IsAlive;
    bool m_IsDeathAnimationFinished=false;
   


    void UpdateAnimations(float dT);

    
    public:
    Rectangle m_Enemy;
    int m_Direction;
     float m_Speed;
    Enemy(float x,float y);
    void Draw();
    void Update(float dT);
    void ChangeState();
    bool GetDeathAnimationFinished(){return m_IsDeathAnimationFinished;}
    Rectangle GetEnemyCollider();
};

class Constraints{
    Vector2 m_Collider;
    public:
    Constraints(float x,float y);
    Rectangle GetCollider();
};
