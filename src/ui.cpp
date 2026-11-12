#include "include/ui.hpp"
#include "include/resourceManager.hpp"

UI::UI()
    : m_CurrHealth(100), m_MaxHealth(100), m_Coins(0), m_MaxHealthWidth(152),m_CurrHealthWidth(152.f)
{
    m_HealthBarPosition = {30.f, 20.f};
    m_HealthPosition = {64.f, 49.f};
    m_CoinsPosition = {50.f, 80.f};
    m_CoinTextPosition = {86.f, 83.f};
    m_HealthTex = &ResourceManager::GetTexture("health_bar");
    m_CoinTex = &ResourceManager::GetTexture("coin");
    m_CoinFont = LoadFontEx("assets/ui/ARCADEPI.TTF", 32, 0, 0);
}

void UI::DrawHealth()
{
    DrawTextureV(*m_HealthTex, m_HealthBarPosition, WHITE);

    DrawRectangleV(m_HealthPosition, {m_CurrHealthWidth, 4}, {220, 73, 73, 255});
}

void UI::UpdateHealthBar(int playerHealth)
{
    m_CurrHealth=playerHealth;
    float healthRatio = m_CurrHealth / (float)m_MaxHealth;
    m_CurrHealthWidth = (float)m_MaxHealthWidth * healthRatio;
}

void UI::DrawCoins()
{
    DrawTextureV(*m_CoinTex, m_CoinsPosition, WHITE);
}

void UI::DrawCoinsText()
{
    std::string coinText=std::to_string(m_Coins);
    float fontSize=(float)m_CoinFont.baseSize;
    Vector2 textSize=MeasureTextEx(m_CoinFont,coinText.c_str(),fontSize,2);
    float coinHeight=float(m_CoinTex->height);
    m_CoinTextPosition={
        m_CoinsPosition.x + (float)m_CoinTex->width + 8.f,
        m_CoinsPosition.y + (coinHeight-textSize.y)/2.f + 2.f
     };

    DrawTextEx(m_CoinFont,coinText.c_str(), m_CoinTextPosition, fontSize, 2, {51, 50, 61, 255});
}

UI::~UI()
{
    UnloadFont(m_CoinFont);
}
