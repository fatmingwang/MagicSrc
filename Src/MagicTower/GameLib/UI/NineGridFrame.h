#pragma once





class cNineGridFrame:public cBaseImage
{
	enum eCorrnerType
	{
		eCorrnerType_LeftUp = 0,//fix size
		eCorrnerType_CenterTop,//dynamic width 
		eCorrnerType_RightUp,//fix size
		eCorrnerType_LeftCenter,//dynamic height
		eCorrnerType_Center,//dynamic width and height
		eCorrnerType_RightCenter,//dynamic height
		eCorrnerType_LeftBottom,//fix size
		eCorrnerType_CenterBottom,//dynamic width 
		eCorrnerType_RightBottom,//fix size
		eCorrnerType_MAX,

	};
	cBaseImage* m_pImageArray[eCorrnerType_MAX];
	s2DVertex	m_Vertices[eCorrnerType_MAX];
	Vector2		m_vOriginalLeftupSize;
	Vector2		m_vOriginalCenterSize;
	Vector2		m_vContentSize;
	void		SetContentSize(Vector2 e_vContentSize);
public:
	cNineGridFrame(const char* e_strImageName,float*e_pfNineTexturesUV);
	//please take order by eCorrnerType_LeftUp to eCorrnerType_RightBottom
	cNineGridFrame(cPuzzleImageUnit*e_pLeftUp, cPuzzleImageUnit*e_pRightBottom);
	~cNineGridFrame();
	virtual	void		Render()override;								//render by texture behavior.
	//cBaseImage(const char* e_strImageName, bool e_bFetchPixels = false);
	//cBaseImage(cBaseImage* e_pBaseImage);					//here is clone,but be sure do not delete original one before all resource release
	//cBaseImage(cTexture* e_pTexture);
	//cPuzzleImageUnit
};