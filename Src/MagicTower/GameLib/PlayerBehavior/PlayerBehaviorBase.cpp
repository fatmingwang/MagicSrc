#include "stdafx.h"
#include "PlayerBehaviorBase.h"
#include "PlayerData.h"
#include "../GameApp/GameApp.h"
#include "../FileNameDefine.h"

cPlayerBehaviorBase::cPlayerBehaviorBase()
{
	m_PrtEmitter = nullptr;// cGameApp::GetPrtEmitter("Fish/Particle/666.prt", L"Glyph3");
	//m_PrtEmitter = cGameApp::GetPrtEmitter("Fish/Particle/666.prt", L"Glyph3");
	m_iPayRateMultier = 1;
	m_pScoreLimit = 0;
	m_iMinScoreBet = 0;
	m_iMaxLevelNumber = 0;
	m_pWeapon = 0;
	m_i64Score = 9999999;
	m_iPlayerID = -1;
	m_bRefundCoin = false;;
	m_bCoinNotEnough = false;
	m_NoMoney_InsertCoinHintTC.SetTargetTime(5.f);

}

cPlayerBehaviorBase::~cPlayerBehaviorBase()
{
}

void	cPlayerBehaviorBase::Init()
{
	InternalInit();
	if (m_PrtEmitter)
	{
		m_PrtEmitter->Init();
		auto l_vPos = this->GetWorldPosition();
		m_PrtEmitter->SetPos(l_vPos);
		m_PrtEmitter->Emit();
	}
}

void	cPlayerBehaviorBase::Update(float e_fElpaseTime)
{
	if (m_PrtEmitter)
	{
		auto l_vPos = this->GetWorldPosition();
		m_PrtEmitter->SetPos(l_vPos);
		m_PrtEmitter->Update(e_fElpaseTime);
	}
	InternalUpdate(e_fElpaseTime);
	if( m_i64Score < m_iMinScoreBet )
	{
		m_NoMoney_InsertCoinHintTC.Update(e_fElpaseTime);
		if( m_NoMoney_InsertCoinHintTC.bTragetTimrReached )
		{

		}
	}
	else
	{
		m_NoMoney_InsertCoinHintTC.Start();
	}

	if(m_bCoinNotEnough)
	{

	}

	if( m_pWeapon )
		m_pWeapon->Update(e_fElpaseTime);
}

void	cPlayerBehaviorBase::Render()
{
	if (m_PrtEmitter)
	{
		m_PrtEmitter->Render();
	}
	InternalRender();

	if( m_pWeapon )
		m_pWeapon->Render();
}

//int	cPlayerBehaviorBase::GetCurrentLevel()
//{
//	return m_pWeapon->GetPower();
//}
//================================================================
//
//
//
//================================================================
cPlayerManager::cPlayerManager()
{
	cClickMouseBehavior::m_sfRepeatTime = 0.1f;
	m_iPlayerCount = 1;
	//m_bSwitchPlayerIOInPut = false;
	m_AutoPlay.SetKeyData(106);
}


cPlayerManager::~cPlayerManager()
{
	this->Destroy();
}

void	cPlayerManager::Init()
{	
	Destroy();
	m_iPlayerCount = 1;
	if(this ->Count()==0)
	{
		bool l_b;
		cNodeISAX	l_NodeISAX;
		std::string	l_strFileName = UT::ComposeMsgByFormat("%s%d.xml",PLAYER_DATA,m_iPlayerCount);
		l_b = l_NodeISAX.ParseDataIntoXMLNode(l_strFileName.c_str());
		if(l_b)
		{
			TiXmlElement*l_pElement = l_NodeISAX.GetRootElement();
			l_pElement = l_pElement->FirstChildElement();
			const wchar_t*l_strName = l_pElement->Value();
			while(l_pElement)
			{
				l_strName = l_pElement->Value();
				COMPARE_NAME("Player")
				{
					cPlayerData*playerData = new  cPlayerData(l_pElement);
					PARSE_ELEMENT_START(l_pElement)
					COMPARE_NAME("ID")
					{
						playerData->m_iPlayerID = VALUE_TO_INT;
					}
					PARSE_NAME_VALUE_END
					this->AddObjectNeglectExist(playerData);
				}
				l_pElement = l_pElement->NextSiblingElement();
			}
		}
		else
		{
			UT::ErrorMsg(L"PlayerData.xml",L"not exit");
		}

	}
	
	
	for(int i=0;i<this->Count();++i)
	{
		this->GetObject(i)->Init();
	}
	
}

void	cPlayerManager::Update( float e_fElpaseTime )
{
	if( cGameApp::m_sbDebugFunctionWorking )
	{
		m_AutoPlay.SingnalProcess();
		m_AutoPlay.Update(e_fElpaseTime);
		if( m_AutoPlay.IsSatisfiedCondition() )
		{
			m_AutoPlay.Init();
			for(int i=0;i<this->Count();++i)
			{
				cPlayerData*l_pPlayerData = (cPlayerData*)this->GetObject(i);
				l_pPlayerData->m_bAutoPlay = !l_pPlayerData->m_bAutoPlay;
			}
		}
	}
	for(int i=0;i<this->Count();++i)
	{
		this->GetObject(i)->Update(e_fElpaseTime);
	}
}

void	cPlayerManager::Render()
{
	for(int i=0;i<this->Count();++i)
	{
		this->GetObject(i)->Render();
	}
}

void	cPlayerManager::Destroy()
{
	for(int i=0;i<this->Count();++i)
	{
		//this->GetObject(i)->Destroy();
	}
}
//================================================================
//
//
//
//================================================================
cPlayerWeapon::cPlayerWeapon()
{
	m_eMovingDirection = eD_MAX;
	m_bFire = false;
}

cPlayerWeapon::~cPlayerWeapon()
{

}

void	cPlayerWeapon::StatusChange(eDirection e_eDirection)
{
	this->m_eMovingDirection = e_eDirection;
}