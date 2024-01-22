#include "stdafx.h"
#include "SpecialTimingCurveClickObject.h"
#include "../Tween/TweenCurve.h"

cSpecialTimingCurveClickObject::cSpecialTimingCurveClickObject(bool e_bTrackCurveNotKeyPoint, const char* e_strDebugLineFileName)
{
	e_bTrackCurveNotKeyPoint = false;
	m_bTrackCurveNotKeyPoint = e_bTrackCurveNotKeyPoint;
	m_pTweenyCurveWithTime = new cTweenyCurveWithTime(2,2, e_strDebugLineFileName);
	auto l_pRenderObject = this->CreateRenderObject();
	l_pRenderObject->AddChild(m_pTweenyCurveWithTime);
	//l_pRenderObject->AddChild(m_pTweenyCurveWithTime->m_pDeugLineImage);
	auto l_CollideTrackCurveFunction = std::bind(&cSpecialTimingCurveClickObject::CollideWithCurveFunction, this, std::placeholders::_1, std::placeholders::_2);
	auto l_CollideKeyPointFunction = std::bind(&cSpecialTimingCurveClickObject::CollideOnlyKeyPointFunction, this, std::placeholders::_1, std::placeholders::_2);
	auto l_CollideFunction = std::bind(&cSpecialTimingCurveClickObject::CollideWithCurveFunction, this, std::placeholders::_1, std::placeholders::_2);
	auto l_MouseDownFunction = std::bind(&cSpecialTimingCurveClickObject::MouseDownFunction, this, std::placeholders::_1, std::placeholders::_2);
	auto l_MouseHorverFunction = std::bind(&cSpecialTimingCurveClickObject::MouseHorverFunction, this, std::placeholders::_1, std::placeholders::_2);
	auto l_MouseUpFunction = std::bind(&cSpecialTimingCurveClickObject::MouseUpFunction, this, std::placeholders::_1, std::placeholders::_2);
	auto l_MouseLeaveFunction = std::bind(&cSpecialTimingCurveClickObject::MouseLeaveFunction, this, std::placeholders::_1, std::placeholders::_2);
	this->SetMouseFunction(
		m_bTrackCurveNotKeyPoint ? l_CollideTrackCurveFunction : l_CollideKeyPointFunction,
		l_MouseDownFunction,
		l_MouseHorverFunction,
		l_MouseUpFunction,
		nullptr,
		l_MouseLeaveFunction);
}

cSpecialTimingCurveClickObject::~cSpecialTimingCurveClickObject()
{
	//SAFE_DELETE(m_pTweenyCurveWithTime);
}

void cSpecialTimingCurveClickObject::Update(float e_fElpaseTime)
{
	cClickBehavior::Update(e_fElpaseTime);
}

//void cSpecialTimingCurveClickObject::Render()
//{
//	if (m_pTweenyCurveWithTime)
//	{
//		m_pTweenyCurveWithTime->Render(m_fRadiusForCollision, m_bColliede?Vector4::Green:Vector4::Red);
//	}
//}

void cSpecialTimingCurveClickObject::DebugRender()
{
}

void cSpecialTimingCurveClickObject::AssignTestingData(int e_iIndex, float e_fOffsetStartTime, float e_fGenerateCurveTime)
{
	if (m_pTweenyCurveWithTime)
	{
		float l_fGenerateCurveTime = 4.f;
		cCurveWithTime*l_pData = new cCurveWithTime();
		//l_Data.AddPoint(Vector3(l_fStartX+0,  l_fStartY+0, 0), 0);
		//l_Data.AddPoint(Vector3(l_fStartX-150, l_fStartY+150, 0), 1);
		//l_Data.AddPoint(Vector3(l_fStartX+350,  l_fStartY+150, 0), 2);
		//l_Data.AddPoint(Vector3(l_fStartX-100, l_fStartY+100, 0), 3);
		//l_Data.AddPoint(Vector3(l_fStartX+0,  l_fStartY+250, 0), 4);
		if (e_iIndex == 0)
		{
			float l_fStartX = 200;
			float l_fStartY = 100;
			l_pData->AddPoint(Vector3(l_fStartX + 0, l_fStartY + 0, 0), 0);
			l_pData->AddPoint(Vector3(l_fStartX + 150, l_fStartY + 0, 0), 1);
			l_pData->AddPoint(Vector3(l_fStartX + 150, l_fStartY + 150, 0), 2);
			l_pData->AddPoint(Vector3(l_fStartX + 0, l_fStartY + 150, 0), 3);// 
			l_pData->AddPoint(Vector3(l_fStartX + 0, l_fStartY + 0, 0), 4);// 
			l_pData->SetLOD(3);
		}
		else
		if (e_iIndex == 1)
		{
			float l_fStartX = 600;
			float l_fStartY = 100;
			l_pData->AddPoint(Vector3(l_fStartX + 0, l_fStartY + 0, 0), 0 );
			l_pData->AddPoint(Vector3(l_fStartX + 150, l_fStartY + 0, 0), 2 );
			l_pData->AddPoint(Vector3(l_fStartX + 150, l_fStartY + 150, 0), 4 );
			l_pData->AddPoint(Vector3(l_fStartX + 0, l_fStartY + 150, 0), 6 );// 
			l_pData->AddPoint(Vector3(l_fStartX + 0, l_fStartY + 0, 0), 8 );// 
			l_pData->SetLOD(3);
		}
		else
		if (e_iIndex == 2)
		{
			float l_fStartX = 200;
			float l_fStartY = 500;
			l_pData->AddPoint(Vector3(l_fStartX + 0, l_fStartY + 0, 0), 0 );
			l_pData->AddPoint(Vector3(l_fStartX + 150, l_fStartY + 0, 0), 3 );
			l_pData->AddPoint(Vector3(l_fStartX + 150, l_fStartY + 150, 0), 6 );
			l_pData->AddPoint(Vector3(l_fStartX + 0, l_fStartY + 150, 0), 9 );// 
			l_pData->AddPoint(Vector3(l_fStartX + 0, l_fStartY + 0, 0), 12 );// 
			l_pData->SetLOD(3);
		}
		else
		if (e_iIndex == 3)
		{
			float l_fStartX = 600;
			float l_fStartY = 500;
			l_pData->AddPoint(Vector3(l_fStartX + 0, l_fStartY + 0, 0), 0 );
			l_pData->AddPoint(Vector3(l_fStartX + 150, l_fStartY + 0, 0), 4 );
			l_pData->AddPoint(Vector3(l_fStartX + 150, l_fStartY + 150, 0), 8 );
			l_pData->AddPoint(Vector3(l_fStartX + 0, l_fStartY + 150, 0), 12 );// 
			l_pData->AddPoint(Vector3(l_fStartX + 0, l_fStartY + 0, 0), 16 );// 
			l_pData->SetLOD(3);
		}
		//m_pTweenyCurveWithTime->SetData(tweeny::easing::enumerated::quadraticInOut, l_pData->GetEndTime(), l_pData, nullptr, e_fOffsetStartTime, e_fOffsetStartTime - 3);
		m_pTweenyCurveWithTime->SetData(tweeny::easing::enumerated::linear, l_pData->GetEndTime(), l_pData, 
			[](unsigned int e_Value)
			{
				int a = 0;
			}, e_fOffsetStartTime, e_fOffsetStartTime - e_fGenerateCurveTime);
	}
}

void cSpecialTimingCurveClickObject::Reset()
{
	if (m_pTweenyCurveWithTime)
	{
		m_pTweenyCurveWithTime->Reset();
	}
}

bool cSpecialTimingCurveClickObject::CollideWithCurveFunction(int e_iPosX, int e_iPosY)
{
	auto l_pCurve = m_pTweenyCurveWithTime->GetCurve();
	float l_fTime = l_pCurve->GetCurrentTime();
	int l_iIndex = l_pCurve->GetTimeRelativeIndexWithFinalPointList(l_fTime,0.2f);
	if (l_iIndex != -1)
	{
		Vector3 l_vPos = l_pCurve->GetPointList()[l_iIndex];
		Vector3 l_vTouchPos((float)e_iPosX, (float)e_iPosY, 0.f);
		float l_fDis = (l_vTouchPos - l_vPos).Length();
		if (l_fDis <= m_fRadiusForCollision)
		{//show mouse positio and sphere position for debug.
			
			m_bColliede = true;
			return true;
		}
		m_bColliede = false;
		//cGameApp::ShowInfoOnScreen(L"Missing");
	}
	return false;
}

bool cSpecialTimingCurveClickObject::CollideOnlyKeyPointFunction(int e_iPosX, int e_iPosY)
{
	auto l_pCurve = m_pTweenyCurveWithTime->GetCurve();
	auto l_CollisionPointIndexOfCurveAndTimeLerpHintMap = m_pTweenyCurveWithTime->GetCollisionPointIndexOfCurveAndTimeLerpHintMap();
	if (l_CollisionPointIndexOfCurveAndTimeLerpHintMap.size())
	{
		Vector3 l_vTouchPos((float)e_iPosX, (float)e_iPosY, 0.f);
		for (auto l_Iterator = l_CollisionPointIndexOfCurveAndTimeLerpHintMap.begin(); l_Iterator != l_CollisionPointIndexOfCurveAndTimeLerpHintMap.end(); ++l_Iterator)
		{
			Vector3 l_vPos = l_pCurve->GetPointList()[l_Iterator->first];
			float l_fDis = (l_vTouchPos - l_vPos).Length();
			if (l_fDis <= m_fRadiusForCollision)
			{//show mouse positio and sphere position for debug.

				m_bColliede = true;
				return true;
			}
			m_bColliede = false;
			//cGameApp::ShowInfoOnScreen(L"Missing");
		}
	}
	return false;
}

bool cSpecialTimingCurveClickObject::MouseDownFunction(int e_iPosX, int e_iPosY)
{
	cGameApp::ShowInfoOnScreen(L"Hitted down");
	return false;
}

bool cSpecialTimingCurveClickObject::MouseHorverFunction(int e_iPosX, int e_iPosY)
{
	return false;
}

bool cSpecialTimingCurveClickObject::MouseUpFunction(int e_iPosX, int e_iPosY)
{
	cGameApp::ShowInfoOnScreen(L"Hitted up");
	return false;
}

bool cSpecialTimingCurveClickObject::MouseLeaveFunction(int e_iPosX, int e_iPosY)
{
	return false;
}