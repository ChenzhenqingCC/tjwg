
// DIPI.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CDIPIApp:
// 有关此类的实现，请参阅 DIPI.cpp
//

class CDIPIApp : public CWinApp
{
public:
	CDIPIApp();
	//最大用户数
	int MAXACCOUNT;
	COleDateTime G_LICENCEDATE;
// 重写
public:
	virtual BOOL InitInstance();
	int GetProcessCount(const TCHAR* szExeName);

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CDIPIApp theApp;