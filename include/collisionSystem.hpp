#pragma once
#include <raylib.h>
#include "player.hpp"
#include "tile.hpp"
#include <vector>
#include"enemy.hpp"

class Collision
{
public:
    ///////////////////////////////////////
    // Player and Terrain Collision
    //////////////////////////////////////
    static void PlayerTileHorizontalCollision(Player &player, std::vector<Tile> &tiles);
    static void PlayerTileVerticalCollision(Player &player, std::vector<Tile> &tiles);
    static void PlayerTreeHorizontalCollision(Player &player, std::vector<Palm> &tiles);
    static void PlayerTreeVerticalCollision(Player &player, std::vector<Palm> &tiles);
    static void PlayerCrateHorizontalCollision(Player &player, std::vector<Crate> &tiles);
    static void PlayerCrateVerticalCollision(Player &player, std::vector<Crate> &tiles);
    static void EnemyConstraintCollison(Enemy&enemy,std::vector<Constraints>&tiles);
    static bool PlayerCoinCollison(Player&player,Coins&coin);
    static void PlayerEnemyCollison(Player&player,std::vector<Enemy>&enemies,Sound&stompSound,Sound&hitSound);
};
