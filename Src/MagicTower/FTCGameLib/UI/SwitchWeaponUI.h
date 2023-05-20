#pragma once



//first version add two arrows to change weapon.
//virtual bool	SetupMPDIAndButtonNameVector(cMPDI*e_pMPDI, std::vector<std::wstring>e_strButtonName, bool e_bKeepMPDIAsBG);
class cSwitchWeaponUI:public cMPDIToGameObject
{
	void	ArrowNext(int e_iPosX, int e_iPosY, cClickBehavior*e_pClickBehavior);
	void	ArrowPrevious(int e_iPosX, int e_iPosY, cClickBehavior* e_pClickBehavior);
public:
	cSwitchWeaponUI();
	virtual ~cSwitchWeaponUI();
};