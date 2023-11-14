#pragma once


//
// 0, 1, 2, 3, 4
// 5, 6, 7, 8, 9
//10,11,12,13,14
//15,16,17,18,19
//
class cCyucelenMazeCell
{
private:
	int row, column;
	bool visited;
	//put some item here?
	bool	m_bIsDeadEnd;
	int		m_iNumWall = 0;

public:
	enum direction { TOP = 0, RIGHT, BOTTOM, LEFT, MAX };
	bool walls[direction::MAX];
	cCyucelenMazeCell(int i, int j,int e_iIndex);
	cCyucelenMazeCell();

	void removeWalls(cCyucelenMazeCell&);

	void	setWalls();
	void	setVisited(bool);
	bool	isVisited();

	int		getRow();
	int		getColumn();
	void	Render();
	void	DebugRender();
	void	DumpInfo();
	//e_pWallPosAndDirectionVector:string is Vector2
	void	DumpWallData(std::map<std::string, bool>* e_pWallPosAndDirectionVector,float e_fStartX, float e_fStartY, float e_fGridSizeX, float e_fGridSizeY);
#ifdef DEBUG
	int iIndex;
#endif
	bool	IsMoveable(eDirection e_eDirection);
	int		GetWallCount();
	cCyucelenMazeCell* GetNeighborCell(direction e_direction, class cCyucelenMazeGrid*e_pCyucelenMazeGrid);
};