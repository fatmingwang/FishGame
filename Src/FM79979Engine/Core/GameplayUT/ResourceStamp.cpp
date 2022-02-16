#include "../stdafx.h"
#include "../Sound/SoundManager.h"
#include "../Image/ImageParser.h"
#include "../Particle/ParticleBase.h"
#include "../Particle/ParticleEmitter.h"
#include "../Particle/ParticleManager.h"
#include "GameApp.h"
#include "ResourceStamp.h"
///#include "GameApp.h"
namespace FATMING_CORE
{
	cResourceStamp::sStampVectorData::sStampVectorData()
	{
		m_AnimationParser.SetFromResource(true);
		m_ImageParser.SetFromResource(true);;
		m_SoundParser.SetFromResource(true);;
		m_PaticleManager.SetFromResource(true);;
		m_BehaviorPaticleManager.SetFromResource(true);;
	}

	void	cResourceStamp::sStampVectorData::Clear()
	{
		m_AnimationParser.Destroy();
		m_ImageParser.Destroy();
		m_SoundParser.Destroy();
		m_PaticleManager.Destroy();
		m_BehaviorPaticleManager.Destroy();	
	}

	cResourceStamp::cResourceStamp()
	{
	
	}
	cResourceStamp::~cResourceStamp()
	{
	
	}
	void	cResourceStamp::AssignNewData(cNamedTypedObjectVector<NamedTypedObject>*e_pStampVectorData1,cNamedTypedObjectVector<NamedTypedObject>*e_pStampVectorData2,cNamedTypedObjectVector<NamedTypedObject>*e_pNewStampVectorData)
	{
		int	l_iOldCount = e_pStampVectorData1->Count();
		int	l_iCount = e_pStampVectorData2->Count();
		for( int i=l_iOldCount;i<l_iCount;++i )
			e_pNewStampVectorData->AddObjectNeglectExist(e_pStampVectorData2->GetObject(i));
	}
	//class	cParticleBase;
	void	cResourceStamp::StampResource(sStampVectorData*e_pStampVectorData)
	{
		e_pStampVectorData->Clear();
		e_pStampVectorData->m_AnimationParser.CopyListPointer(cGameApp::m_spAnimationParser);
		e_pStampVectorData->m_ImageParser.CopyListPointer(cGameApp::m_spImageParser);
		e_pStampVectorData->m_SoundParser.CopyListPointer((cNamedTypedObjectVector<NamedTypedObject>*)cGameApp::m_spSoundParser);
		e_pStampVectorData->m_PaticleManager.CopyListPointer((cNamedTypedObjectVector<NamedTypedObject>*)cGameApp::m_spPaticleManager);
		e_pStampVectorData->m_BehaviorPaticleManager.CopyListPointer(cGameApp::m_spBehaviorPaticleManager);
	}

	void	cResourceStamp::StampResource1()
	{
		StampResource(&m_Stamp1Data);
	}

	void	cResourceStamp::StampResource2()
	{
		StampResource(&m_Stamp2Data);
	}

	void	cResourceStamp::ReleaseNewResource()
	{
		AssignNewData(&m_Stamp1Data.m_AnimationParser,			&m_Stamp2Data.m_AnimationParser,		&m_Stamp3Data.m_AnimationParser);
		AssignNewData(&m_Stamp1Data.m_BehaviorPaticleManager,	&m_Stamp2Data.m_BehaviorPaticleManager,	&m_Stamp3Data.m_BehaviorPaticleManager);
		AssignNewData(&m_Stamp1Data.m_ImageParser,				&m_Stamp2Data.m_ImageParser,			&m_Stamp3Data.m_ImageParser);
		AssignNewData(&m_Stamp1Data.m_PaticleManager,			&m_Stamp2Data.m_PaticleManager,			&m_Stamp3Data.m_PaticleManager);
		AssignNewData(&m_Stamp1Data.m_SoundParser,				&m_Stamp2Data.m_SoundParser,			&m_Stamp3Data.m_SoundParser);
		for(int i=0;i<m_Stamp3Data.m_AnimationParser.Count();++i)
			cGameApp::m_spAnimationParser->RemoveObject(m_Stamp3Data.m_AnimationParser[i]);
		for(int i=0;i<m_Stamp3Data.m_BehaviorPaticleManager.Count();++i)
			cGameApp::m_spBehaviorPaticleManager->RemoveObject(m_Stamp3Data.m_BehaviorPaticleManager[i]);
		for(int i=0;i<m_Stamp3Data.m_ImageParser.Count();++i)
			cGameApp::m_spImageParser->RemoveObject(m_Stamp3Data.m_ImageParser[i]);
		for(int i=0;i<m_Stamp3Data.m_PaticleManager.Count();++i)
			cGameApp::m_spPaticleManager->RemoveObject(m_Stamp3Data.m_PaticleManager[i]);
		for(int i=0;i<m_Stamp3Data.m_SoundParser.Count();++i)
			cGameApp::m_spSoundParser->RemoveObject(m_Stamp3Data.m_SoundParser[i]);
		Clear();
	}
	void	cResourceStamp::Clear()
	{
		m_Stamp1Data.Clear();
		m_Stamp2Data.Clear();
		m_Stamp3Data.Clear();
	}
//end namespace FATMING_CORE
}