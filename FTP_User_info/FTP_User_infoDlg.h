
// FTP_User_infoDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CFTP_User_infoDlg ��ȭ ����
class CFTP_User_infoDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CFTP_User_infoDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FTP_USER_INFO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString m_Server_IP;
	CString m_User_ID;
	CString m_User_Password;
	CIPAddressCtrl m_ctrl_Server_IP;
	CEdit m_ctrl_User_ID;
	CEdit m_ctrl_User_PW;
	afx_msg void OnBnClickedButton2();
	CString GetServerIP();
	CString GetUserID();
	CString GetUserPW();
};
