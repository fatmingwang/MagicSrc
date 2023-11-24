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
protected:
	int		m_iWidth;
	int		m_iHeight;
	int		m_iLastGeneratedPosX;
	int		m_iLastGeneratedPosY;

	bool	m_bGenerationFinished;

	float	m_fGridSizeX;
	float	m_fGridSizeY;

	std::vector<cCyucelenMazeCell> m_CellVector;
	std::stack<cCyucelenMazeCell*> backtrace;
	cCyucelenMazeCell* m_pCurrent;

	void createCells();
	std::vector<cCyucelenMazeCell*> getAvailableNeighbors();
	cCyucelenMazeCell*				findNextCell();
	std::map<std::string, bool>		m_WallPosAndDirectionVector;
	//-1 for generator all at once
	void							generateMaze(int e_iStep = -1);
public:
	cCyucelenMazeGrid(int width, int height);
	virtual ~cCyucelenMazeGrid();
	//void	Render();
	bool	IsGenerationFinished() { return m_bGenerationFinished; }
	//
	//bool true for vertical ,false for horozontal
	//
	void	GetAllWallData(std::map<std::string, bool>* e_pWallPosAndDirectionVector,float e_fStartX, float e_fStartY);
	void	GenRandomMap(float e_fStartX, float e_fStartY,bool e_bRandomlyRemoveWall = false,float e_fPercentage = 0.f);
	cCyucelenMazeCell* GetCell(int e_iX, int e_iY);
	cCyucelenMazeCell* GetCell(int e_iIndex);
	bool	IsMovable(int e_iFromX, int e_iFromY,int e_iToX, int e_iToY);
	int		GetWidth();
	int		GetHeight();
	void	GetLastPoint(int&e_iX, int&e_iY);
	void	GetRightDownCornerPoint(int& e_iX, int& e_iY);
	bool	IsAllowToRemoveWall(int e_iIndex, cCyucelenMazeCell::direction e_Direction);
	POINT	IndexGetRowAndColumn(int e_iIndex);
	int		CalculateIndex(int e_iColumn, int e_iRow);
};