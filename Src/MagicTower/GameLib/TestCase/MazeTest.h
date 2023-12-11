#pragma once

//for GameApp quick switch status mavoid put too much test code on GameApp.cpp.
//class cTestCaseBase:public cPhase
//{
//public:
//	virtual void	Update(float e_fElpaseTime) = 0;
//	virtual void	Render() = 0;
//	virtual	void	MouseDown(int e_iPosX, int e_iPosY) {}
//	virtual	void	MouseMove(int e_iPosX, int e_iPosY) {}
//	virtual	void	MouseUp(int e_iPosX, int e_iPosY) {}
//	virtual	void	KeyDown(unsigned char e_ucKey) {}
//	virtual	void	KeyUp(unsigned char e_ucKey) {}
//};

#include "../Tween/TweenCurve.h"

enum eTestPhase
{
	eTP_MAZE = 0,
	eTP_TWEEN,
	eTP_BATTLE_ATTACK_MOVING_OBJECT_TESTING,
	eTP_MAX
};

class cMazeTest :public cSimplePhase
{
	class cMazeRender* m_pCyucelenMazeGrid = nullptr;
public:
	cMazeTest();
	virtual ~cMazeTest();
	DEFINE_TYPE_INFO();
	virtual	void	Init()override;
	virtual	void	Update(float e_fElpaseTime)override;
	virtual	void	Render()override;
	virtual	void	KeyDown(char e_cKey)override;
};


class cTweenTest :public cSimplePhase
{
	class cTweenyTestObject* m_pTweenyTestObject = nullptr;
	class cTweenyCurveWithTime* m_pTweenyCurveWithTime = nullptr;
public:
	cTweenTest();
	virtual ~cTweenTest();
	DEFINE_TYPE_INFO();
	virtual	void	Init()override;
	virtual	void	Update(float e_fElpaseTime)override;
	virtual	void	Render()override;
	virtual	void	KeyDown(char e_cKey)override;
};

class cSpecialTimingCurveClickObjectTesting :public cSimplePhase
{
	class cSpecialTimingCurveClickObject*m_pSpecialTimingCurveClickObject = nullptr;
public:
	DEFINE_TYPE_INFO();
	cSpecialTimingCurveClickObjectTesting();
	virtual ~cSpecialTimingCurveClickObjectTesting();
	virtual	void	Init()override;
	virtual	void	Update(float e_fElpaseTime)override;
	virtual	void	Render()override;
	virtual void	MouseMove(int e_iPosX, int e_iPosY)override;
	virtual void	MouseDown(int e_iPosX, int e_iPosY)override;
	virtual void    MouseUp(int e_iPosX, int e_iPosY)override;
	virtual void    InternalMouseLeave(int e_iPosX, int e_iPosY)override;
	virtual	void	KeyDown(char e_cKey)override;
};

void AddTestPhase(eTestPhase e_eTestPhase, cPhaseManager&e_PhaseManager,bool e_bSetCurrent = false);
void SetWorkingTestPhase(eTestPhase e_eTestPhase, cPhaseManager& e_PhaseManager);
