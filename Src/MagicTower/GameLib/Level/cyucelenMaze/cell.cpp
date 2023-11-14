#include "stdafx.h"
#include "cell.h"
#include <stdlib.h>
#include <iostream>
#include "grid.h"

cCyucelenMazeCell::cCyucelenMazeCell()
{
	this->row = -1;
	this->column = -1;
#ifdef DEBUG
	iIndex = -1;
#endif
	visited = false;
	m_bIsDeadEnd = false;
	setWalls();
}
cCyucelenMazeCell::cCyucelenMazeCell(int row, int column, int e_iIndex)
{
	this->row = row;
	this->column = column;
#ifdef DEBUG
	iIndex = e_iIndex;
#endif
	visited = false;
	m_bIsDeadEnd = false;
	setWalls();
}

void cCyucelenMazeCell::setWalls()
{
	m_iNumWall = 0;
	for (int i = 0; i < 4; i++)
	{
		walls[i] = true;
	}
}

void cCyucelenMazeCell::setVisited(bool b)
{
	visited = b;
}
bool cCyucelenMazeCell::isVisited() { return visited; }

int cCyucelenMazeCell::getRow() { return row; }
int cCyucelenMazeCell::getColumn() { return column; }

void cCyucelenMazeCell::Render()
{

}

void	cCyucelenMazeCell::DebugRender()
{
}

void cCyucelenMazeCell::DumpInfo()
{
#ifdef DEBUG
	std::string l_strInfo = ValueToString(this->iIndex);
	l_strInfo += ":";
	for (int l_Direction = direction::TOP; l_Direction != direction::MAX; ++l_Direction)
	{
		if (this->walls[l_Direction])
		{
			l_strInfo += "1,";
		}
		else
		{
			l_strInfo += "0,";
		}
	}
	FMLOG(l_strInfo.c_str());
	
#endif
}

void	cCyucelenMazeCell::DumpWallData(std::map<std::string, bool>* e_pWallPosAndDirectionVector, float e_fStartX, float e_fStartY, float e_fGridSizeX, float e_fGridSizeY)
{
	Vector2 l_Pos(e_fStartX + e_fGridSizeX * this->column, e_fStartY + e_fGridSizeY * this->row);
	int l_iMax = direction::MAX;
	int l_Direction = 0;
	l_Pos.y -= e_fGridSizeY / 2-15;
	for (; l_Direction < l_iMax; ++l_Direction)
	{
		if (walls[l_Direction])
		{			
			bool l_bHorizontal = true;
			Vector2 l_FinalPos = l_Pos;
			if (l_Direction == direction::LEFT)
			{
				l_FinalPos.x += -(e_fGridSizeX /2);
			}
			else
			if (l_Direction == direction::TOP)
			{
				l_FinalPos.y -= e_fGridSizeY / 2;
			}
			else
			if (l_Direction == direction::RIGHT)
			{
				l_FinalPos.x += e_fGridSizeX / 2;

			}
			else
			if (l_Direction == direction::BOTTOM)
			{
				l_FinalPos.y += e_fGridSizeY / 2;

			}
			if (l_Direction == direction::LEFT || l_Direction == direction::RIGHT)
			{
				l_bHorizontal = false;
			}
			if (l_FinalPos.x == 0)
			{
				int a = 0;
			}
			e_pWallPosAndDirectionVector->insert(std::make_pair(ValueToString(l_FinalPos), l_bHorizontal));
		}
	}
}

bool cCyucelenMazeCell::IsMoveable(eDirection e_eDirection)
{
	if (e_eDirection == eDirection::eD_UP)
	{
		return !walls[TOP];
	}
	else
	if (e_eDirection == eDirection::eD_RIGHT)
	{
		return !walls[RIGHT];
	}
	else
	if (e_eDirection == eDirection::eD_DOWN)
	{
		return !walls[BOTTOM];
	}
	else
	if (e_eDirection == eDirection::eD_LEFT)
	{
		return !walls[LEFT];
	}
	return false;
}

int cCyucelenMazeCell::GetWallCount()
{
	//m_iNumWall = 0;
	//for (int l_Direction = direction::TOP; l_Direction != direction::MAX; ++l_Direction)
	//{
	//	if (walls[l_Direction])
	//	{
	//		++m_iNumWall;
	//	}
	//}
	return m_iNumWall;
}

cCyucelenMazeCell* cCyucelenMazeCell::GetNeighborCell(direction e_direction, cCyucelenMazeGrid* e_pCyucelenMazeGrid)
{
	int l_iIndex = -1;
	switch (e_direction)
	{
		case direction::LEFT:
			l_iIndex = e_pCyucelenMazeGrid->CalculateIndex(this->column-1,this->row);
			break;
		case direction::TOP:
			l_iIndex = e_pCyucelenMazeGrid->CalculateIndex(this->column, this->row-1);
			break;
		case direction::RIGHT:
			l_iIndex = e_pCyucelenMazeGrid->CalculateIndex(this->column+1, this->row);
			break;
		case direction::BOTTOM:
			l_iIndex = e_pCyucelenMazeGrid->CalculateIndex(this->column, this->row+1);
			break;
	}
	return e_pCyucelenMazeGrid->GetCell(l_iIndex);
}

void cCyucelenMazeCell::removeWalls(cCyucelenMazeCell& next)
{
	int x = column - next.column;
	int y = row - next.row;

	if (x == 1)
	{
		walls[LEFT] = false;
		next.walls[RIGHT] = false;
	}
	else if (x == -1)
	{
		walls[RIGHT] = false;
		next.walls[LEFT] = false;
	}
	else if (y == 1)
	{
		walls[TOP] = false;
		next.walls[BOTTOM] = false;
	}
	else if (y == -1)
	{
		walls[BOTTOM] = false;
		next.walls[TOP] = false;
	}
	int l_iNumDeadEnd = 0;
	m_iNumWall = 0;
	for (int l_Direction = direction::TOP; l_Direction != direction::MAX; ++l_Direction)
	{
		if (!walls[l_Direction])
		{
			++l_iNumDeadEnd;
		}
		else
		{
			++m_iNumWall;
		}
	}
	if (l_iNumDeadEnd >= 3)
	{
		m_bIsDeadEnd = true;
	}
}