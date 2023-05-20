#include "stdafx.h"
#include "WeaponSelection.h"

bool cWeaponSelection::ProcessEvent(int e_ID, NamedTypedObject* e_pSender, void* e_pData)
{
    int l_iValue = *(int*)e_pData;
    m_eShootBulletType = (eShootBulletType)UT::GetLoopIndex(m_eShootBulletType + l_iValue, eSBT_MAX);
    return false;
}
