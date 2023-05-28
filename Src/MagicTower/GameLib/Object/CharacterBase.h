#pragma once

class	cCharacterBase:public cFAIAnimationCharacter
{
public:
	cCharacterBase(TiXmlElement*e_pTiXmlElement);
	~cCharacterBase();
	virtual	void	RenderStatusInfo();
};