#include "stdafx.h"
#include "SmartPointer.h"
#include "XML/StringToStructure.h"
#include "GameplayUT/GameApp.h"

//bool	g_bShowSmartPointerInfo = true;
bool	g_bShowSmartPointerInfo = false;
cSmartObject::cSmartObject()
{
	m_refCount = 0;
}

cSmartObject::cSmartObject(NamedTypedObject*e_pNamedTypedObject)
{
	m_refCount = 0;
	AddRef(e_pNamedTypedObject);
}

cSmartObject::~cSmartObject()
{
	cFUSynchronizedHold hold(&m_refCounterLock);
	assert(m_ReferenceList.Count() == 0);
	assert(m_refCount == 0);
}

int cSmartObject::GetReferenceCount()
{
	return m_refCount;
}

void	DumpInfo(const WCHAR*e_strDescription,cSmartObject*e_pSmartObject,NamedTypedObject*e_pTarget)
{
	if( g_bShowSmartPointerInfo )
	{
		cGameApp::OutputDebugInfoString(L"\n================================\n");
		cGameApp::OutputDebugInfoString(e_strDescription);
		cGameApp::OutputDebugInfoString(FATMING_CORE::ValueToStringW(e_pSmartObject->GetReferenceCount()));
		cGameApp::OutputDebugInfoString(L"\n");
		if( e_pTarget )
		{
			cGameApp::OutputDebugInfoString(L"MyName:");
			cGameApp::OutputDebugInfoString(e_pSmartObject->GetName());
			cGameApp::OutputDebugInfoString(L"\n");
			cGameApp::OutputDebugInfoString(L"\t\t\tType:");
			cGameApp::OutputDebugInfoString(e_pSmartObject->Type());
			cGameApp::OutputDebugInfoString(L"\n");
			cGameApp::OutputDebugInfoString(L"TargetName:");
			cGameApp::OutputDebugInfoString(e_pTarget->GetName());
			cGameApp::OutputDebugInfoString(L"\n");
			cGameApp::OutputDebugInfoString(L"\t\t\tType:");
			cGameApp::OutputDebugInfoString(e_pTarget->Type());
			cGameApp::OutputDebugInfoString(L",GUID:");
			cGameApp::OutputDebugInfoString(FATMING_CORE::ValueToStringW(e_pTarget->GetUniqueID()));
			cGameApp::OutputDebugInfoString(L"\n================================\n");
			if( e_pTarget->GetUniqueID() == 156 )
			{
				int a=0;
			}
		}
	}
}

void cSmartObject::AddRef(NamedTypedObject*e_pNamedTypedObject)
{
	if( e_pNamedTypedObject )
	{
		++m_refCount;
		m_ReferenceList.AddObjectNeglectExist(e_pNamedTypedObject);
		assert(m_ReferenceList.Count() == m_refCount);
	}
#ifdef DEBUG
	if( g_bShowSmartPointerInfo )
	{
		DumpInfo(L"AddRefCount:",this,e_pNamedTypedObject);
	}
#endif
}

int cSmartObject::Release(NamedTypedObject*e_pNamedTypedObject)
{
	if( e_pNamedTypedObject )
	{
		bool	l_b = m_ReferenceList.RemoveObjectWithoutDelete(e_pNamedTypedObject);
		assert(l_b&&"fuck no this pointer!!");
		cFUSynchronizedHold*l_pHold = new cFUSynchronizedHold(&m_refCounterLock);
		m_refCount--;
		int	l_refCount = m_refCount;
		assert(m_ReferenceList.Count() == l_refCount);
#ifdef DEBUG
		if( g_bShowSmartPointerInfo )
		{
			DumpInfo(L"AddRefCount:",this,e_pNamedTypedObject);
		}
#endif
		delete l_pHold;
		if( l_refCount <= 0 )
		{
			//if( e_bDelete )
			{
				//if(this->GetOwner())
				//{
				//	cNamedTypedObjectVector<NamedTypedObject>*l_pNamedTypedObject = dynamic_cast<cNamedTypedObjectVector<NamedTypedObject>*>(this->GetOwner());
				//	l_pNamedTypedObject->RemoveObjectWithoutDelete(this);
				//}
				delete this;
			}
		}
		return l_refCount;
	}
#ifdef DEBUG
	else
	{
		int a=0;
	}
#endif
	assert(0&&"release null pointer!?");
	return m_refCount;
}