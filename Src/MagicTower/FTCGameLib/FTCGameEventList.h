#pragma once


enum eFTC_GameEventList
{
	//cFingerMovingDirection::MouseUp=>eDirection
	eFTC_GEL_GESTURE = 0,
	eFTC_GEL_FIRE_BULLET,//
	eFTC_GEL_SELECT_WEAPON,//-1 for pervious 1 for next
	eFTC_GEL_MAX
};