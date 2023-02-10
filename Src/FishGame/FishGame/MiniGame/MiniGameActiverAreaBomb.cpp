#include "stdafx.h"
#include "MiniGameActiverAreaBomb.h"
#include "../Monster/MonsterManager.h"
#include "../GameApp/GameApp.h"
#include "../Monster/MonsterManager.h"
const wchar_t*         cMiniGameActiverAreaBomb::TypeID( L"cMiniGameActiverAreaBomb" );
cMiniGameActiverAreaBomb::cMiniGameActiverAreaBomb(TiXmlElement*e_pXMLElement):cMiniGameActiverBase(e_pXMLElement)
{
	TiXmlElement*l_pDataXMLElement = GetXmlElementByNameFromElement(L"Data",e_pXMLElement);
	m_fExplosionRadius = 300.f;
	PARSE_ELEMENT_START(l_pDataXMLElement)
		COMPARE_NAME("ExplosionRadius")
		{
			m_fExplosionRadius = VALUE_TO_FLOAT;
		}
	PARSE_NAME_VALUE_END
}

cMiniGameActiverAreaBomb::cMiniGameActiverAreaBomb(cMiniGameActiverAreaBomb*e_pMiniGameActiverAreaBomb)
:cMiniGameActiverBase(e_pMiniGameActiverAreaBomb)
{
	m_fExplosionRadius = e_pMiniGameActiverAreaBomb->m_fExplosionRadius;
}

cMiniGameActiverAreaBomb::~cMiniGameActiverAreaBomb()
{

}

void	cMiniGameActiverAreaBomb::SetupShowData(int e_iPayRate,int e_iMoney,cPlayerBehaviorBase*e_pPlayer,cMonster*e_pMonster)
{
	cMiniGameActiverBase::SetupShowData(e_iPayRate,e_iMoney,e_pPlayer,e_pMonster);
	if( m_pMonster )
	{
		float	l_fRadius = m_pMonster->GetRadius();
		m_pMonster->SetRadius(m_fExplosionRadius);
		cMonster **l_ppMonster = cFishApp::m_spMonsterManager->GetWorkingMonster();
		int	l_iNum = cFishApp::m_spMonsterManager->GetCountMonsterAlive();
		for( int i=0;i<l_iNum;++i )
		{
			if(l_ppMonster[i]->IsStatusAllowToDied()&&l_ppMonster[i]->IsVisible())
			{
				if(!l_ppMonster[i]->IsMiniGameMonster() && l_ppMonster[i]->IsCollide(m_pMonster))
				{
					if( m_pMonster != l_ppMonster[i] )//avoid self
					{
						this->m_HittedMonsterVector.push_back(l_ppMonster[i]);
					}
				}
			}
		}
		m_pMonster->SetRadius(l_fRadius);
	}
}