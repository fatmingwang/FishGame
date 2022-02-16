#ifndef STRING_TO_STRUCT_H
#define STRING_TO_STRUCT_H

#include "../Math/Vector3.h"
#include "../Math/Matrix44.h"
#include "../VS_TO_GNN.h"
class	TiXmlElement;
namespace FATMING_CORE
{
	//x,y
	//"100,200"
	POINT	GetPoint(char* e_str);
	POINT	GetPoint(const char* e_str);
	POINT	GetPoint(const WCHAR* e_str);
	POINT	GetPoint(std::wstring e_str);
	POINT	GetPoint(std::string e_str);

	float	GetFloat(const char* e_str);
	float	GetFloat(const WCHAR* e_str);
	float	GetFloat(std::wstring e_str);
	float	GetFloat(std::string e_str);

	

	int		GetInt(const char* e_str);
	int		GetInt(const WCHAR* e_str);
	int		GetInt(std::wstring e_str);
	int		GetInt(std::string e_str);

	uint64	GetUint64(const WCHAR* e_str);
	uint64	GetUint64(const char* e_str);
	uint64	GetUint64(std::wstring e_str);
	uint64	GetUint64(std::string e_str);

	int64	GetInt64(const WCHAR* e_str);
	int64	GetInt64(const char* e_str);
	int64	GetInt64(std::wstring e_str);
	int64	GetInt64(std::string e_str);
	//left top right bottom
	//"0,0,960,540"
	RECT	GetRectangle(const WCHAR*e_pString);
	RECT	GetRectangle(const char*e_pString);
	RECT	GetRectangle(std::wstring e_str);
	RECT	GetRectangle(std::string e_str);
	RECT*	GetRectangle(int e_iNumRect,WCHAR*e_pString);

	Vector4	GetVector4(const char*e_strValue);
	Vector3	GetVector3(const char*e_strValue);
	Vector2	GetVector2(const char*e_strValue);
	Vector4	GetVector4(const WCHAR*e_strValue);
	Vector3	GetVector3(const WCHAR*e_strValue);
	Vector2	GetVector2(const WCHAR*e_strValue);

	std::vector<Vector2>	GetVector2Vector(TiXmlElement*e_pTiXmlElement);
	std::vector<Vector3>	GetVector3Vector(TiXmlElement*e_pTiXmlElement);
	std::vector<Vector4>	GetVector4Vector(TiXmlElement*e_pTiXmlElement);

	//ensure column order or row order
	cMatrix44	GetMatrix(const char*e_str,bool e_bTranspose = false);
	cMatrix44	GetMatrix(const WCHAR*e_str,bool e_bTranspose = false);
	//this might lost data if parse string again!
	cMatrix44	*GetMatrcies(const char*e_str,int e_iSize,bool e_bTranspose = false);
	//this might lost data if parse string again!
	cMatrix44	*GetMatrcies(const WCHAR*e_str,int e_iSize,bool e_bTranspose = false);


	void	GetUV(char*e_pData,float*e_pUVBuffer);
	void	GetUV(const char*e_pData,float*e_pUVBuffer);
	void	GetUV(const WCHAR*e_pData,float*e_pUVBuffer);


	//input size if u know it,it will reserve the size for vector faster
	std::vector<int>	GetIntegerListByCommaDivide(char*,int e_iSize);
	std::vector<float>	GetFloatListByCommaDivide(char*,int e_iSize);
	std::vector<double>	GetDoubleListByCommaDivide(char*,int e_iSize);

	std::vector<int>	GetIntegerListByCommaDivide(const WCHAR*,int e_iSize);
	std::vector<float>	GetFloatListByCommaDivide(const WCHAR*,int e_iSize);
	std::vector<double>	GetDoubleListByCommaDivide(const WCHAR*,int e_iSize);

	std::vector<std::wstring>	GetWStringListByCommaDivide(const WCHAR*e_str);
	std::vector<std::string>	GetStringListByCommaDivide(const WCHAR*e_str);
	//inpput data and size to generation,only support common type(int float char double....not support structure)
	//although it could add a callback function make it better but slow.
	template<class T>T*	ParseDataToGenerateIntPointer(char*e_strData,int e_iSize)
	{
		T	*l_pData = new	T[e_iSize];
		char*	l_str = strtok(e_strData,", ");
		int	l_iStep = 0;
		while(l_str)
		{
			l_pData[l_iStep] = (T)atof(l_str);

			l_str = strtok(0,", ");
			++l_iStep;
		}
		return l_pData;
	}
	//for
	template<class T>T*	ParseDataToGenerateIntPointer(const WCHAR*e_strData,int e_iSize)
	{
#ifdef ANDROID
		int	l_iLength = wcslen(e_strData);
		char*l_strData = new char[l_iLength+1];
		for(int i=0;i<l_iLength;++i)
			l_strData[i] = (char)(e_strData[i]);
		l_strData[l_iLength] = '\0';
		T	*l_pData = ParseDataToGenerateIntPointer<T>(l_strData,e_iSize);
		delete l_strData;
#else
		T	*l_pData = new	T[e_iSize];
		WCHAR*	l_str = wcstok((WCHAR*)e_strData,L", ");
		int	l_iStep = 0;
		while(l_str)
		{
			l_pData[l_iStep] = (T)_wtof(l_str);
			l_str = wcstok(0,L", ");
			++l_iStep;
		}
		//avoid size is not match
		assert(e_iSize == l_iStep);
#endif
		return l_pData;
	}
	//performance is not good enough...by sometimes we just need it
	template<class T>std::vector<T>	GetValueListByCommaDivide(const WCHAR*e_str)
	{
		std::vector<T>	l_NumerialList;
		WCHAR*	l_str = wcstok((WCHAR*)e_str,L", ");
		while(l_str)
		{
	#ifdef DEBUG
			T	l_i = (T)_wtof(l_str);
			l_NumerialList.push_back(l_i);
	#else
			l_NumerialList.push_back((T)_wtof(l_str));
	#endif
			l_str = wcstok(0,L", ");
		}
		return l_NumerialList;
	}
	//
	std::wstring	ValueToStringW(eDataType e_eDataType);
	std::wstring	ValueToStringW(std::wstring	e_strValue);
	std::wstring	ValueToStringW(std::string	e_strValue);
	std::wstring	ValueToStringW(const WCHAR*e_strValue);
	std::wstring	ValueToStringW(char e_cValue);
	std::wstring	ValueToStringW(const char*e_strValue);
	std::wstring	ValueToStringW(int e_iValue);
	std::wstring	ValueToStringW(size_t e_iValue);
	std::wstring	ValueToStringW(float e_fValue);
	std::wstring	ValueToStringW(double e_fValue);
	std::wstring	ValueToStringW(Vector2 e_vValue);
	std::wstring	ValueToStringW(Vector3 e_vValue);
	std::wstring	ValueToStringW(Vector4 e_vValue);
	std::wstring	ValueToStringW(POINT e_Value);
	std::wstring	ValueToStringW(RECT e_Rect);
	std::wstring	ValueToStringW(cMatrix44 e_mat);
	std::wstring	ValueToStringW(int64 e_uiValue );
	std::wstring	ValueToStringW(uint64 e_uiValue );

	std::string	ValueToString(std::wstring e_strValue);
	std::string	ValueToString(std::string e_strValue);
	std::string	ValueToString(const WCHAR*e_strValue);
	std::string	ValueToString(const char*e_strValue);
	std::string	ValueToString(const char e_cValue);
	std::string	ValueToString(int	e_iValue);
	std::string	ValueToString(size_t e_iValue);
	std::string	ValueToString(float e_fValue);
	std::string	ValueToString(double e_fValue);
	std::string	ValueToString(Vector2 e_vValue);
	std::string	ValueToString(Vector3 e_vValue);
	std::string	ValueToString(Vector4 e_vValue);
	std::string	ValueToString(POINT e_Value);
	std::string	ValueToString(RECT e_Rect);
	std::string	ValueToString(cMatrix44 e_mat);
	std::string	ValueToString(uint64 e_uiValue );
	std::string	ValueToString(int64 e_uiValue );

	std::string	UVToString(float*e_pfUV);

	std::string	MatrixToStringWithIndex(cMatrix44 e_mat);

	std::string	Vector3VectorToString(std::vector<Vector3>* e_pvPosVector);
	std::string	Vector2VectorToString(std::vector<Vector2>* e_pvPosVector);
	std::vector<Vector2>	StringToVector2Vector(WCHAR*e_str,int e_iSize = 0);
	std::vector<Vector3>	StringToVector3Vector(WCHAR*e_str,int e_iSize = 0);
	std::vector<Vector2>	StringToVector2Vector(char*e_str,int e_iSize = 0);
	std::vector<Vector3>	StringToVector3Vector(char*e_str,int e_iSize = 0);

	enum	eShapeType
	{
		eST_RECT = 0,
		eST_CIRCLE,
		eST_MAX,
	};

	enum	eFMAnimationRuleType
	{
		eFM_MPDI = 0,
		eFM_SUB_MPDI,
		eFM_MDPI_LIST,
		eFM_PARTICLE,
		eFM_PARTICLE_GROUP,
		eFM_MAX,
	};

	eShapeType				GetShapeType(const char*e_str);
	eShapeType				GetShapeType(const WCHAR*e_str);
	eFMAnimationRuleType	GetFMAnimationRuleType(const char*e_str);
	eFMAnimationRuleType	GetFMAnimationRuleType(const WCHAR*e_str);

	GLenum	IndexToBlendingIndex( int e_iIndex );
	int BlendingIndexToIndex( GLenum e_eBlendingIndex);

}
#endif