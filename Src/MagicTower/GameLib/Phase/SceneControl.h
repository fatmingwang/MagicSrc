#pragma once

#include "../Level/StageData.h"
#include "WalkingInMapPhase.h"

class	cSceneControl:public cPhaseManager
{
	cImageParser			m_ImageParser;

	//virtual	void	InternalRender(){}
	//virtual	void	InternalUpdate(float e_fElpaseTime){}
	cGameData							m_GameData;
	cNamedTypedObjectVector<cStoreyData>		m_AllStoreyData;;
public:
	cSceneControl();
	~cSceneControl();
	virtual	bool	Load();
	void	KeyDown(char e_char);
};