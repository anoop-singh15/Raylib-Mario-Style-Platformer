#include"include/resourceManager.hpp"


void ResourceManager::LoadTex(const std::string&name,const std::string&filePath)
{
    if(m_Textures.find(name)==m_Textures.end())
    {
        m_Textures[name]=LoadTexture(filePath.c_str());
    }
}

Texture2D& ResourceManager::GetTexture(const std::string&name)
{
    if(m_Textures.find(name)!=m_Textures.end())
    return m_Textures[name];

    return m_Textures["idle"];
}

void ResourceManager::UnloadAll()
{
   for(auto&tex:m_Textures)
   {
     UnloadTexture(tex.second);
   }
   m_Textures.clear();
}


