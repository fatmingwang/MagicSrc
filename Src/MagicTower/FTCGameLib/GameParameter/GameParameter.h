#pragma once

struct sGameAttribute
{
	float	fSpeed;
	int		iStrength;
};

template <class TYPE>struct sItemAttribute
{
	std::vector<TYPE>* pDataVector;
	int		iCurrentLV;

	sItemAttribute(std::vector<TYPE>* e_pDataVector);
	TYPE	GetCurrentLVData();
	TYPE	GetDataByLV(int e_iLV);
};



class cGameParameterManager
{
	//for game item.
	std::map<std::string, sItemAttribute<sGameAttribute>*>	m_NameAndItemAttributeMap;
	//for character
	std::map<std::string, std::vector<int>*>				m_NameAndCharacterAttributeMap;
public:

};