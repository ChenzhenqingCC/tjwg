#pragma once
#include "publicfunction.h"
#include "Autil.h"
#include <deque>
#include <stack>

using namespace std;
#define STATIC_NPC_NUM				200	//静态npc人数
#define CHAR_SKILL_NUM				30	//人物技能数
#define EMAIL_NUM					80	//邮件名片数
#define MENUFLAG					64	//448
#define MAXTALKLENGTH				20

#define SUCCESSFUL					1
#define SENDMSG_ERROR				-2
#define RECVMSG_ERROR				-3
#define CHECKSUM_ERROR				-4
#define SPLITMSG_ERROR				-5
#define GETFUNC_ERROR				-6
#define CDKEY_AND_PWD_ERROR			-7
#define INVALID_INFO				-8
#define GET_ACCOUNT_INFO_ERROR		-9
#define NOT_LOGGED_IN				-10
#define NOACCOUNT					0
#define DELETE_CHAR_ERROR			-1
#define CREATE_NEWCHAR_ERROR		-1
#define LOGIN_ERROR					-1

#define			CHAR_P_STRING_HP				( 1 << 1  )		// 0x00000002
#define			CHAR_P_STRING_MAXHP				( 1 << 2  )		// 0x00000004
#define			CHAR_P_STRING_MP				( 1 << 3  )		// 0x00000008
#define			CHAR_P_STRING_MAXMP				( 1 << 4  )		// 0x00000010
#define			CHAR_P_STRING_VITAL				( 1 << 5  )
#define			CHAR_P_STRING_STR				( 1 << 6  )
#define			CHAR_P_STRING_TOUGH				( 1 << 7  )
#define			CHAR_P_STRING_DEX				( 1 << 8  )
#define			CHAR_P_STRING_EXP				( 1 << 9  )
#define			CHAR_P_STRING_NEXTEXP			( 1 << 10 )
#define			CHAR_P_STRING_LV				( 1 << 11 )
#define			CHAR_P_STRING_ATK				( 1 << 12 )
#define			CHAR_P_STRING_DEF				( 1 << 13 )
#define			CHAR_P_STRING_QUICK				( 1 << 14 )
#define			CHAR_P_STRING_CHARM				( 1 << 15 )
#define			CHAR_P_STRING_LUCK				( 1 << 16 )
#define			CHAR_P_STRING_EARTH				( 1 << 17 )
#define			CHAR_P_STRING_WATER				( 1 << 18 )
#define			CHAR_P_STRING_FIRE				( 1 << 19 )
#define			CHAR_P_STRING_WIND				( 1 << 20 )
#define			CHAR_P_STRING_GOLD				( 1 << 21 )
#define			CHAR_P_STRING_TITLE				( 1 << 22 )
#define			CHAR_P_STRING_DUELPOINT			( 1 << 23 )
#define			CHAR_P_STRING_TRANSMIGRATION	( 1 << 24 )
#define			CHAR_P_STRING_NAME				( 1 << 25 )
#define			CHAR_P_STRING_OWNTITLE			( 1 << 26 )
#define			CHAR_P_STRING_RIDEPET			( 1 << 27 )		// 0x08000000
#define			CHAR_P_STRING_LEARNRIDE			( 1 << 28 )		// 0x10000000
#define			CHAR_P_STRING_BASEBASEIMAGENUMBER	( 1 << 29 )		// 0x20000000

#define			CHAR_N_STRING_OBJINDEX			( 1 << 1  )
#define			CHAR_N_STRING_LV				( 1 << 2  )
#define			CHAR_N_STRING_MAXHP				( 1 << 3  )
#define			CHAR_N_STRING_HP				( 1 << 4  )
#define			CHAR_N_STRING_MP				( 1 << 5  )
#define			CHAR_N_STRING_NAME				( 1 << 6  )

#define			CHAR_K_STRING_BASEIMAGENUMBER   ( 1 << 1  )
#define			CHAR_K_STRING_HP				( 1 << 2  )
#define			CHAR_K_STRING_MAXHP				( 1 << 3  )
#define			CHAR_K_STRING_MP				( 1 << 4  )
#define			CHAR_K_STRING_MAXMP				( 1 << 5  )
#define			CHAR_K_STRING_EXP				( 1 << 6  )
#define			CHAR_K_STRING_NEXTEXP			( 1 << 7  )
#define			CHAR_K_STRING_LV				( 1 << 8  )
#define			CHAR_K_STRING_ATK				( 1 << 9  )
#define			CHAR_K_STRING_DEF				( 1 << 10 )
#define			CHAR_K_STRING_QUICK				( 1 << 11 )
#define			CHAR_K_STRING_AI				( 1 << 12 )
#define			CHAR_K_STRING_EARTH				( 1 << 13 )
#define			CHAR_K_STRING_WATER				( 1 << 14 )
#define			CHAR_K_STRING_FIRE				( 1 << 15 )
#define			CHAR_K_STRING_WIND				( 1 << 16 )
#define			CHAR_K_STRING_SLOT				( 1 << 17 )
#define			CHAR_K_STRING_CHANGENAMEFLG		( 1 << 18 )
#define			CHAR_K_STRING_NAME				( 1 << 19 )
#define			CHAR_K_STRING_USERPETNAME		( 1 << 20 )

#define BC_FLG_NEW			(1<<0)
#define BC_FLG_DEAD			(1<<1)		//死亡
#define BC_FLG_PLAYER		(1<<2)		//玩家,玩家有异常状态时要有此值
#define BC_FLG_POISON		(1<<3)		//中毒
#define BC_FLG_PARALYSIS	(1<<4)		//麻痹
#define BC_FLG_SLEEP		(1<<5)		//昏睡
#define BC_FLG_STONE		(1<<6)		//石化
#define BC_FLG_DRUNK		(1<<7)		//眩晕
#define BC_FLG_CONFUSION	(1<<8)		//混乱
#define BC_FLG_HIDE			(1<<9)		//是否隐藏，地球一周
#define BC_FLG_REVERSE		(1<<10)		//反转

#define		WINDOW_BUTTONTYPE_NONE		(0)
#define		WINDOW_BUTTONTYPE_OK		(1 << 0)
#define		WINDOW_BUTTONTYPE_CANCEL	(1 << 1)
#define		WINDOW_BUTTONTYPE_YES		(1 << 2)
#define		WINDOW_BUTTONTYPE_NO		(1 << 3)
#define		WINDOW_BUTTONTYPE_PREV		(1 << 4)
#define		WINDOW_BUTTONTYPE_NEXT		(1 << 5)

#define		WINDOW_BUTTONTYPE_OKCANCEL	(WINDOW_BUTTONTYPE_OK | WINDOW_BUTTONTYPE_CANCEL)
#define		WINDOW_BUTTONTYPE_YESNO	(WINDOW_BUTTONTYPE_YES | WINDOW_BUTTONTYPE_NO)
typedef enum
{
	WINDOW_MESSAGETYPE_MESSAGE,
	WINDOW_MESSAGETYPE_MESSAGEANDLINEINPUT,
	WINDOW_MESSAGETYPE_SELECT,
	WINDOW_MESSAGETYPE_PETSELECT,
	WINDOW_MESSAGETYPE_PARTYSELECT,
	WINDOW_MESSAGETYPE_PETANDPARTYSELECT,
	WINDOW_MESSAGETYPE_ITEMSHOPMENU,
	WINDOW_MESSAGETYPE_ITEMSHOPMAIN,
	WINDOW_MESSAGETYPE_LIMITITEMSHOPMAIN,
	WINDOW_MESSAGETYPE_PETSKILLSHOP,
	WINDOW_MESSAGETYPE_WIDEMESSAGE,
	WINDOW_MESSAGETYPE_WIDEMESSAGEANDLINEINPUT,
	WINDOW_MESSAGETYPE_POOLITEMSHOPMENU,
	WINDOW_MESSAGETYPE_POOLITEMSHOPMAIN,

	WINDOW_MESSAGETYPE_FAMILYADD,
	WINDOW_MESSAGETYPE_FAMILYJOIN,
	WINDOW_MESSAGETYPE_FAMILYOUT,
	WINDOW_MESSAGETYPE_FAMILYEND
}WINDOW_MESSAGETYPE;
typedef enum
{
    CHAR_WINDOWTYPE_RETURNTOELDER=-1,   /*  墓戏へ提るウィンドウ    */
    CHAR_WINDOWTYPE_RESURRECTION=-2,   /*  牲宠するウィンドウ    */

	CHAR_WINDOWTYPE_SELECTBATTLE = 1,		/* 掐る里飘を联买するウィンドウ */
	CHAR_WINDOWTYPE_SELECTDUEL = 2,  		/* 掐るDUELを联买するウィンドウ */
	CHAR_WINDOWTYPE_SELECTTRADECARD = 3, 	/* 叹簧蛤垂を联买するウィンドウ */
	CHAR_WINDOWTYPE_SELECTPARTY = 4, 		/* パ〖ティを联买するウィンドウ */
	CHAR_WINDOWTYPE_SELECTBATTLEWATCH = 5, 	/* 囱里を联买するウィンドウ */
	CHAR_WINDOWTYPE_MICMESSAGE = 6,			/* MICNPCを蝗って叫るウィンドウ */

	// CoolFish: Trade 2001/4/18
	CHAR_WINDOWTYPE_SELECTTRADE = 7,	/* ユ Window */

	CHAR_WINDOWTYPE_SELECTRENAMEITEM_PAGE1 = 10,	/* 叹涟を恃构するアイテムを联买するウィンドウ */
	CHAR_WINDOWTYPE_SELECTRENAMEITEM_PAGE2 = 11,	/* 叹涟を恃构するアイテムを联买するウィンドウ */
	CHAR_WINDOWTYPE_SELECTRENAMEITEM_PAGE3 = 12,	/* 叹涟を恃构するアイテムを联买するウィンドウ */
	CHAR_WINDOWTYPE_SELECTRENAMEITEM_PAGE4 = 13,	/* 叹涟を恃构するアイテムを联买するウィンドウ */
	CHAR_WINDOWTYPE_SELECTRENAMEITEM_RENAME = 14,	/* 叹涟を掐蜗するウィンドウ */
	CHAR_WINDOWTYPE_SELECTRENAMEITEM_RENAME_ATTENTION  = 15,	/* 叹涟を掐蜗するウィンドウ */
	
	CHAR_WINDOWTYPE_DENGON = 50,			/* 帕咐饶 */

	CHAR_WINDOWTYPE_WINDOWMAN_START = 100,
	CHAR_WINDOWTYPE_WINDOWMAN_STARTMSG = CHAR_WINDOWTYPE_WINDOWMAN_START,
	CHAR_WINDOWTYPE_WINDOWMAN_END = 200,

	CHAR_WINDOWTYPE_JANKEN_START = 210,
	CHAR_WINDOWTYPE_JANKEN_MAIN = 211,
	CHAR_WINDOWTYPE_JANKEN_END = 212,
	
	CHAR_WINDOWTYPE_TRANSMIGRATION_START = 213,
	CHAR_WINDOWTYPE_TRANSMIGRATION_MAIN = 214,
	CHAR_WINDOWTYPE_TRANSMIGRATION_END = 215,
	CHAR_WINDOWTYPE_TRANSMIGRATION_NONE = 216,
	

	CHAR_WINDOWTYPE_WINDOWHEALER_START = 220,
	CHAR_WINDOWTYPE_WINDOWHEALER_STARTMSG = CHAR_WINDOWTYPE_WINDOWHEALER_START,
	CHAR_WINDOWTYPE_WINDOWHEALER_HPMSG = 221,
	CHAR_WINDOWTYPE_WINDOWHEALER_OKHPMSG = 222,
	CHAR_WINDOWTYPE_WINDOWHEALER_SPIRITMSG = 223,
	CHAR_WINDOWTYPE_WINDOWHEALER_OKSPIRITMSG = 224,
	CHAR_WINDOWTYPE_WINDOWHEALER_ALLMSG = 225,
	CHAR_WINDOWTYPE_WINDOWHEALER_OKALLMSG = 226,
	CHAR_WINDOWTYPE_WINDOWHEALER_END = 227,

	CHAR_WINDOWTYPE_WINDOWSAVEPOINT_START = 230,

	CHAR_WINDOWTYPE_WINDOWEVENT_STARTMSG = 231,
	CHAR_WINDOWTYPE_WINDOWEVENT_NOWEVENT = 232,
	CHAR_WINDOWTYPE_WINDOWEVENT_ENDEVENT = 233,
	CHAR_WINDOWTYPE_WINDOWEVENT_REQMAINMSG = 234,
	CHAR_WINDOWTYPE_WINDOWEVENT_ACCMAINMSG = 235,
	CHAR_WINDOWTYPE_WINDOWEVENT_NOMALMSG = 236,
	CHAR_WINDOWTYPE_WINDOWEVENT_CLEANMSG = 237,
	CHAR_WINDOWTYPE_WINDOWEVENT_REQTHANK = 238,
	CHAR_WINDOWTYPE_WINDOWEVENT_ACCTHANK = 239,
	

	CHAR_WINDOWTYPE_WINDOWITEMSHOP_START = 240,
	CHAR_WINDOWTYPE_WINDOWITEMSHOP_STARTMSG = CHAR_WINDOWTYPE_WINDOWITEMSHOP_START,
	CHAR_WINDOWTYPE_WINDOWITEMSHOP_MENU = 241,
	CHAR_WINDOWTYPE_WINDOWITEMSHOP_BUY_MSG = 242,
	CHAR_WINDOWTYPE_WINDOWITEMSHOP_SELL_MSG = 243,
	CHAR_WINDOWTYPE_WINDOWITEMSHOP_END = 244,
	CHAR_WINDOWTYPE_WINDOWITEMSHOP_LIMIT = 245,
	CHAR_WINDOWTYPE_WINDOWITEMSHOP_EXPRESS = 246,

	CHAR_WINDOWTYPE_DUELRANKING_START = 250,
	CHAR_WINDOWTYPE_DUELRANKING_TOPRANKING = 251,
	CHAR_WINDOWTYPE_DUELRANKING_MYRANKING = 252,
	CHAR_WINDOWTYPE_DUELRANKING_WAIT = 253,
	CHAR_WINDOWTYPE_DEFEND_BILLDBOARD = 254,

	CHAR_WINDOWTYPE_WINDOWPETSKILLSHOP = 260,

	CHAR_WINDOWTYPE_WINDOWPETSHOP_START = 261,
	CHAR_WINDOWTYPE_WINDOWPETSHOP_PETSELECT = 262,
	CHAR_WINDOWTYPE_WINDOWPETSHOP_MAIN = 263,
	CHAR_WINDOWTYPE_WINDOWPETSHOP_GOLDOVER 	= 264,
	CHAR_WINDOWTYPE_WINDOWPETSHOP_PETSELECT2 = 265,
	CHAR_WINDOWTYPE_WINDOWPETSHOP_MAIN2 = 266,
	CHAR_WINDOWTYPE_WINDOWPETSHOP_DRAWSELECT = 267,
	CHAR_WINDOWTYPE_WINDOWPETSHOP_ASKDRAW = 268,
	CHAR_WINDOWTYPE_WINDOWPETSHOP_END = 269,

	CHAR_WINDOWTYPE_WINDOWWARPMAN_MAIN = 271,
	CHAR_WINDOWTYPE_WINDOWWARPMAN_ERR = 272,
	CHAR_WINDOWTYPE_WINDOWWARPMAN_END = 273,

	CHAR_WINDOWTYPE_NPCENEMY_START = 281,
	
	CHAR_WINDOWTYPE_CHARM_START = 282,
	CHAR_WINDOWTYPE_CHARM_END = 283,

	CHAR_WINDOWTYPE_QUIZ_START = 284,
	CHAR_WINDOWTYPE_QUIZ_MAIN = 285,
	CHAR_WINDOWTYPE_QUIZ_END = 286,

	CHAR_WINDOWTYPE_CHECKMAN_START = 287,
	CHAR_WINDOWTYPE_CHECKMAN_MAIN = 288,
	CHAR_WINDOWTYPE_CHECKMAN_END = 289,

	CHAR_WINDOWTYPE_FAMILYMAN_START = 320,
	CHAR_WINDOWTYPE_FAMILYMAN_ADD = 321,
	CHAR_WINDOWTYPE_FAMILYMAN_JOIN = 322,
	CHAR_WINDOWTYPE_FAMILYMAN_OUT = 323,
	CHAR_WINDOWTYPE_FAMILYMAN_BROKEN = 324,
	
	CHAR_WINDOWTYPE_BANKMAN = 330,
	
	// CoolFish: FMPKMan 2001/7/4
	CHAR_WINDOWTYPE_FMPKMAN_START = 340,
	CHAR_WINDOWTYPE_FMPKMAN_VIEW = 341,
	CHAR_WINDOWTYPE_FMPKMAN_LEAVEPK = 342,
	CHAR_WINDOWTYPE_FMPKCALLMAN_START = 345,
	CHAR_WINDOWTYPE_FMPKCALLMAN_CALL = 346,
	CHAR_WINDOWTYPE_FMPKCALLMAN_COME = 347,
	CHAR_WINDOWTYPE_FMPKCALLMAN_LEAVE = 348,
	
	// Arminius 7.13 scheduleman
	CHAR_WINDOWTYPE_SCHEDULEMAN_START = 350,
	CHAR_WINDOWTYPE_SCHEDULEMAN_SELECT = 351,
	CHAR_WINDOWTYPE_SCHEDULEMAN_DETAIL = 352,
	
	// Arminius 7.27
	CHAR_WINDOWTYPE_MANORPK_START = 360,
	CHAR_WINDOWTYPE_MANORPK_ASK = 361,
	CHAR_WINDOWTYPE_MANORPK_END = 362,

        // shan
        CHAR_WINDOWTYPE_FM_DENGON      = 370,     // 產壁痙ē狾
        CHAR_WINDOWTYPE_FM_FMSDENGON   = 371,     // 產壁ぇ丁痙ē狾
        CHAR_WINDOWTYPE_FM_MESSAGE1    = 372,     // 弧跌怠(沮翴)
        CHAR_WINDOWTYPE_FM_MESSAGE2    = 373,     // 弧跌怠(Θ)
        CHAR_WINDOWTYPE_FM_SELECT      = 374,     // 匡兜跌怠
        CHAR_WINDOWTYPE_FM_MEMBERLIST  = 375,     // Θ
        CHAR_WINDOWTYPE_FM_POINTLIST   = 376,     // 沮翴
        CHAR_WINDOWTYPE_FM_DPTOP       = 377,     // 眏(玡篯)
        CHAR_WINDOWTYPE_FM_DPME        = 378,     // 眏(篯)
        CHAR_WINDOWTYPE_FM_DPSELECT    = 379,     // 眏匡兜跌怠

	//#ifdef _TRANSER_MAN
	NPC_TRANSERMAN_START = 440,
	NPC_TRANSERMAN_SELECT,
	NPC_TRANSERMAN_WARP,
	NPC_TRANSERMAN_END,
	//#endif
}CHAR_WINDOWTYPE;
//人物类型，玩家，各种npc等编号类型
typedef enum
{
    CHAR_TYPENONE,
    CHAR_TYPEPLAYER,
    CHAR_TYPEENEMY,
    CHAR_TYPEPET,
    CHAR_TYPEDOOR,
    CHAR_TYPEBOX,
    CHAR_TYPEMSG,
    CHAR_TYPEWARP,
    CHAR_TYPESHOP,
    CHAR_TYPEHEALER,
    CHAR_TYPEOLDMAN,
    CHAR_TYPEROOMADMIN,
    CHAR_TYPETOWNPEOPLE,
    CHAR_TYPEDENGON,
    CHAR_TYPEADM,
    CHAR_TYPETEMPLE,
    CHAR_TYPESTORYTELLER,
    CHAR_TYPERANKING,
    CHAR_TYPEOTHERNPC,
    CHAR_TYPEPRINTPASSMAN,
    CHAR_TYPENPCENEMY,
    CHAR_TYPEACTION,
    CHAR_TYPEWINDOWMAN,
    CHAR_TYPESAVEPOINT,
    CHAR_TYPEWINDOWHEALER,
	CHAR_TYPEITEMSHOP,
	CHAR_TYPESTONESHOP,
	CHAR_TYPEDUELRANKING,
	CHAR_TYPEWARPMAN,
	CHAR_TYPEEVENT,
	CHAR_TYPEMIC,
	CHAR_TYPELUCKYMAN,
	CHAR_TYPEBUS,
	CHAR_TYPECHARM,
	CHAR_TYPECHECKMAN,
	CHAR_TYPEJANKEN,
	CHAR_TYPETRANSMIGRATION,
	CHAR_TYPEFMWARPMAN,			// 產壁⑥①初恨瞶
	CHAR_TYPEFMSCHEDULEMAN,		// 產壁⑥①初祅癘
	CHAR_TYPEMANORSCHEDULEMAN,	// 缠堕⑥①初箇
	CHAR_TRANSERMANS=44			//天才传送师
}CHAR_TYPE;
typedef enum
{
	CHAR_COLORWHITE,
	CHAR_COLORCYAN,
	CHAR_COLORPURPLE,
	CHAR_COLORBLUE,
	CHAR_COLORYELLOW,
	CHAR_COLORGREEN,
	CHAR_COLORRED,
	CHAR_COLORGRAY,
	CHAR_COLORBLUE2,
	CHAR_COLORGREEN2,

}CHAR_COLOR;
typedef enum
{
	MAGIC_FIELD_ALL,
	MAGIC_FIELD_BATTLE,	
	MAGIC_FIELD_MAP

}MAGIC_FIELDTYPE;
typedef enum
{
	MAGIC_TARGET_MYSELF,
	MAGIC_TARGET_OTHER,	
	MAGIC_TARGET_ALLMYSIDE,
	MAGIC_TARGET_ALLOTHERSIDE,
	MAGIC_TARGET_ALL,
	MAGIC_TARGET_NONE,
	MAGIC_TARGET_OTHERWITHOUTMYSELF,
	MAGIC_TARGET_WITHOUTMYSELFANDPET,
	MAGIC_TARGET_WHOLEOTHERSIDE
}MAGIC_TARGETTYPE;
typedef enum
{
 	PETSKILL_FIELD_ALL,
 	PETSKILL_FIELD_BATTLE,
 	PETSKILL_FIELD_MAP 
}PETSKILL_FIELDTYPE;
typedef enum
{
 	PETSKILL_TARGET_MYSELF,
 	PETSKILL_TARGET_OTHER,
 	PETSKILL_TARGET_ALLMYSIDE,
 	PETSKILL_TARGET_ALLOTHERSIDE,
 	PETSKILL_TARGET_ALL,
 	PETSKILL_TARGET_NONE,
 	PETSKILL_TARGET_OTHERWITHOUTMYSELF,
 	PETSKILL_TARGET_WITHOUTMYSELFANDPET
}PETSKILL_TARGETTYPE;

typedef enum{
	REST=0,
	FIGHTSTANDBY=1,
	EMAIL=4
}PETSTATE;
//格式：1|CHAR_WHICHTYPE|NPC编号|X坐标|Y坐标|人物方向|人物形象|人物等级|名称颜色|人物名称|人物称号|CHAR_ISOVERED|CHAR_HAVEHEIGHT|英雄称号颜色|家族名|骑宠名|骑宠等级|0|0|0|0
typedef struct{
	int unknown;
	int charwhichtype;
	int objindex;		//npc、人物编号
	int x;
	int y;
	int dir;
	int faceimg;
	int level;
	int namecolor;
	char name[100];
	char owntitle[50];
	int isovered;
	int haveheight;
	int popupnamecolor;
	char familyname[50];
	char ridepetname[50];
	int ridepetlevel;
	int unknown1;
	int unknown2;
	int unknown3;
	int unknown4;
}NPC_INFO;//即可用于保存npc信息，也可表示玩家信息
typedef struct{
	int id;		//精灵所在装备的位置编号（头0，身1，武器2，左饰3，右饰4）
	int kubun;//装备中的精灵是否可用，装上装备为1，取下装备后为0
	int mp;
	int field;
	int target;
	char name[100];
	char comment[200];
}MAGIC;

typedef struct{
	int dataplace;
	int faceimage;
	int	level;
	int maxhp;
	int atk;
	int def;
	int quick;
	int charm;
	int dp;
	int earth;
	int water;
	int fire;
	int wind;
	int logincount;
	char name[50];
	char place[50];
}CHARLIST;
typedef struct{
	int kubun;
	int hp;
	int maxhp;
	int mp;
	int maxmp;
	int vital;
	int str;
	int tough;
	int dex;
	int exp;
	int nextexp;
	int level;
	int attack;
	int defence;
	int fQuick;
	int fCharm;
	int fLuck;
	int fEarth;
	int fWater;
	int fFire;
	int fWind;
	int gold;
	int indexofeqtitle;
	int dp;
	int transmigration;
	int ridepet;		//骑乘宠物编号,无骑宠为-1
	int learnride;
	int faceimg;
	int unknown;
	char name[50];
	char owntitle[50];
	/////////////////////
	int uplevelpoint;
	int fame;			//声望
}CHARDETAIL;
typedef struct{
	int skillid;
	int field;
	int target;
	char name[50];
	char comment[100];
}PETSKILL;
typedef struct{
	int no;//编号
	int islive;//不在身上为0(丢出)，在身上为1
	int image;
	int hp;
	int maxhp;
	int mp;
	int maxmp;
	int exp;
	int nextexp;
	int level;
	int attack;
	int defence;
	int quick;
	int ai;
	int fEarth;
	int fWater;
	int fFire;
	int fWind;
	int slot;
	int changenameflag;
	int transmigration;
	int unknown;
	char oldname[50];
	char newname[50];
	PETSKILL petskill[7];
	///////////////
	int state;		//宠物状态，0=休息，1=战斗|等待，4=邮件	
}PETDETAIL;
typedef struct{
	int useflag;
	int identity;
	int target;
	int kind;
	int icon;
	int costmp;
	int level;
	char name[50];
	char comment[100];
}CHARSKILL;
typedef struct{//物品位置|物品名称|parashow|颜色|物品描述|物品图像编号|能使用的场合|目标|级别|标志|损坏程度|叠加数量|类型|itemlevel
	char name[50];			//名字不为空有物品，否则没有物品
	char paramshow[50];
	int color;
	char comment[100];
	int imgno;
	int ableusefield;
	int target;
	int level;
	int flg;		//不允许(0),邮寄(1)，从某物合成(2)，堆叠(4)等
	char damagecrushe[50];
	int pilenums;
	char element[50];
	int itemlevel;
	char unknown[50];
}ITEMINFO;
typedef struct{
	int floor;
	int maxX;
	int maxY;
	int x;
	int y;
}CHANGEMAP;
typedef struct{
	int floor;
	int x1;
	int y1;
	int x2;
	int y2;
	int unknown1;
	int unknown2;
	int unknown3;
	char floorname[50];
	////////////////地图切换事件编号
	int seqno;
}MAPINFO;
typedef struct{
	char fmname[50];
	int fmleaderflag;
	int workfmchannel;
	int fmsprite;
	int workfmfloor;
}FAMILY;
typedef struct{
	int windowtype;
	int buttontype;
	int seqno;
	int objindex;
	char data[8192];
}DIALOGWINDOW;
typedef struct{
	int use;
	char charname[50];
	int level;
	int dp;
	int online;			//下线为0，上线为4
	int graphicsno;
	int transmigration;
	int unknown;
}MAILCONTACTLIST;
typedef struct{
	int charid;		//人物编号
	int time;		//登入时间
	int fightpet;	//战宠编号,//无战宠为-1,有战宠为其编号
	int menuflg;	//菜单旗标
	int x;			//人物当前位置
	int y;
	int direction;	//人物方向
	int state;		//游戏状态，-1登出，0平时，1战斗
	int round;		//战斗回合数
}CHAROTHERINFO;
typedef struct{
	int whotalk;
	char message[255];
	int color;
}TALKMESSAGE;
//BC|战场属性（0:无属性,1:地,2:水,3:火,4:风）|人物在组队中的位置|人物名称|人物称号|人物形象编号|人物等级(16进制)|当前HP|最大HP|人物状态（死亡，中毒等）|是否骑乘标志(0:未骑，1骑,-1落马)|骑宠名称|骑宠等级|骑宠HP|骑宠最大HP|战宠在队伍中的位置|战宠名称|未知|战宠形象|战宠等级|战宠HP|战宠最大HP|战宠异常状态（昏睡，死亡，中毒等）|0||0|0|0|
//敌人第1排从上到下依次为(13，11，F，10，12)，敌人第2排从上到下为(E,C,A,B,D)
//我方第1排从上到下依次为（9，7，5，6，8），我方第2排从上到下为(4,2,0,1,3)
typedef struct{
	int pos;
	char name[50];
	char owntitle[50];
	int faceimg;
	int level;
	int hp;
	int maxhp;
	int state;
	int isride;
	char ridepetname[50];
	int ridepetlevel;
	int ridepethp;
	int ridepetmaxhp;
	//战宠信息
	int petpos;
	char petname[50];
	char unknown[50];
	int petimage;
	int petlevel;
	int pethp;
	int petmaxhp;
	int petstate;
	int unknown2;
	char unknown3[50];
	int unknown4;
	int unknown5;
	int unknown6;
}BC_CHAR_STATE_STRING;
//敌1位置|敌1名称|未知|敌1形象|敌1等级|敌1HP|敌1最大HP|敌人异常状态（死亡，中毒等）|0||0|0|0|
typedef struct{
	int pos;
	char name[50];
	char unknown[50];
	int image;
	int level;
	int hp;
	int maxhp;
	int enemystate;
	int unknown2;
	char unknown3[50];
	int unknown4;
	int unknown5;
	int unknown6;
}BC_ENEMY_STATE_STRING;
//购买东西的物品结构
typedef struct{
	char name[50];			//物品名称
	int unknown;
	int uselevel;			//多少级可以使用
	int price;				//价格
	int image;				//物品形象
	char comment[100];		//注释
	int costfame;			//购买花费声望,-1代表不要声望
}BUYITEM;
//购买物品时显示信息的结构
typedef struct{
	int unknown;			//为0
	int dataflg;			//为1首次显示商品清单窗口,带有清单数据，为0不是首次显示，后面不带清单数据
	int windowtype;			//窗口类型，类型不同显示的信息不同，见CHAR_WINDOWTYPE_WINDOWITEMSHOP_STARTMSG
	char main_msg[100];		//店名
	char buy_main[100];		//买东西时显示的信息
	char what_msg[100];		//询问要买什么东西
	char howmany_msg[100];	//问买多少
	char level_msg[100];	//人物级别不够时显示的信息
	char really_msg[100];	//确认是否要买
	char itemfull_msg[100];	//道具满时显示的信息
	int num;				//有多项东西可买，自己添加
	BUYITEM item[50];		//物品清单
}BUY_MSG;
//存取道具时，道具信息结构
typedef struct{
	char name[50];		//道具名称
	int cansave;		//是否可存放，1不可以，0可以
	int price;			//道具价格
	int image;
	char comment[100];	//注释
	int num;			//道具数量
	int position;		//道具位置(10-24)
}SAVE_ITEM;
//存道具信息结构
typedef struct{
	int unknown;
	int unknown1;
	char npcname[100];		//npc名字
	char what_msg[100];		//存哪个道具
	char itemfull_msg[100];	//道具满时显示的信息
	char really_msg[100];	//确认是否要存
	SAVE_ITEM saveitem[15];
}SAVE_MSG;
//取道具时，道具信息结构
typedef struct{
	char name[50];		//道具名称
	int unknown;		
	int unknown1;			
	int unknown2;
	int image;
	char comment[100];	//注释
	int num;			//道具数量
}LOAD_ITEM;
//向npc存道具时信息结构
typedef struct{
	int unknown;
	char npcname[100];		//npc名字
	char what_msg[100];		//取哪个道具
	char itemfull_msg[100];	//道具满时显示的信息
	char really_msg[100];	//确认是否要存
	LOAD_ITEM loaditem[50];
}LOAD_MSG;
//从宠物店取宠物时，宠物信息
typedef struct{
	int lv;
	int maxhp;
	char name[50];
}LOAD_PET;
//传送师返回的传送点信息结构
typedef struct{
	char main_msg[100];			//窗口信息
	int num;					//传送点数目
	char trans_point[20][100];	//传送点
}TRANS_POINT;
typedef struct{
	int x;
	int y;
	char direction[2];
}WALKARRAY;
//人物精灵补血
typedef struct{
	int id;			//精灵的索引
	char name[50];	//精灵的名称
	int val;		//补血设定界限
}RECRUITBLOOD;
//宠物补血
typedef struct{
	int skillid;	//技能id
	int val;		//补血设定界限
}PETRECRUITBLOOD;
//定义脚本变量结构
typedef struct{
	CString name;
	int val;
}INTVAR;
//摆滩结构
typedef struct{
	int type;	//类型，0道具，1宠物
	int pos;	//出售物品或宠物的位置
	int price;	//价格
}BAITAN;
//从摆滩那购买物品返回的物品清单
typedef struct{
	int type;			//类型，0道具，1宠物
	int price;			//价格
	char name[50];		//名称
	char othername[50];	//宠物新名,物品没有此项
	int num;			//物品数量，宠物没有此项
	int pos;			//售卖菜单中的位置
}BUY_BAITAN;
class CDpMain
{
public:
	CDpMain(void);
	~CDpMain(void);
	SOCKET socket;
	
	CCriticalSection csLocalSingal;		//信号量
	int scriptNum;						//脚本行数
	CString *script;					//保存脚本
	CMap<CString,LPCTSTR, CString, LPCTSTR> intvar;//脚本变量字典
	USERINFO user;						//帐号信息
	CHARLIST charlist[2];				//人物列表
	NPC_INFO npcinfo[STATIC_NPC_NUM];				//npc信息,包括人物动态信息
	MAGIC magic[6];						//人物身上的精灵信息
	CHARDETAIL chardetail;				//人物详细信息
	PETDETAIL petdetail[5];				//宠物详细信息
	CHANGEMAP changemap;				//改变地图返回的信息
	MAPINFO mapinfo;					//当前地图的信息
	ITEMINFO iteminfo[24];				//物品信息
	CHARSKILL charskill[CHAR_SKILL_NUM];			//人物技能信息
	FAMILY family;						//家族信息
	DIALOGWINDOW windowinfo;			//窗口信息
	BUY_MSG buy_msg;					//购卖窗口信息
	SAVE_MSG save_msg;					//存寄放店时信息结构
	LOAD_MSG load_msg;					//取寄放店时信息结构
	LOAD_PET load_pet[5];					//取宠物时，显示的宠物信息解析后存于此
	BAITAN bai_tan[20];					//保存系统返回的摆滩信息
	BUY_BAITAN buy_baitan[20];			//售卖菜单信息
	BOOL IsBaiTanOk;					//点击摆滩按钮后，服务端是否返回确认信息
	TRANS_POINT trans_point;			//传送点信息
	MAILCONTACTLIST mailcontactlist[EMAIL_NUM];	//邮件联系人列表
	BC_CHAR_STATE_STRING bc_char[5];		//战斗中人物队伍信息
	BC_ENEMY_STATE_STRING bc_enemy[10];		//战斗中敌人队伍信息
	BOOL bExit;							//让游戏线程终止
	BOOL bScriptExit;					//让脚本线程终止
	BOOL bIsReLogin;					//是否刚进行了登录（重登），用于控制脚本的跳转
	BOOL bIsBaiTaning;					//是否正处于摆滩中，摆好滩后置为true，取消摆滩后置为false 
	int IP;								//指令计数器
	int ErrorLine;						//脚本错误指针
	DWORD nStartTime;					//发送连接信息计时器
	DWORD nRecvTime;					//接收连接信息计时器
	CHAROTHERINFO charotherinfo;		//人物的其它信息	
	CAutil autil;
	deque <TALKMESSAGE *> talkmessage;	//保存游戏中说话信息
	stack <int> IPStack;				//指令堆栈
	BOOL IsOnLine;						//是否在线
	BOOL IsLogin;						//人物是否正在登录中
	BOOL IsDispTalk;					//是否显示说话内容
	BOOL IsDispInfoOnTime;				//是否显示实时更新信息
	int maxload;						//最大负重
	int MapSeqNo;						//地图切换事件编号
	PHCALC phcalc;						//用于防脚本计算
	BOOL bIsSendDummy;					//是否发送过dummy信息
	//参数设置
	int  nScriptDelay;				//脚本延时（0-250）
	BOOL bReLogin;					//是否断线重登
	BOOL bScriptErrorReStart;		//脚本出错重新运行
	BOOL nAutoUnlock;				//自动解锁
	BOOL IsSafeCodeUnlock;			//是否已解锁
	BOOL bAutoCapture;				//自动捉宠
	BOOL bAutoEscape;				//自动逃跑
	BOOL bAutoKNPC;					//自动KNPC
	BOOL bFullEscape;				//落马逃跑
	BOOL bFightpetDeadEscape;		//战宠死亡自动逃跑
	BOOL bMakeTeam;					//组队
	BOOL bDuel;						//决斗
	BOOL bCallingCard;				//名片
	BOOL bTrade;					//交易
	char cZDAttack[10][50];			//指定攻击的宠物
	int nZDAttackNum;				//指定攻击的宠物的宠物数量
	char cLockAttack[10][50];		//锁定攻击的宠物
	int nLockAttackNum;
	char cZDEscape[10][50];			//指定逃跑的宠物
	int nZDEscapeNum;
	int nAutoUpPoint[4];			//自动加点，1000以下为体力，1000以上为腕力，2000以上为耐力，3000以上为速度
	BOOL bAutoEatSYC;				//自动吃鱼鳃草
	int	nEatSYCTime;				//上次吃鱼鳃草的时间
	BOOL bAutoExpNut;				//吃智慧果
	int nExpNutTime;				//上次吃智慧果的时间
	BOOL bAutoPile;					//自动堆叠
	BOOL bEatMeat;					//吃补血肉
	BOOL bDiscardMeat;				//丢没有补血功能的血肉
	BOOL bRecruitMpPlaceTime;		//平时补气
	int nLockRidePet;				//锁定骑宠
	int nLockFightPet;				//锁定战宠
	char cDiscardLikeItem[15][50];	//丢弃包含
	int nDiscardLikeItemNum;
	char cDiscardExceptItem[15][50];//丢弃不包含
	int nDiscardExceptItemNum;
	double dFirstDelay;				//首次延迟
	double dAttackDelay;			//攻击延迟
	double dRMpByBlood;				//嗜血补气
	char cCharFirstAction[50];		//人物首次动作
	char cCharAction[50];			//人物一般动作
	char cPetFirstAction[50];		//战宠首次动作
	char cPetAction[50];			//战宠一般动作
	int nCapLevel;					//捕捉等级
	char cCapPetName[5][50];		//捉宠名称
	int nCapPetNum;					//和捉宠名称配合使用，捉宠数量
	int nCapPetBlood;				//捉宠血量
	int nCapCharUseSkill;			//捉宠时人物的技能和捉宠血量
	int nCapPetSkill;				//捉宠时宠物使用的技能
	BOOL bCapEscapeWhenNoPet;		//是否逃跑当没有要捕获的宠物时
	RECRUITBLOOD recruitblood;		//保存人物精灵补血设置(包括战时和平时)
	PETRECRUITBLOOD petrecruitblood;		//宠物技能补血
	BOOL bDeleteChar;				//重登时是否删除帐号

	void Init();
	void Run(USERINFO *puser);
	void RunScript();
	void SetExit(BOOL bexit);
	void SetScriptExit(BOOL bexit);
	int CheckUser(char * cdkey, char * pwd);
	int DeleteChar(int id);
	int CreateNewChar(int dataplace,char *charname,int imgno,int faceimgno,int vital,int str,int tgh,int dex,int earth,int water,int fire,int wind,int hometown);
	int CharLogin(int dataplace);
	BOOL ReceiveData(char *buf,int &datalen);
	int ParseLoginData(char *data,int datalen);
	int DecryptLoginData(char *data,int datalen);
	int ParseData(char *data,int datalen);
	int DecryptData(char *data,int datalen,int &count);
	void Parse_P_StatusString(char *data);
	void Parse_K_StatusString(char *data);
	void Parse_BC_StatusString(char *data);
	void Parse_BA_String(int endblt,char *data,char *enemy);
	void Parse_NPC_String(char *data);
	void Parse_BE_String(char *data,char *dst);
	int GetNearNpcObjIndex(int x, int y);
	void ReleaseTalkDeque();
	void TalkMsgToDeque(int whotalk,char *msg,int color);
	void AutoAddUpLevelPoint();
	void CalcMaxLoad();
	BOOL ItemCanPile(int flg);
	void GetCalcFormula(char *msg,char *dst);
	BOOL TranForumla(CString src);
	BOOL CalcJampAddress(CString szJump,int &lid);
	void GetTarget(char *src,char *dst,char *mypos,char *enemypos);
	void func34(int from ,int to);
	void func25(char *message);
	void func32(char *message);
	//发送指令函数
	int SendOnlineInfo(char *info);
	int SendEOCommand();
	int SendMenuFlag(int flg);
	int SendLogFlag(int flg);
	int SendFightPet(int pet);
	int SendStandbyPet(int pet);
	int SendPetState();
	int SendPetState(int pet,int state);
	int SendRidePet(int pet);
	int SendDiscardPet(int pet);		
	int SendWalkPos(int x,int y,char *direction);
	int SendWalkDirection(int x,int y,char *direction);
	int SendTalk(int x,int y,char *msg,int color,int area);
	int SendSelectWindowButton(int x,int y,int seqno,int objindex,int select,char *data);

	int SendChangeMap(int seqno,int x,int y,int dir=-1);
	int SendAttackCommand(int round,char *enemy);
	int SendDummy();
	int SendUpLevelPoint(int flg);
	int SendMoveItem(int from,int to);
	int SendDiscardItem(int index);
	int SendDiscardMoney(int amount);
	int SendSaveLoadMoney(int num);
	int SendSaveLoadMoneyFromFamily(int num);
	int SendUseItem(int from,int to);
	int SendPickup(int x,int y,int dir);
	int SendEatExpNut();
	int SendEatSYC();
	int SendDiscardLikeItem();
	int SendDiscardExceptItem();
	int SendSafeCode(int seqno);
	int SendEatMeat();
	int SendDiscardMeat();
	int SendRecruitMpPlaceTime();
	int SendRecruitHpPlaceTime(int magicid,int to);
	int SendAutoPile();
	int SendCommand(char *cmd);
	int SendChangeCharName(char *name);
	int SendChangePetName(int pet,char *name);
	int SendPetMail(int towho,int petindex,int itemindex,char *msg,int color);
	int SendExchangeCard();
	int SendDeleteCard(int card);
	int SendMerge(int petid,int petskillid,int toindex,char *data);
	int SendBaiTan(char *data);
	int SendBaiTanOK(int x,int y,int data);
	//脚本指令
	CString GetOperateCode(CString instruction);
	CString GetOperateNum(CString instruction);
	BOOL WalkPos(CString &para);
	int CalcWalkPos(WALKARRAY *walk,int xstart,int ystart,int xend,int yend);
	void CalcCharPosition(WALKARRAY *walk);
	void ParseSetPara(CString &data);
	BOOL BaiTan(CString para);
	BOOL BuyFromBaiTan(CString para);
	BOOL UseItem(CString item);
	int GetLabelNum(CString label);
	void Pickup(CString dir);
	void DoffItem(CString item);
	void DoffPet(CString  pet);
	void DoffStone(CString stone);
	void LoadStone(CString stone);
	void SaveStone(CString stone);
	void LoadStoneFromFamily(CString stone);
	void SaveStoneToFamily(CString stone);
	BOOL LiaoLi(CString para);
	BOOL WaitMap(CString para);
	void BuyItem(CString item);	
	void SaveItem(int towhere,CString item);
	void SellPet(CString item);
	void GetItem(int fromwhere,CString item);
	void SavePet(int towhere,CString item);
	void ParsePetData(char *data);
	void GetPet(int fromwhere,CString item);
	void MoveItem(CString para);	
	BOOL CharTrans(CString para);
	BOOL If_Check(CString para);
	BOOL Let_Set(CString para);
	void GetValue(CString &key,int &val);
	BOOL WaitSay(CString para);
	BOOL WaitDlg(CString para);
	void Button(CString para);	
	BOOL SetPetState(CString para);
	BOOL SignDpCheck(CString para);
	BOOL SignHYDCheck(CString para);
	BOOL SendPetMail(CString para);

	BOOL IfMap(CString para);
	BOOL IfPos(CString para);
	BOOL IfStone(CString para);
	BOOL IfFight(CString para);
	BOOL IfDlg(CString para);
	BOOL IfSay(CString para);
	BOOL IfEquipment(CString szItem,CString szTest,CString szPara,CString szJump);
	BOOL IfItemName(CString szItem,CString szTest,CString szPara,CString szJump);
	BOOL IfItemPosEmpty(CString szItem,CString szTest,CString szPara,CString szJump);
	BOOL IfItemIndexName(CString szItem,CString szTest,CString szPara,CString szJump);
	BOOL IfItemIndexPosEmpty(CString szItem,CString szTest,CString szPara,CString szJump);
	BOOL IfPetName(CString szPet,CString szTest,CString szPara,CString szJump);
	BOOL IfPetPosEmpty(CString szPet,CString szTest,CString szPara,CString szJump);
	BOOL IfPetIndexName(CString szPet,CString szTest,CString szPara,CString szJump);
	BOOL IfPetIndexPosEmpty(CString szPet,CString szTest,CString szPara,CString szJump);
	BOOL CheckChar(CString szPara);
	BOOL CheckCharLV(CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckCharEXP(CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckCharHP(CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckCharHPP(CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckCharMP(CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckCharMPP(CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckCharDP(CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckCharAttack(CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckCharDefence(CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckCharQuick(CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckCharCharm(CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckCharVital(CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckCharStr(CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckCharTough(CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckCharDex(CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckCharTransmigration(CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckCharUpLevelPoint(CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckCharGold(CString szTest,int num,int jump,BOOL iscall);

	BOOL CheckPet(CString szPara);
	BOOL CheckPetJob(int petid,CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckPetLV(int petid,CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckPetEXP(int petid,CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckPetHP(int petid,CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckPetHPP(int petid,CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckPetAttack(int petid,CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckPetDefence(int petid,CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckPetQuick(int petid,CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckPetAI(int petid,CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckPetTransmigration(int petid,CString szTest,int num,int jump,BOOL iscall);
	BOOL CheckMap(CString szPara);
	BOOL CheckPosition(CString szPara);
	BOOL CheckSay(CString szPara);
	BOOL CheckDlg(CString szPara);
	BOOL CheckGameState(CString szPara);
	BOOL CheckRelogin(CString szPara);
	BOOL CheckBaiTaning(CString szPara);
};

