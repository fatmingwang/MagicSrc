#include "stdafx.h"
#include "NineGridFrame.h"

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

	float l_fSizableWidth = e_vContentSize.x - m_pImageArray[eCorrnerType_LeftUp]->GetWidth() - m_pImageArray[eCorrnerType_RightUp]->GetWidth();
	float l_fSizableHeight = e_vContentSize.y - m_pImageArray[eCorrnerType_LeftUp]->GetHeight() - m_pImageArray[eCorrnerType_RightBottom]->GetWidth();

	float l_fHorizontalScale = l_fSizableWidth / l_fCenterImageOriginalWidth;
	float l_fVerticalScale = l_fSizableHeight / l_fCenterImageOriginalHeight;
	float l_fCenterScaleX = l_fHorizontalScale;
	float l_fCenterScaleY = l_fVerticalScale;


	float l_fRescaledWidth = l_fCenterImageOriginalWidth * l_fHorizontalScale;
	float l_fRescaledHeight = l_fCenterImageOriginalHeight * l_fVerticalScale;

	//Position corners
  //[self setAnchorPoint:CGPointMake(0.5f,0.5f)];

	float despx = this->m_vContentSize.x * 0.5f;
	float despy = this->m_vContentSize.y * 0.5f;


	//Position corners
	//[topLeft setPosition : CGPointMake(-rescaledWidth / 2 - topLeft.contentSize.width / 2 + despx, rescaledHeight / 2 + topLeft.contentSize.height * 0.5 + despy)];
	//[topRight setPosition : CGPointMake(rescaledWidth / 2 + topRight.contentSize.width / 2 + despx, rescaledHeight / 2 + topRight.contentSize.height * 0.5 + despy)];
	//[bottomLeft setPosition : CGPointMake(-rescaledWidth / 2 - bottomLeft.contentSize.width / 2 + despx, -rescaledHeight / 2 - bottomLeft.contentSize.height * 0.5 + despy)] ;
	//[bottomRight setPosition : CGPointMake(rescaledWidth / 2 + bottomRight.contentSize.width / 2 + despx, -rescaledHeight / 2 + -bottomRight.contentSize.height * 0.5 + despy)] ;
	//top.scaleX = horizontalScale;
	//[top setPosition : CGPointMake(0 + despx, rescaledHeight / 2 + topLeft.contentSize.height * 0.5 + despy)] ;
	//bottom.scaleX = horizontalScale;
	//[bottom setPosition : CGPointMake(0 + despx, -rescaledHeight / 2 - bottomLeft.contentSize.height * 0.5 + despy)] ;
	//left.scaleY = verticalScale;
	//[left setPosition : CGPointMake(-rescaledWidth / 2 - topLeft.contentSize.width / 2 + despx, 0 + despy)] ;
	//right.scaleY = verticalScale;
	//[right setPosition : CGPointMake(rescaledWidth / 2 + topRight.contentSize.width / 2 + despx, 0 + despy)] ;
	//[centre setPosition : CGPointMake(despx, despy)] ;




	//Position corners
	//[topLeft setPosition : CGPointMake(-rescaledWidth / 2 - topLeft.contentSize.width / 2 + despx, l_fRescaledHeight / 2 + topLeft.contentSize.height * 0.5 + despy)];
	Vector3 l_vLeftUpPos = Vector3(-l_fRescaledWidth/ 2 - m_vOriginalLeftupSize.x / 2 + despx, l_fRescaledHeight / 2 + m_vOriginalLeftupSize.y * 0.5f + despy,0.f);
	Vector3 l_vTopRightPos = Vector3(l_fRescaledWidth / 2 + m_vOriginalLeftupSize.x / 2 + despx, l_fRescaledHeight / 2 + m_vOriginalLeftupSize.y * 0.5f + despy, 0.f);
	Vector3 l_vLeftBottom = Vector3(-l_fRescaledWidth / 2 - m_vOriginalLeftupSize.x / 2 + despx, -l_fRescaledHeight / 2 - m_vOriginalLeftupSize.y * 0.5f + despy, 0.f);
	Vector3 l_vRightBottom = Vector3(l_fRescaledWidth / 2 + m_vOriginalLeftupSize.x / 2 + despx, -l_fRescaledHeight / 2 + -m_vOriginalLeftupSize.y * 0.5f + despy, 0.f);
	Vector3 l_vCenterTop = Vector3(0 + despx, l_fRescaledHeight / 2 + m_vOriginalLeftupSize.y * 0.5f + despy, 0.f);
	Vector3 l_vCenterBottom = Vector3(0 + despx, -l_fRescaledHeight / 2 - m_vOriginalLeftupSize.y * 0.5f + despy, 0.f);
	Vector3 l_vLeftCenter = Vector3(-l_fRescaledWidth / 2 - m_vOriginalLeftupSize.x / 2 + despx, 0 + despy, 0.f);
	Vector3 l_vRightCenter = Vector3(l_fRescaledWidth / 2 + m_vOriginalLeftupSize.x / 2 + despx, 0 + despy, 0.f);
	Vector3 l_vCenter = Vector3(despx, despy, 0.f);
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

void cNineGridFrame::Render()
{
}
