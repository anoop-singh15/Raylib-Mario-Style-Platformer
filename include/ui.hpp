#pragma once
#include<raylib.h>


class UI{
    private:
    int m_CurrHealth;
    int m_MaxHealth;
    int m_Coins;
    int m_MaxHealthWidth;
    float m_CurrHealthWidth;
    Vector2 m_HealthBarPosition;
    Vector2 m_HealthPosition;
    Vector2 m_CoinsPosition;
    Vector2 m_CoinTextPosition;
    Texture2D*m_HealthTex=nullptr;
    Texture2D*m_CoinTex=nullptr;
    Font m_CoinFont;
    public:
    UI();
    void DrawHealth();
    void DrawCoins();
    void DrawCoinsText();
    void UpdateHealthBar(int playerHealth);
    void SetCoins(int amount){m_Coins=amount;}
    ~UI();
};
