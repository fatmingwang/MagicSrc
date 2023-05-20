#pragma once

class	cMonster;
class	cPlayerWeapon;
class	cRefoundCoinDevice;
class	cProbabilityFish;
class	cFileSaver;
class	cPlayerBehaviorBase : public Frame
{
	//cPrtEmitter* cGameApp::GetPrtEmitter(const char* e_strFileName, const wchar_t* e_strName)
	cPrtEmitter*				m_PrtEmitter;
	friend class				cFileSaver;
	friend class				cPlayerManager;
	friend class				cRefoundCoinDevice;
	friend class				cProbabilityFish;
protected:
	UT::sTimeCounter			m_NoMoney_InsertCoinHintTC;	
	Vector3						m_vPlayerPos;
	Vector3						m_vPlayerRot;


	cPlayerWeapon*				m_pWeapon;
	int							m_iPayRateMultier;
	int							m_iPlayerID;
	int64						m_i64Score;
	//
	int64*						m_pScoreLimit;
	int							m_iMinScoreBet;
	int							m_iMaxLevelNumber;
	bool						m_bCoinNotEnough;
	bool						m_bRefundCoin;
	//

	virtual	void				InternalRender() = 0;
	virtual	void				InternalInit() = 0;
	virtual	void				InternalUpdate(float e_fElpaseTime) = 0;
	//
public :

	cPlayerBehaviorBase();
	virtual ~cPlayerBehaviorBase();
	//
	virtual	void				AddScore( cMonster* Fish, int Money ) = 0;
	int							GetPlayerID(){return m_iPlayerID;}
	int64						GetScore(){return m_i64Score;}
	virtual	void				Init();
	virtual	void				Update( float e_fElpaseTime );
	virtual	void				Render();
	cPlayerWeapon*				GetWeapon(){return m_pWeapon;}
	//int						GetCurrentLevel();
	Vector3						GetPlayerRotation(){return m_vPlayerRot;}
	int							GetPayRateMultier(){return m_iPayRateMultier;}
	void						SetPayRateMultier(int e_iPayRateMultier){m_iPayRateMultier = e_iPayRateMultier;}
	//
};
//================================================================
//
//
//
//================================================================
class	cPlayerManager : public cNamedTypedObjectVector<cPlayerBehaviorBase>
{
private:
	cClickMouseBehavior		m_AutoPlay;
public:
	int						m_iPlayerCount;
	cPlayerManager();
	virtual ~cPlayerManager();
	//
	void	Init();
	void	Update( float e_fElpaseTime );
	void	Render();
	void	Destroy();
};
//================================================================
//
//
//
//================================================================
class	cPlayerWeapon:public Frame
{
protected:
	eDirection		m_eMovingDirection;
	bool			m_bFire;
public:
	cPlayerWeapon();
	virtual ~cPlayerWeapon();
	virtual	void	Update(float e_fElpaseTime) = 0;
	virtual	void	Render() = 0;
	void			StatusChange(eDirection e_eDirection);
	virtual	bool	Fire(cPlayerBehaviorBase*e_pPlayerBehaviorBase, float e_fAngleOffset = 0, Vector3 e_vOffsetPos = Vector3::Zero) = 0;
	virtual	bool	IsFire() = 0;
	virtual	int		GetPower() = 0;
	virtual	void	SetPower(int e_iCurrentLevel) = 0;
	virtual void	SetEnergyTube(bool e_bIsSetEnergyTube) = 0;
	virtual	void	LevelUp(bool e_bLevelChange) = 0;

	virtual	void	Destroy(){}
};