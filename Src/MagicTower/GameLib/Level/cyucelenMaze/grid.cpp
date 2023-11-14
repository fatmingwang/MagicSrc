#include "stdafx.h"
#include "grid.h"
#include <stdlib.h>
#include <chrono>
#include <iostream>
#include <thread>
#include "cell.h"

cCyucelenMazeGrid::cCyucelenMazeGrid(int width, int height)
{
	m_iLastGeneratedPosX = -1;
	m_iLastGeneratedPosY = -1;
	m_fGridSizeX = 100;
	m_fGridSizeY = 100;

	m_bGenerationFinished = false;
	this->m_iWidth = width;
	this->m_iHeight = height;
	GenRandomMap(0,0);
}

cCyucelenMazeGrid::~cCyucelenMazeGrid()
{
}

void cCyucelenMazeGrid::createCells()
{
	m_CellVector.clear();
	m_bGenerationFinished = false;
	m_CellVector.resize(m_iWidth* m_iHeight);
	for (int column = 0; column < m_iWidth; column++)
	{
		for (int row = 0; row < m_iHeight; row++)
		{
			int l_iIndex = column + row * m_iWidth;
			m_CellVector[l_iIndex] = cCyucelenMazeCell(row, column, l_iIndex);
		}
	}
	m_pCurrent = &m_CellVector[0];
}

void cCyucelenMazeGrid::generateMaze(int e_iStep)
{
	if (m_bGenerationFinished)
	{
		return;
	}
	int l_iNumStep = e_iStep;
	while (e_iStep == -1 || l_iNumStep)
	{
		--l_iNumStep;
		m_pCurrent->setVisited(true);
		cCyucelenMazeCell* next = findNextCell();

		if (next != nullptr)
		{
			next->setVisited(true);
			backtrace.push(m_pCurrent);
			m_pCurrent->removeWalls(*next);
			m_pCurrent = next;
		}
		else
		if (backtrace.size() > 0)
		{
			m_pCurrent = backtrace.top();
			backtrace.pop();
		}
		else
		if (backtrace.size() == 0)
		{//finish
			m_bGenerationFinished = true;
			m_iLastGeneratedPosX = m_pCurrent->getRow();
			m_iLastGeneratedPosY = m_pCurrent->getColumn();
			break;
		}
	}
#ifdef DEBUG
	for (auto l_Cell : m_CellVector)
	{
		l_Cell.DumpInfo();
	}
#endif
}

//row odd is horizontal    -
//row even is vertical     |
void cCyucelenMazeGrid::GetAllWallData(std::map<std::string, bool>* e_pWallPosAndDirectionVector, float e_fStartX, float e_fStartY)
{
	for (int i=0;i<this->m_iHeight;++i)
	{
		for (int j = 0; j < this->m_iWidth; ++j)
		{
			int l_iIndex = j+i* m_iWidth;
			auto l_Cell = m_CellVector[l_iIndex];
			l_Cell.DumpWallData(e_pWallPosAndDirectionVector, e_fStartX, e_fStartY,this->m_fGridSizeX, this->m_fGridSizeY);
		}
		
	}
}

void	cCyucelenMazeGrid::GenRandomMap(float e_fStartX, float e_fStartY, bool e_bRandomlyRemoveWall,float e_fPercentage)
{
	auto l_iWidth = rand() % 10 + 12;
	auto l_iHeight = rand() % 10 + 5;
	this->m_iWidth = l_iWidth;
	this->m_iHeight = l_iHeight;
	createCells();
	if (e_bRandomlyRemoveWall)
	{
		int l_iTotal = m_iWidth * m_iHeight;
		int l_iTotalRemoveWall = (int)(l_iTotal * e_fPercentage)*4;
		for (int i = 0; i < l_iTotalRemoveWall; ++i)
		{
			int l_iIndex = rand() % l_iTotal;
			int l_iDirection = rand() % cCyucelenMazeCell::direction::MAX;
			if (l_iIndex == 33)
			{
				int a = 0;
			}
			if (IsAllowToRemoveWall(l_iIndex, (cCyucelenMazeCell::direction)l_iDirection))
			{
				auto l_pCell = &m_CellVector[l_iIndex];
				//l_pCell->walls[l_iDirection] = false;
				auto l_NeighborCell = l_pCell->GetNeighborCell((cCyucelenMazeCell::direction)l_iDirection,this);
				if (!l_NeighborCell)
				{
					int a = 0;
				}
				l_pCell->removeWalls(*l_NeighborCell);
				bool l_b = l_pCell->walls[l_iDirection];
				//m_CellVector[l_iIndex].removeWalls();
				int a = 0;
			}
		}
	}
	generateMaze();
	m_WallPosAndDirectionVector.clear();
	GetAllWallData(&m_WallPosAndDirectionVector, e_fStartX,e_fStartY);
}

cCyucelenMazeCell* cCyucelenMazeGrid::GetCell(int e_iX, int e_iY)
{
	int l_iIndex = e_iX + e_iY * m_iWidth;
	return GetCell(l_iIndex);
}

cCyucelenMazeCell* cCyucelenMazeGrid::GetCell(int e_iIndex)
{
	if (e_iIndex >= m_CellVector.size())
	{
		return nullptr;
	}
	cCyucelenMazeCell* l_Cell = &m_CellVector[e_iIndex];
	return l_Cell;
}

bool cCyucelenMazeGrid::IsMovable(int e_iFromX, int e_iFromY, int e_iToX, int e_iToY)
{
	auto l_FromCell = GetCell(e_iFromX, e_iFromY);
	if (l_FromCell)
	{
		auto l_ToCell = GetCell(e_iToX, e_iToY);
		if (l_ToCell)
		{
			
		}
		else
		{
			return false;
		}
	}
	return false;
}

int cCyucelenMazeGrid::GetWidth()
{
	return m_iWidth;
}

int cCyucelenMazeGrid::GetHeight()
{
	return m_iHeight;
}

void cCyucelenMazeGrid::GetLastPoint(int& e_iX, int& e_iY)
{
	e_iX = m_iLastGeneratedPosX;
	e_iY = m_iLastGeneratedPosY;
}

void cCyucelenMazeGrid::GetRightDownCornerPoint(int& e_iX, int& e_iY)
{
	e_iX = this->m_iWidth;
	e_iY = this->m_iHeight;
}

bool cCyucelenMazeGrid::IsAllowToRemoveWall(int e_iIndex, cCyucelenMazeCell::direction e_Direction)
{
	if (m_CellVector.size() > e_iIndex)
	{
		auto l_Cell = &m_CellVector[e_iIndex];
		int l_iColumn = l_Cell->getColumn();
		int l_iRow = l_Cell->getRow();
		if (e_Direction == cCyucelenMazeCell::direction::LEFT && l_iColumn == 0)
		{
			return false;
		}
		else
		if (e_Direction == cCyucelenMazeCell::direction::TOP && l_iRow == 0)
		{
			return false;
		}
		else
		if (e_Direction == cCyucelenMazeCell::direction::RIGHT && l_iColumn == this->m_iWidth-1)
		{
			return false;
		}
		else
		if (e_Direction == cCyucelenMazeCell::direction::BOTTOM && l_iRow == this->m_iHeight - 1)
		{
			return false;
		}
		return true;
	}
	return false;
}

POINT cCyucelenMazeGrid::IndexGetRowAndColumn(int e_iIndex)
{
	POINT l_Index = { -1,-1 };
	if (m_CellVector.size() > e_iIndex)
	{
		auto l_Cell = &m_CellVector[e_iIndex];
		l_Index.x = l_Cell->getColumn();
		l_Index.y = l_Cell->getRow();
	}
	return l_Index;
}

cCyucelenMazeCell* cCyucelenMazeGrid::findNextCell()
{
	std::vector<cCyucelenMazeCell*> availableNeighbors = getAvailableNeighbors();
	if (availableNeighbors.size() > 0)
	{
		return availableNeighbors.at(rand() % availableNeighbors.size());
	}

	return nullptr;
}

std::vector<cCyucelenMazeCell*> cCyucelenMazeGrid::getAvailableNeighbors() {
	std::vector<cCyucelenMazeCell*> neighbors;

	int currentRow = m_pCurrent->getRow();
	int currentColumn = m_pCurrent->getColumn();
#ifdef DEBUG
	int iIndex = m_pCurrent->iIndex;
#endif
	int neighborIndexes[4] =
	{
		CalculateIndex(currentColumn,currentRow - 1),
		CalculateIndex(currentColumn + 1,currentRow),
		CalculateIndex(currentColumn,currentRow + 1),
		CalculateIndex(currentColumn - 1,currentRow),
	};

	for (int i : neighborIndexes)
	{
		if (i != -1 && !m_CellVector[i].isVisited())
		{
			neighbors.push_back(&m_CellVector[i]);
		}
	}

	return neighbors;
}

int cCyucelenMazeGrid::CalculateIndex(int e_iColumn, int e_iRow)
{
	if (e_iRow < 0 || e_iColumn < 0 || e_iColumn > m_iWidth - 1 || e_iRow > m_iHeight - 1)
	{
		return -1;
	}
	return e_iColumn + e_iRow * m_iWidth;
}
