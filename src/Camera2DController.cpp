#include "include/Camera2DController.hpp"
#include"include/global.hpp"
#include <cmath>
#include<cstdlib>



Camera2DController::Camera2DController()
{
    m_BaseOffset={Config::SCREEN_WIDTH / 2.f, Config::SCREEN_HEIGHT / 2.f};
    
    m_Camera.offset = m_BaseOffset;
    m_Camera.target={0.f,Config::SCREEN_HEIGHT / 2.f};
    m_Camera.rotation = 0.0f;
    m_Camera.zoom = 1.f;
}

void Camera2DController::Update(float dT,Vector2 playerPos)
{
    Zoom();
    CameraMovement(dT,playerPos);
    ScreenShake();
   
}

void Camera2DController::Zoom()
{
    m_Camera.zoom = expf(logf(m_Camera.zoom) + ((float)GetMouseWheelMove() * 0.1f));
}

void Camera2DController::CameraMovement(float dT,Vector2 playerPos)
{
    // if (IsKeyDown(KEY_W))
    // {
    //     m_Camera.target.y -= m_CameraSpeed * dT;
    // }
    // if (IsKeyDown(KEY_S))
    // {
    //     m_Camera.target.y += m_CameraSpeed *dT;
    // }
    // if (IsKeyDown(KEY_A))
    // {
    //     m_Camera.target.x -= m_CameraSpeed * dT;
    // }
    // if (IsKeyDown(KEY_D))
    // {
    //     m_Camera.target.x +=m_CameraSpeed* dT;
    // }

   
       m_Camera.target.x+=(playerPos.x-m_Camera.target.x)*m_CameraSpeed*dT;  
    
    
   
}

void Camera2DController::BeginMode() 
{
    BeginMode2D(m_Camera);
}
void Camera2DController::EndMode() 
{
    EndMode2D();
}

void Camera2DController::Shake(float shakeIntensity,float shakeDecay)
{
    m_ShakeIntensity=shakeIntensity;
    m_ShakeDecay=shakeDecay;
}

void Camera2DController::ScreenShake()
{
     if(m_ShakeIntensity>0.1f)
    {
        float randomX=((float)rand()/RAND_MAX)*2.f-1.0f;
        float randomY=((float)rand()/RAND_MAX)*2.f-1.0f;

        m_Camera.offset.x=m_BaseOffset.x+(randomX * m_ShakeIntensity);
        m_Camera.offset.y=m_BaseOffset.y+(randomY * m_ShakeIntensity);
        m_ShakeIntensity*=m_ShakeDecay;
    }
    else{
        m_ShakeIntensity=0.0f;
        m_Camera.offset=m_BaseOffset;
    }
}

Camera2DController::~Camera2DController()
{
}