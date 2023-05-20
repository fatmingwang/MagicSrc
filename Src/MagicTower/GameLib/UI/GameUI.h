#pragma once

class cGameUI:public cClickBehaviorDispatcher, cSingltonTemplate<cGameUI>
{
	cNumeralImage* m_pScoreNumberImage;
	cGameUI();
	virtual ~cGameUI();
public:
	SINGLETON_BASIC_FUNCTION(cGameUI);
	DEFINE_TYPE_INFO();
	virtual void	Init()override;
	virtual void	Update(float e_fElpaseTime)override;
	void	Render();
};