#include "include/background.hpp"
#include "include/global.hpp"

BackGround::BackGround(Texture2D &tex, float offsetY)
{
    m_Tex = &tex;
    m_HeightOffset = offsetY;
}

void BackGround::Draw()
{
    if (m_Tex != nullptr)
    {
        Rectangle source = {0.f, 0.f, (float)m_Tex->width, (float)m_Tex->height};
        Rectangle dest = {0.f, m_HeightOffset, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT/3.f};
        DrawTexturePro(*m_Tex, source, dest, {0.f, 0.f}, 0.f, WHITE);
    }
}

Water::Water(Texture2D &tex, float offsetY)
    : BackGround(tex, offsetY)
{
    m_CurrFrame = 0;
    m_MaxFrames = 4;
    m_FrameTime = 0.f;
    m_UpdatedFrameTime = 1.f / 10.f;
}

void Water::Draw()
{
    if (m_Tex != nullptr)
    {
        float frameWidth = (float)m_Tex->width / m_MaxFrames;
        Rectangle source = {frameWidth * m_CurrFrame, 0.f, (float)frameWidth, (float)m_Tex->height};

        for (float x = 0.f; x < Config::SCREEN_WIDTH; x += frameWidth)
        {
            Rectangle dest = {x, m_HeightOffset, frameWidth, (float)m_Tex->height};
            DrawTexturePro(*m_Tex, source, dest, {0.f, 0.f}, 0.f, WHITE);
        }
    }
}

void Water::Update(float dT)
{
    m_FrameTime += dT;
    if (m_FrameTime >= m_UpdatedFrameTime)
    {
        m_FrameTime = 0.f;
        m_CurrFrame++;
        if (m_CurrFrame >= m_MaxFrames)
        {
            m_CurrFrame = 0;
        }
    }
}

Clouds::Clouds()
{

    int randomTex = GetRandomValue(1, 3);
    if (randomTex == 1)
    {
        m_CloudTex = &ResourceManager::GetTexture("cloud1");
    }
    else if (randomTex == 2)
    {
        m_CloudTex = &ResourceManager::GetTexture("cloud2");
    }
    else
    {
        m_CloudTex = &ResourceManager::GetTexture("cloud3");
    }
    m_Position.x = (float)GetRandomValue(0, Config::SCREEN_WIDTH);
    m_Position.y = (float)GetRandomValue(0, Config::SCREEN_HEIGHT / 3);
    m_Speed=float(GetRandomValue(50,100));

}

void Clouds::Draw()
{

    Rectangle source = {0.f, 0.f, (float)m_CloudTex->width, float(m_CloudTex->height)};
    Rectangle dest = {m_Position.x, m_Position.y, (float)m_CloudTex->width, float(m_CloudTex->height)};

    DrawTexturePro(*m_CloudTex, source, dest, {0.f, 0.f}, 0.f, WHITE);
}

void Clouds::Update(float dT)
{
    m_Position.x += (m_Speed * dT);
}

void Clouds::Spawn(float cameraLeft,float viewWidth)
{
    m_Position.x =cameraLeft+(float)GetRandomValue(0, int(viewWidth));
    m_Position.y = (float)GetRandomValue(0, Config::SCREEN_HEIGHT / 3);
}
bool Clouds::IsOffScreen(float cameraLeft,float viewWidth)
{
    return m_Position.x + m_CloudTex->width < cameraLeft ||
           m_Position.x > cameraLeft + viewWidth;
}

