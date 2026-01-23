#include "FarmMiniGame.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

cFarmMiniGame::cFarmMiniGame(int e_maxScore)
: m_iMaxScore(e_maxScore)
, m_iScore(0)
, m_dbTimeRemaining(15.0)
, m_bFinished(false)
, m_iMisses(0)
{
}

cFarmMiniGame::~cFarmMiniGame()
{
}

std::string cFarmMiniGame::GetName() const
{
    return "farm";
}

void cFarmMiniGame::Start(int e_seed)
{
    if(e_seed == 0) e_seed = (int)time(nullptr);
    srand(e_seed);
    m_iScore = 0;
    m_dbTimeRemaining = 15.0;
    m_bFinished = false;
    m_iMisses = 0;
    m_sResult = sMiniGameResult();
    m_sResult.m_sGame = GetName();
    m_sResult.m_iMaxScore = m_iMaxScore;
}

void cFarmMiniGame::Update(double e_dt)
{
    if(m_bFinished) return;
    m_dbTimeRemaining -= e_dt;
    if(m_dbTimeRemaining <= 0.0)
    {
        m_bFinished = true;
        double scoreRatio = (double)m_iScore / (double)m_iMaxScore;
        double scoreFactor = 1.5;
        double maxMultiplier = 2.0;
        double mult = 1.0 + scoreRatio * scoreFactor;
        if(mult > maxMultiplier) mult = maxMultiplier;
        m_sResult.m_iScore = m_iScore;
        m_sResult.m_dYieldMultiplier = mult;
        m_sResult.m_iQualityScore = (int)floor(scoreRatio * 3.0);
        m_sResult.m_bFailure = (m_iMisses > (m_iMaxScore/10));
        m_sResult.m_lTimestamp = (long)time(nullptr);
        // chance for quality seed
        if(m_sResult.m_iQualityScore >= 2 && (rand()%100) < 25)
        {
            sMiniGameDrop l_d; l_d.m_sItem = "bonus_seed"; l_d.m_iQty = 1; m_sResult.m_vDrops.push_back(l_d);
        }
    }
}

void cFarmMiniGame::OnClick(int e_x, int e_y)
{
    if(m_bFinished) return;
    // simple: 75% chance click is correct
    int l_iR = rand() % 100;
    if(l_iR < 75)
    {
        m_iScore += 4;
    }
    else
    {
        m_iMisses++;
    }
    if(m_iScore > m_iMaxScore) m_iScore = m_iMaxScore;
}

bool cFarmMiniGame::IsFinished() const
{
    return m_bFinished;
}

sMiniGameResult cFarmMiniGame::GetResult() const
{
    return m_sResult;
}
