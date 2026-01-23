#include "WoodcutMiniGame.h"
#include <cstdlib>
#include <ctime>

cWoodcutMiniGame::cWoodcutMiniGame(int e_maxScore)
: m_iMaxScore(e_maxScore)
, m_iScore(0)
, m_dbTimeRemaining(20.0) // default duration
, m_bFinished(false)
, m_iCombo(0)
{
}

cWoodcutMiniGame::~cWoodcutMiniGame()
{
}

std::string cWoodcutMiniGame::GetName() const
{
    return "woodcut";
}

void cWoodcutMiniGame::Start(int e_seed)
{
    if(e_seed == 0)
        e_seed = (int)time(nullptr);
    srand(e_seed);
    m_iScore = 0;
    m_dbTimeRemaining = 20.0;
    m_bFinished = false;
    m_iCombo = 0;
    m_sResult = sMiniGameResult();
    m_sResult.m_sGame = GetName();
    m_sResult.m_iMaxScore = m_iMaxScore;
}

void cWoodcutMiniGame::Update(double e_dt)
{
    if(m_bFinished) return;
    m_dbTimeRemaining -= e_dt;
    if(m_dbTimeRemaining <= 0.0)
    {
        m_bFinished = true;
        // compute result
        double l_dScoreRatio = (double)m_iScore / (double)m_iMaxScore;
        double l_dScoreFactor = 1.5;
        double l_dMaxMultiplier = 2.5;
        double l_dMult = 1.0 + l_dScoreRatio * l_dScoreFactor;
        if(l_dMult > l_dMaxMultiplier) l_dMult = l_dMaxMultiplier;
        m_sResult.m_iScore = m_iScore;
        m_sResult.m_dYieldMultiplier = l_dMult;
        m_sResult.m_iQualityScore = (int)floor(l_dScoreRatio * 3.0); // 3 quality levels
        m_sResult.m_bFailure = false;
        m_sResult.m_lTimestamp = (long)time(nullptr);
        // small chance for rare drop if quality high
        if(m_sResult.m_iQualityScore >= 2 && (rand() % 100) < 20)
        {
            sMiniGameDrop l_d;
            l_d.m_sItem = "rare_wood";
            l_d.m_iQty = 1;
            m_sResult.m_vDrops.push_back(l_d);
        }
    }
}

void cWoodcutMiniGame::OnClick(int e_x, int e_y)
{
    if(m_bFinished) return;
    // simple prototype: each click during active time gives random success depending on timing
    int l_iR = rand() % 100;
    if(l_iR < 70)
    {
        // success
        int l_iGained = 5 + (m_iCombo / 5);
        m_iScore += l_iGained;
        m_iCombo++;
    }
    else
    {
        // fail
        m_iScore -= 3;
        if(m_iScore < 0) m_iScore = 0;
        m_iCombo = 0;
        // small chance of failure flag
        if((rand() % 100) < 5)
            m_sResult.m_bFailure = true;
    }
    if(m_iScore > m_iMaxScore) m_iScore = m_iMaxScore;
}

bool cWoodcutMiniGame::IsFinished() const
{
    return m_bFinished;
}

sMiniGameResult cWoodcutMiniGame::GetResult() const
{
    return m_sResult;
}
