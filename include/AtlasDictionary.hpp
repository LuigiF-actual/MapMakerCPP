#pragma once

#include <iostream>
#include <unordered_map>
#include <filesystem>

#include <raylib.h>


class AtlasManager
{
public:
    static AtlasManager& getInstance()
    {
        static AtlasManager atlas;
        return atlas;
    }

    AtlasManager(const AtlasManager&) = delete; //avoid copying as if it were to be copied 2 instances could 
    AtlasManager& operator=(const AtlasManager&) = delete;

    const Texture2D& getTexture(const std::string& name) const
    {
        return sm_TextureAtlas.at(name);
    }
    const Texture2D& getFirstTexture() const
    {
        return sm_TextureAtlas.begin()->second;
    }

private:
    /* data */
    AtlasManager()
    {

        std::filesystem::path TexturesDirPath = RESOURCES_PATH; //cool trick

        if (std::filesystem::is_directory(TexturesDirPath))
        {
            for (const auto& file : std::filesystem::recursive_directory_iterator{ TexturesDirPath })
            {
                if (!file.is_directory())
                {
                    std::string filename = file.path().stem().string();

                    sm_TextureAtlas.emplace(
                        filename,
                        LoadTexture(file.path().string().c_str())
                    );

 
                }
            }
        }

    }

    ~AtlasManager()
    {
        for (auto& [name, texture] : sm_TextureAtlas)
        {
            UnloadTexture(texture);
        }
    }

private:

    std::unordered_map<std::string, Texture2D> sm_TextureAtlas;
};
