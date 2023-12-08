#include "stdafx.h"
#include "../Battle/BattleAttackMoveObject.h"
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
	//m_pBattleAttackMoveObject = new cBattleAttackMoveObject(eBattleType::eBT_DEFENCE_1);
	//m_pBattleAttackMoveObject->Init();
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
	m_pBattleAttackMoveObject->Render();
}

void cBattleAttackMoveObjectTesting::KeyDown(char e_cKey)
{
}
