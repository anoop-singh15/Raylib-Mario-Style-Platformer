#pragma once
#include<raylib.h>
#include"resourceManager.hpp"

class BackGround{
    public:
    Texture2D*m_Tex=nullptr;  
    float m_HeightOffset;
    BackGround(Texture2D&tex,float offsetY);
    void Draw();
    
};


class Water:public BackGround{
    int m_CurrFrame;
    int m_MaxFrames;
    float m_FrameTime;
    float m_UpdatedFrameTime;
    public:

    Water(Texture2D&tex,float offsetY);
    void Draw();
    void Update(float dT);

};

class Clouds
{
    Vector2 m_Position;
    Texture2D*m_CloudTex=nullptr;
    float m_Speed;
   
    public:
        Clouds();
        void Draw();
        void Update(float dT);
        void Spawn(float cameraLeft,float viewWidth);
        bool IsOffScreen(float cameraLeft,float viewWidth);
       
};