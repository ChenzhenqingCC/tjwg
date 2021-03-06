
// DIPIDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DIPI.h"
#include "DIPIDlg.h"
#include "afxdialogex.h"
#include "DpMain.h"
#include "AccountEdit.h"
#include "tlhelp32.h"
#include "smtp/zstring.h"
#include <windows.h>
#include <ImageHlp.h>
#pragma comment(lib, "DbgHelp.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2,LPARAM lParamSort);
//排序方式，升序或降序
BOOL m_bASC=TRUE;
//当前排序的列
int m_nSordCol=0;
CALLBACK_Func f_Callback = NULL;
WPARAM f_Param = NULL;

//用于向线程传递参数
THREADPARA g_para[100];
CArray <USERINFO> g_userinfo;
//int g_userNum=0;
SERVERINFO g_serverinfo;

long gametime;
CArray<CDpMain*> pDp;
//设置回调函数及其参数
void Set_Callback (CALLBACK_Func func,WPARAM wParam )
{
	f_Callback = func;
	f_Param = wParam;
}
//回调函数
void Callback_func(WPARAM wParam,NOTIFYPARA *pNotifyPara)
{
	CDIPIDlg *p = (CDIPIDlg *)wParam;
	ASSERT ( p);	
	p->CallbackMsg(pNotifyPara);
}
//回调函数，由其他类调用
void Callback_Notify(NOTIFYPARA *pNotifyPara)
{
	if(f_Callback)
		f_Callback(f_Param,pNotifyPara);
}

void CreateDumpFile(LPCSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)
{
	// 创建Dump文件  
	//  
	HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	//MessageBox(NULL, lpstrDumpFilePathName, "提示信息", MB_ICONINFORMATION);
	// Dump信息  
	//  
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;

	// 写入Dump文件内容  
	//  
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);

	CloseHandle(hDumpFile);
}

LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
	//MyCreateDirectory("debug");
	
	time_t tNow = time(NULL);
	CTime cTime(tNow);
	CString time_str = cTime.Format(_T("%Y_%m_%d_%H_%M_%S"));
	CreateDumpFile(time_str + ".dmp", pException);

	CHAR FileName[MAX_PATH] = { 0 };
	STARTUPINFO si;
	PROCESS_INFORMATION pi;


	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	GetModuleFileNameA(NULL, FileName, MAX_PATH);


	if (!CreateProcessA(
		FileName,
		"AUTORUN",
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		(LPSTARTUPINFOA)&si,
		&pi))
	{
		MessageBeep(MB_OK);
	}

	return EXCEPTION_EXECUTE_HANDLER;
}

void InitCrashReport()
{
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
}


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGetregcode();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)	
	ON_BN_CLICKED(ID_GETREGCODE, &CAboutDlg::OnBnClickedGetregcode)
END_MESSAGE_MAP()


// CDIPIDlg 对话框




CDIPIDlg::CDIPIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDIPIDlg::IDD, pParent)
	, m_line(0)
	, m_autorun(FALSE)	
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	TimerOn=0;
	gametime=0;
	GameTimerOn = 0;
}

void CDIPIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTCHAR, m_list);
	DDX_CBIndex(pDX, IDC_LINE, m_line);
	DDX_Control(pDX, IDC_TIME, m_time);
	DDX_Control(pDX, IDC_MESSAGE, m_message);
	DDX_Control(pDX, IDC_LIST2, m_charinfo);
	DDX_Check(pDX, IDC_CHK_RUNSCRIPT, m_autorun);
}

BEGIN_MESSAGE_MAP(CDIPIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_APPEND, &CDIPIDlg::OnBnClickedAppend)
	ON_BN_CLICKED(IDC_DELETE, &CDIPIDlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_CLEAR, &CDIPIDlg::OnBnClickedClear)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_START, &CDIPIDlg::OnBnClickedStart)	
	ON_BN_CLICKED(IDC_STOP, &CDIPIDlg::OnBnClickedStop)
	ON_COMMAND(ID_MENU_RUN, &CDIPIDlg::OnMenuRun)
	ON_COMMAND(ID_MENU_STOP, &CDIPIDlg::OnMenuStop)
	ON_COMMAND(ID_MENU_PAUSE, &CDIPIDlg::OnMenuPause)
	ON_COMMAND(ID_SINGLELOGIN, &CDIPIDlg::OnSinglelogin)
	ON_COMMAND(ID_SINGLELOGOUT, &CDIPIDlg::OnSinglelogout)
	ON_NOTIFY(NM_CLICK, IDC_LISTCHAR, &CDIPIDlg::OnNMClickListchar)
	ON_NOTIFY(NM_RCLICK, IDC_LISTCHAR, &CDIPIDlg::OnNMRClickListchar)
	ON_BN_CLICKED(IDC_CHkTALK, &CDIPIDlg::OnBnClickedChktalk)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST2, &CDIPIDlg::OnLvnColumnclickList2)
	ON_BN_CLICKED(IDC_SAVE, &CDIPIDlg::OnBnClickedSave)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTCHAR, &CDIPIDlg::OnNMDblclkListchar)
	ON_BN_CLICKED(IDC_CHKUPDATE, &CDIPIDlg::OnBnClickedChkupdate)
	ON_BN_CLICKED(IDC_GETPCCODE, &CDIPIDlg::OnBnClickedGetpccode)
	ON_BN_CLICKED(IDC_SAVE2, &CDIPIDlg::OnBnClickedSave2)
	ON_BN_CLICKED(IDC_SEL_SCRIPT, &CDIPIDlg::OnBnClickedSelScript)
END_MESSAGE_MAP()


// CDIPIDlg 消息处理程序

BOOL CDIPIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	InitCrashReport();
	detail_width_dic.SetAt(DETAIL_NAME, 70);
	detail_width_dic.SetAt(DETAIL_SCRIPT, 150);
	detail_width_dic.SetAt(DETAIL_LEVEL, 40);
	detail_width_dic.SetAt(DETAIL_EXP, 80);
	detail_width_dic.SetAt(DETAIL_NEXTEXP, 80);
	detail_width_dic.SetAt(DETAIL_HP, 70);
	detail_width_dic.SetAt(DETAIL_MP, 40);
	detail_width_dic.SetAt(DETAIL_ATK, 40);
	detail_width_dic.SetAt(DETAIL_DEF, 40);
	detail_width_dic.SetAt(DETAIL_MIN, 40);
	detail_width_dic.SetAt(DETAIL_MEI, 40);
	detail_width_dic.SetAt(DETAIL_ZHUAN, 40);
	detail_width_dic.SetAt(DETAIL_SHU, 40);
	detail_width_dic.SetAt(DETAIL_TI, 40);
	detail_width_dic.SetAt(DETAIL_WAN, 40);
	detail_width_dic.SetAt(DETAIL_NAI, 40);
	detail_width_dic.SetAt(DETAIL_SU, 40);
	detail_width_dic.SetAt(DETAIL_DP, 150);
	detail_width_dic.SetAt(DETAIL_CASH, 150);
	detail_width_dic.SetAt(DETAIL_SHENG, 150);
	detail_width_dic.SetAt(DETAIL_LVPOINT, 150);
	detail_width_dic.SetAt(DETAIL_MAP, 150);
	detail_width_dic.SetAt(DETAIL_MAPNAME, 150);
	detail_width_dic.SetAt(DETAIL_POS, 150);
	detail_width_dic.SetAt(DETAIL_STATE, 150);
	detail_width_dic.SetAt(DETAIL_ROUND, 150);
	detail_width_dic.SetAt(DETAIL_FIGHTPETEXP, 250);
	detail_width_dic.SetAt(DETAIL_RIDEPETEXP, 80);
	detail_width_dic.SetAt(DETAIL_TEAMSUM, 100);

	detail_name_dic.SetAt(DETAIL_NAME, _T("人物"));
	detail_name_dic.SetAt(DETAIL_SCRIPT, _T("当前脚本"));
	detail_name_dic.SetAt(DETAIL_LEVEL, _T("Level"));
	detail_name_dic.SetAt(DETAIL_EXP, _T("经验"));
	detail_name_dic.SetAt(DETAIL_NEXTEXP, _T("Next"));
	detail_name_dic.SetAt(DETAIL_HP, _T("HP"));
	detail_name_dic.SetAt(DETAIL_MP, _T("MP"));
	detail_name_dic.SetAt(DETAIL_ATK, _T("攻击"));
	detail_name_dic.SetAt(DETAIL_DEF, _T("防御"));
	detail_name_dic.SetAt(DETAIL_MIN, _T("敏捷"));
	detail_name_dic.SetAt(DETAIL_MEI, _T("魅力"));
	detail_name_dic.SetAt(DETAIL_ZHUAN, _T("转生"));
	detail_name_dic.SetAt(DETAIL_SHU, _T("属性"));
	detail_name_dic.SetAt(DETAIL_TI, _T("体力"));
	detail_name_dic.SetAt(DETAIL_WAN, _T("腕力"));
	detail_name_dic.SetAt(DETAIL_NAI, _T("耐力"));
	detail_name_dic.SetAt(DETAIL_SU, _T("速度"));
	detail_name_dic.SetAt(DETAIL_DP, _T("会员点"));
	detail_name_dic.SetAt(DETAIL_CASH, _T("现金"));
	detail_name_dic.SetAt(DETAIL_SHENG, _T("声望"));
	detail_name_dic.SetAt(DETAIL_LVPOINT, _T("宠物信息"));
	detail_name_dic.SetAt(DETAIL_MAP, _T("地图"));
	detail_name_dic.SetAt(DETAIL_MAPNAME, _T("地图名称"));
	detail_name_dic.SetAt(DETAIL_POS, _T("坐标"));
	detail_name_dic.SetAt(DETAIL_STATE, _T("状态"));
	detail_name_dic.SetAt(DETAIL_ROUND, _T("回合"));
	detail_name_dic.SetAt(DETAIL_FIGHTPETEXP, _T("战宠经验"));
	detail_name_dic.SetAt(DETAIL_RIDEPETEXP, _T("战斗用时"));
	detail_name_dic.SetAt(DETAIL_TEAMSUM, _T("队伍人数"));

	GetDlgItem(IDC_SAVE2)->ShowWindow(FALSE);

	time_t tNow = time(NULL);
	CTime cTime(tNow);
	CString time_str = cTime.Format(_T("%Y %m月%d日 %H:%M:%S"));
	GetDlgItem(IDC_BEGTIME)->SetWindowText("启动时间:" + time_str);

	TCHAR szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0; // 删除文件名，只获得路径字串
	CString str_url = szFilePath;
	GetDlgItem(IDC_ROOTPATH)->SetWindowText("启动路径:" + str_url);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	//默认界面实时更新
	CButton * pbox = (CButton *)GetDlgItem(IDC_CHKUPDATE);
	pbox->SetCheck(1);

	LONG styles;
	styles = m_charinfo.GetExtendedStyle();
	//LVS_EX_FULLROWSELECT整行选中
	//LVS_EX_GRIDLINES加网格线
	//LVS_EX_CHECKBOXES前面加复选框
	m_charinfo.SetExtendedStyle(styles | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	LV_COLUMN lvColumn;
	lvColumn.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_FMT | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_LEFT;

	for (int i = 0; i < DETAIL_ALL; ++i)
	{
		lvColumn.cx = (int)(detail_width_dic[(CHAR_DETAIL)i]);
		lvColumn.iSubItem = 0; // 第一列 
		lvColumn.pszText = (LPSTR)(LPCSTR)(detail_name_dic[(CHAR_DETAIL)i]);
		m_charinfo.InsertColumn(i, &lvColumn);
	}

	//lvColumn.cx=70; 
	//lvColumn.iSubItem=0; // 第一列 
	//lvColumn.pszText=_T("人物"); 
	//m_charinfo.InsertColumn(0,&lvColumn);
	//lvColumn.cx = 150;
	//lvColumn.iSubItem = 24;
	//lvColumn.pszText = _T("当前脚本");
	//m_charinfo.InsertColumn(1, &lvColumn);

	//lvColumn.cx=40;
	//lvColumn.iSubItem=1; // 第二列 
	//lvColumn.pszText=_T("Level"); 
	//m_charinfo.InsertColumn(2,&lvColumn); 
	//lvColumn.cx=80; 
	//lvColumn.iSubItem=2; 
	//lvColumn.pszText=_T("经验"); 
	//m_charinfo.InsertColumn(3,&lvColumn);
	//lvColumn.cx=80; 
	//lvColumn.iSubItem=3; 
	//lvColumn.pszText=_T("Next"); 
	//m_charinfo.InsertColumn(4,&lvColumn); 
	//lvColumn.cx=70; 
	//lvColumn.iSubItem=4;
	//lvColumn.pszText=_T("HP"); 
	//m_charinfo.InsertColumn(5,&lvColumn);
	//lvColumn.iSubItem=5; 
	//lvColumn.pszText=_T("MP"); 
	//m_charinfo.InsertColumn(6,&lvColumn);
	//lvColumn.cx=40; 
	//lvColumn.iSubItem=6;  
	//lvColumn.pszText=_T("攻击"); 
	//m_charinfo.InsertColumn(7,&lvColumn); 
	//lvColumn.iSubItem=7; 
	//lvColumn.pszText=_T("防御"); 
	//m_charinfo.InsertColumn(8,&lvColumn); 
	//lvColumn.iSubItem=8;
	//lvColumn.pszText=_T("敏捷"); 
	//m_charinfo.InsertColumn(9,&lvColumn);
	//lvColumn.iSubItem=9;  
	//lvColumn.pszText=_T("魅力"); 
	//m_charinfo.InsertColumn(10,&lvColumn);
	//lvColumn.iSubItem=10;  
	//lvColumn.pszText=_T("转生"); 
	//m_charinfo.InsertColumn(11,&lvColumn);
	//lvColumn.cx=60; 
	//lvColumn.iSubItem=11; 
	//lvColumn.pszText=_T("属性"); 
	//m_charinfo.InsertColumn(12,&lvColumn);
	//lvColumn.cx=40; 
	//lvColumn.iSubItem=12;
	//lvColumn.pszText=_T("体力"); 
	//m_charinfo.InsertColumn(13,&lvColumn);
	//lvColumn.iSubItem=13;  
	//lvColumn.pszText=_T("腕力"); 
	//m_charinfo.InsertColumn(14,&lvColumn);
	//lvColumn.iSubItem=14;
	//lvColumn.pszText=_T("耐力"); 
	//m_charinfo.InsertColumn(15,&lvColumn);
	//lvColumn.iSubItem=15;  
	//lvColumn.pszText=_T("速度"); 
	//m_charinfo.InsertColumn(16,&lvColumn);
	//lvColumn.cx=80; 
	//lvColumn.iSubItem=16;  
	//lvColumn.pszText=_T("DP"); 
	//m_charinfo.InsertColumn(17,&lvColumn);
	//lvColumn.cx=80; 
	//lvColumn.iSubItem=17;  
	//lvColumn.pszText=_T("现金"); 
	//m_charinfo.InsertColumn(18,&lvColumn);
	//lvColumn.cx=80; 
	//lvColumn.iSubItem=18;  
	//lvColumn.pszText=_T("声望"); 
	//m_charinfo.InsertColumn(19,&lvColumn);
	//lvColumn.cx=60; 
	//lvColumn.iSubItem=19;  
	//lvColumn.pszText=_T("升级点数"); 
	//m_charinfo.InsertColumn(20,&lvColumn);
	//lvColumn.cx=50; 
	//lvColumn.iSubItem=20;  
	//lvColumn.pszText=_T("地图"); 
	//m_charinfo.InsertColumn(21,&lvColumn);
	//lvColumn.cx=120; 
	//lvColumn.iSubItem=21;  
	//lvColumn.pszText=_T("地图名称"); 
	//m_charinfo.InsertColumn(22,&lvColumn);
	//lvColumn.cx=60; 
	//lvColumn.iSubItem=22;  
	//lvColumn.pszText=_T("坐标"); 
	//m_charinfo.InsertColumn(23,&lvColumn);
	//lvColumn.cx=40; 
	//lvColumn.iSubItem=23;  
	//lvColumn.pszText=_T("状态"); 
	//m_charinfo.InsertColumn(24,&lvColumn);
	////lvColumn.cx=150; 
	////lvColumn.iSubItem=24;  
	////lvColumn.pszText=_T("当前脚本"); 
	////m_charinfo.InsertColumn(24,&lvColumn);
	//lvColumn.cx=40; 
	//lvColumn.iSubItem=25;  
	//lvColumn.pszText=_T("回合"); 
	//m_charinfo.InsertColumn(25,&lvColumn);
	//帐号列表的设置
	styles = m_list.GetExtendedStyle();
	m_list.SetExtendedStyle(styles | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	lvColumn.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_FMT | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 70;
	lvColumn.iSubItem = 0; // 第一列 
	lvColumn.pszText = _T("账号");
	m_list.InsertColumn(0, &lvColumn);
	lvColumn.cx = 60;
	lvColumn.iSubItem = 1; // 第二列 
	lvColumn.pszText = _T("角色");
	m_list.InsertColumn(1, &lvColumn);
	lvColumn.cx = 60;
	lvColumn.iSubItem = 2; // 第二列 
	lvColumn.pszText = _T("脚本");
	m_list.InsertColumn(2, &lvColumn);
	/*
	lvColumn.cx = 100;
	lvColumn.iSubItem = 3; // 第二列 
	lvColumn.pszText = _T("脚本");
	m_list.InsertColumn(3, &lvColumn);
	lvColumn.cx = 100;
	lvColumn.iSubItem = 4; // 第二列 
	lvColumn.pszText = _T("序号");
	m_list.InsertColumn(4, &lvColumn);*/

	CString szAccount, szPwd, szSafeCode, szScript, szVal, szCharIdx;
	int nRow, pos, i = 0;
	try {
		CString szVal;
		CStdioFile f(_T("data.txt"), CFile::modeRead | CFile::typeText);
		CDIPIApp *pApp = (CDIPIApp *)AfxGetApp();
		USERINFO uinfo;
		g_userinfo.RemoveAll();
		while (f.ReadString(szVal) && i < pApp->MAXACCOUNT) {
			i++;
			pos = szVal.Find(_T("\r"));
			if (pos > 0)
				szVal = szVal.Left(pos);
			else {
				pos = szVal.Find(_T("\n"));
				if (pos > 0)
					szVal = szVal.Left(pos);
			}
			//if(pos>0){
			pos = 0;
			szAccount = szVal.Tokenize("|", pos);
			szPwd = szVal.Tokenize("|", pos);
			szSafeCode = szVal.Tokenize("|", pos);
			szScript = szVal.Tokenize("|", pos);
			szCharIdx = szVal.Tokenize("|", pos);

			strcpy_s(uinfo.charname, szAccount);
			strcpy_s(uinfo.password, szPwd);
			strcpy_s(uinfo.safecode, szSafeCode);
			strcpy_s(uinfo.scriptName, szScript);
			strcpy_s(uinfo.charidx, szCharIdx);
			uinfo.index = i;
			g_userinfo.Add(uinfo);

			nRow = m_list.InsertItem(m_list.GetItemCount(), szAccount);
			m_list.SetItemText(nRow, 1, "");
			m_list.SetItemText(nRow, 2, szScript);
			//m_list.SetItemText(nRow, 3, szScript);
			//m_list.SetItemText(nRow, 4, szCharIdx);
			//}
		}
		f.Close();
	}
	catch (...) {
	}
	//帐号处理
	int g_userNum = m_list.GetItemCount();
	if (g_userNum > 0) {
		//g_userinfo.RemoveAll();
		//USERINFO uinfo;
		//CDpMain cdm;
		//g_userinfo= new USERINFO[g_userNum];
		for (int i = 0; i < g_userNum; i++) {
			/*
			szAccount = m_list.GetItemText(i, 0);
			szPwd = m_list.GetItemText(i, 1);
			szSafeCode = m_list.GetItemText(i, 2);
			szScript = m_list.GetItemText(i, 3);
			szCharIdx = m_list.GetItemText(i, 4);
			strcpy_s(uinfo.charname, szAccount);
			strcpy_s(uinfo.password, szPwd);
			strcpy_s(uinfo.safecode, szSafeCode);
			strcpy_s(uinfo.scriptName, szScript);
			strcpy_s(uinfo.charidx, szCharIdx);
			uinfo.index = i;
			g_userinfo.Add(uinfo);*/
			//CDpMain cdm;
			pDp.Add(new CDpMain());
		}
		//pDp.RemoveAll();
		//pDp.SetSize(10);
	}
	else
	{
		g_userinfo.RemoveAll();
		for (int i = 0; i < pDp.GetSize(); ++i)
		{
			delete pDp[i];
		}
		pDp.RemoveAll();
	}
		
	gametime=0;
	Set_Callback(Callback_func, WPARAM(this));

	GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_START)->EnableWindow(TRUE);

	int argn;
	LPWSTR s = GetCommandLineW();
	//MessageBox("cmd:" + (CString)s);//这就是文件名了。
	//LPWSTR   *cmdLineAry = CommandLineToArgvW(s, &argn);
	//if (strcmp((CString)s, "AUTORUN") == 0)
	if(strstr((CString)s, "CZQ") == NULL && pDp.GetSize() > 0)
	{
		//MessageBox((CString)cmdLineAry[1] + " " + );//这就是文件名了。
		//m_autorun = TRUE;
		CButton * pbox = (CButton *)GetDlgItem(IDC_CHK_RUNSCRIPT);
		pbox->SetCheck(1);
		OnBnClickedStart();
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDIPIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDIPIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDIPIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//向帐号列表上添加一项
void CDIPIDlg::OnBnClickedAppend()
{
	CString szAccount,szPwd,szSafeCode,szScript,szVal,szCharIdx;
	GetDlgItem(IDC_ACCOUNT)->GetWindowText(szAccount);
	if(szAccount.IsEmpty()){
		AfxMessageBox(_T("请输入帐号！"));
		return;
	}
	GetDlgItem(IDC_PASSWORD)->GetWindowText(szPwd);
	if(szPwd.IsEmpty()){
		AfxMessageBox(_T("请输入密码！"));
		return;
	}
	GetDlgItem(IDC_SAFECODE)->GetWindowText(szSafeCode);
	if(szSafeCode.IsEmpty()){
		AfxMessageBox(_T("请输入安全码！"));
		return;
	}

	GetDlgItem(IDC_SCRIPTNAME)->GetWindowText(szScript);
	if (szScript.IsEmpty()) {
		AfxMessageBox(_T("输入脚本路径！"));
		return;
	}

	GetDlgItem(IDC_CHARIDX)->GetWindowText(szCharIdx);
	if(szCharIdx.IsEmpty()){
		AfxMessageBox(_T("请选择人物序号！"));
		return;
	}

	int num;
	CDIPIApp *pApp=(CDIPIApp *)AfxGetApp();
	if(m_list.GetItemCount()>=pApp->MAXACCOUNT){
		AfxMessageBox("外挂未注册，帐户数目受限制!注册请与作者联系！");
		return;
	}
	num=m_list.GetItemCount();
	int nRow = -1;
	for(int i=0;i<num;i++){
		szVal=m_list.GetItemText(i,0);
		if (szVal.CompareNoCase(szAccount) == 0)
		{
			nRow = i;
			break;
		}	
	}
	int fidx = -1;
	if (nRow == -1)
		nRow = m_list.InsertItem(num, szAccount);
	else
		fidx = nRow;
	m_list.SetItemText(nRow, 1, "");
	//m_list.SetItemText(nRow, 2, szSafeCode);
	m_list.SetItemText(nRow, 2, szScript);
	//m_list.SetItemText(nRow, 4, szCharIdx);

	if (fidx >= 0)
	{
		USERINFO & uinfo = g_userinfo[fidx];
		/*
		szAccount = m_list.GetItemText(fidx, 0);
		szPwd = m_list.GetItemText(fidx, 1);
		szSafeCode = m_list.GetItemText(fidx, 2);
		szScript = m_list.GetItemText(fidx, 3);
		szCharIdx = m_list.GetItemText(fidx, 4);*/
		strcpy_s(uinfo.charname, szAccount);
		strcpy_s(uinfo.password, szPwd);
		strcpy_s(uinfo.safecode, szSafeCode);
		strcpy_s(uinfo.scriptName, szScript);
		strcpy_s(uinfo.charidx, szCharIdx);
		pDp[fidx]->user = uinfo;
	}
	else
	{
		USERINFO uinfo;
		/*
		szAccount = m_list.GetItemText(nRow, 0);
		szPwd = m_list.GetItemText(nRow, 1);
		szSafeCode = m_list.GetItemText(nRow, 2);
		szScript = m_list.GetItemText(nRow, 3);
		szCharIdx = m_list.GetItemText(nRow, 4);*/
		strcpy_s(uinfo.charname, szAccount);
		strcpy_s(uinfo.password, szPwd);
		strcpy_s(uinfo.safecode, szSafeCode);
		strcpy_s(uinfo.scriptName, szScript);
		strcpy_s(uinfo.charidx, szCharIdx);
		g_userinfo.Add(uinfo);
		//CDpMain cdm;
		pDp.Add(new CDpMain());
	}
}

//删除帐号列表上的一项
void CDIPIDlg::OnBnClickedDelete()
{
	CString szAccount,szPwd,szSafeCode,szScript,szVal, szCharIdx;
	int index;
	index=(int)m_list.GetFirstSelectedItemPosition()-1;
	if(index==-1){
		AfxMessageBox(_T("请先选择要删除的帐号！"));
		return;
	}
	m_list.DeleteItem(index);
	if (index < g_userinfo.GetSize())
	{
		g_userinfo.RemoveAt(index);
		delete pDp[index];
		pDp.RemoveAt(index);
	}
}

//清空帐号列表
void CDIPIDlg::OnBnClickedClear()
{
	m_list.DeleteAllItems();
	GetDlgItem(IDC_ACCOUNT)->SetWindowText("");
	GetDlgItem(IDC_PASSWORD)->SetWindowText("");
	GetDlgItem(IDC_SAFECODE)->SetWindowText("");
	GetDlgItem(IDC_SCRIPTNAME)->SetWindowText("");
	((CComboBox*)GetDlgItem(IDC_CHARIDX))->SetCurSel(0);
	g_userinfo.RemoveAll();
	Sleep(1000);
	for (int i = 0; i < pDp.GetSize(); ++i)
	{
		delete pDp[i];
	}

	pDp.RemoveAll();	
}

//用户帐号列表框中被选项发生改变时
void CDIPIDlg::OnNMClickListchar(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int index;
	CString szAccount,szPwd,szSafeCode,szScript,szVal,szCharIdx;
	index=(int)m_list.GetFirstSelectedItemPosition()-1;
	if (index < 0 || index >= m_list.GetItemCount())
		return;
	szAccount=m_list.GetItemText(index,0);
	szPwd= g_userinfo[index].password;
	szSafeCode= g_userinfo[index].safecode;
	szScript= g_userinfo[index].scriptName;
	szCharIdx = g_userinfo[index].charidx;
	GetDlgItem(IDC_ACCOUNT)->SetWindowText(szAccount);
	GetDlgItem(IDC_PASSWORD)->SetWindowText(szPwd);
	GetDlgItem(IDC_SAFECODE)->SetWindowText(szSafeCode);
	GetDlgItem(IDC_SCRIPTNAME)->SetWindowText(szScript);
	if(szCharIdx == _T("人物一"))
		((CComboBox*)GetDlgItem(IDC_CHARIDX))->SetCurSel(0);
	else
		((CComboBox*)GetDlgItem(IDC_CHARIDX))->SetCurSel(1);
	*pResult = 0;
}
//在帐号列表上点击右键时弹出菜单
void CDIPIDlg::OnNMRClickListchar(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CPoint mousePoint;
	GetCursorPos (&mousePoint);

	CMenu m_popMenu;
	m_popMenu.LoadMenu(IDR_MENU1);
	CMenu*   pSubMenu=m_popMenu.GetSubMenu(0);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,mousePoint.x,mousePoint.y,this);
	*pResult = 0;
}

void CDIPIDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	if(TimerOn){
		KillTimer(1);
		TimerOn=0;
	}
	if (GameTimerOn)
	{
		KillTimer(2);
		GameTimerOn = 0;
	}
	//关闭所有线程，并等待所有线程退出
	for(int i=0;i<pDp.GetSize();i++){
		if(!pDp[i]->bScriptExit){
			pDp[i]->SetScriptExit(TRUE);
		}
		if(!pDp[i]->bExit){
			pDp[i]->bAutoEscape=TRUE;
			pDp[i]->bReLogin=FALSE;
			pDp[i]->bEncount = FALSE;
			pDp[i]->SetExit(TRUE);				
		}
	}		
	//_CrtDumpMemoryLeaks();
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	g_userinfo.RemoveAll();
	Sleep(1000);
	for (int i = 0; i < pDp.GetSize(); ++i)
	{
		delete pDp[i];
	}

	pDp.RemoveAll();

}
void RunThread(LPVOID p)
{
	THREADPARA *para=(THREADPARA *)p;
	pDp[para->index]->Run(&g_userinfo[para->index]);
}

void RunScriptThread(LPVOID p)
{
	THREADPARA *para=(THREADPARA *)p;
	pDp[para->index]->RunScript();
}
void CDIPIDlg::OnBnClickedStart()
{
	UpdateData(TRUE);
	m_charinfo.DeleteAllItems();
	m_message.ResetContent();
	//-----------------------------------------------	
	if(!SelectLine()){
		MessageBox("无法获取服务端ip地址！","提示信息",MB_OK|MB_ICONINFORMATION);
		return;
	}	
	gametime=0;
	//帐号登入
	for(int i=0;i<g_userinfo.GetSize();i++){
		CharLogin(i);
		Sleep(100);		
	}
	GetDlgItem(IDC_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_START)->EnableWindow(FALSE);
}

void CDIPIDlg::OnTimer(UINT_PTR nIDEvent)
{
	CString szVal, szStr;
	szVal = "";
	if (nIDEvent == 1)
	{
		CDIPIApp *pApp = (CDIPIApp *)AfxGetApp();
		UpdateData(TRUE);
		gametime++;
		CTime time1(gametime);
		//到期则退出
		if (pApp->G_LICENCEDATE <= COleDateTime(gametime) && !SelfLicenceValid()) {
			if (TimerOn) {
				KillTimer(1);
				TimerOn = 0;
			}
			if (GameTimerOn)
			{
				KillTimer(2);
				GameTimerOn = 0;
			}
			MessageBox("外挂服务已到期，请和作者联系！", "提示信息", MB_OK | MB_ICONINFORMATION);
			for (int i = 0; i<pDp.GetSize(); i++) {
				if (!pDp[i]->bScriptExit) {
					pDp[i]->SetScriptExit(TRUE);
				}
				if (!pDp[i]->bExit) {
					pDp[i]->bAutoEscape = TRUE;
					pDp[i]->SetExit(TRUE);
				}
			}
			GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
			GetDlgItem(IDC_START)->EnableWindow(TRUE);
			return;
		}
		szVal = time1.Format(_T("%d日 %H:%M:%S"));
		m_time.SetWindowText(szVal);
	}
	
	int i;
	for(i=0;i<pDp.GetSize();i++){
		CDpMain & role_info = *(pDp[i]);

		if (nIDEvent == 1)
		{
			//当前线程未退出并且在线,则发送连接信息
			if (!role_info.bExit && role_info.IsOnLine) {
				if ((GetTickCount() - role_info.nStartTime) / 1000 >= 30) {
					if (role_info.SendOnlineInfo("hoge") != SUCCESSFUL) {
						CTime cTime = CTime::GetCurrentTime();
						szVal.Format("%s %s%s", cTime.Format(_T("%H:%M:%S")), role_info.user.charname, "与服务端连接已断！");
						m_message.AddString(szVal);
					}
				}
			}
			//如果当前帐号不在线
			if (!role_info.IsOnLine) {
				//role_info.SetScriptExit(TRUE);
				//启用了断线重登,并且不在线上，并且没在登录过程中
				if (role_info.bReLogin && !role_info.IsOnLine && !role_info.IsLogin) {
					role_info.IsLogin = TRUE;
					CharLogin(i);
				}
			}//END:如果当前帐号不在线

			 //战骑
			 //if (GetTickCount() - role_info.nCheckRideTime > 500)
			 //{
			 //role_info.nCheckRideTime = GetTickCount();
			 //role_info.CheckPetRightFight();
			 //}



			 //自动吃鱼鳃草
			if (role_info.bAutoEatSYC && role_info.charotherinfo.state == 0 && (GetTickCount() - role_info.nEatSYCTime) >= 3600000) {
				role_info.SendEatSYC();
				//pDp[i]->nEatSYCTime=GetTickCount();
			}
			//自动吃智慧果
			if (role_info.bAutoExpNut && role_info.charotherinfo.state == 0 && (GetTickCount() - role_info.nExpNutTime) >= 3600000) {
				role_info.SendEatExpNut();
				//pDp[i]->nExpNutTime=GetTickCount();
			}

			if (gametime % 3 == 0)
			{
				CString szTemp = role_info.chardetail.name;
				CString pet_msg = "";
				for (int k = 0; k < 5; ++k)
				{
					PETDETAIL pet_info = role_info.petdetail[k];
					if (pet_info.islive)
					{
						CString s_msg;
						s_msg.Format("(%d)%s[%s] LV[%d]血[%d|%d]攻[%d]防[%d]敏[%d] ", k+1, pet_info.oldname, pet_info.newname,
							pet_info.level, pet_info.hp, pet_info.maxhp, pet_info.attack, pet_info.defence, pet_info.quick);
						pet_msg.Format("%s%s", pet_msg, s_msg);
					}
				}
				int n = m_charinfo.GetItemCount();
				int midx = 0;
				for (midx = 0; midx<n; midx++) {
					szStr = m_charinfo.GetItemText(midx, 0);
					if (szStr == szTemp)
						break;
				}
				if (midx < n)
				{
					//nRow = midx;
					szTemp = (LPCTSTR)pet_msg;
					m_charinfo.SetItemText(midx, 3 + 19, szTemp);
				}
			}
			
		}
		else if (nIDEvent == 2)
		{
			if (role_info.charotherinfo.state == 0)
			{
				//T人检查
				if (role_info.bCharMemSet && strlen(role_info.bc_char[0].name)>0 && strcmp(role_info.bc_char[0].name, role_info.chardetail.name) == 0)
				{
					for (int i = 1; i<5; i++) 
					{
						char* mem_name = role_info.bc_char[i].name;
						if (strlen(mem_name) > 0)
						{
							bool bIn = FALSE;
							for (int k = 0; k < 10; ++k)
							{
								if (strlen(role_info.cCharTeamMember[k]) > 0 && strcmp(mem_name, role_info.cCharTeamMember[k]) == 0) {
									bIn = TRUE;
									break;
								}
							}
							if (!bIn)
							{
								role_info.SendKickMember(i);
							}
						}
					}
				}
				//人物平时精灵补血
				if (role_info.commonblood.id >= 0 && ((double)role_info.chardetail.hp / role_info.chardetail.maxhp) * 100 <= role_info.commonblood.val) {
					role_info.SendRecruitHpPlaceTime(role_info.commonblood.id, 0);
					role_info.SendRecruitHpPlaceTime(role_info.commonblood.id, 0);
				}
				//宠物平时精灵补血
				if (role_info.commonblood_pet.id >= 0)
				{
					for (int j = 0; j<5; j++) {
						if (((double)(role_info.petdetail[j].hp) / role_info.petdetail[j].maxhp) * 100 <= role_info.commonblood_pet.val) {
							role_info.SendRecruitHpPlaceTime(role_info.commonblood_pet.id, j + 1);
							role_info.SendRecruitHpPlaceTime(role_info.commonblood_pet.id, j + 1);
						}
					}
				}
			}

			if (role_info.bEncount && role_info.charotherinfo.state == 0 && (GetTickCount() - role_info.nEncountTime >= role_info.nEncountDelay))
			{
				role_info.nEncountTime = GetTickCount();
				role_info.SendWalkPos(role_info.charotherinfo.x, role_info.charotherinfo.y, "gcgc");
			}

		}

	}
	CDialogEx::OnTimer(nIDEvent);
}

void CDIPIDlg::CallbackMsg(NOTIFYPARA *pNotifyPara)
{
	CString szVal,szTemp,szStr,szName,szError;
	int i,n,nRow,pos;
	CTime cTime = CTime::GetCurrentTime();
	if(m_message.GetCount()>500)
		m_message.ResetContent();
	int nbeg = 3;
	switch(pNotifyPara->nNotityType){
	case NOTIFY_TIME:
		if (!TimerOn)
		{
			TimerOn = SetTimer(1, 1000, NULL);
			GameTimerOn = SetTimer(2, 100, NULL);
		}
		break;
	case NOTIFY_MSG:		
		szVal.Format("%s %s",cTime.Format(_T("%H:%M:%S")),(LPCTSTR)pNotifyPara->lpNotifyData);
		m_message.AddString(szVal);
		m_message.SendMessage(WM_VSCROLL,SB_BOTTOM,   0);
		break;
	case NOTIFY_TEAMSUM:
		szVal = (LPCTSTR)pNotifyPara->lpNotifyData;
		pos = 0;
		n = m_charinfo.GetItemCount();
		szTemp = szVal.Tokenize("|", pos);
		for (i = 0; i<n; i++) {
			szStr = m_charinfo.GetItemText(i, 0);
			if (szStr == szTemp)
				break;
		}
		if (i < n)
		{
			nRow = i;
			szTemp = szVal.Tokenize("|", pos);
			int sub_idx = DETAIL_TEAMSUM;
			m_charinfo.SetItemText(nRow, sub_idx, szTemp);
		}
		break;
	case NOTIFY_LOGIN:
		szVal=(LPCTSTR)pNotifyPara->lpNotifyData;
		pos=0;
		//取帐号
		szName=szVal.Tokenize("|",pos);		
		n=m_charinfo.GetItemCount();
		//取人物名
		szTemp=szVal.Tokenize("|",pos);

		for (i = 0; i<m_list.GetItemCount(); i++) {
			CString acc = m_list.GetItemText(i, 0);
			if (szName == acc)
			{
				m_list.SetItemText(i, 1, szTemp);
				break;
			}
		}

		//指定帐号是否在列表框中
		for(i=0;i<n;i++){
			szStr=m_charinfo.GetItemText(i,0);
			if (szStr == szTemp)
			{
				break;
			}
				
		}
		

		//登录处理
		
		if(i>=n){
			nRow = m_charinfo.InsertItem(n,szTemp);
			szTemp=szVal.Tokenize("|",pos);			
			m_charinfo.SetItemText(nRow, nbeg + 1, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 2, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 3, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 4, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 5, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 6, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 7, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 8, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 9, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 10, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 11, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 12, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 13, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 14, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 15, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			//m_charinfo.SetItemText(nRow, nbeg + 16, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 17, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			//m_charinfo.SetItemText(nRow, nbeg + 19, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 20, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			if(!szTemp.Trim().IsEmpty())
				m_charinfo.SetItemText(nRow, nbeg + 21, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 22, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 23, szTemp);
			szVal.Format("%s %s帐号已登入！",cTime.Format(_T("%H:%M:%S")),szName);
			m_message.AddString(szVal);
			m_message.SendMessage(WM_VSCROLL,SB_BOTTOM,0);
		}
		else{
			nRow =i;
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 1, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 2, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 3, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 4, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 5, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 6, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 7, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 8, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 9, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 10, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 11, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 12, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 13, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 14, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 15, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			//m_charinfo.SetItemText(nRow, nbeg + 16, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 17, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			//m_charinfo.SetItemText(nRow, nbeg + 19, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 20, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			if(!szTemp.Trim().IsEmpty())
				m_charinfo.SetItemText(nRow, nbeg + 21, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 22, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 23, szTemp);
			szVal.Format("%s %s帐号重新登入！",cTime.Format(_T("%H:%M:%S")),szName);
			m_message.AddString(szVal);
			m_message.SendMessage(WM_VSCROLL,SB_BOTTOM,   0);
		}	
		
		break;
	case NOTIFY_LOGOUT:
		szVal=(LPCTSTR)pNotifyPara->lpNotifyData;
		pos=0;
		n=m_charinfo.GetItemCount();
		szTemp=szVal.Tokenize("|",pos);
		for(i=0;i<n;i++){
			szStr=m_charinfo.GetItemText(i,0);
			if(szStr==szTemp)
				break;
		}
		if(i<n){
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(i, nbeg + 23, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			szVal.Format("%s %s已退出！",cTime.Format(_T("%H:%M:%S")),szTemp);
			m_message.AddString(szVal);
			m_message.SendMessage(WM_VSCROLL,SB_BOTTOM,0);
		}		
		break;
	case NOTIFY_FIGHTPET:
	case NOTIFY_RIDEPET:
		{
			szVal = (LPCTSTR)pNotifyPara->lpNotifyData;
			pos = 0;
			n = m_charinfo.GetItemCount();
			szTemp = szVal.Tokenize("|", pos);
			for (i = 0; i<n; i++) {
				szStr = m_charinfo.GetItemText(i, 0);
				if (szStr == szTemp)
					break;
			}
			if (i < n)
			{
				nRow = i;
				szTemp = szVal.Tokenize("|", pos);
				int sub_idx = pNotifyPara->nNotityType == NOTIFY_FIGHTPET ? DETAIL_FIGHTPETEXP : DETAIL_RIDEPETEXP;
				m_charinfo.SetItemText(nRow, sub_idx, szTemp);
			}
			
		}
		break;
	case NOTIFY_STARTSCRIPT:
		szTemp=(LPCTSTR)pNotifyPara->lpNotifyData;
		szVal.Format("%s %s脚本线程开始运行！",cTime.Format(_T("%H:%M:%S")),szTemp);
		m_message.AddString(szVal);
		m_message.SendMessage(WM_VSCROLL,SB_BOTTOM,0);
		break;
	case NOTIFY_STOPSCRIPT:
		szTemp=(LPCTSTR)pNotifyPara->lpNotifyData;			
		pos=0;
		szStr=szTemp.Tokenize("|",pos);
		szError=szTemp.Tokenize("|",pos);
		n=atoi(szError);
		if(n==0)
			szVal.Format("%s %s脚本线程已退出！",cTime.Format(_T("%H:%M:%S")),szStr);
		else
			szVal.Format("%s %s脚本线程在%d行有错误！",cTime.Format(_T("%H:%M:%S")),szStr,n+1);
		m_message.AddString(szVal);
		m_message.SendMessage(WM_VSCROLL,SB_BOTTOM,0);
		break;
	case NOTIFY_CHARDETAIL:
		szVal=(LPCTSTR)pNotifyPara->lpNotifyData;
		pos=0;
		n=m_charinfo.GetItemCount();
		szTemp=szVal.Tokenize("|",pos);
		for(i=0;i<n;i++){
			szStr=m_charinfo.GetItemText(i,0);
			if(szStr==szTemp)
				break;
		}
		if(i<n){
			nRow =i;
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 1, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 2, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 3, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 4, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 5, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 6, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 7, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 8, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 9, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 10, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 11, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 12, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 13, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 14, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 15, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			//m_charinfo.SetItemText(nRow, nbeg + 16, szTemp);
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 17, szTemp);
		}
		break;
	case NOTIFY_MAP:
		szVal=(LPCTSTR)pNotifyPara->lpNotifyData;
		pos=0;
		n=m_charinfo.GetItemCount();
		szTemp=szVal.Tokenize("|",pos);
		for(i=0;i<n;i++){
			szStr=m_charinfo.GetItemText(i,0);
			if(szStr==szTemp)
				break;
		}
		if(i<n){
			nRow =i;
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 20, szTemp);//地图
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 21, szTemp);//地图名称
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 22, szTemp);//坐标
		}
		break;
	case NOTIFY_XY:
		szVal=(LPCTSTR)pNotifyPara->lpNotifyData;
		pos=0;
		n=m_charinfo.GetItemCount();
		szTemp=szVal.Tokenize("|",pos);
		for(i=0;i<n;i++){
			szStr=m_charinfo.GetItemText(i,0);
			if(szStr==szTemp)
				break;
		}
		if(i<n){
			nRow =i;			
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 22, szTemp);//坐标
		}
		break;
	case NOTIFY_UPLEVELPOINT:
		szVal=(LPCTSTR)pNotifyPara->lpNotifyData;
		pos=0;
		n=m_charinfo.GetItemCount();
		szTemp=szVal.Tokenize("|",pos);
		for(i=0;i<n;i++){
			szStr=m_charinfo.GetItemText(i,0);
			if(szStr==szTemp)
				break;
		}
		if(i<n){
			nRow =i;
			szTemp=szVal.Tokenize("|",pos);
			//m_charinfo.SetItemText(nRow, nbeg + 19, szTemp);
		}
		break;
	case NOTIFY_ROUND:
		szVal=(LPCTSTR)pNotifyPara->lpNotifyData;
		pos=0;
		n=m_charinfo.GetItemCount();
		szTemp=szVal.Tokenize("|",pos);
		for(i=0;i<n;i++){
			szStr=m_charinfo.GetItemText(i,0);
			if(szStr==szTemp)
				break;
		}
		if(i<n){
			nRow =i;
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 24, szTemp);
		}
		break;
	case NOTIFY_FAME:
		szVal=(LPCTSTR)pNotifyPara->lpNotifyData;
		pos=0;
		n=m_charinfo.GetItemCount();
		szTemp=szVal.Tokenize("|",pos);
		for(i=0;i<n;i++){
			szStr=m_charinfo.GetItemText(i,0);
			if(szStr==szTemp)
				break;
		}
		if(i<n){
			nRow =i;
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(nRow, nbeg + 18, szTemp);
		}
		break;
	case NOTIFY_VIPPOINTS:
		szVal = (LPCTSTR)pNotifyPara->lpNotifyData;
		pos = 0;
		n = m_charinfo.GetItemCount();
		szTemp = szVal.Tokenize("|", pos);
		for (i = 0; i<n; i++) {
			szStr = m_charinfo.GetItemText(i, 0);
			if (szStr == szTemp)
				break;
		}
		if (i<n) {
			nRow = i;
			szTemp = szVal.Tokenize("|", pos);
			m_charinfo.SetItemText(nRow, nbeg + 16, szTemp);
		}
		break;
	case NOTIFY_SCRIPT:
		szVal=(LPCTSTR)pNotifyPara->lpNotifyData;
		pos=0;
		n=m_charinfo.GetItemCount();
		szTemp=szVal.Tokenize("|",pos);
		for(i=0;i<n;i++){
			szStr=m_charinfo.GetItemText(i,0);
			if(szStr==szTemp)
				break;
		}
		if(i<n){
			nRow =i;
			szTemp=szVal.Tokenize("^",pos);
			m_charinfo.SetItemText(nRow, DETAIL_SCRIPT, szTemp);
		}
		break;
	case NOTIFY_GAMESTATE:
		szVal=(LPCTSTR)pNotifyPara->lpNotifyData;
		pos=0;
		n=m_charinfo.GetItemCount();
		szTemp=szVal.Tokenize("|",pos);
		for(i=0;i<n;i++){
			szStr=m_charinfo.GetItemText(i,0);
			if(szStr==szTemp)
				break;
		}
		if(i<n){
			szTemp=szVal.Tokenize("|",pos);
			m_charinfo.SetItemText(i, nbeg + 23, szTemp);
		}		
		break;
	}
}
//所有帐号全部登出
void CDIPIDlg::OnBnClickedStop()
{
	CString szVal;
	if(TimerOn){
		KillTimer(1);
		TimerOn=0;
		gametime=0;
	}
	if (GameTimerOn)
	{
		KillTimer(2);
		GameTimerOn = 0;
	}
	for(int i=0;i<pDp.GetSize();i++){
		if(!pDp[i]->bScriptExit){
			pDp[i]->SetScriptExit(TRUE);
		}
		if(!pDp[i]->bExit){
			pDp[i]->bAutoEscape=TRUE;
			pDp[i]->bReLogin=FALSE;
			pDp[i]->bEncount = FALSE;
			pDp[i]->SetExit(TRUE);				
		}
	}		
	
	GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_START)->EnableWindow(TRUE);
}
//根据选择线路获取该线路的ip地址
BOOL CDIPIDlg::SelectLine()
{
	HOSTENT *lpHostEnt;
	struct in_addr inAddr;
	LPSTR lpaddr;
	char ip[30];	
	memset(ip,0,sizeof(ip));
	switch(m_line){
	case 0:
		//strncpy_s(ip,"64dx1.230572.com",strlen("64dx1.230572.com"));
		//lpHostEnt=gethostbyname(ip);
		//if(!lpHostEnt)return FALSE;
		//lpaddr = lpHostEnt->h_addr_list[0];			
		//memmove (&inAddr,lpaddr, 4);
		//220.162.96.66
		sprintf_s(ip,"%d.%d.%d.%d",220, 162, 96, 66);
		strcpy_s(g_serverinfo.ip,ip);
		g_serverinfo.port= 9065;
		break;
	case 1:
		sprintf_s(ip, "%d.%d.%d.%d", 220, 162, 96, 66);
		strcpy_s(g_serverinfo.ip, ip);
		g_serverinfo.port = 9066;
		break;
	case 2:
		strncpy_s(ip,"64dx3.230572.com",strlen("64dx3.230572.com"));
		lpHostEnt=gethostbyname(ip);
		if(!lpHostEnt)return FALSE;
		lpaddr = lpHostEnt->h_addr_list[0];			
		memmove (&inAddr,lpaddr, 4);
		sprintf_s(ip,"%d.%d.%d.%d",inAddr.S_un.S_addr&0xff,(inAddr.S_un.S_addr>>8)&0xff,(inAddr.S_un.S_addr>>16)&0xff,(inAddr.S_un.S_addr>>24)&0xff);
		strcpy_s(g_serverinfo.ip,ip);
		g_serverinfo.port=7003;
		break;
	case 3:
		strncpy_s(ip,"64dx4.230572.com",strlen("64dx4.230572.com"));
		lpHostEnt=gethostbyname(ip);
		if(!lpHostEnt)return FALSE;
		lpaddr = lpHostEnt->h_addr_list[0];			
		memmove (&inAddr,lpaddr, 4);
		sprintf_s(ip,"%d.%d.%d.%d",inAddr.S_un.S_addr&0xff,(inAddr.S_un.S_addr>>8)&0xff,(inAddr.S_un.S_addr>>16)&0xff,(inAddr.S_un.S_addr>>24)&0xff);
		strcpy_s(g_serverinfo.ip,ip);
		g_serverinfo.port=7004;
		break;
	case 4:
		strncpy_s(ip,"64wt1.230572.com",strlen("64wt1.230572.com"));
		lpHostEnt=gethostbyname(ip);
		if(!lpHostEnt)return FALSE;
		lpaddr = lpHostEnt->h_addr_list[0];			
		memmove (&inAddr,lpaddr, 4);
		sprintf_s(ip,"%d.%d.%d.%d",inAddr.S_un.S_addr&0xff,(inAddr.S_un.S_addr>>8)&0xff,(inAddr.S_un.S_addr>>16)&0xff,(inAddr.S_un.S_addr>>24)&0xff);
		strcpy_s(g_serverinfo.ip,ip);
		g_serverinfo.port=7001;
		break;
	case 5:
		strncpy_s(ip,"64wt2.230572.com",strlen("64wt2.230572.com"));
		lpHostEnt=gethostbyname(ip);
		if(!lpHostEnt)return FALSE;
		lpaddr = lpHostEnt->h_addr_list[0];			
		memmove (&inAddr,lpaddr, 4);
		sprintf_s(ip,"%d.%d.%d.%d",inAddr.S_un.S_addr&0xff,(inAddr.S_un.S_addr>>8)&0xff,(inAddr.S_un.S_addr>>16)&0xff,(inAddr.S_un.S_addr>>24)&0xff);
		strcpy_s(g_serverinfo.ip,ip);
		g_serverinfo.port=7002;
		break;
	case 6:
		strncpy_s(ip,"64wt3.230572.com",strlen("64wt3.230572.com"));
		lpHostEnt=gethostbyname(ip);
		if(!lpHostEnt)return FALSE;
		lpaddr = lpHostEnt->h_addr_list[0];			
		memmove (&inAddr,lpaddr, 4);
		sprintf_s(ip,"%d.%d.%d.%d",inAddr.S_un.S_addr&0xff,(inAddr.S_un.S_addr>>8)&0xff,(inAddr.S_un.S_addr>>16)&0xff,(inAddr.S_un.S_addr>>24)&0xff);
		strcpy_s(g_serverinfo.ip,ip);
		g_serverinfo.port=7003;
		break;
	case 7:
		strncpy_s(ip,"64wt4.230572.com",strlen("64wt4.230572.com"));
		lpHostEnt=gethostbyname(ip);
		if(!lpHostEnt)return FALSE;
		lpaddr = lpHostEnt->h_addr_list[0];			
		memmove (&inAddr,lpaddr, 4);
		sprintf_s(ip,"%d.%d.%d.%d",inAddr.S_un.S_addr&0xff,(inAddr.S_un.S_addr>>8)&0xff,(inAddr.S_un.S_addr>>16)&0xff,(inAddr.S_un.S_addr>>24)&0xff);
		strcpy_s(g_serverinfo.ip,ip);
		g_serverinfo.port=7004;
		break;
	case 8:
		strncpy_s(ip,"64dxdan.230572.com",strlen("64dxdan.230572.com"));
		lpHostEnt=gethostbyname(ip);
		if(!lpHostEnt)return FALSE;
		lpaddr = lpHostEnt->h_addr_list[0];			
		memmove (&inAddr,lpaddr, 4);
		sprintf_s(ip,"%d.%d.%d.%d",inAddr.S_un.S_addr&0xff,(inAddr.S_un.S_addr>>8)&0xff,(inAddr.S_un.S_addr>>16)&0xff,(inAddr.S_un.S_addr>>24)&0xff);
		strcpy_s(g_serverinfo.ip,ip);
		g_serverinfo.port=8003;
		break;
	case 9:		
		strncpy_s(ip,"64wtdan.230572.com",strlen("64wtdan.230572.com"));
		lpHostEnt=gethostbyname(ip);
		if(!lpHostEnt)return FALSE;
		lpaddr = lpHostEnt->h_addr_list[0];			
		memmove (&inAddr,lpaddr, 4);
		sprintf_s(ip,"%d.%d.%d.%d",inAddr.S_un.S_addr&0xff,(inAddr.S_un.S_addr>>8)&0xff,(inAddr.S_un.S_addr>>16)&0xff,(inAddr.S_un.S_addr>>24)&0xff);
		strcpy_s(g_serverinfo.ip,ip);
		g_serverinfo.port=8003;
		break;
	}
	return TRUE;
}

//让某帐号运行脚本
void CDIPIDlg::OnMenuRun()
{
	DWORD thread;
	HANDLE hThreads;

	if((int)m_list.GetFirstSelectedItemPosition()-1<0){
		MessageBox("请先选中要操作的帐号！","提示信息",MB_OK|MB_ICONINFORMATION);
		return;
	}
	int pos=(int)m_list.GetFirstSelectedItemPosition()-1;
	g_para[pos].index=pos;
	//向线程传递参数不能用局部变量	
	hThreads=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)RunScriptThread,(void *)&g_para[pos],0,&thread);
	if(!hThreads){
		MessageBox("启动脚本线程失败！","错误信息",MB_OK|MB_ICONEXCLAMATION);
		return;
	}	
}
//让某帐号运行脚本退出
void CDIPIDlg::OnMenuStop()
{
	if((int)m_list.GetFirstSelectedItemPosition()-1<0){
		MessageBox("请先选中要操作的帐号！","提示信息",MB_OK|MB_ICONINFORMATION);
		return;
	}
	int pos=(int)m_list.GetFirstSelectedItemPosition()-1;

	if(!pDp[pos]->bScriptExit){
		pDp[pos]->SetScriptExit(TRUE);						
	}	
}
int lssproto_a62toi( char *a )
{
	int ret = 0;
	int minus ;
	if( a[0] == '-' ){
		minus = -1;
        a++;
	} else {
		minus = 1;
	}
	
	while( *a != '\0' )
	{
		ret *= 62;
		if( '0' <= (*a) && (*a) <= '9' )
			ret += (*a)-'0';
		else
		if( 'a' <= (*a) && (*a) <= 'z' )
			ret += (*a)-'a'+10;
		else
		if( 'A' <= (*a) && (*a) <= 'Z' )
			ret += (*a)-'A'+36;
		else
			return 0;
		a++;
	}
	return ret * minus;
}
void CDIPIDlg::OnMenuPause()
{
	int a=lssproto_a62toi("3yo");
	a=a+1;
}
//帐号登入
BOOL CDIPIDlg::CharLogin(int index)
{
	CString szVal;
	DWORD thread;
	HANDLE hThreads;
	
	g_para[index].index=index;
	//向线程传递参数不能用局部变量	
	hThreads=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)RunThread,(void *)&g_para[index],0,&thread);
	if(!hThreads){
		MessageBox("启动游戏线程失败！","错误信息",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}	
	if(m_autorun && pDp[index]->bScriptExit){
		hThreads=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)RunScriptThread,(void *)&g_para[index],0,&thread);
		if(!hThreads){
			MessageBox("启动脚本线程失败！","错误信息",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;
		}		
	}
	return TRUE;
}

//选中帐号登入
void CDIPIDlg::OnSinglelogin()
{
	UpdateData(TRUE);
	
	if(!SelectLine()){
		MessageBox("无法获取服务端ip地址！","提示信息",MB_OK|MB_ICONINFORMATION);
		return;
	}
	int pos=(int)m_list.GetFirstSelectedItemPosition()-1;
	if(pos<0){
		MessageBox("请先选中要操作的帐号！","提示信息",MB_OK|MB_ICONINFORMATION);
		return;
	}
	if(!TimerOn){
		gametime=0;
	}
	pDp[pos]->SetScriptExit(TRUE);
	Sleep(400);
	if(CharLogin(pos)){	
		GetDlgItem(IDC_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_START)->EnableWindow(FALSE);
	}
}

//选中帐号登出
void CDIPIDlg::OnSinglelogout()
{
	int pos=(int)m_list.GetFirstSelectedItemPosition()-1;
	if(pos<0){
		MessageBox("请先选中要操作的帐号！","提示信息",MB_OK|MB_ICONINFORMATION);
		return;
	}	
	if(!pDp[pos]->bExit){
		pDp[pos]->bReLogin=FALSE;
		pDp[pos]->bAutoEscape=TRUE;
		pDp[pos]->bReLogin=FALSE;
		pDp[pos]->bEncount = FALSE;
		pDp[pos]->SetExit(TRUE);						
	}
}
//是否显示说话内容
void CDIPIDlg::OnBnClickedChktalk()
{
	CButton * pbox=(CButton *)GetDlgItem(IDC_CHkTALK);
	int val=pbox->GetCheck();
	for(int i=0;i<pDp.GetSize();i++){
		pDp[i]->IsDispTalk=(BOOL)val;
	}
}


void CDIPIDlg::OnLvnColumnclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->iSubItem==m_nSordCol)
		m_bASC=!m_bASC;
	else{
		m_bASC=TRUE;
		m_nSordCol=pNMLV->iSubItem;
	}
	int i=m_charinfo.GetItemCount();
    while(i--)m_charinfo.SetItemData(i,i);

	m_charinfo.SortItems((PFNLVCOMPARE)CompareFunc,(DWORD)&m_charinfo);
	*pResult = 0;
}
int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2,LPARAM lParamSort)
{ 
	CListCtrl *pList=(CListCtrl*)lParamSort;
	int nItem1,nItem2;
	LVFINDINFO FindInfo;
	FindInfo.flags=LVFI_PARAM;
	FindInfo.lParam=lParam1;
	nItem1=pList->FindItem(&FindInfo,-1);
	FindInfo.lParam=lParam2;
	nItem2=pList->FindItem(&FindInfo,-1);

	if((nItem1==-1)||(nItem2==-1))
	{
	   return 0;
	}
	CString Str1,Str2,szNum1,szNum2;
	Str1=pList->GetItemText(nItem1,m_nSordCol);
	Str2=pList->GetItemText(nItem2,m_nSordCol);
	szNum1=GetDigitFormString(Str1);
	szNum2=GetDigitFormString(Str2);
	int iCompRes,n1,n2;
	if(IsNumber(szNum1) && IsNumber(szNum2)){
		n1=atoi(szNum1);
		n2=atoi(szNum2);
		if(n1>n2)
			iCompRes=1;
		else if(n1==n2)
			iCompRes=0;
		else
			iCompRes=-1;
		if(!m_bASC)
		   return iCompRes;
		else
		   return iCompRes * -1;
	}
	else{
		iCompRes=Str1.Compare(Str2);
		if(!m_bASC)
		   return iCompRes;
		else
		   return iCompRes * -1;
	}

} 
//保存帐号配置信息
void CDIPIDlg::OnBnClickedSave()
{
	CString szAccount,szPwd,szSafeCode,szScript,szVal,szCharIdx;
	CStdioFile f(_T("data.txt"),CFile::modeCreate|CFile::modeWrite|CFile::typeText);
	for(int i=0;i<m_list.GetItemCount();i++){
		szAccount=m_list.GetItemText(i,0);
		szPwd = g_userinfo[i].password;
		szSafeCode = g_userinfo[i].safecode;
		szScript = g_userinfo[i].scriptName;
		szCharIdx = g_userinfo[i].charidx;
		f.WriteString(szAccount+"|"+szPwd+"|"+szSafeCode+"|"+szScript + "|" + szCharIdx +_T("\r\n"));
	}
	f.Close();
}
void CDIPIDlg::OnNMDblclkListchar(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int index;
	CString szAccount,szPwd,szSafeCode,szScript,szVal;
	index=(int)m_list.GetFirstSelectedItemPosition()-1;
	if(index==-1){
		AfxMessageBox(_T("请先选择要编辑的帐号！"));
		return;
	}
	szAccount=m_list.GetItemText(index,0);
	szPwd=m_list.GetItemText(index,1);
	szSafeCode=m_list.GetItemText(index,2);
	szScript=m_list.GetItemText(index,3);
	AccountEdit dlg;
	dlg.szAccount=szAccount;
	dlg.szPwd=szPwd;
	dlg.szSafeCode=szSafeCode;
	dlg.szScript=szScript;
	if ( dlg.DoModal() == IDOK ){
		m_list.SetItemText(index, 0, dlg.szAccount);
		m_list.SetItemText(index, 2, dlg.szScript);
		//m_list.SetItemText(index, 1, dlg.szPwd);
		//m_list.SetItemText(index, 2, dlg.szSafeCode);
		//m_list.SetItemText(index, 3, dlg.szScript);
		//m_list.SetItemText(index, 4, dlg.szCharIdx);
		strcpy_s(g_userinfo[index].charname,dlg.szAccount);
		strcpy_s(g_userinfo[index].password,dlg.szPwd);
		strcpy_s(g_userinfo[index].safecode,dlg.szSafeCode);
		strcpy_s(g_userinfo[index].scriptName,dlg.szScript);
		strcpy_s(g_userinfo[index].charidx, dlg.szCharIdx);
	}
	*pResult = 0;
}

//获取注册号
void CAboutDlg::OnBnClickedGetregcode()
{
	char buf[1024]={0};
	if(GetNicInfo(buf)){
		GetDlgItem(IDC_REGEDITCODE)->SetWindowText(buf);
	}
	else{
		AfxMessageBox(_T("获取注册码失败！"));
	}
}

//是否显示实时更新信息
void CDIPIDlg::OnBnClickedChkupdate()
{
	CButton * pbox=(CButton *)GetDlgItem(IDC_CHKUPDATE);
	int val=pbox->GetCheck();
	for(int i=0;i<pDp.GetSize();i++){
		pDp[i]->IsDispInfoOnTime=(BOOL)val;
	}
}


void CDIPIDlg::OnBnClickedGetpccode()
{
	//CButton * pbox = NULL;
	//int val = pbox->GetCheck();

	CString source = GetFirstNicInfo();
	if (OpenClipboard() && source.GetLength() > 0)
	{
		HGLOBAL   clipbuffer;
		char   *   buffer;
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, source.GetLength());
		buffer = (char*)GlobalLock(clipbuffer);
		strcpy(buffer, LPCSTR(source));
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT, clipbuffer);
		CloseClipboard();
	}
	// TODO: 在此添加控件通知处理程序代码
	MessageBox("机器码已经复制到剪切板,请在QQ,按Ctrl+V复制到聊天框", "提示");
}


void CDIPIDlg::OnBnClickedSave2()
{
	// TODO: 在此添加控件通知处理程序代码
	//机器码
	CString pc_code;
	GetDlgItem(IDC_ACCOUNT)->GetWindowText(pc_code);
	//有效期
	CString valid_date;
	GetDlgItem(IDC_PASSWORD)->GetWindowText(valid_date);
	//有效多开数
	CString valid_count;
	GetDlgItem(IDC_SAFECODE)->GetWindowText(valid_count);

	if (pc_code.GetLength() == 0 || valid_date.GetLength() == 0 || valid_count.GetLength() == 0)
	{
		MessageBox("生成licence.dat 失败!", "提示");
		return;
	}
	CString valid_str = pc_code + "#" + valid_date + "#" + valid_count;
	CString res_str = EncryptNew(valid_str);

	CStdioFile f(_T("licence.dat"), CFile::modeCreate | CFile::modeWrite | CFile::typeText);
	f.WriteString(res_str);
	f.Close();

	MessageBox("生成licence.dat OK!", "提示");
	//CString szVal;
	//CStdioFile ff(_T("licence.dat"), CFile::modeRead | CFile::typeText);
	//ff.ReadString(szVal);
	//ff.Close();
	//CString bbb = EncryptNew(szVal);
	//int a = 0;
	//CString bbb = EncryptNew(sccc);
	//int a = 0;
}


void CDIPIDlg::OnBnClickedSelScript()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFile = _T("");

	CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files All Files (*.*)|*.*||"), NULL);

	if (dlgFile.DoModal())
	{
		strFile = dlgFile.GetPathName();
		CString work_dir = GetWorkDir() + "\\";
		strFile.Replace(work_dir, "");
		GetDlgItem(IDC_SCRIPTNAME)->SetWindowText(strFile);
	}
}
