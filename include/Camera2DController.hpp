#pragma once
#include<raylib.h>


class Camera2DController
{
    private:
    const float m_CameraSpeed=5.f;
    float m_ShakeIntensity;
    float m_ShakeDecay;
     Vector2 m_BaseOffset;
    


     void Zoom();
     void CameraMovement(float dT,Vector2 playerPos);
     void ScreenShake();
     
     
     
     public:
     Camera2D m_Camera={0};
    Camera2DController();
    void Update(float dT,Vector2 playerPos);
    void BeginMode();
    void EndMode();
    void Shake(float shakeIntensity,float shakeDecay);

    ~Camera2DController();
};