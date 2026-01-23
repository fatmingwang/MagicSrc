#include "MiniGameFactory.h"
#include "WoodcutMiniGame.h"
#include "FarmMiniGame.h"
#include "QuarryMiniGame.h"
#include "GoldMiniGame.h"
#include <algorithm>

cIMiniGame* MiniGameFactory::Create(const std::string& name, int e_param)
{
    std::string n = name;
    std::transform(n.begin(), n.end(), n.begin(), ::tolower);
        if(n == "woodcut" || n == "???")
    {
        int l_iVal = e_param > 0 ? e_param : 100;
        return new cWoodcutMiniGame(l_iVal);
    }
        if(n == "farm" || n == "??")
    {
        int l_iVal = e_param > 0 ? e_param : 100;
        return new cFarmMiniGame(l_iVal);
    }
        if(n == "quarry" || n == "???")
    {
        int l_iVal = e_param > 0 ? e_param : 100;
        return new cQuarryMiniGame(l_iVal);
    }
        if(n == "gold" || n == "???")
    {
        // param can be gridSize for gold
        int l_iGrid = e_param > 0 ? e_param : 3;
        int l_iPicks = 3;
        return new cGoldMiniGame(l_iGrid, l_iPicks);
    }
    // other mini games to add later
    return nullptr;
}
