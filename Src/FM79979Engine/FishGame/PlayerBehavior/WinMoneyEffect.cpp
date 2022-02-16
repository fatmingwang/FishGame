#include "stdafx.h"
#include "PlayerData.h"
#include "CoinEffect.h"
#include "PrizeEffect.h"
#include "WinMoneyEffect.h"

struct WinMoneyEffectConfigData
{
	const WCHAR*			EffectName;
	const WCHAR*			EffectType;
	WinMoneyEffectMPDIData  MPDIData;
};

cWinMoneyEffectManager::cWinMoneyEffectManager()
{
	m_TotalEffectAmount       = 0;
	m_ppWorkingWinMoneyEffect = 0;
}

cWinMoneyEffectManager::~cWinMoneyEffectManager()
{
	if ( m_ppWorkingWinMoneyEffect )
	{
		for ( int i = 0; i < m_TotalEffectAmount; i++ )
			 m_ppWorkingWinMoneyEffect[i] = 0;

		SAFE_DELETE_ARRAY( m_ppWorkingWinMoneyEffect );
	}
}

void cWinMoneyEffectManager::Init()
{
	m_iNumWorkingWinMoneyEffect = 0;
	m_AllWinMoneyEffectList.Destroy();

	LoadWinMoneyEffectXMLData();
}

void cWinMoneyEffectManager::LoadWinMoneyEffectXMLData()
{
	cNodeISAX WinMoneyEffectConfigXML;

	bool bLoadXMLResult = WinMoneyEffectConfigXML.ParseDataIntoXMLNode( WINMONEYEFFECT_DATA );

	if ( bLoadXMLResult )
	{
		int Step(0), CoinEffectMaxAmount(0), PrizeEffectMaxAmount(0);

		TiXmlElement *TempWinMoneyEffectConfig = WinMoneyEffectConfigXML.GetRootElement();

		PARSE_ELEMENT_START( TempWinMoneyEffectConfig )

			switch ( Step )
			{
				case 0: m_TotalEffectAmount  = VALUE_TO_INT; break;
				case 1: CoinEffectMaxAmount  = VALUE_TO_INT; break;
				case 2: PrizeEffectMaxAmount = VALUE_TO_INT; break;
			}

			Step++;

		TO_NEXT_VALUE

		std::vector< WinMoneyEffectConfigData > TotalWinMoneyEffectInfo;

		TotalWinMoneyEffectInfo.empty();

		TempWinMoneyEffectConfig = TempWinMoneyEffectConfig->FirstChildElement();

		while ( TempWinMoneyEffectConfig )
		{
			WinMoneyEffectConfigData TempInfo;

			PARSE_ELEMENT_START( TempWinMoneyEffectConfig )
				COMPARE_NAME( "EffectName" )
				{
					TempInfo.EffectName = l_strValue;
				}
				else COMPARE_NAME( "MPDIList" )
				{
					TempInfo.MPDIData.MPDIListName = l_strValue;
				}
				else COMPARE_NAME( "EffectType" )
				{
					TempInfo.EffectType = l_strValue;
				}
				else COMPARE_NAME( "EffectMPDI" )
				{
					TempInfo.MPDIData.EffectMPDIName = l_strValue;
				}
				else COMPARE_NAME( "NumerialMPDIList" )
				{
					TempInfo.MPDIData.NumerialMPDIListName = l_strValue;
				}
				else COMPARE_NAME( "NumerialMPDI" )
				{
					TempInfo.MPDIData.NumerialMPDIName = l_strValue;

					TotalWinMoneyEffectInfo.push_back( TempInfo );
				}
			PARSE_NAME_VALUE_END

			TempWinMoneyEffectConfig = TempWinMoneyEffectConfig->NextSiblingElement();
		}

		if ( !m_ppWorkingWinMoneyEffect )
		{
			m_ppWorkingWinMoneyEffect = new cWinMoneyEffectBase* [ m_TotalEffectAmount ];

			for ( int i = 0; i < m_TotalEffectAmount; i++ )
				 m_ppWorkingWinMoneyEffect[i] = 0; 
		}

		if ( m_ppWorkingWinMoneyEffect )
		{
			m_AllWinMoneyEffectList.Destroy();

			cNamedTypedObjectVector< cWinMoneyEffectBase > *pWinMoneyList = 0;

			for ( size_t i = 0; i < TotalWinMoneyEffectInfo.size(); i++ )
			{
				 pWinMoneyList = new cNamedTypedObjectVector< cWinMoneyEffectBase >;

				 if ( pWinMoneyList )
				 {
					 pWinMoneyList->SetName( TotalWinMoneyEffectInfo[i].EffectName );

					 m_AllWinMoneyEffectList.AddObjectNeglectExist( pWinMoneyList );

					 int MaxAmount(0);

					 if ( !wcscmp( TotalWinMoneyEffectInfo[i].EffectType, L"Coin" ) )
						 MaxAmount = CoinEffectMaxAmount;
					 else if ( !wcscmp( TotalWinMoneyEffectInfo[i].EffectType, L"BigWin" ) )
						 MaxAmount = PrizeEffectMaxAmount;

					 for ( int j = 0; j < MaxAmount; j++ )
					 {
						  cWinMoneyEffectBase* pTempWinMoneyEffectBase = 0;

						  if ( !wcscmp( TotalWinMoneyEffectInfo[i].EffectType, L"Coin" ) )
							  pTempWinMoneyEffectBase = new cCoinEffect();
						  else if ( !wcscmp( TotalWinMoneyEffectInfo[i].EffectType, L"BigWin" ) )  
							  pTempWinMoneyEffectBase = new PrizeEffect();

						  pTempWinMoneyEffectBase->Init();
						  pTempWinMoneyEffectBase->SetName( TotalWinMoneyEffectInfo[i].EffectName );
						  pTempWinMoneyEffectBase->LoadWinMoneyEffectMPDIData( TotalWinMoneyEffectInfo[i].MPDIData );
						  pWinMoneyList->AddObjectNeglectExist( pTempWinMoneyEffectBase );
					 }
				 }
			}
		}
	}	
}

cWinMoneyEffectBase* cWinMoneyEffectManager::RequirePrizeEffect( const WCHAR* EffectTypeName, cMatrix44 AniMatrix, int Money, cPlayerData* pPlayer, bool e_bForceRequire )
{
	if ( !pPlayer )
		return NULL;

	int KindIndex = m_AllWinMoneyEffectList.GetObjectIndexByName( EffectTypeName );

	if ( KindIndex != -1 )
	{
		int	CloneObjIndex( -1 );

		cNamedTypedObjectVector< cWinMoneyEffectBase > *pList = m_AllWinMoneyEffectList[ KindIndex ];

		for ( int i = 0;i < pList->Count(); i++ )
		{
			 cWinMoneyEffectBase *pWinMoneyEffectBase = pList->GetObject(i);

			 if ( pWinMoneyEffectBase && !pWinMoneyEffectBase->IsUsing() )
			 {
				 m_ppWorkingWinMoneyEffect[ m_iNumWorkingWinMoneyEffect ] = pWinMoneyEffectBase;
				 m_ppWorkingWinMoneyEffect[ m_iNumWorkingWinMoneyEffect ]->Init();
				 ++m_iNumWorkingWinMoneyEffect;

				 pWinMoneyEffectBase->SetData( AniMatrix, Money, pPlayer );

				 return pWinMoneyEffectBase;
			 }
			 else
				 CloneObjIndex = i;
		 }

		 if ( e_bForceRequire && CloneObjIndex != -1 )
		 {
			 cWinMoneyEffectBase *pClone = dynamic_cast< cWinMoneyEffectBase* >( GetObject( CloneObjIndex )->Clone() );

			 m_ppWorkingWinMoneyEffect[ m_iNumWorkingWinMoneyEffect ] = pClone;
			 m_ppWorkingWinMoneyEffect[ m_iNumWorkingWinMoneyEffect ]->Init();
			 ++m_iNumWorkingWinMoneyEffect;

			 pClone->SetData( AniMatrix, Money, pPlayer );
			 pList->AddObjectNeglectExist( pClone );

			 return pClone;
		 }
	}
	return NULL;	
}

cWinMoneyEffectBase* cWinMoneyEffectManager::RequireWinMoneyEffectBase( const WCHAR* EffectTypeName, cMatrix44& AniMatrix, int Money, cPlayerData* pPlayer, eWinMoneyEffectCoinCount MoneyCount, bool e_bForceRequire )
{
	if ( ( MoneyCount >= eWinMoneyEffectCoinCount_Max ) || !pPlayer )
		return NULL;

	int KindIndex = m_AllWinMoneyEffectList.GetObjectIndexByName( EffectTypeName );

	if ( KindIndex != -1 )
	{
		int	CloneObjIndex( -1 );

		cNamedTypedObjectVector< cWinMoneyEffectBase > *pList = m_AllWinMoneyEffectList[ KindIndex ];

		for ( int i = 0;i < pList->Count(); i++ )
		{
			 cWinMoneyEffectBase *pWinMoneyEffectBase = pList->GetObject(i);

			 if ( pWinMoneyEffectBase && !pWinMoneyEffectBase->IsUsing() )
			 {
				 m_ppWorkingWinMoneyEffect[ m_iNumWorkingWinMoneyEffect ] = pWinMoneyEffectBase;
				 m_ppWorkingWinMoneyEffect[ m_iNumWorkingWinMoneyEffect ]->Init();
				 ++m_iNumWorkingWinMoneyEffect;

				 pWinMoneyEffectBase->SetData( AniMatrix, Money, pPlayer );
				 pWinMoneyEffectBase->SetMoneyMPDICoinCount( MoneyCount );

				 return pWinMoneyEffectBase;
			 }
			 else
				 CloneObjIndex = i;
		 }

		 if ( e_bForceRequire && CloneObjIndex != -1 )
		 {
			 cWinMoneyEffectBase *pClone = dynamic_cast< cWinMoneyEffectBase* >( GetObject( CloneObjIndex )->Clone() );

			 m_ppWorkingWinMoneyEffect[ m_iNumWorkingWinMoneyEffect ] = pClone;
			 m_ppWorkingWinMoneyEffect[ m_iNumWorkingWinMoneyEffect ]->Init();
			 ++m_iNumWorkingWinMoneyEffect;

			 pClone->SetData( AniMatrix, Money, pPlayer );
			 pClone->SetMoneyMPDICoinCount( MoneyCount );
			 pList->AddObjectNeglectExist( pClone );

			 return pClone;
		 }
	}
	return NULL;
}

void cWinMoneyEffectManager::Update( float ElapseTime )
{
	for ( int i = 0; i < m_iNumWorkingWinMoneyEffect; i++ )
	{
		 m_ppWorkingWinMoneyEffect[i]->Update( ElapseTime );

		 if ( m_ppWorkingWinMoneyEffect[i]->IsDone() )
		 {
			 m_ppWorkingWinMoneyEffect[i] = m_ppWorkingWinMoneyEffect[ m_iNumWorkingWinMoneyEffect - 1 ];
			 --m_iNumWorkingWinMoneyEffect;
		 }
	}
}

void cWinMoneyEffectManager::Render()
{
	for ( int i = 0; i < m_iNumWorkingWinMoneyEffect; i++ )
		 m_ppWorkingWinMoneyEffect[i]->Render();
}