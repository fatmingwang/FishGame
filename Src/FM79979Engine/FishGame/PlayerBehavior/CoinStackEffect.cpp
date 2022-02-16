#include "stdafx.h"
//#include "CoinStackEffect.h"
//
//#include "../GameApp/GameApp.h"
//#include "../Monster/MonsterManager.h"
//
//CoinStackEffect::CoinStackEffect()
//{
//	m_MaxShowCount       = 0;
//	m_CoinStackMoveTm    = 0.f;
//	m_CoinStackDisplayTm = 0.f;
//}
//
//CoinStackEffect::~coinStackEffect()
//{
//	for ( size_t i = 0; i < m_AllCoinStackFishCardData.size(); i++ )
//	{
//		 for ( size_t j = 0; j < m_AllCoinStackFishCardData[i].size(); j++ )
//			  SAFE_DELETE( m_AllCoinStackFishCardData[i][j].pFishCardMPDI );
//
//		 SAFE_DELETE( m_AllNumerialImage[i] );
//	}
//}
//
//void CoinStackEffect::Init()
//{
//	m_CoinStack.empty();
//
//	m_MaxShowCount       = 0;
//	m_CoinStackMoveTm    = 0.f;
//	m_CoinStackDisplayTm = 0.f;
//
//	LoadCoinStackXMLData();
//}
//
//void CoinStackEffect::LoadCoinStackXMLData()
//{
//	cNodeISAX CoinStackDataXML;
//
//	bool bLoadXMLResult = CoinStackDataXML.ParseDataIntoXMLNode( COINSTACK_DATA );
//
//	if ( bLoadXMLResult )
//	{
//		TiXmlElement *pTempCoinStackData = CoinStackDataXML.GetRootElement();
//
//		pTempCoinStackData = pTempCoinStackData->FirstChildElement();
//
//		while ( pTempCoinStackData )
//		{
//			const WCHAR *l_strValue = pTempCoinStackData->Value();
//
//			COMPARE_VALUE( "CoinStackSetting" )
//			{
//				PARSE_ELEMENT_START( pTempCoinStackData )
//					COMPARE_NAME( "ShowCountPerPlayer" )
//					{
//						m_MaxShowCount = VALUE_TO_INT;
//					}
//					else COMPARE_NAME( "ImageMoveTime" )
//					{
//						m_CoinStackMoveTm = VALUE_TO_FLOAT;
//					}
//					else COMPARE_NAME( "ImageDisplayTime" )
//					{
//						m_CoinStackDisplayTm = VALUE_TO_FLOAT;
//					}
//				PARSE_NAME_VALUE_END
//			}
//			pTempCoinStackData = pTempCoinStackData->NextSiblingElement();
//		}
//
//		LoadAllFishCardData();
//	}
//}
//
//void CoinStackEffect::LoadAllFishCardData()
//{
//	m_AllCoinStackFishCardData.empty();
//
//	cMPDIList* pCoinStackFishCardMPDIList = cGameApp::GetMPDIListByFileName( COINSTACK_FISH_CARD_MPDILIST );
//
//	if ( pCoinStackFishCardMPDIList )
//	{
//		for ( int i = 0; i < m_MaxShowCount; i++ )
//		{
//		 	 vector< CoinStackFishCardData > TempAllFishCardData;
//
//			 for ( std::vector< CardImageData >::iterator It = cFishApp::m_spMonsterManager->m_CoinStackCardImageData.begin(); It != cFishApp::m_spMonsterManager->m_CoinStackCardImageData.end(); ++It )
//			 {
//				  CoinStackFishCardData TempFishCardData;
//
//				  TempFishCardData.FishID        = It->FishID;
//				  TempFishCardData.pFishCardMPDI = new cMPDI( pCoinStackFishCardMPDIList->GetObject( It->ImageName.c_str() ) );
//
//				  if ( TempFishCardData.pFishCardMPDI )
//				  {
//					  TempFishCardData.pFishCardMPDI->Init();
//					  TempAllFishCardData.push_back( TempFishCardData );
//				  }
//			 }
//
//			 m_AllCoinStackFishCardData.push_back( TempAllFishCardData );
//
//			 cPuzzleImage* pNumerial = cGameApp::GetPuzzleImageByFileName( PLAYER_NUMRIAL );
//
//			 if ( pNumerial )
//			 {
//				 cNumerialImage* pStackScoreNumerial = pNumerial->GetNumerialImageByName( L"CoinStackNumerial0", L"CoinStackNumerial9" );
//
//				 m_AllNumerialImage.push_back( pStackScoreNumerial );
//			 }
//		}
//	}
//}
//
//void CoinStackEffect::Update( float ElapseTime )
//{
//	Vector3 NowCardPos = Vector3( 0.f, 0.f, 0.f );
//
//	size_t i = 0;
//
//	while ( i < m_CoinStack.size() && ( int ) i < m_MaxShowCount )
//	{
//		 if ( !m_CoinStack[i].m_bSetData )
//			 m_CoinStack[i].SetCoinStackData( GetCoinStackFishCardByID( m_CoinStack[i].m_FishID ), GetNumerialImage(), m_CoinStackMoveTm );
//
//		 if ( i == 0 )
//		 {
//			 if ( m_CoinStack[i].m_pFishCard && m_CoinStack[i].m_pFishCard->IsDone() )
//				 m_CoinStack.pop_front();
//			 else
//			 {
//				 m_CoinStack[i].m_bUseGrayColor  = false;
//				 m_CoinStack[i].m_bCalculatorPos = true;
// 				 m_CoinStack[i].Update( ElapseTime );
//				 NowCardPos = m_CoinStack[i].m_pFishCard->GetPos();
//				 i++;
//			 }
//		 }
//		 else
//		 {
//			 NowCardPos.x -= 72.f;
//			 m_CoinStack[i].m_bUseGrayColor = true;
//			 m_CoinStack[i].m_pFishCard->SetPos( NowCardPos );
//			 m_CoinStack[i].Update( ElapseTime );
//			 i++;
//		 }
//	}
//}
//
//void CoinStackEffect::Render()
//{
//	size_t i = 0;
//
//	while ( i < m_CoinStack.size() && ( int ) i < m_MaxShowCount )
//	{
//		 m_CoinStack[i].Render();
//		 i++;
//	}
//}
//
//bool CoinStackEffect::PushCoinStackData( CoinStackPushData& CoinStackData )
//{
//	CoinStack TempCoinStack;
//
//	TempCoinStack.m_Money        = CoinStackData.Money;
//	TempCoinStack.m_FishID       = CoinStackData.FishID;
//	TempCoinStack.m_PosRot       = CoinStackData.PosRot;
//	TempCoinStack.m_ShowFrame.SetLocalTransform(CoinStackData.ShowMatrix);
//	TempCoinStack.m_DrawMoneyPos = CoinStackData.ShowMatrix.TransformCoordinate( Vector3( -177.f, -110.f, 0.f ) );
//
//	TempCoinStack.m_TargetPos = Vector3( 72.f, 0.f, 0.f );
//
//	m_CoinStack.push_back( TempCoinStack );
//
//	return true;
//}
//
//cNumerialImage* CoinStackEffect::GetNumerialImage()
//{
//	static int ImageIndex(0);
//
//	size_t ReturnIndex(0);
//
//	if ( ImageIndex < ( int ) m_AllNumerialImage.size() )
//	{
//		ReturnIndex = ImageIndex;
//
//		ImageIndex++;
//
//		if ( ImageIndex >= ( int ) m_AllNumerialImage.size() )
//			ImageIndex++; 
//	}
//
//	return m_AllNumerialImage[ ReturnIndex ];
//}
//
//cMPDI* CoinStackEffect:: GetCoinStackFishCardByID( int FishID )
//{
//	for ( size_t i = 0; i < m_AllCoinStackFishCardData.size(); i++ )
//	{
//		 for ( size_t j = 0; j < m_AllCoinStackFishCardData[i].size(); j++ )
//		 {
//			  if ( m_AllCoinStackFishCardData[i][j].FishID == FishID )
//			  {
//				  if ( m_AllCoinStackFishCardData[i][j].pFishCardMPDI && ( m_AllCoinStackFishCardData[i][j].pFishCardMPDI->GetCurrentProgress() == 0.f || m_AllCoinStackFishCardData[i][j].pFishCardMPDI->IsDone() ) )
//				  {
//					  m_AllCoinStackFishCardData[i][j].pFishCardMPDI->SetPos( Vector3( 0.f, 0.f, 0.f ) );
//					  return m_AllCoinStackFishCardData[i][j].pFishCardMPDI;
//				  }
//
//				  break;
//			  }
//		 }
//	}
//	return NULL;
//}
