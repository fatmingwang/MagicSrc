#pragma once

#include "../Level/LevelData.h"
//#include "MapSceneRule.h"
//#include "VirtualGamePad.h"

class cWalkingInMapPhase:public cSimplePhase
{
	class cGridBehavior*			m_pMonsterGridBehavior;
	POINT							m_GridSize;
	class cVirtualGamePad*			m_pVirtualGamePad;
	class cMainRoleData*			m_pMainRoleData;
	cNamedTypedObjectVector<cStoreyData>	*m_pAllStoreyData;
	cStoreyData						*m_pCurrentStoreyData;
	class cMapSceneViewRule			*m_pMapSceneRule;
	//the scene we want to see,it could be scaled
	Vector2	m_vVisibleResoulution;
	GET_SET_DEC(cOrthogonalCamera,m_OrthogonalCamera,GetOrthogonalCamera,SetOrthogonalCamera);
	//while main character reached stair or hole or some teleportion it will set new storey and set character position
	void	ChangeFloor();
	GET_SET_DEC(bool,m_bWalkingModeIsFix,IsWalkingModeIsFix,SetWalkingModelToFix);
public:
	cWalkingInMapPhase(cMainRoleData*e_pMainRoleData,cNamedTypedObjectVector<cStoreyData>*e_pAllStoreyData,
		int e_iViewableRow,int e_iViewableColumn,POINT e_GridSize,POINT e_iOffsetMapPosIndex, const wchar_t*e_strCurrentStoreyName);
	~cWalkingInMapPhase();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Init();							//it would be called if the stage is changed.
	virtual	void	Render();
	virtual	void	DebugRender();
	virtual	void	MouseDown(int e_iX,int e_iY);
	virtual	void	MouseUp(int e_iX,int e_iY);
	virtual	void	MouseHover(int e_iX,int e_iY);
	void	KeyDown(char e_char);
	virtual	const   WCHAR*	GetNextPhaseName();
	virtual	void*	GetData();
};