// AccountEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "DIPI.h"
#include "AccountEdit.h"
#include "afxdialogex.h"


// AccountEdit 对话框

IMPLEMENT_DYNAMIC(AccountEdit, CDialogEx)

AccountEdit::AccountEdit(CWnd* pParent /*=NULL*/)
	: CDialogEx(AccountEdit::IDD, pParent)
{

}

AccountEdit::~AccountEdit()
{
}

void AccountEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AccountEdit, CDialogEx)
	ON_BN_CLICKED(IDOK, &AccountEdit::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BROWSE, &AccountEdit::OnBnClickedBrowse)
END_MESSAGE_MAP()




void AccountEdit::OnBnClickedOk()
{
	GetDlgItem(IDC_ACCOUNT)->GetWindowText(szAccount);
	GetDlgItem(IDC_PWD)->GetWindowText(szPwd);
	GetDlgItem(IDC_SAFECODE)->GetWindowText(szSafeCode);
	GetDlgItem(IDC_SCRIPT)->GetWindowText(szScript);
	CDialogEx::OnOK();
}


BOOL AccountEdit::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem(IDC_ACCOUNT)->SetWindowText(szAccount);
	GetDlgItem(IDC_PWD)->SetWindowText(szPwd);
	GetDlgItem(IDC_SAFECODE)->SetWindowText(szSafeCode);
	GetDlgItem(IDC_SCRIPT)->SetWindowText(szScript);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void AccountEdit::OnBnClickedBrowse()
{
	CFileDialog dlg(TRUE, _T(".txt"),_T("") , OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T( "脚本文件(*.txt)|*.txt||" ));
	if ( dlg.DoModal() == IDOK ){
		GetDlgItem(IDC_SCRIPT)->SetWindowText(dlg.GetPathName());
	}
}
