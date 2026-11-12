#pragma once
#include <raylib.h>

class Tile
{
private:
public:
    Rectangle m_Tile;
    int m_ScrRow;
    int m_ScrCol;
    float m_TileSize;
    Tile(float x, float y, float size, int scrRow, int scrCol);
    void Draw();
    ~Tile();
};

class Grass : public Tile
{
public:
    Grass(float x, float y, float size, int scrRow, int scrCol);
    void Draw();
    ~Grass();
};

class Crate : public Tile
{
public:
    Crate(float x, float y, float size, int scrRow, int scrCol);
    void Draw();
    Rectangle GetCollider();
    ~Crate();
};

class AnimatedTile
{
public:
    int m_MaxFrames;
    int m_CurrFrame;
    float m_FrameTime;
    float m_MaxFrameTime;
    bool m_Active;
    Vector2 m_Tile;

    AnimatedTile(float x, float y);
    void Update(float dT);
    void UpdateAnimations(float dT);
    ~AnimatedTile();
};

class Coins : public AnimatedTile
{
    Texture2D*m_CoinTex;
    float m_FrameWidth;
    public:
    int m_Type;
    int m_Value=0;
    Coins(float x, float y,int type);
    void Draw();
    Rectangle GetCoinCollider();
};

class Palm:public AnimatedTile
{
    
    public:
    int m_Type;
    Palm(float x,float y,int type);
    void Draw();
     Rectangle GetCollider();
};


class BgPalm:public AnimatedTile
{
    public:
    BgPalm(float x, float y);
    void Draw();
};