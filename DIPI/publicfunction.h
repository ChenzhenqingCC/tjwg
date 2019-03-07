#pragma once
//�����ip��ַ�Ͷ˿ں�
typedef struct{
	char ip[30];
	int port;
}SERVERINFO;
//��¼�ʺ���Ϣ
typedef struct{
	char charname[30];
	char password[30];
	char safecode[30];
	char charidx[30];
	int index;			//�ڼ����ʺ�
	char scriptName[100];//�ű��ļ���
}USERINFO;
//�������̴߳������Ͳ���
typedef struct{
	int index;
}THREADPARA;
//�ص���Ϣ����
enum{
	NOTIFY_TIME,
	NOTIFY_MSG,
	NOTIFY_LOGIN,
	NOTIFY_LOGOUT,
	NOTIFY_CHARDETAIL,
	NOTIFY_MAP,
	NOTIFY_XY,
	NOTIFY_UPLEVELPOINT,
	NOTIFY_SCRIPT,
	NOTIFY_STARTSCRIPT,
	NOTIFY_STOPSCRIPT,
	NOTIFY_GAMESTATE,
	NOTIFY_ROUND,
	NOTIFY_FAME,
	NOTIFY_FIGHTPET,
	NOTIFY_RIDEPET,
	NOTIFY_TEAMSUM,
};

//״̬������
enum CHAR_DETAIL
{
	DETAIL_NAME,
	DETAIL_SCRIPT,
	DETAIL_FIGHTPETEXP,
	DETAIL_RIDEPETEXP,
	DETAIL_LEVEL,
	DETAIL_EXP,
	DETAIL_NEXTEXP,
	DETAIL_HP,
	DETAIL_MP,
	DETAIL_ATK,
	DETAIL_DEF,
	DETAIL_MIN,
	DETAIL_MEI,
	DETAIL_ZHUAN,
	DETAIL_SHU,
	DETAIL_TI,
	DETAIL_WAN,
	DETAIL_NAI,
	DETAIL_SU,
	DETAIL_DP,
	DETAIL_CASH,
	DETAIL_SHENG,
	DETAIL_LVPOINT,
	DETAIL_MAP,
	DETAIL_MAPNAME,
	DETAIL_POS,
	DETAIL_STATE,
	DETAIL_ROUND,
	DETAIL_TEAMSUM,
	DETAIL_ALL
};

//�ص���Ϣ�ṹ
typedef struct{
	UINT nNotityType;		//��Ϣ���ͣ��������ö�ٶ���
	LPVOID lpNotifyData;	//��Ϣ����
}NOTIFYPARA;
typedef enum{
	PHINPUT,
	PHSELECT
}CALCTYPE;
typedef struct{
	int N1;		//��һ��������
	int N2;		//�ڶ���������
	char oper;	//�����
	int result; //������
	CALCTYPE type;//��������
	int selindex;		//ѡ����ʱѡ������
}PHCALC;
typedef void (*CALLBACK_Func) (WPARAM wParam,NOTIFYPARA *msg);
int GetScriptLinsNum(const char *path);
CString Encrypt(CString s);
CString Decrypt(CString s);
BOOL DirExist(LPCTSTR lpszDirName);
BOOL FileExist(LPCTSTR lpszFileName);
void DbgLog( LPCTSTR lpszFormat, ... );
int WriteDataToFile(LPCSTR filename,char * data,long size,LPCSTR mode, int nStartPos=-1 );
CString GetCurTimeString ();
int ConnectServer(SOCKET &rsocket,char *ip,int port);
void Tokenize(char *source,char *dest,char *token,int &nstart);
int mystrstr(char *src,char *substr,int len);
unsigned int HexStrToDec(char *data);
double PerStrToDbl(CString &data);
BOOL IsDigit(char c);
BOOL IsNumber(CString text);
int Myatoi(CString text);
//�����ַ���
int  FindingString(const char* lpszSour, const char* lpszFind, int nStart = 0);
//��ͨ������ַ���ƥ��
bool MatchingString(const char* lpszSour, const char* lpszMatch, bool bMatchCase = true);
//����ƥ��
bool MultiMatching(const char* lpszSour, const char* lpszMatch, int nMatchLogic = 0, bool bRetReversed = 0, bool bMatchCase = true);
int myatoi(CString src);
CString GetDigitFormString(CString str);
//��ȡ���������ַ�������|���ָ�
BOOL GetNicInfo(char *dst);
CString GetFirstNicInfo();
//��Ȩ��⺯��
CString GetUser();
COleDateTime GetLicenceDate();
COleDateTime GetLastUseDate();
COleDateTime GetRegDate();
int GetMaxNum();
BOOL WriteLastUseTime();
int LicenceIsValid();
BOOL SelfLicenceValid();
CString EncryptNew(CString s);
int SplitString(const CString str, char split, CStringArray &strArray);
CString GetWorkDir();

static int en_key[] = { 4, 9, 6, 2, 8, 7, 3 };
static CString licence_name = "licence.dat";
static CString licence_self = "licence_czq8678.dat";

