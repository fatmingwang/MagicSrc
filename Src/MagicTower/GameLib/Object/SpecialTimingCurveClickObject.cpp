#include "stdafx.h"
#include "SpecialTimingCurveClickObject.h"
#include "../Tween/TweenCurve.h"

cSpecialTimingCurveClickObject::cSpecialTimingCurveClickObject()
{
	auto l_CollideFunction = std::bind(&cSpecialTimingCurveClickObject::CollideFunction, this,std::placeholders::_1, std::placeholders::_2);
	auto l_MouseDownFunction = std::bind(&cSpecialTimingCurveClickObject::MouseDownFunction, this, std::placeholders::_1, std::placeholders::_2);
	auto l_MouseHorverFunction = std::bind(&cSpecialTimingCurveClickObject::MouseHorverFunction, this, std::placeholders::_1, std::placeholders::_2);
	auto l_MouseUpFunction = std::bind(&cSpecialTimingCurveClickObject::MouseUpFunction, this, std::placeholders::_1, std::placeholders::_2);
	auto l_MouseLeaveFunction = std::bind(&cSpecialTimingCurveClickObject::MouseLeaveFunction, this, std::placeholders::_1, std::placeholders::_2);
	SetMouseFunction(l_CollideFunction,
		l_MouseDownFunction,
		l_MouseHorverFunction,
		l_MouseUpFunction,
		nullptr,
		l_MouseLeaveFunction);
}

cSpecialTimingCurveClickObject::~cSpecialTimingCurveClickObject()
{
}

void cSpecialTimingCurveClickObject::Update(float e_fElpaseTime)
{
	cClickBehavior::Update(e_fElpaseTime);
}

void cSpecialTimingCurveClickObject::Render()
{
}

bool cSpecialTimingCurveClickObject::CollideFunction(int e_iPosX, int e_iPosY)
{
	auto l_pCurve = m_pTweenyCurveWithTime->GetCurve();
	float l_fTime = l_pCurve->GetCurrentTime();
	int l_iIndex = l_pCurve->GetTimeRelativeIndex(l_fTime);
	if (l_iIndex != -1)
	{
		Vector3 l_vPos = l_pCurve->GetOriginalPointList()[l_iIndex];
		Vector3 l_vTouchPos((float)e_iPosX, (float)e_iPosY, 0.f);
		float l_fDis = (l_vTouchPos - l_vPos).Length();
		if (l_fDis <= m_fRadiusForCollision)
		{
			return true;
		}
	}
	return false;
}

bool cSpecialTimingCurveClickObject::MouseDownFunction(int e_iPosX, int e_iPosY)
{
	return false;
}

bool cSpecialTimingCurveClickObject::MouseHorverFunction(int e_iPosX, int e_iPosY)
{
	return false;
}

bool cSpecialTimingCurveClickObject::MouseUpFunction(int e_iPosX, int e_iPosY)
{
	return false;
}

bool cSpecialTimingCurveClickObject::MouseLeaveFunction(int e_iPosX, int e_iPosY)
{
	return false;
}