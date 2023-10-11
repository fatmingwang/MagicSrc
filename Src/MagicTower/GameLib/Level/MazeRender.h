#pragma once
#include "cyucelenMaze/grid.h"

//	SetLocalPosition(Vector3(150,50,0));
class cMazeRender :public cCyucelenMazeGrid,public Frame
{
private:
	struct sBatchData*	m_pBatchData;
	cBaseImage*			m_pWallImae;
public:
	cMazeRender(int e_iWidth, int e_iHeight);
	virtual ~cMazeRender();
	void	GenRandomMap();
	void	Render();
	void	DebugRender(bool e_bDoStrip);
	void	GenerateMaze(int e_iStep = -1);

	bool					IsMovable(int e_iNowPosX, int e_iNowPosY,eDirection e_eDirection);
	bool					IsMovable(int e_iNowPosX, int e_iNowPosY,int e_iTargetPosX, int e_iTargetPosY);
	bool					NextJunction(int e_iNowPosX, int e_iNowPosY,eDirection e_eDirection,int&e_iPosX, int& e_iPosY);
	std::vector<eDirection>	GetMovableDirection(int e_iNowPosX, int e_iNowPosY);
	bool					GetExitPoint(int& e_iPosX, int& e_iPosY);
};