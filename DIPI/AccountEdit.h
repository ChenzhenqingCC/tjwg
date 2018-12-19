#pragma once


// AccountEdit 对话框

class AccountEdit : public CDialogEx
{
	DECLARE_DYNAMIC(AccountEdit)

public:
	AccountEdit(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~AccountEdit();

// 对话框数据
	enum { IDD = IDD_ACCOUNT_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString szAccount;
	CString szPwd;
	CString szSafeCode;
	CString szScript;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBrowse();
};
