#pragma once

class	cUIInfo
{
	cMPDI*		m_pUIInfoMPDI;
public:
	cUIInfo();
	~cUIInfo();
	void	Update(float e_fElpaseTime);
	void	Render();
	cMPDI*	GetUIInfoMPDI(){return m_pUIInfoMPDI;}
};