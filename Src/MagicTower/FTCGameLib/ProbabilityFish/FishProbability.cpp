#include "stdafx.h"
#include "FishProbability.h"
#include "../GameApp/GameApp.h"
#include "../Monster/MonsterManager.h"
#include "../GameBullet/Bullet.h"
#include "../PlayerBehavior/PlayerData.h"
#include "../FileNameDefine.h"

cProbabilityFish::cProbabilityFish()
{
	m_iMaxScoreInOneFire = 0;
	m_fMachinRunTime = 0.f;
	m_fSafeToGiveProbabilityOffset = 0.01f;
	//
	//m_pProbability_MasterLee = 0;

}

cProbabilityFish::~cProbabilityFish()
{
	//SAFE_DELETE( m_pProbability_MasterLee );

}

bool	cProbabilityFish::MyParse( TiXmlElement *e_pRoot )
{
	return true;
}

void	cProbabilityFish::Init()
{
	std::string	l_strLogFile = "Log/FishProbability";
	l_strLogFile += UT::WcharToChar(UT::GetSystemTimeForFile(true).c_str());
	l_strLogFile += ".txt";
	m_fSafeToGiveProbabilityOffset = 0.01f;
	if(!ParseWithMyParse(FISH_PROBABILITY))
	{
		UT::ErrorMsg(L"FISH_PROBABILITY",L"parse failed");
	}
	//
	//if( cFishApp::m_sbIsUsingMasterLeeProbability )
	//{
	//	SAFE_DELETE( m_pProbability_MasterLee );
	//	m_pProbability_MasterLee = new cProbability_MasterLee();
	//	m_pProbability_MasterLee->Initial();
	//}
}

void	cProbabilityFish::Update(float e_fElpaseTime)
{
	m_fMachinRunTime += e_fElpaseTime;
	if( cFishApp::m_spMonsterManager)
	{
		//if( cFishApp::m_sbIsUsingMasterLeeProbability )
		//{
		//	m_pProbability_MasterLee->Update( e_fElpaseTime );
		//}
	}

}

void	cProbabilityFish::Render()
{
}

void	cProbabilityFish::DebugRender()
{

}

void	cProbabilityFish::RevenueMinus( int e_iMoney, int e_iPlayerID)
{
}

void	cProbabilityFish::RevenueAdd( int e_iMoney, int e_iPlayerID )
{
	if(e_iMoney == 0)
		return;
	if( !cFishApp::m_sbIsUsingMasterLeeProbability )
	{
		float l_fMiniGameStoreMoney = 0;
		if( l_fMiniGameStoreMoney -(int) l_fMiniGameStoreMoney >0 )
		{
			l_fMiniGameStoreMoney += 1.f;
		}
	}
}

void	cProbabilityFish::WrtiePlayerMoney(float e_fElpaseTime)
{	
}
void	ClearLatestRecord(bool e_bClearPlayerMoney)
{

}

void	cProbabilityFish::ClearLatestRecord(bool e_bClearPlayerMoney)
{

}

void	cProbabilityFish::ExchangeIn( int64 e_i64Value )
{

}

void	cProbabilityFish::ExchangeOut( int64 e_i64Value )
{

}

void	cProbabilityFish::ToastCoin( int64 e_i64Value )
{

}

void	cProbabilityFish::RefundCoin( int64 e_i64Value )
{

}

extern bool g_bEasyMonsterDied;

bool cProbabilityFish::MonsterToDied( sHittedFishData *e_pHittedFishData, int e_iOutMoney, bool e_bForceToDied, bool e_bMasterLi )
{
	if( e_iOutMoney == 0 )
	{
		int a=0;
	}
	if( !e_bForceToDied && !e_pHittedFishData->m_pMonster->IsStatusAllowToDied() )
	{
		return false;
	}
	//
	e_pHittedFishData->m_pPlayer->AddScore( e_pHittedFishData->m_pMonster , e_iOutMoney );
	e_pHittedFishData->m_pMonster->SetMonsterStatus( eMS_DIED_SHOW) ;
	e_pHittedFishData->m_pMonster->m_pHittedPlayer = e_pHittedFishData->m_pPlayer;
	e_pHittedFishData->m_pMonster->m_pBullet = e_pHittedFishData->m_pBullet;
	RevenueMinus( e_iOutMoney, e_pHittedFishData->m_pPlayer->m_iPlayerID);
	
	if( e_pHittedFishData->m_pMonster->IsMiniGameMonster() )
	{
		int	l_iPayrate = e_iOutMoney/e_pHittedFishData->m_pBullet->GetPayRate();
	}

	if( m_iMaxScoreInOneFire < e_iOutMoney )
	{
		m_iMaxScoreInOneFire = e_iOutMoney;
	}
	return true;
}


//1280:720 = 17 :10,but 27 is not even so make it as 26
//const int			g_iTotalDirectionOrder = 26;//17+10
//const int			g_iDirectionOrder[26] = {0,1,2,3,1,3,//6
//											 0,1,2,3,1,3,//6
//											 0,1,2,3,1,	 //5
//											 0,1,2,3,3,	 //5
//											 0,1,2,3};	 //4

const int			g_iTotalDirectionOrder = 8;
//const int			g_iDirectionOrder[12] = {0,2,0,2,0,2,1,3,1,3,1,3};	 //8
//const int			g_iDirectionOrder[12] = {1,3,1,3,1,3,1,3,1,3,1,3};	 //8
const int			g_iDirectionOrder[8] = {2,2,2,1,0,0,0,3};	 //8

UINT				g_suiDirection = 0;








Vector3		cProbabilityFish::GetRandomPos( float e_fRadius )
{
	sMinMaxData<float>	l_fPosX;
	sMinMaxData<float>	l_fPosY;
	l_fPosX.Max = cGameApp::m_spOpenGLRender->m_vGameResolution.x;		l_fPosX.Min = 0.f;
	l_fPosY.Max = cGameApp::m_spOpenGLRender->m_vGameResolution.y;		l_fPosY.Min = 0.f;
	//
	++g_suiDirection ;
	//
	if( g_suiDirection >= g_iTotalDirectionOrder )
	{
		g_suiDirection = 0;
	}
	int	l_iDirection = g_iDirectionOrder[g_suiDirection];
	Vector3	l_vPos;

	switch( l_iDirection )
	{
	case 0://eD_LEFT
		l_vPos.x = -e_fRadius;
		l_vPos.y = l_fPosY.Rand();
		break;
	case 1://eD_UP
		l_vPos.x = l_fPosX.Rand();
		l_vPos.y = -e_fRadius;
		break;
	case 2://eD_RIGHT:
		l_vPos.x = cGameApp::m_spOpenGLRender->m_vGameResolution.x+e_fRadius;
		l_vPos.y = l_fPosY.Rand();
		break;
	case 3://eD_DOWN:
		l_vPos.x = l_fPosX.Rand();
		l_vPos.y = cGameApp::m_spOpenGLRender->m_vGameResolution.y+e_fRadius;
		break;
	} //end switch
	l_vPos.z = 0.f;
	return l_vPos;
}

Vector3		cProbabilityFish::GetRandomPos( cMonster*e_pMonster )
{
	return GetRandomPos( e_pMonster->m_fRadius );
}

void	cProbabilityFish::MonsterToKeepGoing( sHittedFishData*e_pHittedFishData, float e_fAddProbability )
{
	if( e_pHittedFishData->m_pMonster->m_fRestProbabilityValueToDied >= e_fAddProbability )
	{
		e_pHittedFishData->m_pMonster->m_fRestProbabilityValueToDied -= e_fAddProbability;
	}
	//e_pHittedFishData->m_pMonster->SetMonsterStatus(eMS_ALIVE);
}

float	cProbabilityFish::GetCurrentRealProbability()
{

	return 0.f;
}

float	cProbabilityFish::GetCurrentProbability()
{
	return 0.99f;
}
float	cProbabilityFish::GetTargetProbability()
{
	return 0.99f;
}
float	cProbabilityFish::GetOffsetProbability()
{
	float l_fTargetProbability = GetTargetProbability();
	float l_fProbability = GetCurrentRealProbability();
	if( l_fProbability < 0 )
	{
		return 0;
	}
	return l_fTargetProbability - l_fProbability;
}

int64	cProbabilityFish::GetLeatestInMoney()
{
	int64	l_i64Value = 100;
	if( l_i64Value < 0 )
	{
		l_i64Value = 0;
	}
	//avoid total in is zero
	return l_i64Value+1;
}

float	cProbabilityFish::GetNewProbailityByAddMoney(int e_iOutMoney)
{
	int64	l_iNewTotalOutMoney = 100;
	return (float)((double)l_iNewTotalOutMoney/(double)GetLeatestInMoney());
}

bool	cProbabilityFish::IsProbailitySafeToOut(float e_fProbaility)
{
	if(e_fProbaility<GetTargetProbability())
	{
		return true;
	}
	return false;
}