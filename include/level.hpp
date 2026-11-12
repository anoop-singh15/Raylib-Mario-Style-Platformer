#pragma once
#include<raylib.h>
#include<vector>
#include"player.hpp"
#include"tile.hpp"
#include"enemy.hpp"
#include "include/json.hpp"

using json = nlohmann::json;

class Level
{
    private:
    std::vector<Tile>m_Tiles;
    std::vector<Grass>m_Grasses;
    std::vector<Crate>m_Crates;
    std::vector<Coins>m_Coins;
    std::vector<Palm>m_FgTrees;
    std::vector<BgPalm>m_BgTrees;
    std::vector<Enemy>m_Enemies;
    std::vector<Constraints>m_Constraints;
    
   
    Player m_Player;
    LevelEnd m_End;

    Sound m_CoinSound;
    Sound m_StompSound;
    Sound m_HitSound;

   
    int GetFirstGid(const json&map,const std::string&name);
    
    
    public:
    Level(int levelIndex=0);
    void LoadLevel(const std::string&path);
    void Draw();
    void Update(float dT,int &score);
    inline Vector2 GetPlayerPos()const{
        return m_Player.GetPlayerCenter();
    }
    inline int GetPlayerHealth()const
    {
        return m_Player.GetHealth();
    }
    bool GetResult();

    

    ~Level();
};
