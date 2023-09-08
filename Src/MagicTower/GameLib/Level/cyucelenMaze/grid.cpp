#include "stdafx.h"
#include "grid.h"
#include <stdlib.h>
#include <chrono>
#include <iostream>
#include <thread>
#include "cell.h"

cCyucelenMazeGrid::cCyucelenMazeGrid(int width, int height)
{
	m_fStartX = 50;
	m_fStartY = 50;
	m_fGridSizeX = 100;
	m_fGridSizeY = 100;

	m_bGenerationFinished = false;
	this->m_iWidth = width;
	this->m_iHeight = height;
	createCells();
	m_pCurrent = &m_CellVector[0];
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

void cCyucelenMazeGrid::render()
{
	for (cCyucelenMazeCell& l_Cell : m_CellVector)
	{
		l_Cell.Render();
	}
}

void cCyucelenMazeGrid::DebugRender(int e_iPosX, int e_iPosY, bool e_bDoStrip)
{
	float	l_fGridSizeX = m_fGridSizeX;
	float	l_fGridSizeY = m_fGridSizeY;
	float	l_fStartPosX = m_fStartX + e_iPosX;
	float	l_fStartPosY = m_fStartY + e_iPosY;
	float	l_fCurrentPosX = l_fStartPosX;
	float	l_fCurrentPosY = l_fStartPosY;
	float	l_fWallToCenterX = l_fGridSizeX/2;
	float	l_fWallToCenterY = l_fGridSizeY/2;
	for (int i = 0; i < m_iWidth; ++i)
	{
		l_fCurrentPosX = i * l_fGridSizeX + l_fStartPosX;
		for (int j = 0; j < m_iHeight; ++j)
		{
			l_fCurrentPosY = l_fStartPosY + j * l_fGridSizeY;
			int l_iIndex = i + j * m_iWidth;
			if (l_iIndex >= m_iWidth * m_iHeight)
			{
				int a = 0;
			}
			cCyucelenMazeCell& l_Cell = m_CellVector[l_iIndex];
			bool l_bIsLeftExists = false;
			bool l_bIsUpExists = false;
			for (int k = 0; k < 4; ++k)
			{
				if (!l_Cell.walls[k])
				{
					continue;
				}
				Vector2 l_vPosOffset(0, 0);
				const wchar_t* l_strWall = nullptr;
				switch (k)
				{
				case cCyucelenMazeCell::direction::LEFT:
					if (i > 0 && e_bDoStrip)
					{
						continue;
					}
					l_vPosOffset.x = -l_fWallToCenterX;
					l_strWall = L"O";
					break;
				case cCyucelenMazeCell::direction::TOP:
					if (j > 0 && e_bDoStrip)
					{
						continue;
					}
					l_strWall = L"O";
					l_vPosOffset.y = -l_fWallToCenterX;
					break;
				case cCyucelenMazeCell::direction::RIGHT:
					l_strWall = L"O";
					l_vPosOffset.x = l_fWallToCenterX;
					break;
				case cCyucelenMazeCell::direction::BOTTOM:
					l_strWall = L"O";
					l_vPosOffset.y = l_fWallToCenterX;
					break;
				}
				if (l_strWall )
				{
					cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(1, 1, 0, 1));
					cGameApp::RenderFont(l_fCurrentPosX+ l_vPosOffset.x, l_fCurrentPosY+ l_vPosOffset.y, l_strWall);
				}
				//else
				//{
				//	cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(0, 1, 0, 1));
				//	cGameApp::RenderFont(l_fCurrentPosX + l_vPosOffset.x, l_fCurrentPosY + l_vPosOffset.y, L"R");
				//}
			}
			cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(1, 1, 1, 1));
			cGameApp::RenderFont(l_fCurrentPosX, l_fCurrentPosY, ValueToStringW(l_iIndex).c_str());
		}
	}
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
			l_Cell.DumpWallData(e_pWallPosAndDirectionVector, e_fStartX+this->m_fStartX,e_fStartY + this->m_fStartY,this->m_fGridSizeX, this->m_fGridSizeY);
		}
		
	}
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
