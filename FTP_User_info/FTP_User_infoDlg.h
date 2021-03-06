
// FTP_User_infoDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CFTP_User_infoDlg 대화 상자
class CFTP_User_infoDlg : public CDialogEx
{
// 생성입니다.
public:
	CFTP_User_infoDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FTP_USER_INFO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
