#include "stdafx.h"
#include "IOSMManager.h"
namespace IOSM
{
	cIOSMIOManager::cIOSMIOManager()
	{
		m_eIODeviceBehaviorList = eIODBL_MAX;
		//m_pIOSMDriverInterface = new cIOSMDriverInterface();
		m_pIOSMDriverInterface = 0;
		m_pCurrentIOSMIOData = 0;
		m_pCurrentIOSMBaseDevice = 0;
		m_pCurrentIOSMDeviceBehavior = 0;
	}

	cIOSMIOManager::~cIOSMIOManager()
	{
		SAFE_DELETE(m_pIOSMDriverInterface);
	}

	void	cIOSMIOManager::HandleElementData(TiXmlElement*e_pTiXmlElement)
	{
		const WCHAR*l_strValue = e_pTiXmlElement->Value();
		COMPARE_VALUE("Device")
		{
			m_eIODeviceBehaviorList = eIODBL_BASE_DEVICE;
		}
		else
		COMPARE_VALUE("IOSM")
		{
			m_eIODeviceBehaviorList = eIODBL_IOSM_ROOT;
		}
		else
		COMPARE_VALUE("DeviceBehavior")
		{
			l_strValue = e_pTiXmlElement->FirstAttribute()->Value();
			cIOSMDeviceBehavior*l_pIOSMDeviceBehavior = 0;
			COMPARE_VALUE_WITH_DEFINE(cProbableIO::TypeID)
			{
				l_pIOSMDeviceBehavior = new cProbableIO(m_pCurrentIOSMBaseDevice);
				m_eIODeviceBehaviorList = eIODBL_PROBABLE_IO;
			}
			else
			COMPARE_VALUE_WITH_DEFINE(cActiveIOTimesWithSingnalCircle::TypeID)
			{
				l_pIOSMDeviceBehavior = new cActiveIOTimesWithSingnalCircle(m_pCurrentIOSMBaseDevice);
				m_eIODeviceBehaviorList = eIODBL_ACTIVE_IO_TIMES_WITH_SINGNAL_CIRCLE;
			}
			else
			COMPARE_VALUE_WITH_DEFINE(cListenMultiSingnalToActiveIO::TypeID)
			{
				l_pIOSMDeviceBehavior = new cListenMultiSingnalToActiveIO(m_pCurrentIOSMBaseDevice);
				m_eIODeviceBehaviorList = eIODBL_LISTEN_MULTI_SINGNAL_TO_ACTIVE_IO;
			}
			else
			COMPARE_VALUE_WITH_DEFINE(cSendHighSingnal::TypeID)
			{
				l_pIOSMDeviceBehavior = new cSendHighSingnal(m_pCurrentIOSMBaseDevice);
				m_eIODeviceBehaviorList = eIODBL_SEND_HIGN_SINGNAL;
			}
			else
			COMPARE_VALUE_WITH_DEFINE(cSendLowSingnal::TypeID)
			{
				l_pIOSMDeviceBehavior = new cSendLowSingnal(m_pCurrentIOSMBaseDevice);
				m_eIODeviceBehaviorList = eIODBL_SEND_LOW_SINGNAL;
			}
			else
			{
				UT::ErrorMsg((WCHAR*)l_strValue,L"no such behavior");
			}
			m_pCurrentIOSMDeviceBehavior = l_pIOSMDeviceBehavior;
			l_pIOSMDeviceBehavior->SetName(l_strValue);
			bool	l_b = m_pCurrentIOSMBaseDevice->AddObject(l_pIOSMDeviceBehavior);
			assert(l_b);
			e_pTiXmlElement->m_bDone  = true;//to skip current parse
		}
		if( e_pTiXmlElement->m_bDone )
			return;
		switch(m_eIODeviceBehaviorList)
		{
			case eIODBL_IOSM_ROOT:
				ProcessIOSMData();
				break;
			case eIODBL_BASE_DEVICE:
				ProcessBaseDeviceData();
				break;
			case eIODBL_PROBABLE_IO:
				ProcessProbableIOData();
				break;
			case eIODBL_ACTIVE_IO_TIMES_WITH_SINGNAL_CIRCLE:
				ProcessActiveIOTimesWithSingnalCircleData();
				break;
			case eIODBL_LISTEN_MULTI_SINGNAL_TO_ACTIVE_IO:
				ProcessListenMultiSingnalToActiveIOData();
				break;
			case eIODBL_SEND_HIGN_SINGNAL:
				ProcessSendHighSingnalData();
				break;
			case eIODBL_SEND_LOW_SINGNAL:
				ProcessSendLowSingnalData();
				break;
		}
	}

	void	cIOSMIOManager::ProcessProbableIOData()
	{
		cProbableIO*l_pProbableIO = (cProbableIO*)this->m_pCurrentIOSMDeviceBehavior;
		PARSE_NAME_VALUE_START
			COMPARE_NAME("Probability")
			{
				l_pProbableIO->m_iProbability = _wtoi(l_strValue);
			}
			else
			COMPARE_NAME("IOList")
			{
				WCHAR*l_strForIOList = wcstok((WCHAR*)l_strValue,L",");
				while(l_strForIOList)
				{
					assert(this->m_pIOSMDriverInterface->GetObject(l_strForIOList)&&"fail to get IOData");
					bool	l_b = l_pProbableIO->m_ProbableIOList.AddObject(this->m_pIOSMDriverInterface->GetObject(l_strForIOList));
					assert(l_b);
					l_strForIOList = wcstok(0,L",");
				}
			}
			else
			COMPARE_NAME("Singnal")
			{
				l_pProbableIO->m_bSendSingnal = _wtoi(l_strValue)?true:false;
			}
		PARSE_NAME_VALUE_END
	}

	void	cIOSMIOManager::ProcessActiveIOTimesWithSingnalCircleData()
	{
		cActiveIOTimesWithSingnalCircle*l_pActiveIOTimesWithSingnalCircle = (cActiveIOTimesWithSingnalCircle*)this->m_pCurrentIOSMDeviceBehavior;
		PARSE_NAME_VALUE_START
			COMPARE_NAME("Times")
			{
				l_pActiveIOTimesWithSingnalCircle->m_iTimes = _wtoi(l_strValue);
			}
		PARSE_NAME_VALUE_END
	}

	void	ProcessIOListData(WCHAR*e_strValue,cIOSMDriverInterface*e_pIOSMDriverInterface,cNamedTypedObjectVector<cIOSMIOData>*e_pList)
	{
		WCHAR*l_strForIOList = wcstok(e_strValue,L",");
		while(l_strForIOList)
		{
			assert(e_pIOSMDriverInterface->GetObject(l_strForIOList)&&"fail to get IOData");
			bool	l_b = e_pList->AddObject(e_pIOSMDriverInterface->GetObject(l_strForIOList));
			assert(l_b);
			l_strForIOList = wcstok(0,L",");
		}	
	}

	void	cIOSMIOManager::ProcessListenMultiSingnalToActiveIOData()
	{
		cListenMultiSingnalToActiveIO*l_pListenMultiSingnalToActiveIO = (cListenMultiSingnalToActiveIO*)this->m_pCurrentIOSMDeviceBehavior;
		PARSE_NAME_VALUE_START
			COMPARE_NAME("IOList")
			{
				ProcessIOListData((WCHAR*)l_strValue,m_pIOSMDriverInterface,&l_pListenMultiSingnalToActiveIO->m_ActiveIOList);
			}
			else
			COMPARE_NAME("Singnal")
			{
				l_pListenMultiSingnalToActiveIO->m_bSendSingnal = _wtoi(l_strValue)?true:false;
			}
		PARSE_NAME_VALUE_END
	}

	void	cIOSMIOManager::ProcessSendHighSingnalData()
	{
		cSendHighSingnal*l_pSendHighSingnal = (cSendHighSingnal*)this->m_pCurrentIOSMDeviceBehavior;
		PARSE_NAME_VALUE_START
			COMPARE_NAME("IOList")
			{
				ProcessIOListData((WCHAR*)l_strValue,m_pIOSMDriverInterface,&l_pSendHighSingnal->m_ActiveIOList);
			}
		PARSE_NAME_VALUE_END	
	}

	void	cIOSMIOManager::ProcessSendLowSingnalData()
	{
		cSendLowSingnal*l_pSendLowSingnal = (cSendLowSingnal*)this->m_pCurrentIOSMDeviceBehavior;
		PARSE_NAME_VALUE_START
			COMPARE_NAME("IOList")
			{
				ProcessIOListData((WCHAR*)l_strValue,m_pIOSMDriverInterface,&l_pSendLowSingnal->m_ActiveIOList);
			}
		PARSE_NAME_VALUE_END		
	}

	void	cIOSMIOManager::ProcessIOSMData()
	{
		SAFE_DELETE(this->m_pIOSMDriverInterface);
		m_pIOSMDriverInterface = new cIOSMDriverInterface;
		//const WCHAR*l_strFileName = ;
		std::wstring	l_strFileName = UT::CharToWchar(this->m_strCurrentDirectory);
		l_strFileName += this->m_pCurrentTiXmlElement->FirstAttribute()->Value();
		bool	l_b = m_pIOSMDriverInterface->Parse(UT::WcharToChar(l_strFileName.c_str()));
		assert(l_b);
	}

	void	cIOSMIOManager::ProcessBaseDeviceData()
	{
		cIOSMBaseDevice*l_pIOSMBaseDevice;
		const WCHAR*l_strType = 0;
		const WCHAR*l_strDeviceName = 0;
		PARSE_NAME_VALUE_START
			COMPARE_NAME("Name")
			{
				l_strDeviceName = l_pAttribute->Value();
			}
			else
			COMPARE_NAME("Type")
			{
				l_strType = l_pAttribute->Value();
			}
		PARSE_NAME_VALUE_END
		m_pCurrentIOSMIOData = this->m_pIOSMDriverInterface->GetObject(l_strDeviceName);
		assert(m_pCurrentIOSMIOData);
		l_pIOSMBaseDevice = new cIOSMBaseDevice(m_pCurrentIOSMIOData);
		m_pCurrentIOSMBaseDevice = l_pIOSMBaseDevice;
		bool	l_b = this->AddObject(l_pIOSMBaseDevice);
		assert(l_b);
	}

	void	cIOSMIOManager::Init()
	{
	
	}

	void	cIOSMIOManager::Update(float e_fElpaseTime)
	{
		int	l_iNum = m_pIOSMDriverInterface->Count();
		if( l_iNum )
		{
			cIOSMIOData**l_ppIOSMIOData = &(*m_pIOSMDriverInterface->GetList())[0];
			for(int i=0;i<l_iNum;++i)
			{
				l_ppIOSMIOData[i]->Update(e_fElpaseTime);
			}
		}
		l_iNum = this->Count();
		if( l_iNum )
		{
			cIOSMBaseDevice**l_ppIOSMBaseDevice = &this->m_ObjectList[0];
			for(int i=0;i<l_iNum;++i)
			{
				l_ppIOSMBaseDevice[i]->Update(e_fElpaseTime);
			}
		}
	}

#ifdef WIN32
	void	AddListDataToAttribute(cNamedTypedObjectVector<cIOSMIOData>*e_pList,ATG::XMLWriter*e_pXMLWriter)
	{
		wstring	l_strForAllI;
		for( int j=0;j<e_pList->Count();++j )
		{
			l_strForAllI += (*e_pList)[j]->GetName();
			if( j != e_pList->Count()-1 )
				l_strForAllI += L",";
		}
		e_pXMLWriter->AddAttribute("IOList",UT::WcharToChar(l_strForAllI.c_str()));	
	}

	bool	cIOSMIOManager::Export(CHAR* strFileName)
	{
		char*l_strDirectory = UT::GetDirectoryWithoutFileName(strFileName);
		std::string	l_strIODataFileName = l_strDirectory;
		l_strIODataFileName += UT::GetFileNameWithoutFullPath(strFileName);
		l_strIODataFileName += "IOData__.xml";
		if(!m_pIOSMDriverInterface->Export((char*)l_strIODataFileName.c_str()))
		{
			return false;
		}
		ATG::XMLWriter	l_XMLWriter(strFileName);
		l_XMLWriter.StartElement("IOSM");
		l_XMLWriter.AddAttribute("IODatafile",UT::GetFileNameWithoutFullPath((char*)l_strIODataFileName.c_str()));
		int	l_iNumDevice = this->Count();
		for( int i=0;i<l_iNumDevice;++i )
		{
			cIOSMBaseDevice*l_pIOSMBaseDevice = (*this)[i];
			l_XMLWriter.StartElement("Device");
			l_XMLWriter.AddAttribute("Type",l_pIOSMBaseDevice->Type());
			l_XMLWriter.AddAttribute("Name",l_pIOSMBaseDevice->GetName());
			int	l_iNumBaseDevice = l_pIOSMBaseDevice->Count();
			for( int k=0;k<l_iNumBaseDevice;++k )
			{
				cIOSMDeviceBehavior*l_pIOSMDeviceBehavior = (*l_pIOSMBaseDevice)[k];
				l_XMLWriter.StartElement("DeviceBehavior");
				l_XMLWriter.AddAttribute("Type",l_pIOSMDeviceBehavior->Type());
				l_XMLWriter.StartElement("Data");
				if( !wcscmp(l_pIOSMDeviceBehavior->Type(),cProbableIO::TypeID) )
				{
					cProbableIO*l_pProbableIO = (cProbableIO*)l_pIOSMDeviceBehavior;
					l_XMLWriter.AddAttribute("Probability",l_pProbableIO->m_iProbability);
					l_XMLWriter.AddAttribute("Singnal",l_pProbableIO->m_bSendSingnal?1:0);
					AddListDataToAttribute(&l_pProbableIO->m_ProbableIOList,&l_XMLWriter);
				}
				else
				if( !wcscmp(l_pIOSMDeviceBehavior->Type(),cActiveIOTimesWithSingnalCircle::TypeID) )
				{
					cActiveIOTimesWithSingnalCircle*l_pActiveIOTimesWithSingnalCircle = (cActiveIOTimesWithSingnalCircle*)l_pIOSMDeviceBehavior;
					l_XMLWriter.AddAttribute("Times",l_pActiveIOTimesWithSingnalCircle->m_iTimes);
				}
				else
				if( !wcscmp(l_pIOSMDeviceBehavior->Type(),cListenMultiSingnalToActiveIO::TypeID) )
				{
					cListenMultiSingnalToActiveIO*l_pMultiActiveIO = (cListenMultiSingnalToActiveIO*)l_pIOSMDeviceBehavior;
					l_XMLWriter.AddAttribute("On",l_pMultiActiveIO->m_bSendSingnal?true:false);
					AddListDataToAttribute(&l_pMultiActiveIO->m_ActiveIOList,&l_XMLWriter);
				}
				else
				if( !wcscmp(l_pIOSMDeviceBehavior->Type(),cSendLowSingnal::TypeID) )
				{
					cSendLowSingnal*l_pSendLowSingnal = (cSendLowSingnal*)l_pIOSMDeviceBehavior;
					AddListDataToAttribute(&l_pSendLowSingnal->m_ActiveIOList,&l_XMLWriter);
				}
				else
				if( !wcscmp(l_pIOSMDeviceBehavior->Type(),cSendHighSingnal::TypeID) )
				{
					cSendHighSingnal*l_pSendHighSingnal = (cSendHighSingnal*)l_pIOSMDeviceBehavior;
					AddListDataToAttribute(&l_pSendHighSingnal->m_ActiveIOList,&l_XMLWriter);
				}
				else
				{
					assert(0&&"device type error");
				}
				l_XMLWriter.EndElement();//end Data
				l_XMLWriter.EndElement();//end DeviceBehavior
			}
			l_XMLWriter.EndElement();//end Device
		}
		l_XMLWriter.EndElement();//end IOSM
		l_XMLWriter.Close();
		return true;
	}
//end WIN32
#endif
//end namespace
}