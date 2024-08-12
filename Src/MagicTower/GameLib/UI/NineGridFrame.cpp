#include "stdafx.h"
#include "NineGridFrame.h"
#include <GameplayUT/OpenGL/GLSL/Shader.h>
//reference GetDrawQuadFVFDataByTwoTriangles

void	CenterPosTo4Vertices(Vector3 e_vCenterPos,Vector2 e_vSize,Vector3*e_pOut4Vertices)
{
	Vector2 l_vHalfSize(e_vSize.x / 2.f, e_vSize.y / 2.f);
	//left up,right up,left down,right down
	e_pOut4Vertices[FOUR_VERTICES_ORDER_0_LEFT_UP] =	Vector3(e_vCenterPos.x - l_vHalfSize.x, e_vCenterPos.y - l_vHalfSize.y,0.f);
	e_pOut4Vertices[FOUR_VERTICES_ORDER_0_RIGHT_UP] =	Vector3(e_vCenterPos.x + l_vHalfSize.x, e_vCenterPos.y - l_vHalfSize.y, 0.f);
	e_pOut4Vertices[FOUR_VERTICES_ORDER_0_LEFT_DOWN] =	Vector3(e_vCenterPos.x - l_vHalfSize.x, e_vCenterPos.y + l_vHalfSize.y, 0.f);
	e_pOut4Vertices[FOUR_VERTICES_ORDER_0_RIGHT_DOWN] = Vector3(e_vCenterPos.x + l_vHalfSize.x, e_vCenterPos.y + l_vHalfSize.y, 0.f);
	//e_vCenterPos
}
void	QuadTo2TriangleVertices(Vector3 *e_pv4Vertices,std::vector<Vector3>&e_vPosVector)
{
	int l_iArrayQuadIndexTo2Triangles[] = {2,3,0,0,3,1};
	for (int i = 0; i < TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT; ++i)
	{
		e_vPosVector.push_back(Vector3(e_pv4Vertices[l_iArrayQuadIndexTo2Triangles[i]]));
	}
}

cNineGridFrame* cNineGridFrame::GenerateDebugObject(Vector2 e_vOriginalCenterSize, Vector2 e_vOriginalLeftupSize, Vector2 e_vContentSize)
{
	cNineGridFrame* l_pNineGridFrame = new cNineGridFrame(e_vOriginalCenterSize, e_vOriginalLeftupSize);
	l_pNineGridFrame->SetContentSize(e_vContentSize);
	return l_pNineGridFrame;
}

//https://gist.github.com/jandujar/1108831
void cNineGridFrame::SetContentSize(Vector2 e_vContentSize)
{
	if (this->m_vContentSize == e_vContentSize)
	{
		return;
	}
	this->m_vContentSize = e_vContentSize;
	float l_fCenterImageOriginalWidth = m_vOriginalCenterSize.x;
	float l_fCenterImageOriginalHeight = m_vOriginalCenterSize.y;
	
	//float l_fSizableWidth = e_vContentSize.x - m_pImageArray[eCorrnerType_LeftUp]->GetWidth() - m_pImageArray[eCorrnerType_RightUp]->GetWidth();
	//float l_fSizableHeight = e_vContentSize.y - m_pImageArray[eCorrnerType_LeftUp]->GetHeight() - m_pImageArray[eCorrnerType_RightBottom]->GetWidth();
	float l_fSizableWidth = e_vContentSize.x - (m_vOriginalLeftupSize.x*2);
	float l_fSizableHeight = e_vContentSize.y - (m_vOriginalLeftupSize.y*2);
	float l_fHorizontalScale = l_fSizableWidth / l_fCenterImageOriginalWidth;
	float l_fVerticalScale = l_fSizableHeight / l_fCenterImageOriginalHeight;
	float l_fCenterScaleX = l_fHorizontalScale;
	float l_fCenterScaleY = l_fVerticalScale;


	float l_fRescaledCenterWidth = l_fCenterImageOriginalWidth * l_fHorizontalScale;
	float l_fRescaledCenterHeight = l_fCenterImageOriginalHeight * l_fVerticalScale;

	//Position corners
  //[self setAnchorPoint:CGPointMake(0.5f,0.5f)];

	float l_fCenterPosX = this->m_vContentSize.x * 0.5f;
	float l_fCenterPosY = this->m_vContentSize.y * 0.5f;
	Vector2	l_vHalfCornerSize(m_vOriginalLeftupSize.x * 0.5f, m_vOriginalLeftupSize.y * 0.5f);
	Vector2	l_vHalfCenterSize(l_fRescaledCenterWidth*0.5f, l_fRescaledCenterHeight*0.5f);
	

	//Position corners
	//[topLeft setPosition : CGPointMake(-rescaledWidth / 2 - topLeft.contentSize.width / 2 + l_fCenterPosX, rescaledHeight / 2 + topLeft.contentSize.height * 0.5 + l_fCenterPosY)];
	//[topRight setPosition : CGPointMake(rescaledWidth / 2 + topRight.contentSize.width / 2 + l_fCenterPosX, rescaledHeight / 2 + topRight.contentSize.height * 0.5 + l_fCenterPosY)];
	//[bottomLeft setPosition : CGPointMake(-rescaledWidth / 2 - bottomLeft.contentSize.width / 2 + l_fCenterPosX, -rescaledHeight / 2 - bottomLeft.contentSize.height * 0.5 + l_fCenterPosY)] ;
	//[bottomRight setPosition : CGPointMake(rescaledWidth / 2 + bottomRight.contentSize.width / 2 + l_fCenterPosX, -rescaledHeight / 2 + -bottomRight.contentSize.height * 0.5 + l_fCenterPosY)] ;
	//top.scaleX = horizontalScale;
	//[top setPosition : CGPointMake(0 + l_fCenterPosX, rescaledHeight / 2 + topLeft.contentSize.height * 0.5 + l_fCenterPosY)] ;
	//bottom.scaleX = horizontalScale;
	//[bottom setPosition : CGPointMake(0 + l_fCenterPosX, -rescaledHeight / 2 - bottomLeft.contentSize.height * 0.5 + l_fCenterPosY)] ;
	//left.scaleY = verticalScale;
	//[left setPosition : CGPointMake(-rescaledWidth / 2 - topLeft.contentSize.width / 2 + l_fCenterPosX, 0 + l_fCenterPosY)] ;
	//right.scaleY = verticalScale;
	//[right setPosition : CGPointMake(rescaledWidth / 2 + topRight.contentSize.width / 2 + l_fCenterPosX, 0 + l_fCenterPosY)] ;
	//[centre setPosition : CGPointMake(l_fCenterPosX, l_fCenterPosY)] ;



	//try below function to generate 2 triangle vertices data
	//GetDrawQuadFVFDataByTwoTriangles
	//Position corners
	//[topLeft setPosition : CGPointMake(-rescaledWidth / 2 - topLeft.contentSize.width / 2 + l_fCenterPosX, l_vHalfCenterSize.y + topLeft.contentSize.height * 0.5 + l_fCenterPosY)];
	Vector3 l_vLeftUpPos	=	Vector3(-l_vHalfCenterSize.x - l_vHalfCornerSize.x + l_fCenterPosX, l_fCenterPosY-l_vHalfCenterSize.y - l_vHalfCornerSize.y ,0.f);
	Vector3 l_vCenterTop	=	Vector3( l_fCenterPosX, l_vLeftUpPos.y,0.f);
	Vector3 l_vTopRightPos	=	Vector3( l_vHalfCenterSize.x + l_vHalfCornerSize.x + l_fCenterPosX, l_vLeftUpPos.y, 0.f);

	Vector3 l_vLeftCenter	=	Vector3( l_vLeftUpPos.x,  l_fCenterPosY, 0.f);
	Vector3 l_vCenter		=	Vector3( l_vCenterTop.x,  l_vLeftCenter.y, 0.f);
	Vector3 l_vRightCenter	=	Vector3( l_vTopRightPos.x,l_vLeftCenter.y, 0.f);

	Vector3 l_vLeftBottom	=	Vector3( l_vLeftUpPos.x, l_fCenterPosY +l_vHalfCenterSize.y + l_vHalfCornerSize.y, 0.f);
	Vector3 l_vCenterBottom	=	Vector3( l_vCenterTop.x, l_vLeftBottom.y, 0.f);
	Vector3 l_vRightBottom	=	Vector3( l_vTopRightPos.x, l_vLeftBottom.y, 0.f);

	Vector3 l_vGridCenterPosArray[eCorrnerType::eCorrnerType_MAX] = 
	{
		l_vLeftUpPos,l_vCenterTop,l_vTopRightPos,l_vLeftCenter,l_vCenter,l_vRightCenter,l_vLeftBottom,l_vCenterBottom,l_vRightBottom
	};
	Vector2 l_vGridSizeArray[eCorrnerType::eCorrnerType_MAX] =
	{
		m_vOriginalLeftupSize,
		Vector2(l_fRescaledCenterWidth,m_vOriginalCenterSize.y),
		m_vOriginalLeftupSize,
		//
		Vector2(m_vOriginalLeftupSize.x,l_fRescaledCenterHeight),
		Vector2(l_fRescaledCenterWidth,l_fRescaledCenterHeight),
		Vector2(m_vOriginalLeftupSize.x,l_fRescaledCenterHeight),
		//
		m_vOriginalLeftupSize,
		Vector2(l_fRescaledCenterWidth,m_vOriginalCenterSize.y),
		m_vOriginalLeftupSize,
	};
	Vector3 l_vTempPos[A_QUAD_4_VERTICES];
	for (int i = 0; i < eCorrnerType::eCorrnerType_MAX; ++i)
	{
		CenterPosTo4Vertices(l_vGridCenterPosArray[i], l_vGridSizeArray[i], l_vTempPos);
		QuadTo2TriangleVertices(l_vTempPos, m_vAllGridPosVector);
	}
}



cNineGridFrame::cNineGridFrame(Vector2 e_vOriginalCenterSize, Vector2 e_vOriginalLeftupSize) :cBaseImage(L"QoocNineGridFrame")
{
	m_vOriginalLeftupSize = e_vOriginalLeftupSize;
	m_vOriginalCenterSize = e_vOriginalCenterSize;
}

cNineGridFrame::cNineGridFrame(const char* e_strImageName, float* e_pfNineTexturesUV) :cBaseImage(e_strImageName)
{
}

cNineGridFrame::cNineGridFrame(cPuzzleImageUnit* e_pLeftUp, cPuzzleImageUnit* e_pRightBottom):cBaseImage(e_pRightBottom)
{
	cPuzzleImageUnit* l_pPIUnit = (cPuzzleImageUnit*)e_pLeftUp;
	m_vOriginalLeftupSize = Vector2(l_pPIUnit->GetWidth(), l_pPIUnit->GetHeight());
	for (int i = 0; i < eCorrnerType_MAX; ++i)
	{
		m_pImageArray[i] = l_pPIUnit;
		if (l_pPIUnit)
		{
			float* l_pUV = UVToTriangleStrip(l_pPIUnit->GetUV());
			memcpy(m_Vertices[i].fUV, l_pUV, sizeof(Vector2)*4);
			if (i == eCorrnerType_Center)
			{
				m_vOriginalCenterSize = Vector2(l_pPIUnit->GetWidth(), l_pPIUnit->GetHeight());
			}
			l_pPIUnit->GetNext();
		}
		//m_pfTexCoordinate[i*4  ] = ;
		//m_pfTexCoordinate[i*4+1] = m_fUV[1];
		//m_pfTexCoordinate[i*4+2] = i*l_fStep+l_fStep+m_fUV[0];
		//m_pfTexCoordinate[i*4+3] = m_fUV[3];
	}
}

cNineGridFrame::~cNineGridFrame()
{
}


void	cNineGridFrame::Render()
{
}

void	cNineGridFrame::DebugRender()
{
	m_vAllGridPosVector;
}