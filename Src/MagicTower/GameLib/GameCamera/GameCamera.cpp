#include "stdafx.h"
#include "GameCamera.h"

//#define	FTC_GAME_RESOLUTION_WIDTH	720
//#define	FTC_GAME_RESOLUTION_HEIGHT	1280

#define	FTC_GAME_RESOLUTION_WIDTH	1280
#define	FTC_GAME_RESOLUTION_HEIGHT	720

TYPDE_DEFINE_MARCO(cGameCamera)

//class cCameraZoomChangeBehavior:public NamedTypedObject
//{
//	sMoveToDestTimeCounter<Vector4>	m_CameraZoomChangeBehavior;
//	cOrthogonalCamera				*m_pCamera;
//public:
//	cCameraZoomChangeBehavior(cOrthogonalCamera* e_pCamera)
//	{
//		m_pCamera = e_pCamera;
//	}
//	virtual ~cCameraZoomChangeBehavior()
//	{
//
//	}
//	void Update(float e_fElpaseTime)
//	{
//		if (m_pCamera&&!m_CameraZoomChangeBehavior.IsDone())
//		{
//			m_CameraZoomChangeBehavior.Update(e_fElpaseTime);
//			m_pCamera->SetViewRect(m_CameraZoomChangeBehavior.vCurrentPos);
//		}
//	}
//
//	void	SetupDate(Vector4 e_vCurrent, Vector4 e_vEnd,float e_fTime)
//	{
//		m_CameraZoomChangeBehavior.SetTimeAndPosData(
//			e_vEnd, e_vEnd, e_fTime);
//		m_CameraZoomChangeBehavior.Start();
//	}
//};



cGameCamera::cGameCamera()
{
	m_bZoomOut = false;
	m_vTargetPosPos = Vector2::Zero;
	//m_Camera.SetViewRect(Vector4(0, 0, FTC_GAME_RESOLUTION_WIDTH, FTC_GAME_RESOLUTION_HEIGHT));
	auto l_vViewRect = Vector4(0, 0, FTC_GAME_RESOLUTION_WIDTH, FTC_GAME_RESOLUTION_HEIGHT);
	m_CameraZoomChangeBehavior.vCurrentPos = l_vViewRect;
	m_vCurrentViewRect = l_vViewRect;
	m_Camera.SetViewRect(l_vViewRect);
	m_CameraZoomChangeBehavior.MoveToDestinationTC.bTragetTimrReached = true;

	m_RandomAssignValueWithTime.SetData(1.f, -15.f, 15.f);
	m_RandomAssignValueWithTime.m_TC.bTragetTimrReached = true;
	cFAIBaseBehave::m_sfGetGameViewRect = std::bind(&cGameCamera::GetViewRect, this);
}

cGameCamera::~cGameCamera()
{
}

void cGameCamera::Update(float e_fElpaseTime)
{
	e_fElpaseTime = 0.016f;
	if(!m_CameraZoomChangeBehavior.IsDone())
	{
		m_CameraZoomChangeBehavior.Update(e_fElpaseTime);
		m_Camera.SetViewRect(m_CameraZoomChangeBehavior.vCurrentPos);
		m_vCurrentViewRect = m_CameraZoomChangeBehavior.vCurrentPos;
	}

	if (!m_RandomAssignValueWithTime.IsFinish())
	{
		m_RandomAssignValueWithTime.Update(e_fElpaseTime);
		if (m_RandomAssignValueWithTime.IsFinish())
		{
			m_Camera.SetViewRect(m_vCurrentViewRect);			
		}
		else
		{
			int  l_iMovingDirection = rand() % 4;
			auto l_vViewRect = m_vCurrentViewRect;
			float l_fValue = m_RandomAssignValueWithTime.GetData();
			switch (l_iMovingDirection)
			{
				case 0:
					l_vViewRect.x -= l_fValue;
					l_vViewRect.y -= l_fValue;
					l_vViewRect.z -= l_fValue;
					l_vViewRect.w -= l_fValue;
					break;
				case 1:
					l_vViewRect.x += l_fValue;
					l_vViewRect.y -= l_fValue;
					l_vViewRect.z += l_fValue;
					l_vViewRect.w -= l_fValue;
					break;
				case 2:
					l_vViewRect.x += l_fValue;
					l_vViewRect.y += l_fValue;
					l_vViewRect.z -= l_fValue;
					l_vViewRect.w -= l_fValue;
					break;
				case 3:
					l_vViewRect.x -= l_fValue;
					l_vViewRect.y += l_fValue;
					l_vViewRect.z -= l_fValue;
					l_vViewRect.w += l_fValue;
					break;
			}
			m_Camera.SetViewRect(l_vViewRect);
		}
	}
}

void cGameCamera::Render()
{
	//auto l_vPos = m_Camera.GetWorldPosition();
	//m_Camera.Render(Vector2(l_vPos.x, l_vPos.y));
	m_Camera.Render();
}

void	cGameCamera::DebugRender()
{
	auto l_vRect = m_Camera.GetViewRect();
	auto l_vPos = l_vRect.GetCenter();
	auto l_Width = l_vRect.Width();
	auto l_Height = l_vRect.Height();
	l_vPos.x = l_vRect.x;
	l_vPos.y = l_vRect.y;
	GLRender::RenderRectangle(l_vRect.Width(), l_vRect.Height(),cMatrix44::TranslationMatrix(l_vPos),Vector4::One,3);
}


void cGameCamera::KeyUp(unsigned char e_ucKey)
{
	if (e_ucKey == 'T')
	{
		float l_fTime = 0.3f;
		Vector2 l_vSize(720, 1280);
		if (m_bZoomOut)
		{
			m_CameraZoomChangeBehavior.SetTimeAndPosData(
				Vector4(-l_vSize.x / 2, -l_vSize.y / 2, l_vSize.x + l_vSize.x / 2, l_vSize.y + l_vSize.y / 2),
				this->m_CameraZoomChangeBehavior.vCurrentPos, l_fTime
			);
		}
		else
		{
			m_CameraZoomChangeBehavior.SetTimeAndPosData(
				Vector4(0, 0, l_vSize.x, l_vSize.y),
				m_CameraZoomChangeBehavior.vCurrentPos,
				l_fTime);
		}
		m_CameraZoomChangeBehavior.Start();
		m_bZoomOut = !m_bZoomOut;
	}
	else
	if (e_ucKey == 'U')
	{
		m_RandomAssignValueWithTime.Start();
	}
}

Vector4		cGameCamera::GetViewRect()
{
	return m_Camera.GetViewRect();
	return Vector4();
}

void cGameCamera::SetCurrentPos(Vector2 e_vPos)
{
	//lazy,do object size offset.
	e_vPos.x += 100;
	e_vPos.y += 100;
	if (m_vTargetPosPos == e_vPos)
	{
		return;
	}
	m_TweenyObject.ChancelTween(m_uiLastTweenID, false);
	m_uiLastTweenID = -1;
	float l_fTime = (m_vTargetPosPos - e_vPos).Length()/200.f;
	if (l_fTime > 1.f)
	{
		l_fTime = 1.f;
	}
	m_TweenyObject.AddTweeny(tweeny::easing::enumerated::linear, m_Camera.GetCameraPos(), e_vPos, l_fTime,
		[this](Vector2 e_Vector2)
		{
			m_Camera.SetViewRectByCameraPos(e_Vector2);
		},
		[this]()
		{
			m_Camera.SetViewRectByCameraPos(m_vTargetPosPos);
		}
	);
	//auto l_vPos = m_Camera.GetCameraPos();
	m_vTargetPosPos = e_vPos;
	//m_Camera.SetViewRectByCameraPos(e_vPos);
	//m_Camera.Reset();
	//m_CameraZoomChangeBehavior.vCurrentPos = e_vPos;
	//m_CameraZoomChangeBehavior.vCurrentPos.z = 0;
}
