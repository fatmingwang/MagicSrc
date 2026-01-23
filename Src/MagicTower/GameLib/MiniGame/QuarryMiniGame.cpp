#include "QuarryMiniGame.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

cQuarryMiniGame::cQuarryMiniGame(int e_maxScore)
: m_iMaxScore(e_maxScore)
, m_iScore(0)
, m_dbTimeRemaining(18.0)
, m_bFinished(false)
, m_iMisclicks(0)
{
}

cQuarryMiniGame::~cQuarryMiniGame()
{
}

std::string cQuarryMiniGame::GetName() const
{
    return "quarry";
}

void cQuarryMiniGame::Start(int e_seed)
{
    if(e_seed == 0) e_seed = (int)time(nullptr);
    srand(e_seed);
    m_iScore = 0;
    m_dbTimeRemaining = 18.0;
    m_bFinished = false;
    m_iMisclicks = 0;
    m_sResult = sMiniGameResult();
    m_sResult.m_sGame = GetName();
    m_sResult.m_iMaxScore = m_iMaxScore;
}

void cQuarryMiniGame::Update(double e_dt)
{
    if(m_bFinished) return;
    m_dbTimeRemaining -= e_dt;
    if(m_dbTimeRemaining <= 0.0)
    {
        m_bFinished = true;
        double scoreRatio = (double)m_iScore / (double)m_iMaxScore;
        double scoreFactor = 2.0;
        double maxMultiplier = 3.0;
        double mult = 1.0 + scoreRatio * scoreFactor;
        if(mult > maxMultiplier) mult = maxMultiplier;
        m_sResult.m_iScore = m_iScore;
        m_sResult.m_dYieldMultiplier = mult;
        m_sResult.m_iQualityScore = (int)floor(scoreRatio * 4.0);
        // failure if too many misclicks
        m_sResult.m_bFailure = (m_iMisclicks > 5);
        m_sResult.m_lTimestamp = (long)time(nullptr);
        // rare gem drop chance
        if(m_sResult.m_iQualityScore >= 3 && (rand()%100) < 15)
        {
            sMiniGameDrop l_d; l_d.m_sItem = "rare_gem"; l_d.m_iQty = 1; m_sResult.m_vDrops.push_back(l_d);
        }
    }
}

void cQuarryMiniGame::OnClick(int e_x, int e_y)
{
    if(m_bFinished) return;
    // timing based: high chance to succeed but small chance to hit forbidden zone
    int l_iR = rand() % 100;
    if(l_iR < 65)
    {
        m_iScore += 6;
    }
    else if(l_iR < 85)
    {
        m_iScore += 2; // mediocre hit
    }
    else
    {
        // bad hit
        m_iMisclicks++;
        m_iScore -= 4;
        if(m_iScore < 0) m_iScore = 0;
    }
    if(m_iScore > m_iMaxScore) m_iScore = m_iMaxScore;
}

bool cQuarryMiniGame::IsFinished() const
{
    return m_bFinished;
}

sMiniGameResult cQuarryMiniGame::GetResult() const
{
    return m_sResult;
}
