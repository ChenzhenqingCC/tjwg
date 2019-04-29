
// DIPI.cpp : ����Ӧ�ó��������Ϊ��
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


// CDIPIApp ����

CDIPIApp::CDIPIApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDIPIApp ����

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



// CDIPIApp ��ʼ��

BOOL CDIPIApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
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
				MessageBox(NULL, "��Ҷ࿪��������!��Ȩ����������ϵ��", "��ʾ��Ϣ", MB_ICONINFORMATION);
				exit(0);
			}
		}
		else
		{
			if (gycout > 0 && gycout > max_account / MAXACCOUNT && !is_crash)
			{
				MessageBox(NULL, "��Ҷ࿪��������!��Ȩ����������ϵ��", "��ʾ��Ϣ", MB_ICONINFORMATION);
				exit(0);
			}
		}
		
	}

	

	G_LICENCEDATE = GetLicenceDate();
	//�������Ƿ���Ȩ
	if(MAXACCOUNT <= 0){		
		//ֻ����һ�������ʵ��
		MessageBox(NULL, "���δ��Ȩ,������ȡ������󷢸����߻�ȡ��Ȩ", "��ʾ��Ϣ", MB_ICONINFORMATION);
	}
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	//CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CDIPIDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	// (pShellManager != NULL)
	//{
		//delete pShellManager;
	//}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

