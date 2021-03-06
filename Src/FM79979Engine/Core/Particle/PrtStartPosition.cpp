#include "../stdafx.h"
#include "ParticleBase.h"
#include "PrtStartPosition.h"
namespace FATMING_CORE
{
	const wchar_t*         cPrtStartPositionInitByFrame::TypeID( L"cPrtStartPositionInitByFrame" );
	const wchar_t*         cPrtStartPositionInitBySquareRange::TypeID( L"cPrtStartPositionInitBySquareRange" );

	char*	cPrtStartPositionInitBySquareRange::UsageExplanation =
	{
		"width weight deepth equal xyz,it will appear by -value,value\nwidth weight deepth 就是xyz,例子會出現在正值跟負值之間的亂數"
	};
	char*	cPrtStartPositionInitByFrame::UsageExplanation =
	{
		"lazy to do"
	};
	NamedTypedObject*  cPrtStartPositionInitBySquareRange::Clone()
	{
		cPrtStartPositionInitBySquareRange* l_p = new cPrtStartPositionInitBySquareRange; 
		l_p->SetDataByDataString(this->GetOutputDataString());
		return l_p; 
	};
	char*	cPrtStartPositionInitBySquareRange::GetDataInfo()
	{
		sprintf(m_sTemp,"Width:=%.2f\nHeight:%.2f\nDeepth:=%.2f\0",m_fWidth,m_fHeight,m_fDeepth);
		return m_sTemp;
	}
	char*	cPrtStartPositionInitBySquareRange::GetOutputDataString()
	{
		sprintf(m_sTemp,"%.2f,%.2f,%.2f\0",m_fWidth,m_fHeight,m_fDeepth);
		return m_sTemp;
	}
	//input the output data string,and analyze it
	bool	cPrtStartPositionInitBySquareRange::SetDataByDataString(char*e_pString)
	{
		char*Nexttoken = 0;
		char* l_pString = strtok(e_pString,",");
		this->m_fWidth = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_fHeight = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_fDeepth = (float)atof(l_pString);
		if( strtok(0,",") )
			return false;
		return true;
	}

	void	cPrtStartPositionInitByFrame::SetSubFrameName(WCHAR*e_pString)
	{
		assert(e_pString&&"the attach frame name is empty");
		int	l_iLength = wcslen(e_pString);
		memcpy(m_psSubFrameName,e_pString,l_iLength);		
		m_psSubFrameName[l_iLength] = L'\0';
	}
	NamedTypedObject*  cPrtStartPositionInitByFrame::Clone()
	{
		cPrtStartPositionInitByFrame* l_p = new cPrtStartPositionInitByFrame; 
		l_p->SetFrame(m_pFrame);
		l_p->SetAttachFrameType(m_eAttachFrameType);
		l_p->SetSubFrameName(m_psSubFrameName);
		l_p->SetOffsetPos(this->m_vOffsetPos);
		l_p->SetName(GetName());
		return l_p; 
	};
	char*	cPrtStartPositionInitByFrame::GetDataInfo()
	{
		if(!m_pFrame)
			return "none Data ot Data error";
		std::string	l_strAttachFrameType = UT::WcharToChar(AttachFrameTypeToString(m_eAttachFrameType));
		sprintf(m_sTemp,"Type:%s\nModelName:%s\nSubFrameName:%s\nOffsetPos:%.2f,%.2f,%.2f\0",l_strAttachFrameType.c_str(),m_pFrame->GetName(),wcslen(this->m_psSubFrameName)?m_psSubFrameName:L"",m_vOffsetPos.x,m_vOffsetPos.y,m_vOffsetPos.z);
		return m_sTemp;
	}
	char*	cPrtStartPositionInitByFrame::GetOutputDataString()
	{
		WCHAR*l_strName = 0;
		switch(m_eAttachFrameType)
		{
			case eAFT_CAMERA:
				l_strName = L"Camera";
				break;
			case eAFT_FRAME_OF_FRAME:
				if( m_pFrame )
					l_strName = (WCHAR*)m_pFrame->GetName();
				else
					l_strName = L" ";
				break;
			case eAFT_FRAME_OF_MESH:
				if( m_pSubFrameOfModel )
					l_strName = (WCHAR*)m_pSubFrameOfModel->GetName();
				else
					l_strName = L" ";
				break;
		}

		WCHAR *l_strNAme = L"";
        if(wcslen(this->m_psSubFrameName))
            l_strNAme = (WCHAR*)m_psSubFrameName;
		std::string	l_strFrameName = m_pFrame?m_pFrame->GetCharName():" ";
		std::string	l_strSubFrameName = wcslen(this->m_psSubFrameName)?UT::WcharToChar(m_psSubFrameName):" ";
		sprintf(m_sTemp,"%d,%s,%s,%.2f,%.2f,%.2f\0",(int)m_eAttachFrameType,l_strFrameName.c_str(),l_strSubFrameName.c_str(),m_vOffsetPos.x,m_vOffsetPos.y,m_vOffsetPos.z);
		return m_sTemp;
	}
	//input the output data string,and analyze it
	bool	cPrtStartPositionInitByFrame::SetDataByDataString(char*e_pString)
	{
		char*Nexttoken = 0;
		char* l_pString = strtok(e_pString,",");
		m_eAttachFrameType = (eAttachFrameType)atoi(l_pString);

		l_pString = strtok(0,",");
		if( m_eAttachFrameType != eAFT_CAMERA )
		{
			assert(0&&"current not support");
			//WCHAR	l_tempForGetObject[MAX_PATH];
			//CHAR_TO_WCHAR(l_pString,l_tempForGetObject);
			//m_pFrame = g_pPrt->GetMeshList()->GetObject(l_tempForGetObject);
			//CT::ErrorMsg(m_pFrame?true:false,l_pString,"can't find mesh");
		}
		l_pString = strtok(0,",");
		CHAR_TO_WCHAR(l_pString,m_psSubFrameName);
		l_pString = strtok(0,",");//offset pos x
		this->m_vOffsetPos.x = (float)atof(l_pString);
		l_pString = strtok(0,",");//offset pos y
		this->m_vOffsetPos.y = (float)atof(l_pString);
		l_pString = strtok(0,",");//offset pos z
		this->m_vOffsetPos.z = (float)atof(l_pString);
		if( strtok(0,",") )
			return false;
		switch(m_eAttachFrameType)
		{
			case eAFT_CAMERA:
				assert(0&&"current not support");
				//m_pFrame = g_pEngine->m_pCamera;
				break;
			case eAFT_FRAME_OF_FRAME:
				if( m_pFrame )
				{
					assert(0&&"current not support");
					//cGameApp::OutputDebugInfoString(L"!!----I am lazy to write more flexible,support Frame-----!!\n");
					//if( m_pFrame->Type() == MESH::CDXUTMeshFrame::TypeID || 
					//	m_pFrame->Type() == MESH::CDXUTMeshFile::TypeID )
					//{
					//	MESH::CDXUTMeshFrame*l_pFrame = (MESH::CDXUTMeshFrame*)m_pFrame;
					//	m_pSubFrameOfModel = l_pFrame->FindMesh(m_psSubFrameName);
					//	CT::ErrorMsgW(m_pSubFrameOfModel?true:false,m_psSubFrameName,L"not exist");
					//}
					//else
					//	assert(0&&"eAFT_FRAME_OF_FRAME type error");
				}
				break;
			case eAFT_FRAME_OF_MESH:
				assert(0&&"current not support");
				//if( m_pFrame )
				//if( m_pFrame->Type() == MESH::CAnimationAgent::TypeID )
				//{
				//	CHAR	l_tempStringForFrame[MAX_PATH];
				//	WCHAR_TO_CHAR(m_psSubFrameName,l_tempStringForFrame);
				//	MESH::CAnimationAgent*l_p = (MESH::CAnimationAgent*)m_pFrame;
				//	this->m_pD3DMatrix = CT::GetFrameMatrixByName(l_tempStringForFrame,l_p->GetRootFrame());
				//	CT::ErrorMsgW(m_pD3DMatrix?true:false,m_psSubFrameName,L"not exist");
				//}
				break;
		}
		return true;
	}

	WCHAR*	AttachFrameTypeToString(eAttachFrameType e_eAttachFrameType)
	{
		switch(e_eAttachFrameType)
		{
			case eAFT_CAMERA:
				return L"CAMERA";
				break;
			case eAFT_MODEL:
				return L"MODEL";
				break;
			case eAFT_FRAME_OF_FRAME:
				return L"FRAME_OF_FRAME";
				break;
			case eAFT_FRAME_OF_MESH:
				return L"FRAME_OF_MESH";
				break;
		}
		return 0;
	}
//emd namespace FATMING_CORE
}