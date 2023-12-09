#include "stdafx.h"
#include "../Battle/BattleAttackMoveObject.h"
#include "../Object/MoveObject.h"
#include "GamePlayTesting.h"

TYPDE_DEFINE_MARCO(cBattleAttackMoveObjectTesting);

cBattleAttackMoveObjectTesting::cBattleAttackMoveObjectTesting()
{
	this->SetName(cBattleAttackMoveObjectTesting::TypeID);
}

cBattleAttackMoveObjectTesting::~cBattleAttackMoveObjectTesting()
{
	SAFE_DELETE(m_pBattleAttackMoveObject);
}

void cBattleAttackMoveObjectTesting::Init()
{
	m_pBattleAttackMoveObject = new cBattleAttackMoveObject(eBattleType::eBT_DEFENCE_1);
	m_pBattleAttackMoveObject->Init();
}

void cBattleAttackMoveObjectTesting::Update(float e_fElpaseTime)
{
	if (m_pBattleAttackMoveObject)
	{
		m_pBattleAttackMoveObject->Update(e_fElpaseTime);
	}
}

void cBattleAttackMoveObjectTesting::Render()
{
	if (m_pBattleAttackMoveObject)
	{
		m_pBattleAttackMoveObject->Render();
	}
}

void cBattleAttackMoveObjectTesting::KeyDown(char e_cKey)
{
}

void cBattleAttackMoveObjectTesting::MouseMove(int e_iPosX, int e_iPosY)
{
	if (m_pBattleAttackMoveObject)
	{
		//cSimplePhase::MouseMove(e_iPosX, e_iPosY);
		m_pBattleAttackMoveObject->m_pTwoMovingObjectController->MouseMove(e_iPosX,e_iPosY);
	}
}

void cBattleAttackMoveObjectTesting::MouseDown(int e_iPosX, int e_iPosY)
{
	//cSimplePhase::MouseDown(e_iPosX, e_iPosY);
	if (m_pBattleAttackMoveObject)
	{
		m_pBattleAttackMoveObject->m_pTwoMovingObjectController->MouseDown(e_iPosX, e_iPosY);
	}
}

void cBattleAttackMoveObjectTesting::MouseUp(int e_iPosX, int e_iPosY)
{
	//cSimplePhase::MouseUp(e_iPosX, e_iPosY);
	if (m_pBattleAttackMoveObject)
	{
		m_pBattleAttackMoveObject->m_pTwoMovingObjectController->MouseUp(e_iPosX, e_iPosY);
	}
}

void cBattleAttackMoveObjectTesting::InternalMouseLeave(int e_iPosX, int e_iPosY)
{
	if (m_pBattleAttackMoveObject)
	{
		m_pBattleAttackMoveObject->m_pTwoMovingObjectController->MouseLeave(e_iPosX, e_iPosY);
	}
}