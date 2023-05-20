#pragma once


class cFingerMovingDirection:public cClickBehavior
{
	float					m_fMinAllowToMoveDis = 30.f;
    virtual bool			Collide(int e_iPosX, int e_iPosY)override;
    virtual cClickBehavior* MouseUp(int e_iPosX, int e_iPosY)override;
public:
	cFingerMovingDirection();
	~cFingerMovingDirection();
};