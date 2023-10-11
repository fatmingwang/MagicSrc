#include "stdafx.h"
#include "MazeRender.h"

cMazeRender::cMazeRender(int e_iWidth, int e_iHeight):cCyucelenMazeGrid(e_iWidth, e_iHeight)
{
	m_pWallImae = new cBaseImage("MagicTower/Image/Wall.png");
	m_pBatchData = new sBatchData();
}

cMazeRender::~cMazeRender()
{
	SAFE_DELETE(m_pWallImae);
	SAFE_DELETE(m_pBatchData);
}

void cMazeRender::GenRandomMap()
{
	auto l_vPos = this->GetWorldPosition();
	cCyucelenMazeGrid::GenRandomMap(l_vPos.x, l_vPos.y);
}

void cMazeRender::Render()
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

void cMazeRender::DebugRender(bool e_bDoStrip)
{
	auto l_vPos = this->GetWorldPosition();
	float	l_fGridSizeX = m_fGridSizeX;
	float	l_fGridSizeY = m_fGridSizeY;
	float	l_fStartPosX = l_vPos.x;
	float	l_fStartPosY = l_vPos.y;
	float	l_fCurrentPosX = l_fStartPosX;
	float	l_fCurrentPosY = l_fStartPosY;
	float	l_fWallToCenterX = l_fGridSizeX / 2;
	float	l_fWallToCenterY = l_fGridSizeY / 2;
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
				if (l_strWall)
				{
					cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(1, 1, 0, 1));
					cGameApp::RenderFont(l_fCurrentPosX + l_vPosOffset.x, l_fCurrentPosY + l_vPosOffset.y, l_strWall);
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

void cMazeRender::GenerateMaze(int e_iStep)
{
	this->generateMaze(e_iStep);
	if (m_bGenerationFinished)
	{
		if (m_pBatchData)
		{

		}
	}
}

bool cMazeRender::IsMovable(int e_iNowPosX, int e_iNowPosY,eDirection e_eDirection)
{
	cCyucelenMazeCell*l_pCellt = GetCell(e_iNowPosX,e_iNowPosY);
	if (!l_pCellt)
	{
		return false;
	}
	return l_pCellt->IsMoveable(e_eDirection);
}

bool cMazeRender::IsMovable(int e_iNowPosX, int e_iNowPosY, int e_iTargetPosX, int e_iTargetPosY)
{
	//cCyucelenMazeCell* l_pCellt = GetCell(e_iNowPosX, e_iNowPosY);
	//if (!l_pCellt)
	//{
	//	return false;
	//}
	//return l_pCellt->IsMoveable(e_eDirection);
	return false;
}

bool cMazeRender::NextJunction(int e_iNowPosX, int e_iNowPosY,eDirection e_eDirection, int& e_iPosX, int& e_iPosY)
{
	cCyucelenMazeCell*l_pCellt = GetCell(e_iNowPosX, e_iNowPosY);
	if (!l_pCellt)
	{
		e_iPosX = -1;
		e_iPosY = -1;
		return false;
	}
	e_iPosX = l_pCellt->getRow();
	e_iPosY = l_pCellt->getColumn();
	while (l_pCellt)
	{
		if (l_pCellt->IsMoveable(e_eDirection))
		{
			e_iPosX = l_pCellt->getRow();
			e_iPosY = l_pCellt->getColumn();
			switch (e_eDirection)
			{
				case eD_LEFT:
					--e_iNowPosX;
					break;
				case eD_UP:
					--e_iNowPosY;
					break;
				case eD_RIGHT:
					++e_iNowPosX;
					break;
				case eD_DOWN:
					++e_iNowPosY;
					break;
			}
			l_pCellt = GetCell(e_iNowPosX, e_iNowPosY);
		}
		else
		{
			break;
		}
	}

	return true;
}

std::vector<eDirection> cMazeRender::GetMovableDirection(int e_iNowPosX, int e_iNowPosY)
{
	std::vector<eDirection> l_Vector;
	cCyucelenMazeCell*l_pCellt = GetCell(e_iNowPosX, e_iNowPosY);
	if (l_pCellt)
	{
		eDirection l_eLazyArray[4] = { eD_LEFT ,eD_UP,eD_RIGHT,eD_DOWN};
		for (int i = 0; i < 4; ++i)
		{
			if (l_pCellt->IsMoveable(l_eLazyArray[i]))
			{
				l_Vector.push_back(l_eLazyArray[i]);
			}
		}
	}
	return l_Vector;
}

bool cMazeRender::GetExitPoint(int& e_iPosX, int& e_iPosY)
{
	this->GetLastPoint(e_iPosX, e_iPosY);
	return true;
}