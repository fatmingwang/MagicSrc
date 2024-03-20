#pragma once





//a moving point while point close to key index detect touch 
//

class cSpecialTimingCurveClickObject:public cRenderAndClickObject
{
public:
	enum eTrackTYpe
	{
		eTY_KEY_POINT = 0,
		eTY_FOLLOW_CURVE,
		eTY_DRAW_CURVE,
		eTY_MAX
	};
protected:
	struct sDrawCurveTouchData
	{
		int		iCurrentIndex;
		bool	bFinish;
		float	fStartTime;
		float	fEndTime;
		float   fAllownToFinishTime;
		void	SetDataByCurve(cTweenyCurveWithTime*e_pTweenyCurveWithTime);
		//float	GetWorkingResultPercentage();
	};
	sDrawCurveTouchData*		m_pDrawCurveTouchData = nullptr;

	bool						m_bColliede;
	float						m_fRadiusForCollision = 30.f;
	float						m_fTimeOffset = 0.5f;

	//put a internal function for collision to caculate score?
	bool						CollideWithCurveFunction(int e_iPosX, int e_iPosY);
	bool						CollideOnlyKeyPointFunction(int e_iPosX,int e_iPosY);
	bool						CollideDrawCurveFunction(int e_iPosX, int e_iPosY);
	bool						MouseDownFunction(int e_iPosX, int e_iPosY);
	bool						MouseHorverFunction(int e_iPosX, int e_iPosY);
	bool						MouseUpFunction(int e_iPosX, int e_iPosY);
	bool						MouseLeaveFunction(int e_iPosX, int e_iPosY);
	class cTweenyCurveWithTime* m_pTweenyCurveWithTime = nullptr;
	eTrackTYpe					m_eTrackTYpe;
	std::vector<int>			m_ColliededKeyPointVector;
public:
	cSpecialTimingCurveClickObject(eTrackTYpe e_eTrackTYpe = eTrackTYpe::eTY_KEY_POINT, const char* e_strDebugLineFileName = "MagicTower/Image/V_Wall.png", float e_fTimeOffset = 3.5f, float e_fRadiusForCollision = 30.f);
	virtual ~cSpecialTimingCurveClickObject();
	static cSpecialTimingCurveClickObject* CreateWithData(eTrackTYpe e_eTrackTYpe = eTrackTYpe::eTY_KEY_POINT, const char* e_strDebugLineFileName = "MagicTower/Image/V_Wall.png", float e_fTimeOffset = 0.5f, float e_fRadiusForCollision = 30.f);
	class cTweenyCurveWithTime*		GetTweenyCurveWithTime() { return m_pTweenyCurveWithTime; }
	virtual void    				Update(float e_fElpaseTime)override;
	//void							Render();
	void							DebugRender();
	void							AssignTestingData(int e_iIndex,float e_fOffsetStartTime = 0,float e_fGenerateCurveTime = 3);
	void							Reset();
	//time over or all clicked?
	bool							GetWorkingResultPercentage(float&e_fScore);
};