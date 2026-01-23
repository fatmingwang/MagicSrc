#pragma once

#include "IMiniGame.h"

class cGoldMiniGame : public cIMiniGame
{
public:
    cGoldMiniGame(int e_gridSize = 3, int e_picks = 3);
    virtual ~cGoldMiniGame();

    virtual std::string GetName() const override;
    virtual void Start(int e_seed = 0) override;
    virtual void Update(double e_dt) override;
    virtual void OnClick(int e_x, int e_y) override;
    virtual bool IsFinished() const override;
    virtual sMiniGameResult GetResult() const override;

private:
    int m_iGridSize;
    int m_iPicksLeft;
    bool m_bFinished;
    sMiniGameResult m_sResult;
};