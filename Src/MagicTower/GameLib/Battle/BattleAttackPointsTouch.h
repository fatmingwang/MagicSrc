#pragma once

#include "BattlAttackBase.h"

class	cBattleAttackPointsTouch:public cBattleBase
{
public:
	cBattleAttackPointsTouch(eBattleType e_eBattleType);
	~cBattleAttackPointsTouch();
	virtual	void	Init();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
};
