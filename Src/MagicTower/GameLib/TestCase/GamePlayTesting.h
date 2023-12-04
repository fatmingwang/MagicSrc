#pragma once


class cBattleAttackMoveObjectTesting :public cSimplePhase
{
	class cBattleAttackMoveObject* m_pBattleAttackMoveObject = nullptr;
public:
	cBattleAttackMoveObjectTesting();
	~cBattleAttackMoveObjectTesting();
	DEFINE_TYPE_INFO();
	virtual	void	Init()override;
	virtual	void	Update(float e_fElpaseTime)override;
	virtual	void	Render()override;
	virtual	void	KeyDown(char e_cKey)override;
};