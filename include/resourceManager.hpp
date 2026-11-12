#pragma once
#include<unordered_map>
#include<raylib.h>
#include<string>



class ResourceManager{
    private:
    static inline std::unordered_map<std::string,Texture2D>m_Textures;
    public:
    static void LoadTex(const std::string&name,const std::string&filePath);
    static Texture2D& GetTexture(const std::string&name);
    static void UnloadAll();
};
