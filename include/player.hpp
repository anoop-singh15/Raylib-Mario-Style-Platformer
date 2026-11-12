#pragma once
#include<raylib.h>
#include<string>

class Player
{
    private:
    float m_Width;
    float m_Height;
    float m_FrameTime;
    float m_MaxFrameTime;
    int m_CurrFrame;
    int m_MaxFrames;
    int  m_FacingDir;
     int m_Health=100;
    std::string m_Status;
    
    // Dust Particles
    // bool m_DustActive;
    bool m_PrevGrounded;
    int m_CurrDustFrame;
    int m_MaxDustFrames;
    float m_DustFrameTime;
    float m_MaxDustFrameTime;
    std::string m_DustStatus;

    //Invincibility
   
    float m_InvincibilityTimer=0.f;
    float m_InvincibilityFlashTimer=0.f;
    float m_InvincibilityDuration=1.5f;
   
    
    Vector2 m_DustPosition;
    
    void HandleInput(float dT);
   
    void Gravity(float dT);
    void UpdateAnimations(float dT);
    void UpdateDustAnimations(float dT);
    void SpawnDust(Vector2 position);
    void SpawnLandDust(Vector2 position);
    void GetStatus();
    void UpdateDustStatus();
    void UpdateInvincibility(float dT);
    
    
    
   
    public:
    Sound m_JumpSound;
    Rectangle m_Player;
    Vector2 m_Direction;
    bool m_IsInvincible=false;
    bool m_IsGrounded;
    int m_JumpCount;
    Player();
    void Draw();
    void SetPlayerPosition(Vector2 pos);
    void Update(float dT);
    inline Vector2 GetPlayerCenter()const{
        return {m_Player.x+m_Width/2.f,m_Player.y+m_Height/2.f};
    }
    void Jump();
    Rectangle PlayerTextureHitBox();
    Vector2 DustTextureHitBox();
    void CheckLandingDust();
    void StartInvincibility();
    inline int GetHealth()const{return m_Health;}
    void SetHealth(){m_Health-=10;}
    void GameOver();
    ~Player();
};


struct LevelEnd{
    Vector2 m_EndPos;
    void Draw();
    void SetPosition(Vector2);
    Rectangle GetEndPosition()const{
        return {m_EndPos.x,m_EndPos.y,32.f,32.f};
    }
};
