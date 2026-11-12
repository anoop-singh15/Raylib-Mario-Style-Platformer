#include "include/enemy.hpp"
#include "include/resourceManager.hpp"

Enemy::Enemy(float x, float y)
{
    m_CurrFrame = 0;
    m_MaxFrames = 6;
    m_FrameTime = 0.f;
    m_UpdatedFrameTime = 1.f / 9.f;
    m_Speed = (float)GetRandomValue(100, 150);
    m_Enemy = {x, y, 51.f, 46.f};
    m_Direction = 1;
    m_IsAlive = true;
    m_IsDeathAnimationFinished=false;
}

void Enemy::Draw()
{
    if (!m_IsAlive && !m_IsDeathAnimationFinished)
    {
        Texture2D &deathParticle = ResourceManager::GetTexture("death_particle");

        float frameWidth = (float)deathParticle.width / m_MaxFrames;
      
        Rectangle source = {frameWidth * m_CurrFrame, 0.f, frameWidth , (float)deathParticle.height};
        Rectangle dest =m_DeathPos;
        dest.width=frameWidth;
        dest.height=(float)deathParticle.height;

        DrawTexturePro(deathParticle, source, dest, {0.f, 0.f}, 0.f, WHITE);

        return;
    }

    Texture2D &enemyTex = ResourceManager::GetTexture("enemy_run");

    float frameWidth = (float)enemyTex.width / m_MaxFrames;
    int facingDir = -1 * m_Direction;
    Rectangle source = {frameWidth * m_CurrFrame, 0.f, frameWidth * facingDir, (float)enemyTex.height};
    Rectangle dest = GetEnemyCollider();

    DrawTexturePro(enemyTex, source, dest, {0.f, 0.f}, 0.f, WHITE);
}

void Enemy::Update(float dT)
{
    UpdateAnimations(dT);
}

void Enemy::ChangeState()
{
    if (m_IsAlive)
    {
        m_IsAlive = false;
        m_CurrFrame = 0;
        m_MaxFrames = 7;
        m_FrameTime = 0.f;
        m_UpdatedFrameTime = 1.f / 50.f;
        float offsetX=-115.f;
        float offsetY=-105.f;
        m_DeathPos={m_Enemy.x+offsetX,m_Enemy.y+offsetY,64.f,64.f};
    }
}

void Enemy::UpdateAnimations(float dT)
{
    m_FrameTime += dT;
    if (m_FrameTime >= m_UpdatedFrameTime)
    {

        m_FrameTime = 0.f;
        m_CurrFrame++;
        if (m_CurrFrame >= m_MaxFrames)
        {
            if (!m_IsAlive)
            {
                m_IsDeathAnimationFinished=true;
                return;
            }
            m_CurrFrame = 0;
        }
    }
}

Rectangle Enemy::GetEnemyCollider()
{
    return {m_Enemy.x, m_Enemy.y + 18.f, m_Enemy.width, m_Enemy.height};
}

Constraints::Constraints(float x, float y)
    : m_Collider({x, y})
{
}

Rectangle Constraints::GetCollider()
{
    return {m_Collider.x, m_Collider.y, 64.f, 64.f};
}
