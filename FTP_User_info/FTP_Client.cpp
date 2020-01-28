// FTP_Client.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "FTP_User_info.h"
#include "FTP_Client.h"
#include "afxdialogex.h"
#include "FTP_User_infoDlg.h"
#include <WinInet.h>

// CFTP_Client ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CFTP_Client, CDialogEx)

CFTP_Client::CFTP_Client(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FTP_INFO_DIALOG, pParent)
	, m_get_Server_IP(_T(""))
	, m_get_User_ID(_T(""))
	, m_get_User_PW(_T(""))
	, m_ftpCon(NULL)												// CFtpConnection ������ ��ü
	, m_session(NULL)												// CInternetSession ������ ��ü
	, m_Local_Dir(_T(""))
	, m_FtpDir(_T(""))
{

}

CFTP_Client::~CFTP_Client()
{
}

void CFTP_Client::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_Edit_FtpDir);
	DDX_Control(pDX, IDC_LIST2, m_List_Ftp_File_List);
	DDX_Control(pDX, IDC_LIST1, m_List_Local_File_List);
	DDX_Control(pDX, IDC_EDIT1, m_Edit_LocalDir);
}


BEGIN_MESSAGE_MAP(CFTP_Client, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON9, &CFTP_Client::OnBnClickedButton9)

	ON_BN_CLICKED(IDC_BUTTON7, &CFTP_Client::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CFTP_Client::OnBnClickedButton8)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CFTP_Client �޽��� ó�����Դϴ�.


BOOL CFTP_Client::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	AllocConsole();
	freopen("CONOUT$", "wt", stdout);

	// User Info Dialog �ڵ��� �����´�.
	CFTP_User_infoDlg *UserInfoDlg = (CFTP_User_infoDlg*)AfxGetMainWnd();
	// User Info Dialog �� ����.
	UserInfoDlg->PostMessage(WM_CLOSE);
	// User Info Dialog�� ���� ������ ������ �ֱ� ������ �ռ� ������� Get �Լ��� �� �ʿ䰡 ����.
	m_get_Server_IP = UserInfoDlg->m_Server_IP;
	m_get_User_ID = UserInfoDlg->m_User_ID;
	m_get_User_PW = UserInfoDlg->m_User_Password;
	// m_get ������ User Info Dialog�� �������� �޾ƿ´�.


	/******************************** FTP Connection ���� **************************************/
	BeginWaitCursor();

	m_session = new CInternetSession(_T("FTP Session"));
	try {
		m_ftpCon = m_session->GetFtpConnection(m_get_Server_IP, m_get_User_ID, m_get_User_PW);
	}
	catch (CInternetException *e) {
		EndWaitCursor();
		e->ReportError();
		m_session->Close();
		m_ftpCon = NULL;
		e->Delete();
		return FALSE;
	}

	EndWaitCursor();
	/******************************************************************************************/
	// Local PC, FTP Server ��ο� �ִ� ������ �ҷ� �´�.
	GetLocalPCFile();
	GetFTPFile();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CFTP_Client::OnBnClickedButton9()	// Cancel Button
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	::PostQuitMessage(WM_QUIT);
	FreeConsole();
}


void CFTP_Client::GetLocalPCFile()		// FTP ���� �� Local Directory�� �����´�.
{
	CFileFind local_file_finder;
	CString LocalFileName;
	CString RootFile;
	// Local PC�� �ִ� Directory�� �о�´�.
	TCHAR str[128];
	::GetCurrentDirectory(128, str);
	m_Local_Dir = str;					
	printf("Local Dir = %S\n", m_Local_Dir);
	bool localfinding = local_file_finder.FindFile(m_Local_Dir+_T("*.*"));
	while (localfinding) {
		localfinding = local_file_finder.FindNextFile();
		if (local_file_finder.IsArchived()) {
			LocalFileName = local_file_finder.GetFileName();
			//RootFile = local_file_finder.GetRoot();
			m_List_Local_File_List.AddString(LocalFileName);
		}

	}
	RootFile = local_file_finder.GetRoot();
	m_Edit_LocalDir.SetWindowText(RootFile);

}


void CFTP_Client::GetFTPFile()			// FTP ���� �� FTP Directory�� �����´�.
{
	CFtpFileFind ftp_file_finder(m_ftpCon);
	//CString ftpDir;						���� ������ �ٽ� ����.

	// FTP Server �� Directory �� ftpDir�� �����Ѵ�.
	// m_FtpDir �� FTP ������ ��ΰ� �����.
	m_ftpCon->GetCurrentDirectory(m_FtpDir);
	m_Edit_FtpDir.SetWindowTextW(m_FtpDir);
	bool ftpfinding = ftp_file_finder.FindFile(_T("*.*"));

	while (ftpfinding) {
		// FTP Server File�� �ݺ��ؼ� ã�� List�� �߰��Ѵ�.
		ftpfinding = ftp_file_finder.FindNextFile();
		m_List_Ftp_File_List.AddString((LPCTSTR)ftp_file_finder.GetFileName());
	}

}



void CFTP_Client::OnBnClickedButton7()						// Send To FTP Server!!
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// Local PC ----> FTP Server
	printf("Click Send To FTP Button!\n");
	CString Local_File_Name;
	CString FTP_Server;
	CString Local_PC;
	int Local_File_Index = m_List_Local_File_List.GetCurSel();
	if (Local_File_Index != LB_ERR){
		m_List_Local_File_List.GetText(Local_File_Index, Local_File_Name);
	}
	else{
		AfxMessageBox(_T("Choose Local PC File!!!"));
	}
	m_FtpDir.ReleaseBuffer();
	FTP_Server = m_FtpDir + Local_File_Name;
	printf("FTP_Server : %S\n", FTP_Server);
	Local_PC = m_Local_Dir + _T("\\") + Local_File_Name;
	printf("Local_PC : %S\n", Local_PC);

	try{
		m_ftpCon->PutFile(Local_PC, FTP_Server);
	}
	catch (CInternetException *e){
		EndWaitCursor();
		e->ReportError();
		e->Delete();
	}
	m_List_Ftp_File_List.ResetContent();
	GetFTPFile(); 

}


void CFTP_Client::OnBnClickedButton8()						// Send To Local PC!!
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// FTP Server ----> Local PC

}


void CFTP_Client::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	FreeConsole();
}
