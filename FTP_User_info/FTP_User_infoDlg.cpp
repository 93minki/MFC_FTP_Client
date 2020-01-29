
// FTP_User_infoDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "FTP_User_info.h"
#include "FTP_User_infoDlg.h"
#include "afxdialogex.h"
#include "FTP_Client.h"
#include <WinInet.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFTP_User_infoDlg ��ȭ ����



CFTP_User_infoDlg::CFTP_User_infoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FTP_USER_INFO_DIALOG, pParent)
	, m_Server_IP(_T(""))
	, m_User_ID(_T(""))
	, m_User_Password(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFTP_User_infoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ctrl_Server_IP);
	DDX_Control(pDX, IDC_USER_ID, m_ctrl_User_ID);
	DDX_Control(pDX, IDC_USER_PW, m_ctrl_User_PW);
}

BEGIN_MESSAGE_MAP(CFTP_User_infoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CFTP_User_infoDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CFTP_User_infoDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CFTP_User_infoDlg �޽��� ó����

BOOL CFTP_User_infoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	/**************Test�� ���߿� ����***************/
	m_ctrl_Server_IP.SetAddress(192, 168, 30, 62);
	SetDlgItemText(IDC_USER_ID, _T("user_t"));
	SetDlgItemText(IDC_USER_PW, _T("1234"));

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CFTP_User_infoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CFTP_User_infoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CFTP_User_infoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFTP_User_infoDlg::OnBnClickedButton1()						// Connect Button
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	BYTE ipFirst, ipSecond, ipThird, ipForth;
	m_ctrl_Server_IP.GetAddress(ipFirst, ipSecond, ipThird, ipForth);

	/* Connection ȭ�鿡�� �Է��� IP, ID, PW ���� */
	m_Server_IP.Format(_T("%d.%d.%d.%d"), ipFirst, ipSecond, ipThird, ipForth);
	GetDlgItemText(IDC_USER_ID, m_User_ID);
	GetDlgItemText(IDC_USER_PW, m_User_Password);
	/* �� m_Server_IP, m_User_ID, m_User_PW �� ���� */

	/* FTP_Client Dialog ����*/
	CFTP_Client ClientDlg;
	ClientDlg.DoModal();

	
}


void CFTP_User_infoDlg::OnBnClickedButton2()						// Cancel Button
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	::PostQuitMessage(WM_QUIT);
}


CString CFTP_User_infoDlg::GetServerIP()
{
	return m_Server_IP;
}


CString CFTP_User_infoDlg::GetUserID()
{
	return m_User_ID;
}


CString CFTP_User_infoDlg::GetUserPW()
{
	return m_User_Password;
}
