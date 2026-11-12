#pragma once
#include"level.hpp"
#include "Camera2DController.hpp"
#include"background.hpp"
#include<memory>
#include<vector>
#include"overworld.hpp"
#include"ui.hpp"



enum class GameState{
    OVERWORLD,
    LEVEL
};

class Game{

    private:
    GameState m_State=GameState::OVERWORLD;
    std::unique_ptr<Level>m_Level;
    std::unique_ptr<OverWorld>overWorld;
    Camera2DController camera;
    std::unique_ptr<BackGround>skyTop;
    std::unique_ptr<BackGround>skyMiddle;
    std::unique_ptr<BackGround>skyBottom;
    std::unique_ptr<OverWorldBackground>skyOverWorldTop;
    std::unique_ptr<OverWorldBackground>skyOverWorldMiddle;
    std::unique_ptr<OverWorldBackground>skyOverWorldBottom;

    std::unique_ptr<Water>water;
   std::vector<Clouds>clouds;
   std::vector<OverWorldPalm>m_OverWorldPalm;
   std::vector<OverWorldClouds>m_OverWorldClouds;
   std::unique_ptr<UI>m_Ui;
   int m_Score=0;
   int m_MaxLevel{};
  
    Music m_LevelMusic;
    Music m_OverWorldMusic;
    void ImportTextures();
    void SpawnTrees();
    void GameOver();
    
    public:
    
    Game();
    void Draw();
    void Update(float dT);
    ~Game();
};
