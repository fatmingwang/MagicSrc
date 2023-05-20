#pragma once
//
#include "PlayerBehaviorBase.h"

class	cPlayerManager;

class	cPlayerData : public cPlayerBehaviorBase,public cMessageSender
{
	friend class				cPlayerManager;
	//
	void						ProcessPlayer( TiXmlElement* e_pElement );
	void						ProcessPlayerData( TiXmlElement* e_pElement );
	void						ProcessWeapon( TiXmlElement* e_pElement );
	void						ProcessPlayerIO( TiXmlElement* e_pElement );
private:
	float						m_fMovingSpeed;

	bool						m_bAutoPlay;
	bool						m_bIsEnergyPower;

	float						m_fLevelClickRepeatTime;
	float						m_fIsLevelKeyDownTC;
	float						m_fChangeLevelClickRepeatTime;

	UT::sTimeCounter			m_AutoFireTC;
	float						m_fAutoFireTime;
	//
	eDirection					m_eMouseEventDireciton;
	bool						ProcessEvent(int e_ID, NamedTypedObject* e_pSender, void* e_pData);
	void						KeyUpdate(float e_fElpaseTime);
	void						ShootBullet(float e_fAngleOffset = 0,Vector3 e_vOffsetPos = Vector3::Zero);

public:
	//
	cPlayerData( TiXmlElement* e_pElement );
	virtual ~cPlayerData();

	
	unsigned char				m_cInput_Right;
	unsigned char				m_cInput_Left;
	unsigned char				m_cInput_Down;
	unsigned char				m_cInput_Up;
	unsigned char				m_cInput_Level;

	cClickMouseBehavior			m_FireClick;
	cClickMouseBehavior			m_LevelClick;

	virtual	void				InternalRender();
	virtual	void				InternalInit();
	virtual	void				InternalUpdate(float e_fElpaseTime);
	virtual	void				Destroy();
	//
	virtual	void				AddScore( cMonster* Fish, int Money );

	void						SetEnergyPower(bool e_bIsEnergyPower,int e_iMultiplier);
	bool						IsEnergyPower(){return m_bIsEnergyPower;}
	void						AutoPlayUpdate(float e_fElpaseTime);

	cMatrix44					GetBulletPos();
	//while game start player's weapon level not we expect so adjust it,
	void						WeaponLVImageCheck();
	//
	void						MouseDown(int e_iPosX, int e_iPosY);
	void						MouseMove(int e_iPosX, int e_iPosY);
	void						MouseUp(int e_iPosX, int e_iPosY);
};