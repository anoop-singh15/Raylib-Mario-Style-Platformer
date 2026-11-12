#include "include/level.hpp"
#include "include/global.hpp"
#include "include/collisionSystem.hpp"

#include "include/resourceManager.hpp"
#include"include/overworld.hpp"

#include <iostream>
#include <fstream>
#include <raylib.h>



Level::Level(int levelIndex)
{
    std::string path = "assets/map/level" + std::to_string(levelIndex) + ".json";
    LoadLevel(path);
    m_CoinSound=LoadSound("assets/audio/effects/coin.wav");
    m_StompSound=LoadSound("assets/audio/effects/stomp.wav");
    m_HitSound=LoadSound("assets/audio/effects/hit.wav");
   
}

void Level::Draw()
{

    

    for (auto &crate : m_Crates)
    {
        crate.Draw();
    }
    for (auto &coin : m_Coins)
    {
        coin.Draw();
    }
    for (auto &bgtrees : m_BgTrees)
    {
        bgtrees.Draw();
    }

    m_Player.Draw();

    for (auto &grass : m_Grasses)
    {
        grass.Draw();
    }
    for (auto &tile : m_Tiles)
    {
        tile.Draw();
    }
    for (auto &enemy : m_Enemies)
    {
        if(!enemy.GetDeathAnimationFinished())
        enemy.Draw();
    }

    for (auto &fgtrees : m_FgTrees)
    {
        fgtrees.Draw();
    }

    m_End.Draw();
}

void Level::LoadLevel(const std::string &path)
{

    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cout<<"Map Unloaded"<<std::endl;
        return;
    }

    json map;
    file >> map;

    /////////////////////////////////
    // Map info
    ////////////////////////////////

    int mapWidth = map["width"];
    int mapHeight = map["height"];

    int tileWidth = map["tilewidth"];
    int tileheight = map["tileheight"];

    ///////////////////////////////////
    // Terrain Layer and Player layer
    ///////////////////////////////////

    const json *terrainLayer = nullptr;
    const json *playerLayer = nullptr;
    const json *grassLayer = nullptr;
    const json *crateLayer = nullptr;
    const json *coinLayer = nullptr;
    const json *fgPalmLayer = nullptr;
    const json *bgPalmLayer = nullptr;
    const json *enemyLayer = nullptr;
    const json *constraintLayer = nullptr;

    for (const auto &layer : map["layers"])
    {
        if (layer["name"] == "terrain")
        {
            terrainLayer = &layer;
        }
        if (layer["name"] == "player")
        {
            playerLayer = &layer;
        }
        if (layer["name"] == "grass")
        {
            grassLayer = &layer;
        }
        if (layer["name"] == "crate")
        {
            crateLayer = &layer;
        }
        if (layer["name"] == "coins")
        {
            coinLayer = &layer;
        }
        if (layer["name"] == "fg_palm")
        {
            fgPalmLayer = &layer;
        }
        if (layer["name"] == "bg_palm")
        {
            bgPalmLayer = &layer;
        }
        if (layer["name"] == "enemies")
        {
            enemyLayer = &layer;
        }
        if (layer["name"] == "constraints")
        {
            constraintLayer = &layer;
        }
    }
    if (!terrainLayer || !playerLayer || !grassLayer || !crateLayer || !coinLayer || !fgPalmLayer || !bgPalmLayer || !enemyLayer || !constraintLayer)
    {
        return;
    }

    //////////////////////////////////
    // Terrain Data and Player Data
    //////////////////////////////////

    const auto &terrainData = (*terrainLayer)["data"];
    const auto &playerData = (*playerLayer)["data"];
    const auto &grassData = (*grassLayer)["data"];
    const auto &crateData = (*crateLayer)["data"];
    const auto &coinData = (*coinLayer)["data"];
    const auto &fgPalmData = (*fgPalmLayer)["data"];
    const auto &bgPalmData = (*bgPalmLayer)["data"];
    const auto &enemyData = (*enemyLayer)["data"];
    const auto &constraintData = (*constraintLayer)["data"];


    //////////////////////////////////
    // Getting First Gid
    //////////////////////////////////
    int terrainFirstGid=GetFirstGid(map,"terrain_tiles.tsx");
    int playerFirstGid=GetFirstGid(map,"setup_tiles.tsx");
    int grassFirstGid=GetFirstGid(map,"grass_tiles.tsx");
    int coinFirstGid=GetFirstGid(map,"coin_tiles.tsx");
    int fgPalmFirstGid=GetFirstGid(map,"palm_tileset.tsx");

    m_Tiles.clear();
    m_Grasses.clear();
    m_Crates.clear();
    m_Coins.clear();
    m_FgTrees.clear();
    m_BgTrees.clear();
    m_Enemies.clear();
    m_Constraints.clear();
    for (int row = 0; row < mapHeight; row++)
    {
        for (int col = 0; col < mapWidth; col++)
        {
            int index = row * mapWidth + col;
            int playerGid = playerData[index];
            float worldX = col * tileWidth;
            float worldY = row * tileheight;

            if (playerGid == playerFirstGid)
            {
                m_Player.SetPlayerPosition({worldX, worldY});
            }
            else if (playerGid == playerFirstGid+1)
            {
                m_End.SetPosition({worldX, worldY});
            }

            int gid = terrainData[index];
            if (gid != 0)
            {
                ////////////////////////
                // gid to local id
                ///////////////////////
                int localId = gid - terrainFirstGid;
                int tilesetColumn = 4;
                int srcRow = localId / tilesetColumn;
                int srcCol = localId % tilesetColumn;

                m_Tiles.emplace_back(Tile({worldX, worldY, (float)tileheight, srcRow, srcCol}));
            }
            int grassGid = grassData[index];
            if (grassGid != 0)
            {
                int localGid = grassGid - grassFirstGid;
                int tilesetColumn = 6;
                int srcRow = 0;
                int srcCol = localGid % tilesetColumn;
                m_Grasses.emplace_back(Grass({worldX, worldY, (float)tileheight, srcRow, srcCol}));
            }
            int crateGid = crateData[index];
            if (crateGid != 0)
            {
                int srcRow = 0;
                int srcCol = 0;
                m_Crates.emplace_back(Crate({worldX, worldY, (float)tileheight, srcRow, srcCol}));
            }

            int coinGid = coinData[index];
            if (coinGid != 0)
            {
                int localGid = coinGid - coinFirstGid;
                m_Coins.emplace_back(worldX, worldY, localGid);
            }
            int fgTreeGid = fgPalmData[index];
            if (fgTreeGid != 0)
            {
                int localGid = fgTreeGid - fgPalmFirstGid;
                m_FgTrees.emplace_back(Palm(worldX, worldY, localGid));
            }
            int bgTreeGid = bgPalmData[index];
            if (bgTreeGid != 0)
            {
                m_BgTrees.emplace_back(BgPalm(worldX, worldY));
            }
            int enemyGid = enemyData[index];
            if (enemyGid != 0)
            {
                m_Enemies.emplace_back(Enemy(worldX, worldY));
            }
            int constraintGid = constraintData[index];
            if (constraintGid != 0)
            {
                m_Constraints.emplace_back(Constraints(worldX, worldY));
            }
        }
    }
}

void Level::Update(float dT,int &score)
{
    
    m_Player.Update(dT);

    /////////////////////////////////////////
    // x movement and collision
    /////////////////////////////////////////
    m_Player.m_Player.x += m_Player.m_Direction.x * PlayerConfig::PLAYER_SPEED * dT;

    Collision::PlayerTileHorizontalCollision(m_Player, m_Tiles);
    Collision::PlayerTreeHorizontalCollision(m_Player, m_FgTrees);
    Collision::PlayerCrateHorizontalCollision(m_Player, m_Crates);

    /////////////////////////////////////////
    // y movement and Collision
    /////////////////////////////////////////
    m_Player.m_IsGrounded = false;
    m_Player.m_Player.y += m_Player.m_Direction.y * dT;
    Collision::PlayerTileVerticalCollision(m_Player, m_Tiles);
    Collision::PlayerTreeVerticalCollision(m_Player, m_FgTrees);
    Collision::PlayerCrateVerticalCollision(m_Player, m_Crates);

    m_Player.CheckLandingDust();

   
    for (auto it=m_Coins.begin();it!=m_Coins.end();)
    {
        it->Update(dT);
        if(Collision::PlayerCoinCollison(m_Player,*it))
        {
            score+=it->m_Value;
            it=m_Coins.erase(it);
            PlaySound(m_CoinSound);
            break;
        }
        else{
            ++it;
        }
    }
    for (auto &fgtrees : m_FgTrees)
    {
        fgtrees.Update(dT);
    }
    for (auto &bgtrees : m_BgTrees)
    {
        bgtrees.Update(dT);
    }
    for (auto &enemy : m_Enemies)
    {
        enemy.Update(dT);
        enemy.m_Enemy.x += (enemy.m_Speed * dT * enemy.m_Direction);
        Collision::EnemyConstraintCollison(enemy, m_Constraints);
    }
    Collision::PlayerEnemyCollison(m_Player,m_Enemies,m_StompSound,m_HitSound);
}

int Level::GetFirstGid(const json &map, const std::string &name)
{
    for (const auto &tileset : map["tilesets"])
    {
        if (tileset.contains("source") && tileset["source"].get<std::string>().find(name) != std::string::npos)
        {
            return tileset["firstgid"].get<int>();
        }
    }
    return -1;
}

bool Level::GetResult()
{
    if (CheckCollisionRecs(m_Player.m_Player, m_End.GetEndPosition()))
    {
        return true;
    }
    return false;
}

Level::~Level()
{
    UnloadSound(m_HitSound);
    UnloadSound(m_StompSound);
    UnloadSound(m_CoinSound);
}
