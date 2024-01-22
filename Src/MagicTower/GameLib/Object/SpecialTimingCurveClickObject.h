#pragma once


//a moving point while point close to key index detect touch 
//

class cSpecialTimingCurveClickObject:public cRenderAndClickObject
{
	bool						m_bColliede;
	float						m_fRadiusForCollision = 30.f;
	float						m_fTimeOffset = 0.5f;

	//
	bool						CollideWithCurveFunction(int e_iPosX, int e_iPosY);
	bool						CollideOnlyKeyPointFunction(int e_iPosX,int e_iPosY);
	bool						MouseDownFunction(int e_iPosX, int e_iPosY);
	bool						MouseHorverFunction(int e_iPosX, int e_iPosY);
	bool						MouseUpFunction(int e_iPosX, int e_iPosY);
	bool						MouseLeaveFunction(int e_iPosX, int e_iPosY);
	class cTweenyCurveWithTime* m_pTweenyCurveWithTime = nullptr;
	bool						m_bTrackCurveNotKeyPoint;
	std::vector<int>			m_ColliededKeyPointVector;
public:
	cSpecialTimingCurveClickObject(bool e_bTrackCurveNotKeyPoint = true, const char* e_strDebugLineFileName = "MagicTower/Image/V_Wall.png");
	virtual ~cSpecialTimingCurveClickObject();
	class cTweenyCurveWithTime*		GetTweenyCurveWithTime() { return m_pTweenyCurveWithTime; }
	virtual void    				Update(float e_fElpaseTime)override;
	//void							Render();
	void							DebugRender();
	void							AssignTestingData(int e_iIndex,float e_fOffsetStartTime = 0,float e_fGenerateCurveTime = 3);
	void							Reset();
};