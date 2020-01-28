// FTP_Client.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FTP_User_info.h"
#include "FTP_Client.h"
#include "afxdialogex.h"
#include "FTP_User_infoDlg.h"
#include <WinInet.h>

// CFTP_Client 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFTP_Client, CDialogEx)

CFTP_Client::CFTP_Client(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FTP_INFO_DIALOG, pParent)
	, m_get_Server_IP(_T(""))
	, m_get_User_ID(_T(""))
	, m_get_User_PW(_T(""))
	, m_ftpCon(NULL)												// CFtpConnection 포인터 객체
	, m_session(NULL)												// CInternetSession 포인터 객체
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


// CFTP_Client 메시지 처리기입니다.


BOOL CFTP_Client::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	AllocConsole();
	freopen("CONOUT$", "wt", stdout);

	// User Info Dialog 핸들을 가져온다.
	CFTP_User_infoDlg *UserInfoDlg = (CFTP_User_infoDlg*)AfxGetMainWnd();
	// User Info Dialog 는 종료.
	UserInfoDlg->PostMessage(WM_CLOSE);
	// User Info Dialog에 대한 포인터 변수가 있기 때문에 앞서 만들었던 Get 함수는 쓸 필요가 없다.
	m_get_Server_IP = UserInfoDlg->m_Server_IP;
	m_get_User_ID = UserInfoDlg->m_User_ID;
	m_get_User_PW = UserInfoDlg->m_User_Password;
	// m_get 변수에 User Info Dialog의 변수들을 받아온다.


	/******************************** FTP Connection 시작 **************************************/
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
	// Local PC, FTP Server 경로에 있는 파일을 불러 온다.
	GetLocalPCFile();
	GetFTPFile();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CFTP_Client::OnBnClickedButton9()	// Cancel Button
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	::PostQuitMessage(WM_QUIT);
	FreeConsole();
}


void CFTP_Client::GetLocalPCFile()		// FTP 접속 시 Local Directory를 가져온다.
{
	CFileFind local_file_finder;
	CString LocalFileName;
	CString RootFile;
	// Local PC에 있는 Directory를 읽어온다.
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


void CFTP_Client::GetFTPFile()			// FTP 접속 시 FTP Directory를 가져온다.
{
	CFtpFileFind ftp_file_finder(m_ftpCon);
	//CString ftpDir;						전역 변수로 다시 만듬.

	// FTP Server 의 Directory 를 ftpDir에 저장한다.
	// m_FtpDir 에 FTP 서버의 경로가 저장됨.
	m_ftpCon->GetCurrentDirectory(m_FtpDir);
	m_Edit_FtpDir.SetWindowTextW(m_FtpDir);
	bool ftpfinding = ftp_file_finder.FindFile(_T("*.*"));

	while (ftpfinding) {
		// FTP Server File을 반복해서 찾고 List에 추가한다.
		ftpfinding = ftp_file_finder.FindNextFile();
		m_List_Ftp_File_List.AddString((LPCTSTR)ftp_file_finder.GetFileName());
	}

}



void CFTP_Client::OnBnClickedButton7()						// Send To FTP Server!!
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// FTP Server ----> Local PC

}


void CFTP_Client::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	FreeConsole();
}
