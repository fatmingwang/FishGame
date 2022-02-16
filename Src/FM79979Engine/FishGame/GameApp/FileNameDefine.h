#ifndef _FISH_FILE_NAME_DEFINE_H_
#define _FISH_FILE_NAME_DEFINE_H_







#define	MAX_PLAYER	14

#define USING_GH8051

#define	PLAYER_DATA						"Fish/Player/PlayerData"
#define	COIN_DATA						"Fish/Player/Coin.xml"
#define	PLAYERUI_MPDI					L"Fish/Image/PlayerUI/PlayerUI.mpdi"
#define	PLAYER_NUMRIAL					L"Fish/Image/PlayerUI/PlayerUI.pi"

#define	IO8051_DATA_Refound				"Fish/IO/RefoundCoin.xml"

#define MESSAGE_MPDILIST				L"Fish/Image/Message.mpdi"

//fatming
#define	MONSTER_DATA					"Fish/MonsterData/FishData.xml"
#define	FISH_PROBABILITY				"Fish/Probability/Probability.xml"
#define	SCENE_CHANGE_SETUP_FILE			"Fish/Image/BG/BGSetup.xml"
#define	BG_WATER_MPDI					L"Fish/Image/Water.mpdi"
#define	SCENE_CHANGE_PRT				"Fish/Image/Bobble/Bobble.prt"
#define	SCENE_CHANGE_ALERT				L"Fish/Image/Message.mpdi"
#define	SCENE_CHANGE_BG_ANIMATION		L"Fish/Image/BG/BGAnimation/BGAnimation"
#define	BOSS_SETUP_FILE					"Fish/MonsterData/Boss/Boss.xml"
#define	BANK_FORMAT_PROBABILITY_FILE	"Fish/Probability/BankFormatProbability.xml"

#define	BULLET_DATA						"Fish/Bullet/Bullet.xml"

#define	FISH_GROUP_FILE					"Fish/MonsterData/Group/Winnings03.mpdi"
//#define	MONSTER_MANAGER_FISH_GROUP_FILE	"Fish/MonsterData/Group/Group.mpdi"

#define	TEST_LOG_FILE					"Fish/LogSetup/LogSetup.txt"
//first i64 is total in,second is total out rest is player's money
//but if auto shoot is enable player money won't be assign
#define	PLAYER_MONEY_DATA				"log.b"
//fatming end

//Added By Terry Begin
#define	MISSILE_DATA					"Fish/Player/MissileData.xml"
#define	MINIGAME_DATA					"Fish/MiniGame/MiniGameConfig.xml"
#define	COINSTACK_DATA					"Fish/Player/CoinStackSetting.xml"
#define WINMONEYEFFECT_DATA				"Fish/Player/WinMoneyEffectConfig.xml"
#define	CONTROLPANEL_VERSION_DATA		"Fish/ControlPanel/Version.xml"
#define	CONTROLPANEL_TIMESTAMP_DATA		"Fish/ControlPanel/TimeStamp.xml"

#define BIGWINPI						L"Fish/Image/BigWinImage/BigWin.pi"

#define BIGWIN_PARTICLE					"Fish/Image/bigwin_show_3_coin/BigPrize.prt"

#define COINSTACK_FISH_CARD_MPDILIST	L"Fish/Image/CoinEffect/ShowGetMoney.mpdi"

#define MISSILE_AIMLINE_MPDILIST		L"Fish/Image/AimImage/Line.mpdi"
#define MISSILE_AIMCARD_MPDILIST		L"Fish/Image/AimImage/AimCard.mpdi"

#define SMALL_FONT_FILENAME				"Fish/ControlPanel/EventFont_12"
//Added By Terry End

#endif