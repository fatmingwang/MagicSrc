#include "stdafx.h"
#include "grid.h"
#include <stdlib.h>
#include <chrono>
#include <iostream>
#include <thread>
#include "cell.h"

cCyucelenMazeGrid::cCyucelenMazeGrid(int width, int height)
{
	m_pWallImae = new cBaseImage("MagicTower/Image/Wall.png");
	m_pBatchData = new sBatchData();
	SetLocalPosition(Vector3(150,50,0));
	m_fGridSizeX = 100;
	m_fGridSizeY = 100;

	m_bGenerationFinished = false;
	this->m_iWidth = width;
	this->m_iHeight = height;
	GenRandomMap();
}

cCyucelenMazeGrid::~cCyucelenMazeGrid()
{
	SAFE_DELETE(m_pWallImae);
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
			break;
		}
	}
#ifdef DEBUG
	for (auto l_Cell : m_CellVector)
	{
		l_Cell.DumpInfo();
	}
#endif
	if (m_bGenerationFinished)
	{
		if (m_pBatchData)
		{

		}
	}
}

void cCyucelenMazeGrid::Render()
{
	int l_iIndex = 0;
	for (auto l_IT = m_WallPosAndDirectionVector.begin(); l_IT != m_WallPosAndDirectionVector.end(); ++l_IT)
	{
		auto l_Data = l_IT;
		Vector2 l_vPos = GetVector2(l_Data->first.c_str());
		if (l_vPos.x < 0)
		{
			int a = 0;
		}
		bool l_bHorizontal = l_Data->second;
		auto l_Mat = cMatrix44::TranslationMatrix(l_vPos);
		if (l_bHorizontal)
		{
			l_Mat *= cMatrix44::ZAxisRotationMatrix(3.14f / 2);
		}
		m_pWallImae->Render(l_Mat);
		//cGameApp::RenderFont(l_vPos.x, l_vPos.y+15,ValueToStringW(l_iIndex));
		++l_iIndex;
	}
	//if (m_pBatchData)
	//{
	//	int l_iOutNumVertex = 0;
	//	Vector3 l_vOutPos[6];
	//	Vector2 l_vOutUV[6];
	//	Vector4 l_vOutColor[6];
	//	auto l_pTecture = e_pRenderObject->GetTriangulatorRenderDataForBatchRendering(l_iOutNumVertex, l_vOutPos, l_vOutUV, l_vOutColor);
	//	if (l_pTecture)
	//	{
	//		m_pBatchData->AddData(l_iOutNumVertex, (float*)l_vOutPos, (float*)l_vOutUV, (float*)l_vOutColor, l_pTecture);
	//	}
	//}
	//else
	//{
	//	e_pRenderObject->Render();
	//}
}

void cCyucelenMazeGrid::DebugRender(bool e_bDoStrip)
{
	auto	l_vPos = this->GetWorldPosition();
	float	l_fGridSizeX = m_fGridSizeX;
	float	l_fGridSizeY = m_fGridSizeY;
	float	l_fStartPosX = l_vPos.x;
	float	l_fStartPosY = l_vPos.y;
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
void cCyucelenMazeGrid::GetAllWallData(std::map<std::string, bool>* e_pWallPosAndDirectionVector)
{
	auto l_vPos = GetWorldPosition();
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
			l_Cell.DumpWallData(e_pWallPosAndDirectionVector, l_vPos.x, l_vPos.y,this->m_fGridSizeX, this->m_fGridSizeY);
		}
		
	}
}

void	cCyucelenMazeGrid::GenRandomMap()
{
	auto l_iWidth = rand() % 5 + 6;
	auto l_iHeight = rand() % 5 + 2;
	this->m_iWidth = l_iWidth;
	this->m_iHeight = l_iHeight;
	createCells();
	generateMaze();
	m_WallPosAndDirectionVector.clear();
	GetAllWallData(&m_WallPosAndDirectionVector);
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
