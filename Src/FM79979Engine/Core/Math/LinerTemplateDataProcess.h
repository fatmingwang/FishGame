#ifndef _LINER_TEMPLATE_DATA_PROCESS_H_
#define _LINER_TEMPLATE_DATA_PROCESS_H_

#include <vector>
#include "../XML/tinyxml.h"
#include "../XML/StringToStructure.h"
namespace UT
{
	extern float	GetFloatModulus(float e_fValue,float e_fDivision);
};

namespace	FATMING_CORE
{

	class	cLinerTemplateDataInterface//:public cFMAnimationRule
	{
	protected:
		std::vector<float>					m_TimeVector;
		int									GetTimeVectorDataIndex(float e_fPassTime,bool e_bLoop,float*e_pfLerpValue,int*e_piNextDataIndex,float*e_fRestTimeToNextStep,float*e_pfNextStepTimeDiff);
	public:
		virtual	void						Init() = 0;
		virtual	void						Update(float e_fElpaseTime) = 0;
		virtual	void						UpdateByGlobeTime(float e_fElpaseTime) = 0;
		virtual	void						Clear() = 0;
		virtual	float						GetEndTime();
		virtual	void						Rearrange(float e_fNewTime) = 0;
		virtual	float						GetCurrentLerpValue() = 0;
		virtual	void						InvertOrder() = 0;
		void								ChangeTime(int e_iIndex,float e_fTime);
		void								SetTimeVector(std::vector<float>e_TimeVector){ m_TimeVector = e_TimeVector; }
		float*								GetTime(int e_iIndex){if((int)m_TimeVector.size()>e_iIndex && e_iIndex!= -1)return &m_TimeVector[e_iIndex];return 0;}
	};

	template<class T>class	cLinerTemplateDataProcess:public cLinerTemplateDataInterface
	{
		//those data must be equal.
		std::vector<T>				m_LinerDataVector;
		int							m_iCurrentIndex;
		bool						m_bLoop;
		float						m_fRestTimeToNextStep;
		float						m_fNextStepTimeDiff;
		float						m_fTotalPassTime;
		T							m_CurrentData;
		float						m_fCurrentStepLerpValue;
		//void						DoReachLastData()
		//{
		//	int	l_iSize = (int)m_TimeVector.size();
		//	if( l_iSize > 0 )
		//	{
		//		if( this->m_bLoop )
		//		int	l_iLastDataIndex = l_iSize-1;
		//		m_fTotalPassTime = m_TimeVector[l_iLastDataIndex];
		//		m_iCurrentIndex = l_iLastDataIndex;
		//	}
		//}
	public:
		cLinerTemplateDataProcess(){ m_iCurrentIndex = -1;m_bLoop = false;m_fRestTimeToNextStep = 0.f; m_CurrentData = m_CurrentData-m_CurrentData; m_fTotalPassTime = 0.f; }
		virtual ~cLinerTemplateDataProcess(){}
		std::vector<T>*				GetLinerDataVector(){return  &m_LinerDataVector;}
		void						SetLinerDataVector(std::vector<T>e_LinerDataVector){ m_LinerDataVector = m_LinerDataVector; }
		T*							GetData(int e_iIndex){if((int)m_LinerDataVector.size()>e_iIndex && e_iIndex!= -1)return &m_LinerDataVector[e_iIndex];return 0;}
		bool						IsLegal(){ if( m_iCurrentIndex != -1 )return true;return false; }
		virtual	void				Init()
		{
			m_iCurrentIndex = 0;
			m_fCurrentStepLerpValue = 0.f;
			m_fTotalPassTime = 0.f;
			if( m_TimeVector.size() >= 1 )
			{
				m_fRestTimeToNextStep = m_TimeVector[0];
			}
			else
			{
				m_fRestTimeToNextStep = 0.f;
				m_iCurrentIndex = -1;
			}
			m_fNextStepTimeDiff = m_fRestTimeToNextStep;
		}
		virtual	void				Update(float e_fElpaseTime)
		{
			if( m_iCurrentIndex == -1)
				return;
			//if data length is zero return 0
			float	l_fLastTime = this->GetEndTime();
			if( m_fTotalPassTime < l_fLastTime )
				m_fTotalPassTime += e_fElpaseTime;
			else
			{
				if( this->m_bLoop )
				{
					m_fTotalPassTime = UT::GetFloatModulus(m_fTotalPassTime+e_fElpaseTime,l_fLastTime);
				}
				else
				{
					m_fTotalPassTime = l_fLastTime;
					return;
				}
			}
			//data length 0 is not posible to be here
			if( m_TimeVector.size() == 1 )
			{
				m_CurrentData = m_LinerDataVector[0];
				m_fCurrentStepLerpValue = 1.f;
				m_iCurrentIndex = 0;
				return;
			}

			if( m_iCurrentIndex != -1 )
			{
				m_fRestTimeToNextStep -= e_fElpaseTime;
				if( m_fRestTimeToNextStep <= 0.f )
				{
					int	l_iNextDataIndex = 0;
					m_iCurrentIndex = GetTimeVectorDataIndex(m_fTotalPassTime,m_bLoop,&m_fCurrentStepLerpValue,&l_iNextDataIndex,&m_fRestTimeToNextStep,&m_fNextStepTimeDiff);
					m_CurrentData = (m_LinerDataVector[l_iNextDataIndex]-m_LinerDataVector[m_iCurrentIndex])*m_fCurrentStepLerpValue+m_LinerDataVector[m_iCurrentIndex];
				}
				else
				{
					m_fCurrentStepLerpValue = m_fRestTimeToNextStep/m_fNextStepTimeDiff;
					m_CurrentData = (m_LinerDataVector[m_iCurrentIndex+1]-m_LinerDataVector[m_iCurrentIndex])*m_fCurrentStepLerpValue+m_LinerDataVector[m_iCurrentIndex];
				}
			}
		}
		virtual	void				UpdateByGlobeTime(float e_fElpaseTime)
		{
			if( m_fTotalPassTime == e_fElpaseTime )
			{
				return;
			}
			m_fTotalPassTime = e_fElpaseTime;
			this->Init();
			Update(m_fTotalPassTime);
		}
		virtual	void				Clear(){ m_LinerDataVector.clear();m_TimeVector.clear(); }
		virtual	void				Rearrange(float e_fNewTime)
		{
			float	l_fLastTime = GetEndTime();
			if( l_fLastTime >= 0.f )
			{
				float	l_fPercent = e_fNewTime/l_fLastTime;
				int	l_iSize = (int)m_TimeVector.size();
				for( int i=0;i<l_iSize;++i )
					m_TimeVector[i] *= l_fPercent;
			}
		}
		virtual	float				GetCurrentLerpValue()
		{
			float	l_fLastTime = GetEndTime();
			if( l_fLastTime >= 0.f )
			{
				return m_fTotalPassTime/l_fLastTime;
			}
			return 1.f;
		}
		virtual	void				InvertOrder()
		{
			std::vector<T>		l_LinerDataVector = m_LinerDataVector;
			std::vector<float>	l_TimeVector = m_TimeVector;
			int	l_iSize = (int)m_TimeVector.size();
			int	l_iSize2 = (int)m_TimeVector.size()-1;
			for( int i=0;i<l_iSize;++i )
			{
				m_TimeVector[i] = l_TimeVector[l_iSize2-i];
				m_LinerDataVector[i] = l_LinerDataVector[l_iSize2-i];
			}
		}
		T							GetCurrentData(){ return m_CurrentData; }
		void						AddData(T e_Data,float e_fTime){ m_LinerDataVector.push_back(e_Data); m_TimeVector.push_back(e_fTime); }
		void						ChangeData(int e_iIndex,T e_Data,float e_fTime){ m_LinerDataVector[e_iIndex] = e_Data; m_TimeVector[e_iIndex] = e_fTime; }
		void						InsertData(int e_iIndex,T e_Data,float e_fTime){ m_LinerDataVector.insert(m_LinerDataVector.begin()+e_iIndex,e_Data); m_TimeVector.insert(m_TimeVector.begin()+e_iIndex,e_fTime); }
		void						RemoveDtaa(int e_iIndex){m_LinerDataVector.erase(m_LinerDataVector.begin()+e_iIndex);m_TimeVector.erase(m_TimeVector.begin()+e_iIndex);  }
		void						Export(TiXmlElement*e_pParent)
		{
			int				l_iSize = m_TimeVector.size();
			if( l_iSize == 0 )
				return;
			std::wstring	l_strTimeXmlElementData;
			std::wstring	l_strAnimationXmlElementData;
			TiXmlElement*l_pTime = new TiXmlElement(L"Time");
			TiXmlElement*l_pAnimationData = new TiXmlElement(L"AnimationData");
			e_pParent->LinkEndChild(l_pTime);
			e_pParent->LinkEndChild(l_pAnimationData);
			for( int i=0;i<l_iSize;++i )
			{
				l_strTimeXmlElementData += ValueToStringW(*GetTime(i));
				l_strAnimationXmlElementData = ValueToStringW(*GetData(i));
				std::wstring	l_strData = L"Data";
				l_strData += ValueToStringW(i);
				if( i != l_iSize-1 )
				{
					l_strTimeXmlElementData += L",";
				}
				l_pAnimationData->SetAttribute(l_strData,l_strAnimationXmlElementData);
			}
			l_pTime->SetAttribute(L"Time",l_strTimeXmlElementData.c_str());
		}
	};

	//just a quick update interface
	class	cLinerTemplateDataContainer
	{
	public:
		std::vector<cLinerTemplateDataInterface*>	m_ContainerVector;
		void	AddData(cLinerTemplateDataInterface*e_pData){ m_ContainerVector.push_back(e_pData); }

		void	Init()
		{
			int	l_iSize = (int)m_ContainerVector.size();
			for( int i=0;i<l_iSize;++i )
			{
				m_ContainerVector[i]->Init();
			}
		}
		void	Update(float e_fElpaseTime)
		{
			int	l_iSize = (int)m_ContainerVector.size();
			for( int i=0;i<l_iSize;++i )
			{
				m_ContainerVector[i]->Update(e_fElpaseTime);
			}
		}
		void	UpdateByGlobeTime(float e_fElpaseTime)
		{
			int	l_iSize = (int)m_ContainerVector.size();
			for( int i=0;i<l_iSize;++i )
			{
				m_ContainerVector[i]->UpdateByGlobeTime(e_fElpaseTime);
			}	
		}

		void	Rearrange(float e_fNewTime)
		{
			int	l_iSize = (int)m_ContainerVector.size();
			for( int i=0;i<l_iSize;++i )
			{
				m_ContainerVector[i]->UpdateByGlobeTime(e_fNewTime);
			}
		}

		float	GetEndTime()
		{
			float	l_fLastTime = 0.f;
			int	l_iSize = (int)m_ContainerVector.size();
			for( int i=0;i<l_iSize;++i )
			{
				float l_fLastTime2 = m_ContainerVector[i]->GetEndTime();
				if( l_fLastTime < l_fLastTime2 )
				{
					l_fLastTime = l_fLastTime2;
				}
			}
			return l_fLastTime;
		}

		virtual	void				InvertOrder()
		{
			int	l_iSize = (int)m_ContainerVector.size();
			for( int i=0;i<l_iSize;++i )
			{
				m_ContainerVector[i]->InvertOrder();
			}	
		}

		virtual	void				Destory()
		{
			int	l_iSize = (int)m_ContainerVector.size();
			for( int i=0;i<l_iSize;++i )
			{
				m_ContainerVector[i]->Clear();
			}		
		}
	};
//end namespace
}
//endif
#endif