#include"include/tile.hpp"
#include"include/resourceManager.hpp"

Tile::Tile(float x,float y,float size,int scrRow,int scrCol)
{
    m_Tile={x,y,size,size};
    m_TileSize=size;
    m_ScrRow=(scrRow);
    m_ScrCol=(scrCol);
   
}

void Tile::Draw()
{ 
     Texture2D&terrainTex=ResourceManager::GetTexture("terrain");
    Rectangle source= {m_ScrCol*m_TileSize,m_ScrRow*m_TileSize,m_TileSize,m_TileSize};
    DrawTexturePro(terrainTex,source,m_Tile,{0.f,0.f},0.f,WHITE);
}

Tile::~Tile()
{

}


Grass::Grass(float x,float y,float size,int scrRow,int scrCol)
      :Tile(x,y,size,scrRow,scrCol)
{  
}

void Grass::Draw()
{ 
     Texture2D&grassTex=ResourceManager::GetTexture("grass");
    Rectangle source= {m_ScrCol*m_TileSize,m_ScrRow*m_TileSize,m_TileSize,m_TileSize};
    DrawTexturePro(grassTex,source,m_Tile,{0.f,0.f},0.f,WHITE);
}

Grass::~Grass()
{

}


Crate::Crate(float x,float y,float size,int scrRow,int scrCol)
      :Tile(x,y,size,scrRow,scrCol)
{  
}

void Crate::Draw()
{ 
    Texture2D&crateTex=ResourceManager::GetTexture("crate");
    Rectangle source= {m_ScrCol*m_TileSize,m_ScrRow*m_TileSize,(float)crateTex.width,(float)crateTex.height};
    Rectangle dest={m_Tile.x,m_Tile.y+25.f,(float)crateTex.width,(float)crateTex.height};
    DrawTexturePro(crateTex,source,dest,{0.f,0.f},0.f,WHITE);

   
}

Crate::~Crate()
{

}

Rectangle Crate::GetCollider()
{
    float offsetY=25.f;
    return{
        m_Tile.x,
        m_Tile.y+offsetY,
        58,
        42
    };
}

AnimatedTile::AnimatedTile(float x, float y)
{
    m_Tile={x,y};
    m_Active=true;

}



void AnimatedTile::Update(float dT)
{
    if(m_Active)
    {
        UpdateAnimations(dT);
    }
}

void AnimatedTile::UpdateAnimations(float dT)
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

AnimatedTile::~AnimatedTile()
{

}

Coins::Coins(float x,float y,int type)
      :AnimatedTile(x,y)
{
    m_CurrFrame=0;
    m_MaxFrames=4;
    m_FrameTime=0.f;
    m_MaxFrameTime=1.f/6.f;
    m_Type=type;
    

    
    if(m_Type==1)
    {
        m_CoinTex=&ResourceManager::GetTexture("silver_coin");
        m_Value=1;
    }
    else{
        m_CoinTex=&ResourceManager::GetTexture("gold_coin");
        m_Value=5;
    }
    m_FrameWidth=static_cast<float>(m_CoinTex->width)/m_MaxFrames;
}

void Coins::Draw()
{
    
    
    Rectangle source={m_FrameWidth*m_CurrFrame,0.f,m_FrameWidth,static_cast<float>(m_CoinTex->height)};
    Rectangle dest={m_Tile.x-17.f,m_Tile.y-16.f,100.f,100.f};
    DrawTexturePro(*m_CoinTex,source,dest,{0.f,0.f},0.f,WHITE);
   

}

Rectangle Coins::GetCoinCollider()
{
    
    return {m_Tile.x+29.f,m_Tile.y+28.f,10.f,10.f};
}

Palm::Palm(float x,float y,int type)
      :AnimatedTile(x,y)
{
    m_CurrFrame=0;
    m_MaxFrames=4;
    m_FrameTime=0.f;
    m_MaxFrameTime=1.f/5.f;
    m_Type=type;
}

void Palm::Draw()
{
    Texture2D*palmTex=nullptr;
    if(m_Type==0)
    {
        palmTex=&ResourceManager::GetTexture("palm_small");
    }
    else if(m_Type==1){
        palmTex=&ResourceManager::GetTexture("palm_large");
    }
    float frameWidth=static_cast<float>(palmTex->width)/m_MaxFrames;
    Rectangle source={frameWidth*m_CurrFrame,0.f,frameWidth,static_cast<float>(palmTex->height)};
    Rectangle dest={m_Tile.x,m_Tile.y,frameWidth,static_cast<float>(palmTex->height)};

    if(m_Type==0)
    {
        dest.y+=-38.f;
    }
    else if(m_Type==1)
    {
        dest.y+=-71.f;
    }
    DrawTexturePro(*palmTex,source,dest,{0.f,0.f},0.f,WHITE);
    
    // Rectangle collider=GetCollider();
    // DrawRectangleLines(collider.x,collider.y,collider.width,collider.height,PURPLE);


}

Rectangle Palm::GetCollider()
{
   float offsetX = 2.0f;
    float colliderWidth = 64.0f;
    float colliderHeight = 32.0f;

    float offsetY = 0.0f;

    if (m_Type == 0)
    {
        offsetY = -38.0f;
    }
    else if (m_Type == 1)
    {
        offsetY = -71.0f;
    }

    return {
        m_Tile.x + offsetX,
        m_Tile.y + offsetY,
        colliderWidth,
        colliderHeight
    };
}

BgPalm::BgPalm(float x,float y)
      :AnimatedTile(x,y)
{
    m_CurrFrame=0;
    m_MaxFrames=4;
    m_FrameTime=0.f;
    m_MaxFrameTime=1.f/5.f;
    
}

void BgPalm::Draw()
{
    Texture2D &bgPalmTex=ResourceManager::GetTexture("bg_palm");
    
    float frameWidth=static_cast<float>(bgPalmTex.width)/m_MaxFrames;
    Rectangle source={frameWidth*m_CurrFrame,0.f,frameWidth,static_cast<float>(bgPalmTex.height)};
    Rectangle dest={m_Tile.x,m_Tile.y,frameWidth,static_cast<float>(bgPalmTex.height)};

    dest.y+=(-64.f);
      DrawTexturePro(bgPalmTex,source,dest,{0.f,0.f},0.f,WHITE);

}

