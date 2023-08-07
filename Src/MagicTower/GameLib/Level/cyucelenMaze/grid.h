#pragma once

#include <stack>
#include <vector>
#include "cell.h"
//#include "ncurses.h"

class cCyucelenMazeGrid
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
	int width;
	int height;
	bool	m_bGenerationFinished;

	std::vector<cCyucelenMazeCell> m_CellVector;
	std::stack<cCyucelenMazeCell*> backtrace;
	cCyucelenMazeCell* m_pCurrent;

	void createCells();
	int CalculateIndex(int i, int j);
	std::vector<cCyucelenMazeCell*> getAvailableNeighbors();
	cCyucelenMazeCell* findNextCell();
	void render();

public:
	cCyucelenMazeGrid(int width, int height);
	//-1 for generator all at once
	void	generateMaze(int e_iStep = -1);
	void	DebugRender();
	bool	IsGenerationFinished() { return m_bGenerationFinished; }
};