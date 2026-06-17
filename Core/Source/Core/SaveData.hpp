#pragma once 

#include <string>
#include <ranges>
#include <vector>

namespace Core {

    struct TileSaveData
    {
        std::string spriteName;
        int srcX = 1;
        int srcY = 1;
    };

    struct WorldGridData
    {
        int gridW = 1;
        int gridH = 1;
    };

    struct SaveData
    {
        WorldGridData WGridData;
        std::vector<TileSaveData> TileSDataVec;
    };

}