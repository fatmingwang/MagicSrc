#pragma once

#include <string>
#include <vector>

struct sMiniGameDrop
{
    std::string m_sItem;
    int m_iQty = 0;
};

struct sMiniGameResult
{
    std::string m_sGame; // e.g. "woodcut"
    int m_iScore = 0;
    int m_iMaxScore = 100;
    double m_dYieldMultiplier = 1.0;
    int m_iQualityScore = 0;
    bool m_bFailure = false;
    std::vector<sMiniGameDrop> m_vDrops;
    long m_lTimestamp = 0; // epoch
};
