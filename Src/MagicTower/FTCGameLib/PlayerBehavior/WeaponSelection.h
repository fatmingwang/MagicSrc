#pragma once

enum eShootBulletType
{
	eSBT_ONE_STRIGHT = 0,
	eSBT_THREE_BULLETS,
	eSBT_CIRCLE_SHOOT,
	eSBT_MAX
};


class cWeaponSelection:public cMessageSender
{
	bool				ProcessEvent(int e_ID, NamedTypedObject* e_pSender, void* e_pData);
	eShootBulletType	m_eShootBulletType = eSBT_ONE_STRIGHT;
public:
	cWeaponSelection();
	~cWeaponSelection();
};