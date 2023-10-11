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
			if (l_iIndex == 12)
			{
				int a = 0;
			}
			l_Cell.DumpWallData(e_pWallPosAndDirectionVector, e_fStartX, e_fStartY,this->m_fGridSizeX, this->m_fGridSizeY);
		}
		
	}
}

void	cCyucelenMazeGrid::GenRandomMap(float e_fStartX, float e_fStartY)
{
	auto l_iWidth = rand() % 5 + 6;
	auto l_iHeight = rand() % 5 + 2;
	this->m_iWidth = l_iWidth;
	this->m_iHeight = l_iHeight;
	createCells();
	generateMaze();
	m_WallPosAndDirectionVector.clear();
	GetAllWallData(&m_WallPosAndDirectionVector, e_fStartX,e_fStartY);
}

cCyucelenMazeCell* cCyucelenMazeGrid::GetCell(int e_iX, int e_iY)
{
	int l_iIndex = e_iX + e_iY * m_iWidth;
	if (l_iIndex >= m_CellVector.size())
	{
		return nullptr;
	}
	cCyucelenMazeCell*l_Cell = &m_CellVector[l_iIndex];
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
		CalculateIndex(currentRow - 1, currentColumn),
		CalculateIndex(currentRow, currentColumn + 1),
		CalculateIndex(currentRow + 1, currentColumn),
		CalculateIndex(currentRow, currentColumn - 1),
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

int cCyucelenMazeGrid::CalculateIndex(int row, int column)
{
	if (row < 0 || column < 0 || column > m_iWidth - 1 || row > m_iHeight - 1)
	{
		return -1;
	}
	return column + row * m_iWidth;
}
