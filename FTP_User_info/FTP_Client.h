#pragma once
// CFtpConnection, CInternetSession Class를 사용하기 위한 헤더파일 include
#include "afxwin.h"
#include "afxinet.h"

// CFTP_Client 대화 상자입니다.

class CFTP_Client : public CDialogEx
{
	DECLARE_DYNAMIC(CFTP_Client)

public:
	CFTP_Client(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFTP_Client();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FTP_INFO_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton9();
	CString m_get_Server_IP;
	CString m_get_User_ID;
	CString m_get_User_PW;
	CFtpConnection* m_ftpCon;
	CInternetSession* m_session;
	void GetLocalPCFile(CString localdir);
	void GetFTPFile();
//	afx_msg void OnEnChangeEdit2();
	CEdit m_Edit_FtpDir;
	CListBox m_List_Ftp_File_List;
	CString m_LocalDir;
	CListBox m_List_Local_File_List;
	CEdit m_Edit_LocalDir;
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	CString m_FtpDir;
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnLbnDblclkList1();
	afx_msg void OnLbnDblclkList2();
	void SendToFTPServer(CString L_FileName);
	void SendToLocalPC(CString F_FileName);
	TCHAR m_str[128];
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton2();
};
