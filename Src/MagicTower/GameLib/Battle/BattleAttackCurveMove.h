#pragma once

#include "BattlAttackBase.h"

class	cBattleAttackCurveMove:public cBattleBase
{
public:
	cBattleAttackCurveMove(eBattleType	m_eBattleType);
	~cBattleAttackCurveMove();
	virtual	void	Init();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
};