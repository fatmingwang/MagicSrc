#include "GoldMiniGame.h"
#include <cstdlib>
#include <ctime>

, m_iGridSize(e_gridSize)
, m_iPicksLeft(e_picks)
, m_bFinished(false)
{
}

cGoldMiniGame::~cGoldMiniGame()
{
}

std::string cGoldMiniGame::GetName() const
{
    return "gold";
}

void cGoldMiniGame::Start(int e_seed)
{
    if(e_seed == 0) e_seed = (int)time(nullptr);
    srand(e_seed);
    m_picksLeft = std::max(1, m_picksLeft);
    m_finished = false;
    m_result = sMiniGameResult();
    m_result.game = GetName();
    m_result.maxScore = m_gridSize * m_gridSize;
}

void cGoldMiniGame::Update(double e_dt)
{
    // gold mini game is mostly input driven; no timer by default
}

void cGoldMiniGame::OnClick(int e_x, int e_y)
{
    if(m_bFinished) return;
    if(m_iPicksLeft <= 0) return;
    m_iPicksLeft--;
    int l_iR = rand() % 100;
    if(l_iR < 10)
    {
        // jackpot
        m_sResult.m_iScore += 10;
        sMiniGameDrop l_d; l_d.m_sItem = "gold_nugget"; l_d.m_iQty = 1; m_sResult.m_vDrops.push_back(l_d);
    }
    else if(l_iR < 60)
    {
        m_sResult.m_iScore += 2; // stone
    }
    else if(l_iR < 80)
    {
        m_sResult.m_iScore += 4; // small gold
    }
    else
    {
        // trap
        m_sResult.m_bFailure = true;
    }
    if(m_iPicksLeft == 0)
    {
        m_bFinished = true;
        double scoreRatio = (double)m_sResult.m_iScore / (double)m_sResult.m_iMaxScore;
        double mult = 1.0 + scoreRatio * 3.0;
        if(mult > 5.0) mult = 5.0;
        m_sResult.m_dYieldMultiplier = mult;
        m_sResult.m_iQualityScore = (int)floor(scoreRatio * 4.0);
        m_sResult.m_lTimestamp = (long)time(nullptr);
    }
}

bool cGoldMiniGame::IsFinished() const
{
    return m_bFinished;
}

sMiniGameResult cGoldMiniGame::GetResult() const
{
    return m_sResult;
}
