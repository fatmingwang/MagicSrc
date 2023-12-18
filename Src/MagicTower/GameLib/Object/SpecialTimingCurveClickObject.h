#pragma once


//a moving point while point close to key index detect touch 
//

class cSpecialTimingCurveClickObject:public cClickBehavior
{
	bool				m_bColliede;
	float				m_fRadiusForCollision = 30.f;
	float				m_fTimeOffset = 0.5f;

	//
	bool	CollideFunction(int e_iPosX,int e_iPosY);
	bool	MouseDownFunction(int e_iPosX, int e_iPosY);
	bool	MouseHorverFunction(int e_iPosX, int e_iPosY);
	bool	MouseUpFunction(int e_iPosX, int e_iPosY);
	bool	MouseLeaveFunction(int e_iPosX, int e_iPosY);
	//CollideFunction					m_CollideFunction;
	class cTweenyCurveWithTime* m_pTweenyCurveWithTime = nullptr;
	//std::vector<Vector3>		m_KeyIndexPosVector;
	//std::vector<float>		m_TimeVector;
public:
	cSpecialTimingCurveClickObject();
	virtual ~cSpecialTimingCurveClickObject();
	class cTweenyCurveWithTime*		GetTweenyCurveWithTime() { return m_pTweenyCurveWithTime; }
	virtual void    				Update(float e_fElpaseTime)override;
	void							Render();
	void							DebugRender();
	void							AssignTestingData();
};