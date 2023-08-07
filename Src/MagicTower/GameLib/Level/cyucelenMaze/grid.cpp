#include "stdafx.h"
#include "grid.h"
#include <stdlib.h>
#include <chrono>
#include <iostream>
#include <thread>
#include "cell.h"

cCyucelenMazeGrid::cCyucelenMazeGrid(int width, int height)
{
	m_bGenerationFinished = false;
	this->width = width;
	this->height = height;
	createCells();
	m_pCurrent = &m_CellVector[0];
}

void cCyucelenMazeGrid::createCells()
{
	m_CellVector.clear();
	m_bGenerationFinished = false;
	for (int row = 0; row < height; row++)
	{
		for (int column = 0; column < width; column++)
		{
			m_CellVector.push_back(cCyucelenMazeCell(row, column));
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
}

void cCyucelenMazeGrid::render()
{
	for (cCyucelenMazeCell& l_Cell : m_CellVector)
	{
		l_Cell.Render();
	}
}

void cCyucelenMazeGrid::DebugRender()
{
	float	l_fGridSizeX = 100;
	float	l_fGridSizeY = 100;
	float	l_fWallToCenterX = 25;
	float	l_fWallToCenterY = 25;
	float	l_fOffsetX = 0;
	float	l_fOffsetY = 0;
	float	l_fStartPosX = 50;
	float	l_fStartPosY = 50;
	float	l_fCurrentPosX = l_fStartPosX;
	float	l_fCurrentPosY = l_fStartPosY;
	for (int i = 0; i < width; ++i)
	{
		l_fCurrentPosX = i * l_fGridSizeX + l_fStartPosX;
		for (int j = 0; j < height; ++j)
		{
			l_fCurrentPosY = l_fStartPosY + j * l_fGridSizeY;
			int l_iIndex = i + j * width;
			if (l_iIndex >= width * height)
			{
				int a = 0;
			}
			cCyucelenMazeCell& l_Cell = m_CellVector[l_iIndex];
			for (int k = 0; k < 4; ++k)
			{
				Vector2 l_vPosOffset(0, 0);
				if (l_Cell.walls[k])
				{
					const wchar_t* l_strWall = nullptr;
					switch (k)
					{
					case cCyucelenMazeCell::direction::LEFT:
						l_vPosOffset.x = -l_fWallToCenterX;
						l_strWall = L"|";
						break;
					case cCyucelenMazeCell::direction::TOP:
						l_strWall = L"-";
						l_vPosOffset.y = -l_fWallToCenterX;
						break;
					case cCyucelenMazeCell::direction::RIGHT:
						l_strWall = L"|";
						l_vPosOffset.x = l_fWallToCenterX;
						break;
					case cCyucelenMazeCell::direction::BOTTOM:
						l_strWall = L"_";
						l_vPosOffset.y = l_fWallToCenterX;
						break;
					}
					cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(1, 1, 0, 1));
					cGameApp::RenderFont(l_fCurrentPosX+ l_vPosOffset.x, l_fCurrentPosY+ l_vPosOffset.y, l_strWall);
				}
				else
				{

				}
			}
			cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(1, 1, 1, 1));
			cGameApp::RenderFont(l_fCurrentPosX, l_fCurrentPosY, ValueToStringW(l_iIndex).c_str());
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
	if (row < 0 || column < 0 || column > width - 1 || row > height - 1)
	{
		return -1;
	}
	return column + row * width;
}
