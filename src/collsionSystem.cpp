#include "include/collisionSystem.hpp"
#include "include/global.hpp"

void Collision::PlayerTileHorizontalCollision(Player &player, std::vector<Tile> &tiles)
{
    /////////////////////////////////////////
    // x  collision
    /////////////////////////////////////////

    for (auto &tile : tiles)
    {
        if (CheckCollisionRecs(player.m_Player, tile.m_Tile))
        {
            if (player.m_Direction.x < 0.f)
            {
                player.m_Player.x = tile.m_Tile.x + tile.m_Tile.width;
            }
            else if (player.m_Direction.x > 0.f)
            {
                player.m_Player.x = tile.m_Tile.x - player.m_Player.width;
            }
        }
    }
}

void Collision::PlayerTileVerticalCollision(Player &player, std::vector<Tile> &tiles)
{
    /////////////////////////////////////////
    // y collision
    /////////////////////////////////////////

    for (auto &tile : tiles)
    {

        if (CheckCollisionRecs(player.m_Player, tile.m_Tile))
        {

            if (player.m_Direction.y < 0.f)
            {
                player.m_Player.y = tile.m_Tile.y + tile.m_Tile.height;
                player.m_Direction.y = 0.f;
            }
            else if (player.m_Direction.y > 0.f)
            {
                player.m_Player.y = tile.m_Tile.y - player.m_Player.height;
                player.m_Direction.y = 0;
                player.m_IsGrounded = true;
                player.m_JumpCount = 0;
            }
        }
    }
}

void Collision::PlayerTreeHorizontalCollision(Player &player, std::vector<Palm> &tiles)
{
    /////////////////////////////////////////
    // x  collision
    /////////////////////////////////////////

    for (auto &tile : tiles)
    {
        Rectangle collider = tile.GetCollider();
        if (CheckCollisionRecs(player.m_Player, collider))
        {
            if (player.m_Direction.x < 0.f)
            {
                player.m_Player.x = collider.x + collider.width;
            }
            else if (player.m_Direction.x > 0.f)
            {
                player.m_Player.x = collider.x - player.m_Player.width;
            }
        }
    }
}

void Collision::PlayerTreeVerticalCollision(Player &player, std::vector<Palm> &tiles)
{
    /////////////////////////////////////////
    // y collision
    /////////////////////////////////////////

    for (auto &tile : tiles)
    {
        Rectangle collider = tile.GetCollider();
        if (CheckCollisionRecs(player.m_Player, collider))
        {

            if (player.m_Direction.y < 0.f)
            {
                player.m_Player.y = collider.y + collider.height;
                player.m_Direction.y = 0.f;
            }
            else if (player.m_Direction.y > 0.f)
            {
                player.m_Player.y = collider.y - player.m_Player.height;
                player.m_Direction.y = 0;
                player.m_IsGrounded = true;
                player.m_JumpCount = 0;
            }
        }
    }
}

void Collision::PlayerCrateHorizontalCollision(Player &player, std::vector<Crate> &tiles)
{
    /////////////////////////////////////////
    // x  collision
    /////////////////////////////////////////

    for (auto &tile : tiles)
    {
        Rectangle collider = tile.GetCollider();
        if (CheckCollisionRecs(player.m_Player, collider))
        {
            if (player.m_Direction.x < 0.f)
            {
                player.m_Player.x = collider.x + collider.width;
            }
            else if (player.m_Direction.x > 0.f)
            {
                player.m_Player.x = collider.x - player.m_Player.width;
            }
        }
    }
}

void Collision::PlayerCrateVerticalCollision(Player &player, std::vector<Crate> &tiles)
{
    /////////////////////////////////////////
    // y collision
    /////////////////////////////////////////

    for (auto &tile : tiles)
    {
        Rectangle collider = tile.GetCollider();
        if (CheckCollisionRecs(player.m_Player, collider))
        {

            if (player.m_Direction.y < 0.f)
            {
                player.m_Player.y = collider.y + collider.height;
                player.m_Direction.y = 0.f;
            }
            else if (player.m_Direction.y > 0.f)
            {
                player.m_Player.y = collider.y - player.m_Player.height;
                player.m_Direction.y = 0;
                player.m_IsGrounded = true;
                player.m_JumpCount = 0;
            }
        }
    }
}

void Collision::EnemyConstraintCollison(Enemy &enemy, std::vector<Constraints> &tiles)
{

    Rectangle enemyCollider = enemy.m_Enemy;
    for (auto &tile : tiles)
    {
        Rectangle constraintCollider = tile.GetCollider();
        if (CheckCollisionRecs(enemyCollider, constraintCollider))
        {
            if (enemy.m_Direction == 1)
            {
                enemy.m_Direction = -1;
            }
            else if (enemy.m_Direction == -1)
            {
                enemy.m_Direction = 1;
            }
        }
    }
}

bool Collision::PlayerCoinCollison(Player &player, Coins &coin)
{
    if (CheckCollisionRecs(player.m_Player, coin.GetCoinCollider()))
    {
        return true;
    }
    return false;
}

void Collision::PlayerEnemyCollison(Player &player, std::vector<Enemy> &enemies,Sound&stompSound,Sound&hitSound)
{
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        
        if(it->GetDeathAnimationFinished())
        {
            it=enemies.erase(it);
            continue;
        }
        
        Rectangle &playerCollider = player.m_Player;
        Rectangle enemyCollider = it->GetEnemyCollider();

        if (player.m_IsInvincible)
            break;
        if (CheckCollisionRecs(playerCollider, enemyCollider))
        {

            if (player.m_Direction.y > 0.f && playerCollider.y + playerCollider.height <= enemyCollider.y + 10.f)
            {
                playerCollider.y = enemyCollider.y - playerCollider.height;
                player.Jump();
                it->ChangeState();
                PlaySound(stompSound);
               ++it;
            }
            else
            {
                player.SetHealth();
                player.StartInvincibility();
                ++it;
                PlaySound(hitSound);
            }
        }
        else
        {
            ++it;
        }
    }
}
