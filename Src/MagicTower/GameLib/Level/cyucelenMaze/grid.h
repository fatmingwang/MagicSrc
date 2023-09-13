#pragma once

#include <stack>
#include <vector>
#include "cell.h"
//#include "ncurses.h"


// __ __ __ __ __ __
//| 0| 1| 2| 3| 4| 5|
// -- -- -- -- -- --
//| 6| 7| 8| 9|10|11|
// -- -- -- -- -- --



class cCyucelenMazeGrid :public Frame
{
	////let random be more stright.
	//struct sRandomData
	//{
	//	int iDirection;//eDirection
	//	int iKeepDirectionCountMax;
	//	int iKeepDirectionCountSmall;
	//	int iRestTimeToKeepDirectionC;
	//};
private:
	struct sBatchData* m_pBatchData;
	int		m_iWidth;
	int		m_iHeight;
	bool	m_bGenerationFinished;

	float m_fGridSizeX;
	float m_fGridSizeY;

	std::vector<cCyucelenMazeCell> m_CellVector;
	std::stack<cCyucelenMazeCell*> backtrace;
	cCyucelenMazeCell* m_pCurrent;

	void createCells();
	int CalculateIndex(int i, int j);
	std::vector<cCyucelenMazeCell*> getAvailableNeighbors();
	cCyucelenMazeCell* findNextCell();
	cBaseImage* m_pWallImae;
	std::map<std::string, bool>		m_WallPosAndDirectionVector;
public:
	cCyucelenMazeGrid(int width, int height);
	virtual ~cCyucelenMazeGrid();
	//-1 for generator all at once
	void	generateMaze(int e_iStep = -1);
	void	DebugRender(bool e_bDoStrip);
	void	Render();
	bool	IsGenerationFinished() { return m_bGenerationFinished; }
	//
	//bool true for vertical ,false for horozontal
	//
	void	GetAllWallData(std::map<std::string, bool>* e_pWallPosAndDirectionVector);
	void	GenRandomMap();

};