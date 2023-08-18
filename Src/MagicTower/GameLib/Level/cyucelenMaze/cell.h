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
	bool m_bIsDeadEnd;

public:
	enum direction { TOP, RIGHT, BOTTOM, LEFT, MAX };
	bool walls[4];
	cCyucelenMazeCell(int i, int j,int e_iIndex);

	void removeWalls(cCyucelenMazeCell&);

	void setWalls();
	void setVisited(bool);
	bool isVisited();

	int		getRow();
	int		getColumn();
	void	Render();
	void	DebugRender();
	void	DumpInfo();
#ifdef DEBUG
	int iIndex;
#endif
};