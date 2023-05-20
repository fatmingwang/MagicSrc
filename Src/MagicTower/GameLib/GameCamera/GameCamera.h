#pragma once


class cGameCamera :public NamedTypedObject, cSingltonTemplate<cGameCamera>
{
	sRandomAssignValueWithTime<float>		m_RandomAssignValueWithTime;

	Vector2							m_vViewRange;
	Vector4							m_vCurrentViewRect;
	cOrthogonalCamera				m_Camera;
	sMoveToDestTimeCounter<Vector4>	m_CameraZoomChangeBehavior;

	bool							m_bZoomOut;
	//
	cGameCamera();
	~cGameCamera();
public:
	SINGLETON_BASIC_FUNCTION(cGameCamera);
	DEFINE_TYPE_INFO();
	void		Update(float e_fElpaseTime);
	void		Render();
	void		DebugRender();
	void		KeyUp(unsigned char e_ucKey);

	Vector4		GetViewRect();
};