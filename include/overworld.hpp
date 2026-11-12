#pragma once
#include"global.hpp"
#include<vector>

class OverWorld{
    bool m_IsMoving;
    bool m_EnterLevel;
    int m_LevelIndex;
    int m_MaxLevel;
    int m_CurrFrame;
    int m_MaxFrames;
    float m_FrameTime;
    float m_MaxFrameTime;
    float m_FrameWidth;
    const float m_Speed=OverWorldData::PLAYER_SPEED;
    std::vector<OverWorldData::STAGEINFO>m_Stages;
    Vector2 m_Direction;
    Vector2 m_PlayerPos;
    Vector2 m_TargetPos;
    Texture2D*m_PlayerTex=nullptr;
    Texture2D*m_BgTex=nullptr;
    

    void HandleInput();
    void PlayerMovement(float dT);
    void UpdateAnimations(float dT);
    Vector2 GetDirection();
    Vector2 GetPlayerCenter(int levelIndex);
   
    public:
    OverWorld(int levelIndex,int maxLevel);
    void Draw();
    void DrawLines();
    void DrawPlayer();
    void Update(float dT);
    bool ShouldEnterLevel();
    void ResetEnterRequest();
    int GetLevel()const{return m_LevelIndex;}
    void  SetLevelIndex(int index){ m_LevelIndex=index;}
    void  SetMaxLevelIndex(int index){ m_MaxLevel=index;}
    void ResetPlayerPosition();
};

class OverWorldPalm{
     int m_CurrFrame;
    int m_MaxFrames;
    float m_FrameTime;
    float m_MaxFrameTime;
    Texture2D*m_PalmTex;
    void UpdateAnimations(float dT);
    public:
    Vector2 m_Position;
    OverWorldPalm(float posX);
    void Draw();
    void Update(float dT);

};

struct OverWorldBackground{
    float m_HeightOffset;
    float m_TextureHeight;
    Texture2D*m_Tex;
    void TextureInit(Texture2D&tex,float heightOffset,float);
    void Draw();
};

class OverWorldClouds{
    Texture2D*m_CloudTex;
    Vector2 m_Position;
    public:
    OverWorldClouds();
    void Draw();
};
