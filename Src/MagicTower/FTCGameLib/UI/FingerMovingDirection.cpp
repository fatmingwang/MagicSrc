#include "stdafx.h"
#include "FingerMovingDirection.h"
#include "../FTCGameEventList.h"


cFingerMovingDirection::cFingerMovingDirection()
{
}

cFingerMovingDirection::~cFingerMovingDirection()
{
}

cClickBehavior* cFingerMovingDirection::MouseUp(int e_iPosX, int e_iPosY)
{
	auto l_Clicked = cClickBehavior::MouseUp(e_iPosX, e_iPosY);
	if (l_Clicked)
	{
		eDirection l_MovingDirection = eDirection::eD_MAX;
		auto l_vDownUpDis = -m_MouseMoveData.DownUp();
		if (l_vDownUpDis.Length() < m_fMinAllowToMoveDis)
		{
			cCommonApp::EventMessageShot(eFTC_GameEventList::eFTC_GEL_FIRE_BULLET,nullptr);
			
			return l_Clicked;
		}
		Vector3 l_MovedDirection = l_vDownUpDis.Normalize();
		std::string l_strDirection;
		if (abs(l_MovedDirection.x) > abs(l_MovedDirection.y))
		{
			if (l_MovedDirection.x > 0)
			{
				l_MovingDirection = eDirection::eD_RIGHT;
				l_strDirection = "Right";
			}
			else
			{
				l_MovingDirection = eDirection::eD_LEFT;
				l_strDirection = "Left";
			}
		}
		else
		{
			if (l_MovedDirection.y < 0)
			{
				l_MovingDirection = eDirection::eD_UP;
				l_strDirection = "Up";
			}
			else
			{
				l_MovingDirection = eDirection::eD_DOWN;
				l_strDirection = "Down";
			}
		}
		if (l_MovingDirection != eDirection::eD_MAX)
		{
			FMLOG("MovingDirection is %s", l_strDirection.c_str());
			cCommonApp::EventMessageShot(eFTC_GameEventList::eFTC_GEL_GESTURE,&l_MovingDirection,sizeof(l_MovingDirection));
		}
	}
	return l_Clicked;
}
bool cFingerMovingDirection::Collide(int e_iPosX, int e_iPosY)
{
    if (this->m_bEnable)
    {
        return true;
    }
    return false;
}