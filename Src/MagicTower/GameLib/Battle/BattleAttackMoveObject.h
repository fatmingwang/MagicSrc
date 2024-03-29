#pragma once
#include "BattlAttackBase.h"
//===================
//
//2 moving objects keep moving at a direction,while they are overlap,click button
//and get a attack power
//
//===================
class	cBattleAttackMoveObject:public cBattleBase
{
    virtual void			InternalMouseMove(int e_iPosX,int e_iPosY);
    virtual void			InternalMouseDown(int e_iPosX,int e_iPosY);
    virtual void    		InternalMouseUp(int e_iPosX,int e_iPosY);
    virtual void    		InternalMouseLeave(int e_iPosX,int e_iPosY);
public:
	cBattleAttackMoveObject(eBattleType	m_eBattleType);
	~cBattleAttackMoveObject();
	virtual	void	Init();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
	virtual	void	DebugRender();
	struct sTwoMovingObjectController* m_pTwoMovingObjectController;
};