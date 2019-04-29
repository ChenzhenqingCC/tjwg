
// DIPI.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "DIPI.h"
#include "DIPIDlg.h"
#include "tlhelp32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDIPIApp

BEGIN_MESSAGE_MAP(CDIPIApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CDIPIApp 构造

CDIPIApp::CDIPIApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CDIPIApp 对象

CDIPIApp theApp;

int CDIPIApp::GetProcessCount(const TCHAR* szExeName)
{
	int count = 0;
	try
	{
		TCHAR sztarget[MAX_PATH];
		lstrcpy(sztarget, szExeName);
		CharLowerBuff(sztarget, MAX_PATH);
		PROCESSENTRY32 my;
		HANDLE l = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (((int)l) != -1)
		{
			my.dwSize = sizeof(my);
			if (Process32First(l, &my))
			{
				do {
					CharLowerBuff(my.szExeFile, MAX_PATH);
					if (lstrcmp(sztarget, my.szExeFile) == 0) {
						count++;
					}
				} while (Process32Next(l, &my));
			}
			CloseHandle(l);
		}
	}
	catch (std::exception e)
	{
		//std::cout << e.what() << std::endl;
	}	return count;
}



// CDIPIApp 初始化

BOOL CDIPIApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	
	CWinApp::InitInstance();

	if (SelfLicenceValid())
	{
		MAXACCOUNT = 100;
	}
	else
	{
		MAXACCOUNT = 5;
		int gycout = GetProcessCount("DIPI.exe");
		int max_account = LicenceIsValid();
		LPWSTR s = GetCommandLineW();
		bool is_crash = (strcmp((CString)s, "AUTORUN") == 0);
		if (max_account <= 0)
		{
			MAXACCOUNT = 0;
			if (gycout > 1)
			{
				MessageBox(NULL, "外挂多开超过数量!授权请与作者联系！", "提示信息", MB_ICONINFORMATION);
				exit(0);
			}
		}
		else
		{
			if (gycout > 0 && gycout > max_account / MAXACCOUNT && !is_crash)
			{
				MessageBox(NULL, "外挂多开超过数量!授权请与作者联系！", "提示信息", MB_ICONINFORMATION);
				exit(0);
			}
		}
		
	}

	

	G_LICENCEDATE = GetLicenceDate();
	//检测软件是否授权
	if(MAXACCOUNT <= 0){		
		//只运行一个服务端实例
		MessageBox(NULL, "外挂未授权,请点击获取机器码后发给作者获取授权", "提示信息", MB_ICONINFORMATION);
	}
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	//CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CDIPIDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 删除上面创建的 shell 管理器。
	// (pShellManager != NULL)
	//{
		//delete pShellManager;
	//}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

