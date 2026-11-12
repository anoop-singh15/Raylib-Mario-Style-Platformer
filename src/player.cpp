#include "include/player.hpp"
#include "include/global.hpp"
#include "include/resourceManager.hpp"
#include <cmath>

Player::Player()
{
    m_Width = PlayerConfig::WIDTH;
    m_Height = PlayerConfig::HEIGHT;
    m_Direction = {0.f, 0.f};
    m_IsGrounded = true;
    m_FrameTime = 0.f;
    m_MaxFrameTime = 1.f / 10.f;
    m_CurrFrame = 0;
    m_MaxFrames = 5;
    m_Status = "idle";
    m_FacingDir = 1;

    m_DustFrameTime = 0.0f;
    m_MaxDustFrames = 0;
    m_MaxDustFrameTime = 0.f;
    m_CurrDustFrame = 0;
    m_DustStatus = "null_dust";
    m_PrevGrounded = true;
    m_JumpCount = 0;
    m_JumpSound = LoadSound("assets/audio/effects/jump.wav");
    SetSoundVolume(m_JumpSound, 0.5f);
}

void Player::Draw()
{

    // DrawRectangleRec(m_Player,RED);
    Texture2D &playerTex = ResourceManager::GetTexture(m_Status);
    if (playerTex.id == 0)
        return;

    float frameWidth = (float)playerTex.width / m_MaxFrames;
    Rectangle source = {frameWidth * m_CurrFrame, 0.f, frameWidth * m_FacingDir, (float)playerTex.height};
    Rectangle dest = PlayerTextureHitBox();
    Vector2 origin = {0.f, 0.f};

    unsigned char alpha = 255;
    if (m_IsInvincible)
    {
        alpha = (sinf((m_InvincibilityFlashTimer * 30.f) + 1.f) > 0.f) ? 255 : 0;
    }
    DrawTexturePro(playerTex, source, dest, origin, 0.f, {255, 255, 255, alpha});

    /////////////////////////
    // Drawing Dust Particles
    /////////////////////////
    if (m_DustStatus == "null_dust")
    {
        return;
    }
    Texture2D &dustTex = ResourceManager::GetTexture(m_DustStatus);
    if (dustTex.id == 0)
        return;
    float dustFrameWidth = (float)dustTex.width / m_MaxDustFrames;
    Rectangle dustSource = {dustFrameWidth * m_CurrDustFrame, 0.f, dustFrameWidth * m_FacingDir, (float)dustTex.height};
    Rectangle dustDest = {DustTextureHitBox().x, DustTextureHitBox().y, dustFrameWidth, (float)dustTex.height};
    if (m_DustStatus == "jump_dust" || m_DustStatus == "fall_dust")
    {
        dustDest.x = m_DustPosition.x;
        dustDest.y = m_DustPosition.y;
    }

    DrawTexturePro(dustTex, dustSource, dustDest, origin, 0.f, WHITE);
}

void Player::Update(float dT)
{
    if (m_Health > 0)
    {
        GameOver();
        m_PrevGrounded = m_IsGrounded;
        HandleInput(dT);
        Gravity(dT);
        GetStatus();
        UpdateAnimations(dT);
        UpdateDustStatus();
        UpdateDustAnimations(dT);
        UpdateInvincibility(dT);
    }
}

void Player::SetPlayerPosition(Vector2 pos)
{
    m_Player = {pos.x, pos.y, m_Width, m_Height};
}

void Player::HandleInput(float dT)
{
    if (IsKeyDown(KEY_RIGHT))
    {
        m_Direction.x = 1.f;
        m_FacingDir = 1;
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        m_Direction.x = -1.f;
        m_FacingDir = -1;
    }
    else
    {
        m_Direction.x = 0;
    }
    if (IsKeyPressed(KEY_SPACE) && (m_IsGrounded == true || m_JumpCount < 2))
    {
        SpawnDust({m_Player.x, m_Player.y});
        Jump();
        m_JumpCount++;
        m_IsGrounded = false;
    }
}

void Player::Gravity(float dT)
{
    m_Direction.y += (PlayerConfig::GRAVITY * dT);
    if (m_Direction.y > PlayerConfig::MAX_GRAVITY)
    {
        m_Direction.y = PlayerConfig::MAX_GRAVITY;
    }
}

void Player::Jump()
{
    m_Direction.y = PlayerConfig::JUMP_SPEED;
    PlaySound(m_JumpSound);
}

void Player::UpdateAnimations(float dT)
{
    m_FrameTime += dT;
    if (m_FrameTime >= m_MaxFrameTime)
    {
        m_FrameTime = 0.f;
        m_CurrFrame++;
        if (m_CurrFrame >= m_MaxFrames)
        {
            m_CurrFrame = 0.f;
        }
    }
}

void Player::UpdateDustAnimations(float dT)
{
    if (m_DustStatus == "null_dust")
        return;
    m_DustFrameTime += dT;
    if (m_DustFrameTime >= m_MaxDustFrameTime)
    {
        m_DustFrameTime = 0.f;
        m_CurrDustFrame++;
        if (m_CurrDustFrame >= m_MaxDustFrames)
        {
            if (m_DustStatus == "jump_dust" || m_DustStatus == "fall_dust")
            {
                m_DustStatus = "null_dust";
                m_CurrDustFrame = 0;
                m_DustFrameTime = 0.f;
                return;
            }
            m_CurrDustFrame = 0.f;
        }
    }
}

void Player::GetStatus()
{
    std::string prevStatus = m_Status;

    if (m_Direction.y < 0.f)
    {
        m_Status = "jump";
        m_MaxFrames = 3.f;
        m_MaxFrameTime = 1.f / 5.f;
    }
    else if (!m_IsGrounded)
    {
        m_Status = "fall";
        m_MaxFrames = 1.f;
        m_MaxFrameTime = 1.f;
    }
    else if (m_IsGrounded)
    {
        if (m_Direction.x != 0)
        {
            m_Status = "run";
            m_MaxFrames = 6.f;
            m_MaxFrameTime = 1.f / 10.f;
        }
        else
        {
            m_Status = "idle";
            m_MaxFrames = 5.f;
            m_MaxFrameTime = 1.f / 10.f;
        }
    }
    if (prevStatus != m_Status)
    {
        m_CurrFrame = 0;
        m_FrameTime = 0.f;
    }
}

void Player::UpdateDustStatus()
{
    if (m_DustStatus == "jump_dust" || m_DustStatus == "fall_dust")
        return;

    if (m_IsGrounded && m_Direction.x != 0)
    {
        if (m_DustStatus == "null_dust")
        {
            m_DustStatus = "run_dust";
            m_MaxDustFrames = 6;
            m_MaxDustFrameTime = 1.f / 12.f;
            m_CurrDustFrame = 0;
            m_DustFrameTime = 0.f;
        }
    }
    else if (m_DustStatus == "run_dust")
    {
        m_DustStatus = "null_dust";
        m_CurrDustFrame = 0;
        m_DustFrameTime = 0.f;
    }
}

Rectangle Player::PlayerTextureHitBox()
{

    float width = 64.f * 3;
    float height = 64 * 3;

    float offsetX;
    if (m_FacingDir < 0)
    {
        offsetX = -100.f;
    }
    else
    {
        offsetX = -60.f;
    }
    float offsetY = -64.f;

    return Rectangle{
        m_Player.x + offsetX,
        m_Player.y + offsetY,
        width,
        height};
}

Vector2 Player::DustTextureHitBox()
{

    float offsetX = -22.f;
    float offsetY = 43.f;

    if (m_FacingDir == -1)
    {
        offsetX = 3.f;
    }

    return Vector2{
        m_Player.x + offsetX,
        m_Player.y + offsetY,
    };
}

void Player::SpawnDust(Vector2 pos)
{
    float offsetY = m_Height - 19.f;
    float offsetX = -10.f;
    m_DustPosition = pos;
    m_DustPosition.y += offsetY;
    m_DustPosition.x += offsetX;

    m_DustStatus = "jump_dust";

    m_MaxDustFrames = 7;
    m_MaxDustFrameTime = 1.f / 12.f;

    m_CurrDustFrame = 0;
    m_DustFrameTime = 0.f;
}

void Player::SpawnLandDust(Vector2 pos)
{
    float offsetY = m_Height - 19.f;
    float offsetX = -10.f;
    m_DustPosition = pos;
    m_DustPosition.y += offsetY;
    m_DustPosition.x += offsetX;

    m_DustStatus = "fall_dust";

    m_MaxDustFrames = 6;
    m_MaxDustFrameTime = 1.f / 12.f;

    m_CurrDustFrame = 0;
    m_DustFrameTime = 0.f;
}

void Player::CheckLandingDust()
{
    if (!m_PrevGrounded && m_IsGrounded)
    {
        SpawnLandDust({m_Player.x, m_Player.y});
    }
}

void Player::UpdateInvincibility(float dT)
{
    if (!m_IsInvincible)
        return;

    m_InvincibilityTimer -= dT;
    m_InvincibilityFlashTimer += dT;
    if (m_InvincibilityTimer <= 0.f)
    {
        m_IsInvincible = false;
        m_InvincibilityTimer = 0.f;
        m_InvincibilityFlashTimer = 0.f;
    }
}

void Player::StartInvincibility()
{
    if (m_IsInvincible)
        return;
    m_IsInvincible = true;
    m_InvincibilityTimer = m_InvincibilityDuration;
    m_InvincibilityFlashTimer = 0.f;
}

void Player::GameOver()
{
    if (m_Player.y >= Config::SCREEN_HEIGHT)
    {
        m_Health = 0;
    }
}

Player::~Player()
{
    UnloadSound(m_JumpSound);
}

void LevelEnd::SetPosition(Vector2 pos)
{
    m_EndPos = pos;
}

void LevelEnd::Draw()
{
    Texture2D &endTex = ResourceManager::GetTexture("hat");
    Rectangle source = {0.f, 0.f, (float)endTex.width, (float)endTex.height};
    Rectangle dest = {m_EndPos.x + 10.f, m_EndPos.y + 10.f, (float)endTex.width, (float)endTex.height};
    DrawTexturePro(endTex, source, dest, {0.f, 0.f}, 0.f, WHITE);
}
