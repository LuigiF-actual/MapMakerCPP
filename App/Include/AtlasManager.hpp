#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <filesystem>

#include "raylib.h"
#include "Core/PopUpWarnings.hpp"
#include "Config.hpp"

class AtlasManager
{
public:
    static AtlasManager& getInstance()
    {
        static AtlasManager atlas;
        return atlas;
    }

    const Texture2D& getTexture(const std::string& name) const
    {
        return m_TextureAtlas.at(name);
    }

    const Texture2D& getFirstTexture() const
    {
        return m_TextureAtlas.begin()->second;
    }

    const std::string& getTextureName(const Texture2D* tex) const
    {
        //If the texture is not found on the atlas for some reason it will use the default GrassComplete 
        static std::string lastResource = "GrassComplete";

        for (const auto& [name, texture] : m_TextureAtlas)
        {
            if (texture.id == tex->id)
            {
                return name;
            }
        }

        Core::Warnings::messageBox("Fatal", "A texture was not found, make sure all your textures are in the texture folder", "ok", "error", 0);

        return lastResource;
    }

private:

    AtlasManager()
    {

        std::filesystem::path TexturesDirPath = Config::getImagesDir();

        if (std::filesystem::is_directory(TexturesDirPath))
        {
            for (const auto& file : std::filesystem::recursive_directory_iterator{ TexturesDirPath })
            {
                if (!file.is_directory())
                {
                    std::string filename = file.path().stem().string();

                    m_TextureAtlas.emplace(
                        filename,
                        LoadTexture(file.path().string().c_str())
                    );


                }
            }
        }

    }

    ~AtlasManager()
    {
        for (auto& [name, texture] : m_TextureAtlas)
        {
            UnloadTexture(texture);
        }
    }

private:

    std::unordered_map<std::string, Texture2D> m_TextureAtlas;
};
