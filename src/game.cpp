#include "include/game.hpp"
#include "include/global.hpp"
#include "include/resourceManager.hpp"

Game::Game()
{

    ImportTextures();
    skyTop = std::make_unique<BackGround>(ResourceManager::GetTexture("sky_top"), 0.f);

    skyMiddle = std::make_unique<BackGround>(ResourceManager::GetTexture("sky_middle"), Config::SCREEN_HEIGHT / 3.f);
    skyBottom = std::make_unique<BackGround>(ResourceManager::GetTexture("sky_bottom"), Config::SCREEN_HEIGHT / 3.f + Config::SCREEN_HEIGHT / 3.f);

    water = std::make_unique<Water>(ResourceManager::GetTexture("water"), Config::SCREEN_HEIGHT - 32.f);

    clouds.reserve(3);
    float cameraLeft = camera.m_Camera.target.x - camera.m_Camera.offset.x / camera.m_Camera.zoom;
    float viewWidth = Config::SCREEN_WIDTH / camera.m_Camera.zoom;
    for (int i = 0; i < 3; i++)
    {
        clouds.emplace_back();
        clouds.back().Spawn(cameraLeft, viewWidth);
    }


    m_LevelMusic=LoadMusicStream("assets/audio/level_music.wav");
    m_OverWorldMusic=LoadMusicStream("assets/audio/overworld_music.wav");

    m_OverWorldPalm.reserve(15);
    SpawnTrees();

    m_MaxLevel=0;
    overWorld = std::make_unique<OverWorld>(0, m_MaxLevel);

    skyOverWorldTop = std::make_unique<OverWorldBackground>();
    skyOverWorldTop->TextureInit(ResourceManager::GetTexture("sky_top"), 0.f, Config::SCREEN_HEIGHT - (3 * 64.f));

    skyOverWorldMiddle = std::make_unique<OverWorldBackground>();
    skyOverWorldMiddle->TextureInit(ResourceManager::GetTexture("sky_middle"), 8 * 64.f, (1 * 64.f));

    skyOverWorldBottom = std::make_unique<OverWorldBackground>();
    skyOverWorldBottom->TextureInit(ResourceManager::GetTexture("sky_bottom"), 9 * 64.f, Config::SCREEN_HEIGHT - (1 * 64.f));

    m_OverWorldClouds.reserve(10);
    for (int i = 0; i < 7; i++)
    {
        m_OverWorldClouds.emplace_back();
    }

    m_Ui = std::make_unique<UI>();
    PlayMusicStream(m_OverWorldMusic);
}

void Game::Draw()
{

    if (m_State == GameState::OVERWORLD)
    {
        skyOverWorldTop->Draw();
        for (auto &overworldClouds : m_OverWorldClouds)
        {
            overworldClouds.Draw();
        }
        skyOverWorldMiddle->Draw();
        skyOverWorldBottom->Draw();

        for (auto &overWorldPalm : m_OverWorldPalm)
        {
            overWorldPalm.Draw();
        }
        overWorld->DrawLines();
        overWorld->Draw();
        overWorld->DrawPlayer();
        return;
    }

    skyTop->Draw();

    skyMiddle->Draw();
    skyBottom->Draw();

    ///////////////////
    // Camera Starts
    //////////////////
    camera.BeginMode();
    for (auto &cloud : clouds)
    {
        cloud.Draw();
    }
    m_Level->Draw();

    camera.EndMode();
    ///////////////////
    // Camera Ends
    ///////////////////
    m_Ui->DrawHealth();
    m_Ui->DrawCoins();
    m_Ui->DrawCoinsText();
    water->Draw();
}

void Game::Update(float dT)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        StopMusicStream(m_LevelMusic);
        m_State = GameState::OVERWORLD;
        PlayMusicStream(m_OverWorldMusic);
        SpawnTrees();
        m_OverWorldClouds.clear();
        for (int i = 0; i < 7; i++)
        {
            m_OverWorldClouds.emplace_back();
        }
       
    }

    if (m_State == GameState::OVERWORLD)
    {
        UpdateMusicStream(m_OverWorldMusic);

        overWorld->Update(dT);
        for (auto &overWorldPalm : m_OverWorldPalm)
        {
            overWorldPalm.Update(dT);
        }
        if (overWorld->ShouldEnterLevel())
        {
            camera.m_Camera.target = {0.f, Config::SCREEN_HEIGHT / 2.f};
            m_Level = std::make_unique<Level>(overWorld->GetLevel());
            m_State = GameState::LEVEL;
            overWorld->ResetEnterRequest();
              StopMusicStream(m_OverWorldMusic);
              PlayMusicStream(m_LevelMusic);
        }
        return;
    }
    if (!m_Level)
    {
        m_State = GameState::OVERWORLD;
        return;
    }
    
    GameOver();
    
    UpdateMusicStream(m_LevelMusic);

    camera.Update(dT, m_Level->GetPlayerPos());
    m_Level->Update(dT,m_Score);
   
    m_Ui->SetCoins(m_Score);
    m_Ui->UpdateHealthBar(m_Level->GetPlayerHealth());
    water->Update(dT);
    float cameraLeft = camera.m_Camera.target.x - camera.m_Camera.offset.x / camera.m_Camera.zoom;
    float viewWidth = Config::SCREEN_WIDTH / camera.m_Camera.zoom;
    for (auto &cloud : clouds)
    {
        cloud.Update(dT);
        if (cloud.IsOffScreen(cameraLeft, viewWidth))
        {
            cloud.Spawn(cameraLeft, viewWidth);
        }
    }

    if (m_Level->GetResult())
    {
        StopMusicStream(m_LevelMusic);
        m_State = GameState::OVERWORLD;
        PlayMusicStream(m_OverWorldMusic);
        m_MaxLevel=overWorld->GetLevel() + 1;
        overWorld->SetMaxLevelIndex(m_MaxLevel);
    }
}

void Game::ImportTextures()
{
    ResourceManager::LoadTex("idle", "assets/sprites/character/idle/idle.png");
    ResourceManager::LoadTex("run", "assets/sprites/character/run/run.png");
    ResourceManager::LoadTex("jump", "assets/sprites/character/jump/jump.png");
    ResourceManager::LoadTex("fall", "assets/sprites/character/fall/fall.png");
    ResourceManager::LoadTex("fall_dust", "assets/sprites/character/dust/fall_dust.png");
    ResourceManager::LoadTex("jump_dust", "assets/sprites/character/dust/jump_dust.png");
    ResourceManager::LoadTex("run_dust", "assets/sprites/character/dust/run_dust.png");
    ResourceManager::LoadTex("terrain", "assets/sprites/terrain/terrain_tiles.png");
    ResourceManager::LoadTex("grass", "assets/sprites/grass/grass.png");
    ResourceManager::LoadTex("crate", "assets/sprites/terrain/crate.png");
    ResourceManager::LoadTex("gold_coin", "assets/sprites/coins/gold_coin.png");
    ResourceManager::LoadTex("silver_coin", "assets/sprites/coins/silver_coin.png");
    ResourceManager::LoadTex("palm_small", "assets/sprites/palm/palm_small.png");
    ResourceManager::LoadTex("palm_large", "assets/sprites/palm/palm_large.png");
    ResourceManager::LoadTex("bg_palm", "assets/sprites/palm/bg_palm.png");
    ResourceManager::LoadTex("enemy_run", "assets/sprites/enemy/enemy_run.png");
    ResourceManager::LoadTex("sky_top", "assets/sprites/background/sky/sky_top.png");
    ResourceManager::LoadTex("sky_middle", "assets/sprites/background/sky/sky_middle.png");
    ResourceManager::LoadTex("sky_bottom", "assets/sprites/background/sky/sky_bottom.png");
    ResourceManager::LoadTex("water", "assets/sprites/background/water/water.png");
    ResourceManager::LoadTex("cloud1", "assets/sprites/background/clouds/1.png");
    ResourceManager::LoadTex("cloud2", "assets/sprites/background/clouds/2.png");
    ResourceManager::LoadTex("cloud3", "assets/sprites/background/clouds/3.png");
    ResourceManager::LoadTex("hat", "assets/sprites/character/hat.png");
    ResourceManager::LoadTex("level0", "assets/sprites/overworld/level0.png");
    ResourceManager::LoadTex("level1", "assets/sprites/overworld/level1.png");
    ResourceManager::LoadTex("level2", "assets/sprites/overworld/level2.png");
    ResourceManager::LoadTex("level3", "assets/sprites/overworld/level3.png");
    ResourceManager::LoadTex("level4", "assets/sprites/overworld/level4.png");
    ResourceManager::LoadTex("level5", "assets/sprites/overworld/level5.png");
    ResourceManager::LoadTex("big_palm", "assets/sprites/overworld/big_palm.png");
    ResourceManager::LoadTex("small_palm", "assets/sprites/overworld/small_palm.png");
    ResourceManager::LoadTex("overworld_cloud1", "assets/sprites/overworld/clouds/1.png");
    ResourceManager::LoadTex("overworld_cloud2", "assets/sprites/overworld/clouds/2.png");
    ResourceManager::LoadTex("overworld_cloud3", "assets/sprites/overworld/clouds/3.png");
    ResourceManager::LoadTex("health_bar", "assets/ui/health_bar.png");
    ResourceManager::LoadTex("coin", "assets/ui/coin.png");
    ResourceManager::LoadTex("death_particle","assets/sprites/enemy/death_particle.png");
}

void Game::SpawnTrees()
{
    m_OverWorldPalm.clear();
    int dist = GetRandomValue(30, 70);
    int size = 7;
    int attempts = 0;
    int maxAttempts = 1000;

    while ((int)m_OverWorldPalm.size() <= size && attempts < maxAttempts)
    {
        attempts++;
        bool valid = true;
        float posX = GetRandomValue(0, Config::SCREEN_WIDTH - 80.f);
        for (auto &tree : m_OverWorldPalm)
        {
            if (fabs(tree.m_Position.x - posX) <= dist)
            {
                valid = false;
                break;
            }
        }
        if (valid)
        {
            m_OverWorldPalm.emplace_back(posX);
        }
    }
}

void Game::GameOver()
{
    if(m_Level->GetPlayerHealth()<=0)
    {
        StopMusicStream(m_LevelMusic);
        m_State=GameState::OVERWORLD;
        m_Score=0;
        m_MaxLevel=0;
        overWorld->SetMaxLevelIndex(m_MaxLevel);
        overWorld->SetLevelIndex(m_MaxLevel);
        overWorld->ResetPlayerPosition();
        PlayMusicStream(m_OverWorldMusic);
        return;
    }
}


Game::~Game()
{

    ResourceManager::UnloadAll();
    UnloadMusicStream(m_OverWorldMusic);
    UnloadMusicStream(m_LevelMusic);
}
