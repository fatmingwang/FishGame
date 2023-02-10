#include "stdafx.h"
#include "MiniGameActiverFullScreenBomb.h"
#include "../Monster/MonsterManager.h"
#include "../GameApp/GameApp.h"
#include "../Monster/MonsterManager.h"

const wchar_t*         cMiniGameActiverFullScreenBomb::TypeID( L"cMiniGameActiverFullScreenBomb" );
cMiniGameActiverFullScreenBomb::cMiniGameActiverFullScreenBomb(TiXmlElement*e_pXMLElement):cMiniGameActiverBase(e_pXMLElement)
{
	//e_pXMLElement = ISAXCallback::GetXmlElementByNameFromElement(L"Data",e_pXMLElement);
}

cMiniGameActiverFullScreenBomb::cMiniGameActiverFullScreenBomb(cMiniGameActiverFullScreenBomb*e_pMiniGameActiverFullScreenBomb)
:cMiniGameActiverBase(e_pMiniGameActiverFullScreenBomb)
{

}

cMiniGameActiverFullScreenBomb::~cMiniGameActiverFullScreenBomb()
{

}

void	cMiniGameActiverFullScreenBomb::SetupShowData(int e_iPayRate,int e_iMoney,cPlayerBehaviorBase*e_pPlayer,cMonster*e_pMonster)
{
	cMiniGameActiverBase::SetupShowData(e_iPayRate,e_iMoney,e_pPlayer,e_pMonster);
	cMonster **l_ppMonster = cFishApp::m_spMonsterManager->GetWorkingMonster();
	int	l_iNum = cFishApp::m_spMonsterManager->GetCountMonsterAlive();
	for( int i=0;i<l_iNum;++i )
	{
		if(l_ppMonster[i]->IsStatusAllowToDied()&&l_ppMonster[i]->IsVisible())
		{
			if( !l_ppMonster[i]->IsMiniGameMonster() )
			{
				if( m_pMonster != l_ppMonster[i] )//avoid self
				{
					this->m_HittedMonsterVector.push_back(l_ppMonster[i]);
				}
			}
		}
	}
}