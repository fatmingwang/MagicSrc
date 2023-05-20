#include "stdafx.h"
#include "PlayerData.h"
#ifdef WIN32
#include <direct.h>
#endif
#include "../GameBullet/Bullet.h"
#include "../GameApp/GameApp.h"
#include "../GameBullet/BulletManager.h"
#include "../Monster/MonsterManager.h"
#include "../ProbabilityFish/FishProbability.h"
#include "Weapon.h"
#include "../FileNameDefine.h"
#include "../FTCGameEventList.h"

class cMovindDirectionRule
{
	unsigned char	m_DirectionKey[eDirection::eD_MAX];
public:
	void	ProcessData(TiXmlElement* e_pElement)
	{
		PARSE_ELEMENT_START(e_pElement)
			COMPARE_NAME("Right")
			{
				m_DirectionKey[eDirection::eD_RIGHT] = (unsigned char)(l_strValue[0]);
			}
			else
			COMPARE_NAME("Left")
			{
				m_DirectionKey[eDirection::eD_LEFT] = (unsigned char)(l_strValue[0]);
			}
			else
			COMPARE_NAME("Up")
			{
				m_DirectionKey[eDirection::eD_LEFT_UP] = (unsigned char)(l_strValue[0]);
			}
			else
			COMPARE_NAME("Down")
			{
				m_DirectionKey[eDirection::eD_DOWN] = (unsigned char)(l_strValue[0]);
			}
		PARSE_NAME_VALUE_END
	}
	static eDirection m_eCurrentDirection;
	void KeyDown(unsigned char e_ucKey)
	{
		//for (int  l_iDirection = eDirection::eD_LEFT;
		//	l_iDirection < eDirection::eD_MAX; ++l_iDirection)
		//{
		//	if (e_ucKey == m_DirectionKey[l_iDirection])
		//	{
		//		m_eCurrentDirection = (eDirection)i;
		//		break;
		//	}
		//}
	}

	static void	Update(float e_fElpaseTime,
		Frame*e_pPlayerFrame,
		Vector4 e_vViewRect,
		eDirection&e_eDirection,
		float e_fMovingSpeed)
	{
		auto l_vPos = e_pPlayerFrame->GetWorldPosition();
		float l_fMovingDis = e_fMovingSpeed* e_fElpaseTime;
		switch (e_eDirection)
		{
			case eDirection::eD_LEFT:
				l_vPos.x -= l_fMovingDis;
				break;
			case eDirection::eD_RIGHT:
				l_vPos.x += l_fMovingDis;
				break;
			case eDirection::eD_UP:
				l_vPos.y -= l_fMovingDis;
				break;
			case eDirection::eD_DOWN:
				l_vPos.y += l_fMovingDis;
				break;
		}
		//e_fMovingSpeed
		Vector4 l_vWallDistance(
			l_vPos.x - e_vViewRect.x,
			l_vPos.y - e_vViewRect.y,
			e_vViewRect.z - l_vPos.x,
			e_vViewRect.w - l_vPos.y);
		bool l_bCloseUp = l_vWallDistance.y > l_vWallDistance.w ? false : true;
		bool l_bCloseLeft = l_vWallDistance.x > l_vWallDistance.z ? false : true;
		if (l_vPos.x < e_vViewRect.x)
		{
			l_bCloseUp ? 
				e_eDirection = eDirection::eD_DOWN : 
				e_eDirection = eDirection::eD_UP;
			l_vPos.x = e_vViewRect.x;
		}
		else
		if (l_vPos.y < e_vViewRect.y)
		{
			l_bCloseLeft ?
				e_eDirection = eDirection::eD_RIGHT :
				e_eDirection = eDirection::eD_LEFT;
			l_vPos.y = e_vViewRect.y;
		}
		else
		if (l_vPos.x > e_vViewRect.z)
		{
			l_bCloseUp ?
				e_eDirection = eDirection::eD_DOWN :
				e_eDirection = eDirection::eD_UP;
			l_vPos.x = e_vViewRect.z;
		}
		else
		if (l_vPos.y > e_vViewRect.w)
		{
			l_bCloseLeft ?
				e_eDirection = eDirection::eD_RIGHT :
				e_eDirection = eDirection::eD_LEFT;
			l_vPos.y = e_vViewRect.w;
		}
		e_pPlayerFrame->SetWorldPosition(l_vPos);
	}

};

eDirection cMovindDirectionRule::m_eCurrentDirection = eDirection::eD_RIGHT;



cPlayerData::cPlayerData( TiXmlElement*e_pElement )
{
	m_eMouseEventDireciton = eDirection::eD_MAX;
	m_fMovingSpeed = 250.f;
	m_bAutoPlay = false;
	m_fIsLevelKeyDownTC =0.0f;
	m_fLevelClickRepeatTime =0.f;
	m_fChangeLevelClickRepeatTime = 0.f;
	m_bIsEnergyPower =false;
	
	m_fAutoFireTime = 0.0f;
	ProcessPlayer(e_pElement);
	REG_DELEGATE(eFTC_GameEventList::eFTC_GEL_GESTURE, &cPlayerData::ProcessEvent);
	REG_DELEGATE(eFTC_GameEventList::eFTC_GEL_FIRE_BULLET, &cPlayerData::ProcessEvent);
	
}
cPlayerData::~cPlayerData()
{
	SAFE_DELETE(m_pWeapon);
}

void cPlayerData::AddScore( cMonster* e_pFish, int Money )
{	
	m_i64Score += Money;

	if ( !e_pFish)
		return;
}

void cPlayerData::ProcessPlayer( TiXmlElement*e_pElement )
{

	TiXmlElement*l_pTiXmlElement = e_pElement->FirstChildElement();
	const wchar_t*l_strName = l_pTiXmlElement->Value();
	while(l_pTiXmlElement)
	{
		l_strName = l_pTiXmlElement->Value();
		COMPARE_NAME("PlayerData")
		{
			this->ProcessPlayerData(l_pTiXmlElement);
		}
		else
		COMPARE_NAME("IO")
		{
			this->ProcessPlayerIO(l_pTiXmlElement);
		}
		else
		COMPARE_NAME("Weapon")
		{
			this->ProcessWeapon(l_pTiXmlElement);
		}
		else
		COMPARE_NAME("PlayerBody")
		{
		}
		l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
	}
}
void cPlayerData::ProcessWeapon( TiXmlElement*e_pElement )
{
	this->m_pWeapon = new cFishPlayerWeapon(e_pElement,this);
}

void cPlayerData::ProcessPlayerData( TiXmlElement*e_pElement )
{
	PARSE_ELEMENT_START(e_pElement)
		COMPARE_NAME("Position")
		{
			m_vPlayerPos = VALUE_TO_VECTOR3;
		}
		else
		COMPARE_NAME("Rotation")
		{
			m_vPlayerRot = VALUE_TO_VECTOR3;
		}
	PARSE_NAME_VALUE_END
	this->SetLocalTransform( cMatrix44::TranslationMatrix(m_vPlayerPos)*cMatrix44::RotationMatrix(m_vPlayerRot));
}	








void cPlayerData::ProcessPlayerIO( TiXmlElement*e_pElement )
{
	PARSE_ELEMENT_START(e_pElement)
		COMPARE_NAME("Right")
		{
			this->m_cInput_Right= (unsigned char)(l_strValue[0]);
		}
		else
		COMPARE_NAME("Left")
		{
			this->m_cInput_Left= (unsigned char)(l_strValue[0]);
		}
		else
		COMPARE_NAME("Up")
		{
			this->m_cInput_Up= (unsigned char)(l_strValue[0]);
		}
		else
		COMPARE_NAME("Down")
		{
			this->m_cInput_Down= (unsigned char)(l_strValue[0]);
		}
		else
		COMPARE_NAME("Fire")
		{
			m_FireClick.SetKeyData((unsigned char)(l_strValue[0]));
			m_FireClick.SetPressedRepeatMouseUp(true);
			m_FireClick.SetRepeatTime(0.4f);
		}
		else
		COMPARE_NAME("Level")
		{
			this->m_cInput_Level = (unsigned char)(l_strValue[0]);
			m_LevelClick.SetKeyData(m_cInput_Level);
			m_LevelClick.SetPressedRepeatMouseUp(true);
		}
		else
		COMPARE_NAME("FireRepeat")
		{
			m_FireClick.SetRepeatTime(VALUE_TO_FLOAT);
			m_fAutoFireTime = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("LevelUpRepeat")
		{
			m_fLevelClickRepeatTime = VALUE_TO_FLOAT;
			m_fChangeLevelClickRepeatTime =m_fLevelClickRepeatTime;
			m_LevelClick.SetRepeatTime(m_fChangeLevelClickRepeatTime);
		}
		else
		COMPARE_NAME("MovingSpeed")
		{
			m_fMovingSpeed = VALUE_TO_FLOAT;
		}
	PARSE_NAME_VALUE_END
#ifdef WASM
		if (m_iPlayerID == 0)
		{
			this->m_cInput_Left = 'a';
			//this->m_cInput_Down = 's';
			this->m_cInput_Right = 'd';
			m_FireClick.SetKeyData((unsigned char)('f'));
			this->m_cInput_Level = (unsigned char)('g');
			m_LevelClick.SetKeyData(m_cInput_Level);
	}
#endif
		
}

void cPlayerData::InternalInit()
{	
	m_FireClick.Init();
	m_LevelClick.Init();
	m_AutoFireTC.SetTargetTime(m_fAutoFireTime);
	m_AutoFireTC.Start();
}








void cPlayerData::InternalUpdate( float e_fElpaseTime )
{
	if(!m_bCoinNotEnough)
	{
		KeyUpdate(e_fElpaseTime);
	}


	if(m_bAutoPlay)
	{
		AutoPlayUpdate(e_fElpaseTime);
	}
}








void cPlayerData::InternalRender()
{
}

void cPlayerData::Destroy()
{

}


bool cPlayerData::ProcessEvent(int e_ID,NamedTypedObject*e_pSender, void*e_pData)
{
	if (e_ID == eFTC_GEL_FIRE_BULLET)
	{
		auto l_iValue = ::rand()%3;
		if (l_iValue == 0)
		{
			ShootBullet(0);
		}
		else
		if (l_iValue == 1)
		{
			ShootBullet(0);
			ShootBullet(30);
			ShootBullet(-30);
		}
		else
		{
			ShootBullet(0, Vector3(-50, 10, 0));
			ShootBullet(0, Vector3(  0, 10, 0));
			ShootBullet(0, Vector3( 50, 10, 0));
		}
	}
	else
	if (e_ID == eFTC_GEL_GESTURE)
	{
		eDirection l_Direction = *(eDirection*)e_pData;
		m_eMouseEventDireciton = l_Direction;
		float l_fAngle = 0.f;
		switch (l_Direction)
		{
		case eDirection::eD_LEFT:
			this->SetLocalTransform(cMatrix44::TranslationMatrix(this->GetLocalPosition())*cMatrix44::ZAxisRotationMatrix(0));
			break;
		case eDirection::eD_RIGHT:
			l_fAngle = 180.f;
			this->SetLocalTransform(cMatrix44::TranslationMatrix(this->GetLocalPosition()) * cMatrix44::ZAxisRotationMatrix(D3DXToRadian(l_fAngle)));
			break;
		case eDirection::eD_UP:
			l_fAngle = 90.f;
			this->SetLocalTransform(cMatrix44::TranslationMatrix(this->GetLocalPosition()) * cMatrix44::ZAxisRotationMatrix(D3DXToRadian(l_fAngle)));
			l_fAngle = 270.f;
			break;
		case eDirection::eD_DOWN:
			l_fAngle = 270.f;
			this->SetLocalTransform(cMatrix44::TranslationMatrix(this->GetLocalPosition()) * cMatrix44::ZAxisRotationMatrix(D3DXToRadian(l_fAngle)));
			l_fAngle = 90.f;
			break;
		}
		((cFishPlayerWeapon*)m_pWeapon)->SetAngle(l_fAngle);
	}
	return true;
}

void	cPlayerData::KeyUpdate(float e_fElpaseTime)
{
	eDirection	&l_eDirection = cMovindDirectionRule::m_eCurrentDirection;
	Vector3 l_vPos = Vector3::Zero;
	//float l_fMovingSpeed = m_fMovingSpeed* e_fElpaseTime;
	if( cGameApp::m_sucKeyData[this->m_cInput_Left])
	{
		l_eDirection = eD_LEFT;
		//l_vPos.x -= l_fMovingSpeed;
	}
	else if( cGameApp::m_sucKeyData[this->m_cInput_Right])
	{
		l_eDirection = eD_RIGHT;
		//l_vPos.x += l_fMovingSpeed;
	}
	else if ( cGameApp::m_sucKeyData[ m_cInput_Up ] )
	{
		l_eDirection = eD_UP;
		//l_vPos.y -= l_fMovingSpeed;
	}
	else if ( cGameApp::m_sucKeyData[ m_cInput_Down ] )
	{
		l_eDirection = eD_DOWN;
		//l_vPos.y += l_fMovingSpeed;
	}

	if (m_eMouseEventDireciton != eDirection::eD_MAX)
	{
		l_eDirection = m_eMouseEventDireciton;
		m_eMouseEventDireciton = eDirection::eD_MAX;
	}

	cMovindDirectionRule::Update(e_fElpaseTime, this, Vector4(0, 0, 720, 1280), l_eDirection, m_fMovingSpeed);

	//if (l_eDirection != eD_MAX)
	//{
	//	auto l_vPlayerPos = this->GetLocalPosition();
	//	this->SetLocalPosition(l_vPlayerPos + l_vPos);
	//}

	m_FireClick.SingnalProcess();
	m_FireClick.Update(e_fElpaseTime);
	//if( m_FireClick.IsSatisfiedCondition() && !g_pIOSMDriverInterface->IsIOError() )
	if (m_FireClick.IsSatisfiedCondition())
	{
		ShootBullet(0);
	}
	//Level Key
	if ( cGameApp::m_sucKeyData[ m_cInput_Level ] )
	{
		m_fIsLevelKeyDownTC+=e_fElpaseTime;
	}
	else
	{
		m_fIsLevelKeyDownTC =0.f;

	}
	m_LevelClick.SingnalProcess();
	m_LevelClick.Update(e_fElpaseTime);
	if(m_LevelClick.IsSatisfiedCondition())
	{
		float	l_fReduceTimne = 0.03f;
		if(m_fIsLevelKeyDownTC>=1.0f)
		{
			m_fChangeLevelClickRepeatTime -= l_fReduceTimne;
		}
		else
		{
			m_fChangeLevelClickRepeatTime =m_fLevelClickRepeatTime;
		}
		if(m_fChangeLevelClickRepeatTime <= 0.1f)
		{
			m_fChangeLevelClickRepeatTime = 0.1f;
		}


		m_LevelClick.SetRepeatTime(m_fChangeLevelClickRepeatTime);
		m_LevelClick.Init();;

		if( m_pWeapon )
			m_pWeapon->LevelUp(true);
	}
	//Level Key End

	if( m_pWeapon )
		m_pWeapon->StatusChange(l_eDirection);
}

void	cPlayerData::ShootBullet(float e_fAngle, Vector3 e_vOffsetPos)
{
	m_FireClick.Init();
	if (m_i64Score >= m_iMinScoreBet)
	{
		int	l_iPayrate = this->m_pWeapon->GetPower();
		if (m_i64Score < l_iPayrate)
		{
			l_iPayrate = (int)m_i64Score;
		}

		//if (cFishApp::m_spMonsterManager->GetCountMonsterVisible() > 0 && m_pWeapon && !m_pWeapon->IsFire())
		{
			m_pWeapon->Fire(this, e_fAngle, e_vOffsetPos);
			this->m_i64Score -= l_iPayrate;
			cFishApp::m_spProbabilityFish->RevenueAdd(l_iPayrate, GetPlayerID());
			//cGameApp::SoundPlay(L"3", true);
		}
	}
}

void cPlayerData::AutoPlayUpdate(float e_fElpaseTime)
{
	m_AutoFireTC.Update(e_fElpaseTime);
	if(m_AutoFireTC.bTragetTimrReached)
	{
		int	l_iPayrate =0;
		if(m_i64Score>this->m_pWeapon->GetPower())
		{
			l_iPayrate = this->m_pWeapon->GetPower();
		}
		else
		{
			l_iPayrate = (int)m_i64Score;
		}
		if(cFishApp::m_spMonsterManager->GetCountMonsterVisible()>0 && m_pWeapon && !m_pWeapon->IsFire() && m_i64Score >= m_iMinScoreBet)
		{
			((cFishPlayerWeapon*)m_pWeapon)->SetWeaponAnimationTime( rand(0.0f,1.0f) );
			((cFishPlayerWeapon*)m_pWeapon)->AngleUpdate();

			m_pWeapon->Fire(this);
			this->m_i64Score -= l_iPayrate;
			cFishApp::m_spProbabilityFish->RevenueAdd( l_iPayrate, GetPlayerID());
			//cGameApp::SoundPlay(L"3",true);
		}
		m_AutoFireTC.SetTargetTime(m_fAutoFireTime);
		m_AutoFireTC.Start();	
	}
}

void cPlayerData::SetEnergyPower(bool e_bIsEnergyPower,int e_iMultiplier)
{
	m_bIsEnergyPower = e_bIsEnergyPower;
	m_iPayRateMultier = e_iMultiplier;
	if( m_pWeapon )
		m_pWeapon->SetEnergyTube(e_bIsEnergyPower);
}



cMatrix44 cPlayerData::GetBulletPos()
{
	Vector3 l_vTubeOffsetPos,l_vTubeRotation;
	l_vTubeOffsetPos =( (cFishPlayerWeapon*)m_pWeapon )->GetTubeOffsetPos();
	l_vTubeRotation = Vector3( 0,0,-(  ( (cFishPlayerWeapon*)m_pWeapon )->GetTubeAngle()+270 ) );
	return cMatrix44::TranslationMatrix( l_vTubeOffsetPos ) * cMatrix44::RotationMatrix( l_vTubeRotation );
}

void	cPlayerData::WeaponLVImageCheck()
{
	//for(int i=0; i < m_iLevelCount ; ++i)
	//{
	//	if(this->m_iLevelNumber>=this->m_vLevelMin[i] && this->m_iLevelNumber<=this->m_vLevelMax[i])
	//	{
	//		this->m_iLevel = this->m_vLevelIndex[i];
	//	}
	//}
	//std::wstring	l_str = UT::ComposeMsgByFormat( L"TubePos%d", (this->m_iLevel+1) );
	//int l_iIndexPlayerUI = m_pPlayerImage->GetPuzzleImage( L"PlayerUI" )->GetObjectIndexByName( l_str.c_str() );
	//m_pPlayerImage->GetObject(L"TubePos")->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
	//m_pPlayerImage->GetObject(L"TubePos")->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;
	//m_pPlayerImage->UpdateByGlobalTime(m_fTubeTime+0.1f);
}

void cPlayerData::MouseDown(int e_iPosX, int e_iPosY)
{
}

void cPlayerData::MouseMove(int e_iPosX, int e_iPosY)
{
}

void cPlayerData::MouseUp(int e_iPosX, int e_iPosY)
{
}
