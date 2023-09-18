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

};