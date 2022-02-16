#include "../stdafx.h"
#include "LinerTemplateDataProcess.h"
namespace	FATMING_CORE
{
	int		cLinerTemplateDataInterface::GetTimeVectorDataIndex(float e_fPassTime,bool e_bLoop,float*e_pfLerpValue,int*e_piNextDataIndex,float*e_fRestTimeToNextStep,float*e_pfNextStepTimeDiff)
	{
		int	l_iSize = (int)m_TimeVector.size();
		if( l_iSize == 0 )			return -1;
		float	l_fEndTime = m_TimeVector[l_iSize-1];
		if( l_iSize == 1 || l_fEndTime == 0.f || e_fPassTime < m_TimeVector[0] )
		{
			*e_pfLerpValue = 1.f;
			*e_piNextDataIndex = 0;
			return 0;
		}

		if( l_fEndTime <= e_fPassTime )
		{
			if( e_bLoop )
				e_fPassTime = UT::GetFloatModulus(e_fPassTime,l_fEndTime);
			else
			{
				int	l_iEndDataIndex = l_iSize-1;
				*e_pfLerpValue = 1.f;
				*e_piNextDataIndex = l_iEndDataIndex;
				return l_iEndDataIndex;
			}
		}

		for( int i=1;i<l_iSize;++i )
		{
			if( e_fPassTime <= m_TimeVector[i] )
			{
				int	l_iEndDataIndex = l_iSize-1;
				if( i != l_iEndDataIndex )
				{
					float	l_fPreviousTime = m_TimeVector[i-1];
					float	l_fTimeOverdValue = e_fPassTime-l_fPreviousTime;
					float	l_fTimeGap = m_TimeVector[i]-l_fPreviousTime;
					*e_fRestTimeToNextStep = l_fTimeGap-l_fTimeOverdValue;
					*e_pfNextStepTimeDiff = l_fTimeGap;
					if( l_fTimeGap != 0.f )
					{
						*e_pfLerpValue = l_fTimeOverdValue/l_fTimeGap;
						*e_piNextDataIndex = i;
						return i-1;
					}
					else
					{
						*e_pfLerpValue = 1.f;
						*e_piNextDataIndex = i;
					}
				}
				else
				{
					*e_pfLerpValue = 1.f;
					*e_piNextDataIndex = i;
				}
				return i;
			}
		}
		//impossible
		assert(0 &&" GetTimeVectorDataIndex");
		return -1;
	}

	float	cLinerTemplateDataInterface::GetEndTime(){	int	l_iSize = (int)m_TimeVector.size();	if( l_iSize > 0 ) return m_TimeVector[l_iSize-1] ;return 0.f;}
	void	cLinerTemplateDataInterface::ChangeTime(int e_iIndex,float e_fTime)
	{
		if( (int)m_TimeVector.size() > e_iIndex )
			m_TimeVector[e_iIndex] = e_fTime; 	
	}
}