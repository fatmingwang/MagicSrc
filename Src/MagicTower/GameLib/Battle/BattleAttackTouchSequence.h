#pragma once

#include "BattlAttackBase.h"

class	cBattleAttackTouchSequence:public cBattleBase
{
public:
	cBattleAttackTouchSequence(eBattleType	e_eBattleType);
	~cBattleAttackTouchSequence();
	virtual	void	Init();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
};