#include "include/overworld.hpp"
#include"include/resourceManager.hpp"

#include<raymath.h>

OverWorld::OverWorld(int levelIndex,int maxLevel)
          :m_LevelIndex(levelIndex),m_MaxLevel(maxLevel)
{
    m_Stages={
        {{110.f,400.f},"level0",0,1},
        {{300.f,220.f},"level1",1,2},
        {{480.f,600.f},"level2",2,3},
        {{610.f,350.f},"level3",3,4},
        {{880.f,210.f},"level4",4,5},
        {{1050.f,400.f},"level5",5,-1},
    };
    m_PlayerTex=&ResourceManager::GetTexture("hat");
    m_BgTex=&ResourceManager::GetTexture("level0");
    m_CurrFrame=0;
    m_MaxFrames=4;
    m_FrameTime=0.f;
    m_MaxFrameTime=1.f/6.f;
    m_FrameWidth=(float)m_BgTex->width/m_MaxFrames;
    m_Direction={0.f,0.f};
    m_PlayerPos=GetPlayerCenter(m_LevelIndex);
    m_IsMoving=false;
    m_EnterLevel=false;
    m_TargetPos=m_PlayerPos;
  

}

void OverWorld::Draw()
{
    for(int i=0;i<(int)m_Stages.size();i++)
    {
        m_BgTex=&ResourceManager::GetTexture("level"+std::to_string(i));
        float frameWidth=(float)m_BgTex->width/m_MaxFrames;

        int frame=(i<=m_MaxLevel)?m_CurrFrame:0;

        Rectangle source={frameWidth*frame,0.f,frameWidth,(float)m_BgTex->height};
        Rectangle dest={m_Stages[i].position.x-frameWidth/3.f,m_Stages[i].position.y-(float)m_BgTex->height/1.5f,(float)frameWidth,(float)m_BgTex->height};

        Color tint=(i<=m_MaxLevel)?WHITE:BLACK;

        
        DrawTexturePro(*m_BgTex,source,dest,{0.f,0.f},0.f,tint);
       
        
    }
}

void OverWorld::DrawLines()
{
     for(int i=0;i<(int)m_Stages.size()-1;i++)
    {
        
        if(i<m_MaxLevel)
        {
            Vector2 nextPos=m_Stages[i+1].position;
            nextPos.x+=(m_FrameWidth/2.f-(float)m_FrameWidth/3.f);
            nextPos.y+=((float)m_BgTex->height/2.f-(float)m_BgTex->height/1.5f);

            Vector2 pos=m_Stages[i].position;
            pos.x+=(m_FrameWidth/2.f-(float)m_FrameWidth/3.f);
            pos.y+=((float)m_BgTex->height/2.f-(float)m_BgTex->height/1.5f);
            DrawLineEx(pos,nextPos,6.f,{160,79,69,255});

        }
    }
}


void OverWorld::DrawPlayer()
{
   

    DrawTextureV(*m_PlayerTex,m_PlayerPos,WHITE);
    
}

void OverWorld::Update(float dT)
{
    HandleInput(); 
    PlayerMovement(dT);
        UpdateAnimations(dT);
   
    
}

void OverWorld::UpdateAnimations(float dT)
{
    m_FrameTime+=dT;
    if(m_FrameTime>=m_MaxFrameTime)
    {
        m_FrameTime=0.f;
        m_CurrFrame++;
        if(m_CurrFrame>=m_MaxFrames)
        {
            m_CurrFrame=0;
        }
    }
}

void OverWorld::HandleInput()
{
    if(m_IsMoving)
    {
        return;
    }
    if(IsKeyPressed(KEY_LEFT) && m_LevelIndex>0)
    {
        m_TargetPos=GetPlayerCenter(m_LevelIndex-1);
        m_LevelIndex--;
         m_IsMoving=true;
       
    }
    else if(IsKeyPressed(KEY_RIGHT) && m_LevelIndex<m_MaxLevel)
    {
        m_TargetPos=GetPlayerCenter(m_LevelIndex+1);
        m_LevelIndex++;
        m_IsMoving=true;
       
    }
    else if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
    {
        m_EnterLevel=true;
    }
}

Vector2 OverWorld::GetDirection()
{
     Vector2 start=GetPlayerCenter(m_LevelIndex);
     Vector2 end=GetPlayerCenter(m_LevelIndex+1);

    return Vector2Normalize(Vector2Subtract(end,start));
}

Vector2 OverWorld::GetPlayerCenter(int levelIndex)
{
    Vector2 pos=m_Stages[levelIndex].position;
    pos.x+=((float)m_FrameWidth/2.f-(float)m_FrameWidth/3.f)-(float)m_PlayerTex->width/2.f;
    pos.y+=((float)m_BgTex->height/2.f-(float)m_BgTex->height/1.5f)-(float)m_PlayerTex->height/2.f;
    return pos;
}


void OverWorld::PlayerMovement(float dT)
{
     if(m_IsMoving)
     {
        Vector2 difference=Vector2Subtract(m_TargetPos,m_PlayerPos);

        float distance=std::sqrtf((difference.x*difference.x)+(difference.y*difference.y));

        if(distance<= m_Speed *dT)
        {
            m_PlayerPos=m_TargetPos;
            m_IsMoving=false;
            return;
        }
        ///////////////
        // Normaliztion
        ///////////////
        m_Direction={
            difference.x/distance,
            difference.y/distance
        };
        m_PlayerPos.x+=m_Direction.x*m_Speed*dT;
        m_PlayerPos.y+=m_Direction.y*m_Speed*dT;
        
     }
}

bool OverWorld::ShouldEnterLevel()
{
    return m_EnterLevel && !m_IsMoving;
}

void OverWorld::ResetEnterRequest()
{
    m_EnterLevel=false;
}

void OverWorld::ResetPlayerPosition()
{
     m_PlayerPos=GetPlayerCenter(0);
}

OverWorldPalm::OverWorldPalm(float posX)
              
{
    m_CurrFrame=0;
    m_MaxFrames=4;
    m_FrameTime=0.f;
    m_MaxFrameTime=1.f/6.f;
    m_Position.x=posX;
   

    int choice=GetRandomValue(0,1);
    if(choice==0)
    {
        m_PalmTex=&ResourceManager::GetTexture("big_palm");
         m_Position.y=(6*Config::TILE_SIZE)+44.f;
    }
    else{
        m_PalmTex=&ResourceManager::GetTexture("small_palm");
         m_Position.y=(6*Config::TILE_SIZE)+75.f;
    }
}

void OverWorldPalm::Draw()
{
    float frameWidth=(float)m_PalmTex->width/m_MaxFrames;
     Rectangle source={frameWidth*m_CurrFrame,0.f,frameWidth,(float)m_PalmTex->height};
    Rectangle dest={m_Position.x,m_Position.y,(float)frameWidth,(float)m_PalmTex->height};
    DrawTexturePro(*m_PalmTex,source,dest,{0.f,0.f},0.f,WHITE);
}

void OverWorldPalm::Update(float dT)
{
    UpdateAnimations(dT);
}

void OverWorldPalm::UpdateAnimations(float dT)
{
      m_FrameTime+=dT;
    if(m_FrameTime>=m_MaxFrameTime)
    {
        m_FrameTime=0.f;
        m_CurrFrame++;
        if(m_CurrFrame>=m_MaxFrames)
        {
            m_CurrFrame=0;
        }
    }
}


void OverWorldBackground::TextureInit(Texture2D&tex,float heightOffset,float texHeight)
{
    m_Tex=&tex;
    m_HeightOffset=heightOffset;
    m_TextureHeight=texHeight;
}

void OverWorldBackground::Draw()
{
    if (m_Tex != nullptr)
    {
        Rectangle source = {0.f, 0.f, (float)m_Tex->width, (float)m_Tex->height};
        Rectangle dest = {0.f, m_HeightOffset, Config::SCREEN_WIDTH, m_TextureHeight};
        DrawTexturePro(*m_Tex, source, dest, {0.f, 0.f}, 0.f, WHITE);
    }
}

OverWorldClouds::OverWorldClouds()
{
    int choice=GetRandomValue(0,2);
    if(choice==0)
    {
        m_CloudTex=&ResourceManager::GetTexture("overworld_cloud1");
    }
    else if(choice==1)
    {
        m_CloudTex=&ResourceManager::GetTexture("overworld_cloud2");

    }
    else{
        m_CloudTex=&ResourceManager::GetTexture("overworld_cloud3");
    }
    m_Position={(float)GetRandomValue(0,Config::SCREEN_WIDTH-128),(float)GetRandomValue(10,Config::SCREEN_HEIGHT/3.f)};
}

void OverWorldClouds::Draw()
{
    DrawTextureV(*m_CloudTex,m_Position,WHITE);
}


