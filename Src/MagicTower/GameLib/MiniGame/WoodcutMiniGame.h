#pragma once

#include "IMiniGame.h"

class cWoodcutMiniGame : public cIMiniGame
{
public:
    cWoodcutMiniGame(int e_maxScore = 100);
    virtual ~cWoodcutMiniGame();

    virtual std::string GetName() const override;
    virtual void Start(int e_seed = 0) override;
    virtual void Update(double e_dt) override;
    virtual void OnClick(int e_x, int e_y) override;
    virtual bool IsFinished() const override;
    virtual sMiniGameResult GetResult() const override;

private:
    int m_iMaxScore;
    int m_iScore;
    double m_dbTimeRemaining;
    bool m_bFinished;
    int m_iCombo;
    sMiniGameResult m_sResult;
};