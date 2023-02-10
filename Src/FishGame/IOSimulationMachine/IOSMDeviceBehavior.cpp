#include "stdafx.h"
#include "IOSMDeviceBehavior.h"
#include "IOSMBaseDevice.h"
namespace IOSM
{
	const WCHAR* cIOSMDeviceBehavior::TypeID( L"cIOSMDeviceBehavior" );
	const WCHAR* cProbableIO::TypeID( L"cProbableIO" );
	const WCHAR* cActiveIOTimesWithSingnalCircle::TypeID( L"cActiveIOTimesWithSingnalCircle" );
	const WCHAR* cListenMultiSingnalToActiveIO::TypeID( L"cListenMultiSingnalToActiveIO" );
	const WCHAR* cSendHighSingnal::TypeID( L"cSendHighSingnal" );
	const WCHAR* cSendLowSingnal::TypeID( L"cSendLowSingnal" );
	
	int	GetDeviceBehaviorTypeIDIndex(const WCHAR*e_strType)
	{
		for( int i=0;i<g_iAllBehaviorType;++i )
		{
			if(!wcscmp(g_strAllBehaviorName[i],e_strType))
				return i;
		}
		return -1;
	}

	cIOSMDeviceBehavior::cIOSMDeviceBehavior(cIOSMBaseDevice*e_pIOSMBaseDevice)
	{
		m_pIOSMBaseDevice = e_pIOSMBaseDevice;
		m_pIOSMIOData = m_pIOSMBaseDevice->m_pIOSMIOData;
		m_bActived = false;
		m_bSendSingnal = false;
	}
	cIOSMDeviceBehavior::~cIOSMDeviceBehavior()
	{
	
	}


	cProbableIO::cProbableIO(cIOSMBaseDevice*e_pIOSMBaseDevice):cIOSMDeviceBehavior(e_pIOSMBaseDevice)
	{
		m_iProbability = 0;
		m_ProbableIOList.SetFromResource(true);
	}
	cProbableIO::~cProbableIO()
	{
	
	}

	void	cProbableIO::Update(float e_fElpaseTime)
	{
		if(m_pIOSMIOData->IsActive())
		{
			if( m_bActived&&!this->m_pIOSMIOData->m_bFirstActive )
				return;
			m_bActived = true;
			int	l_iNum = this->m_ProbableIOList.Count();
			int	l_iForObjectProbility = m_iProbability/l_iNum;
			int	l_iPercent = rand()%this->m_iProbability;
			int	l_iCurrentProbabilty = l_iForObjectProbility;
			for( int i=0;i<l_iNum;++i )
			{
				if( l_iForObjectProbility>=l_iPercent )//hit!
				{
					this->m_ProbableIOList[i]->SetSingnal(this->m_bSendSingnal,this->m_pIOSMIOData);
					break;
				}
				l_iCurrentProbabilty += l_iForObjectProbility;
			}
		}
		else
		{
			m_bActived = false;
		}
	}

	cActiveIOTimesWithSingnalCircle::cActiveIOTimesWithSingnalCircle(cIOSMBaseDevice*e_pIOSMBaseDevice):cIOSMDeviceBehavior(e_pIOSMBaseDevice)
	{
		m_iTimes = 0;
		assert( this->m_pIOSMIOData->GetLowCircle()!=0.f && this->m_pIOSMIOData->GetHighCircle()!=0.f);
		m_bStartToCount = false;
	}

	cActiveIOTimesWithSingnalCircle::~cActiveIOTimesWithSingnalCircle()
	{
	
	}

	void	cActiveIOTimesWithSingnalCircle::Update(float e_fElpaseTime)
	{
		assert(this->m_iTimes>1&&"if only one time need to be ecxuted,why u set this!?");
		if( m_bStartToCount )
		{
			if( m_bCurrentIOSingnal != this->m_pIOSMIOData->GetCurrentSingnal() )
			{
				m_bCurrentIOSingnal = !m_bCurrentIOSingnal;
				if( m_iRestTimeToCount != 79979 )//for infinity
				--m_iRestTimeToCount;
			}

			if( m_iRestTimeToCount == 0 )
			{
				m_bStartToCount	= false;
				this->m_pIOSMIOData->SetEnable(false);
			}
		}
		else
		if(m_pIOSMIOData->IsActive())
		{
			m_bStartToCount = true;
			m_iRestTimeToCount = this->m_iTimes*2;//low and high
			if( m_iRestTimeToCount == 0 )//time is 0 for infinity,set magic number as 79979
				m_iRestTimeToCount = 79979;
			m_bCurrentIOSingnal = this->m_pIOSMIOData->GetCurrentSingnal();
			this->m_pIOSMIOData->SetEnable(true);
		}
		else
		{
			m_bActived = false;
		}
	}

	cListenMultiSingnalToActiveIO::cListenMultiSingnalToActiveIO(cIOSMBaseDevice*e_pIOSMBaseDevice):cIOSMDeviceBehavior(e_pIOSMBaseDevice)
	{
		m_ActiveIOList.SetFromResource(true);
		m_WaitLineIOList.SetFromResource(true);
	}

	cListenMultiSingnalToActiveIO::~cListenMultiSingnalToActiveIO()
	{
	
	}

	void	cListenMultiSingnalToActiveIO::Update(float e_fElpaseTime)
	{
		int	l_iNum = m_WaitLineIOList.Count();
		for( int i=0;i<l_iNum;++i )
		{
			cIOSMIOData*l_pIOSMIOData = m_WaitLineIOList[i];
			bool	l_bCurrentSingnal = false;
			if(l_pIOSMIOData->ForceReadIO(&l_bCurrentSingnal))
			{
				if( m_WaitLineIOList[i]->IsWorkingSingnal() != l_bCurrentSingnal )
				{
					if(this->m_pIOSMIOData->IsEnable())
					{
						this->m_pIOSMIOData->SetEnable(false);
					}
					return;
				}
			}
		}
		if(!this->m_pIOSMIOData->IsEnable())
		{
			this->m_pIOSMIOData->SetEnable(true);
		}
	}

	void	cListenMultiSingnalToActiveIO::SetSingnal(cIOSMIOData*e_cIOSMIOData,bool e_bOn)
	{
		assert(0&&"this is not what I thought,instead by update");
		if( e_bOn == this->m_pIOSMIOData->IsWorkingSingnal() )
		{
			m_WaitLineIOList.AddObject(e_cIOSMIOData);
			if( m_WaitLineIOList.Count() == this->m_ActiveIOList.Count() )
			{
				this->m_pIOSMIOData->SetEnable( true );
			}
		}
		else
		{
			this->m_pIOSMIOData->SetEnable(false);
		}
	}

	cSendHighSingnal::cSendHighSingnal(cIOSMBaseDevice*e_pIOSMBaseDevice):cIOSMDeviceBehavior(e_pIOSMBaseDevice)
	{
		m_ActiveIOList.SetFromResource(true);
	}

	cSendHighSingnal::~cSendHighSingnal()
	{
	
	}

	void	cSendHighSingnal::Update(float e_fElpaseTime)
	{
		if(m_pIOSMIOData->IsActive())
		{
			if( m_bActived&&!this->m_pIOSMIOData->m_bFirstActive )
				return;
			m_bActived = true;
			int	l_iNum = this->m_ActiveIOList.Count();
			if( l_iNum == 1 )
			{
				m_ActiveIOList[0]->SetCurrentSingnal(true);
			}
			else
			{
				for( int i=0;i<l_iNum;++i )
				{
					m_ActiveIOList[i]->SetCurrentSingnal(true);
				}
			}
		}
		else
		{
			m_bActived = false;
		}	
	}

	cSendLowSingnal::cSendLowSingnal(cIOSMBaseDevice*e_pIOSMBaseDevice):cIOSMDeviceBehavior(e_pIOSMBaseDevice)
	{
		m_ActiveIOList.SetFromResource(true); 
	}

	cSendLowSingnal::~cSendLowSingnal()
	{
	
	}

	void	cSendLowSingnal::Update(float e_fElpaseTime)
	{
		if(m_pIOSMIOData->IsActive())
		{
			if( m_bActived&&!this->m_pIOSMIOData->m_bFirstActive )
				return;
			m_bActived = true;
			int	l_iNum = this->m_ActiveIOList.Count();
			if( l_iNum == 1 )
			{
				m_ActiveIOList[0]->SetCurrentSingnal(false);
			}
			else
			{
				for( int i=0;i<l_iNum;++i )
				{
					m_ActiveIOList[i]->SetCurrentSingnal(false);
				}
			}
		}
		else
		{
			m_bActived = false;
		}
	}

//end namespace IOSM
};