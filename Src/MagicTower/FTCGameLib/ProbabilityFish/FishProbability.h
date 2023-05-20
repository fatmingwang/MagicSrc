#pragma once
//
class	cMonster;
class	cBullet;
class	cPlayerBehaviorBase;
struct	sHittedFishData;
class	cFileSaver;
class	cProbabilityFish: public cNodeISAX
{
	virtual	bool						MyParse(TiXmlElement*e_pRoot);
private:
	//
	float								m_fMachinRunTime;
	int									m_iMaxScoreInOneFire;
public:
	bool								MonsterToDied( sHittedFishData *e_pHittedFishData, int e_iOutMoney, bool e_bForceToDied, bool e_bMasterLi );
	void								MonsterToKeepGoing( sHittedFishData *e_pHittedFishData, float e_fAddProbability );

	//cProbability_MasterLee				*m_pProbability_MasterLee;
	//
	float								m_fSafeToGiveProbabilityOffset;
	//
	cProbabilityFish();
	virtual ~cProbabilityFish();
	void								Init();
	void								Update( float e_fElpaseTime );
	void								Render();
	void								DebugRender();
	void								RevenueAdd( int e_iMoney, int e_iPlayerID );
	void								RevenueMinus( int e_iMoney, int e_iPlayerID);
	float								GetCurrentRealProbability();
	float								GetCurrentProbability();;
	float								GetTargetProbability();
	float								GetOffsetProbability();
	int64								GetLeatestInMoney();
	float								GetNewProbailityByAddMoney(int e_iOutMoney);
	bool								IsProbailitySafeToOut(float e_fProbaility);
	//
	Vector3								GetRandomPos( cMonster *e_pMonster );
	Vector3								GetRandomPos( float e_fRadius );
	//
	//for control panel
	void								ClearLatestRecord(bool e_bClearPlayerMoney);
	//
	//for player exchange and coin behavior.
	void								ExchangeIn( int64 e_i64Value );
	void								ExchangeOut( int64 e_i64Value );
	void								ToastCoin( int64 e_i64Value );
	void								RefundCoin( int64 e_i64Value );
	//
	void								WrtiePlayerMoney(float e_fElpaseTime);
};