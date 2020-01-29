// FTP_Client.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "FTP_User_info.h"
#include "FTP_Client.h"
#include "afxdialogex.h"
#include "FTP_User_infoDlg.h"
#include <WinInet.h>

/*************************************************************************/
// Window , Dos ������ Directory �����ڸ� \\ (W- ǥ��)
// Internet ������ Directory �����ڸ� /�� ǥ��
/*************************************************************************/

// CFTP_Client ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CFTP_Client, CDialogEx)

CFTP_Client::CFTP_Client(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FTP_INFO_DIALOG, pParent)
	, m_get_Server_IP(_T(""))
	, m_get_User_ID(_T(""))
	, m_get_User_PW(_T(""))
	, m_ftpCon(NULL)												// CFtpConnection ������ ��ü
	, m_session(NULL)												// CInternetSession ������ ��ü
	, m_LocalDir(_T(""))
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
	ON_BN_CLICKED(IDC_BUTTON1, &CFTP_Client::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CFTP_Client::OnBnClickedButton4)
	ON_LBN_DBLCLK(IDC_LIST1, &CFTP_Client::OnLbnDblclkList1)
	ON_LBN_DBLCLK(IDC_LIST2, &CFTP_Client::OnLbnDblclkList2)
	ON_BN_CLICKED(IDC_BUTTON5, &CFTP_Client::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CFTP_Client::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON2, &CFTP_Client::OnBnClickedButton2)
END_MESSAGE_MAP()


// CFTP_Client �޽��� ó�����Դϴ�.


BOOL CFTP_Client::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	AllocConsole();
	freopen("CONOUT$", "wt", stdout);
	/*********************************Initial Directory******************************************/
	::GetCurrentDirectory(128, m_str);
	m_LocalDir = m_str;
	/********************************************************************************************/

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
		::PostQuitMessage(WM_QUIT);
	}

	EndWaitCursor();
	/******************************************************************************************/
	// Local PC, FTP Server ��ο� �ִ� ������ �ҷ� �´�.
	GetLocalPCFile(m_LocalDir);
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


void CFTP_Client::GetLocalPCFile(CString str)		// FTP ���� �� Local Directory�� �����´�.
{
	CFileFind local_file_finder;
	CString LocalFileName;
	// Local PC�� �ִ� Directory�� �о�´�.
	//TCHAR str[128];
	//::GetCurrentDirectory(128, str);
	//m_LocalDir = m_str;					
	printf("GetLocalPCFile Local Dir = %S\n", str);
	m_Edit_LocalDir.SetWindowText(str);
	bool localfinding = local_file_finder.FindFile(str + _T("\/*.*"));
	while (localfinding) {
		localfinding = local_file_finder.FindNextFile();
		// Directory �̵��� ���� ., .. ǥ��
		if (local_file_finder.IsDirectory()){
			LocalFileName = local_file_finder.GetFileName();
			if (LocalFileName != _T(".") && LocalFileName != _T("..")){
				m_List_Local_File_List.AddString(_T("[Dir]" + LocalFileName));
			}
			else{
				m_List_Local_File_List.AddString(LocalFileName);
			}
		}
		if (local_file_finder.IsArchived()) {
			LocalFileName = local_file_finder.GetFileName();
			m_List_Local_File_List.AddString(LocalFileName);
		}
	}
}


void CFTP_Client::GetFTPFile()			// FTP ���� �� FTP Directory�� �����´�.
{
	CFtpFileFind ftp_file_finder(m_ftpCon);
	CString FtpFileName;
	//CString ftpDir;						���� ������ �ٽ� ����.

	// FTP Server �� Directory �� ftpDir�� �����Ѵ�.
	// m_FtpDir �� FTP ������ ��ΰ� �����.
	m_ftpCon->GetCurrentDirectory(m_FtpDir);
	printf("GetFTPFile FTP Dir = %S\n", m_FtpDir);
	m_Edit_FtpDir.SetWindowTextW(m_FtpDir);
	bool ftpfinding = ftp_file_finder.FindFile(m_FtpDir+_T("\/*.*"));

	while (ftpfinding) {
		// FTP Server File�� �ݺ��ؼ� ã�� List�� �߰��Ѵ�.
		ftpfinding = ftp_file_finder.FindNextFile();
		if (ftp_file_finder.IsDirectory()){
			FtpFileName = ftp_file_finder.GetFileName();
			m_List_Ftp_File_List.AddString(_T("[Dir]" + FtpFileName)); 
		}
		else{
			FtpFileName = ftp_file_finder.GetFileName();
			m_List_Ftp_File_List.AddString(FtpFileName);
		}
	}
}



void CFTP_Client::OnBnClickedButton7()						// Send To FTP Server Button !!
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// Local PC ----> FTP Server
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
	m_FtpDir.ReleaseBuffer();	// �̰� ���ϸ� m_FtpDir �ڿ� ��θ� ���� ��������.
	// ftp ��� -> m_FtpDir
	// Local ��� -> m_LocalDir
	FTP_Server = m_FtpDir + Local_File_Name;
	printf("FTP_Server : %S\n", FTP_Server);
	Local_PC = m_LocalDir + _T("\\") + Local_File_Name;
	printf("Local_PC : %S\n", Local_PC);

	try{
		m_ftpCon->PutFile(Local_PC, FTP_Server);
		printf("PutFile!!\n");
	}
	catch (CInternetException *e){
		EndWaitCursor();
		e->ReportError();
		e->Delete();
	}
	m_List_Ftp_File_List.ResetContent();
	GetFTPFile();

}


void CFTP_Client::OnBnClickedButton8()						// Send To Local PC Button !!
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// FTP Server ----> Local PC
	CString Ftp_File_Name;
	CString _LocalDir;
	int Ftp_File_Index = m_List_Ftp_File_List.GetCurSel();
	if (Ftp_File_Index != LB_ERR){
		m_List_Ftp_File_List.GetText(Ftp_File_Index, Ftp_File_Name);
	}
	else{
		AfxMessageBox(_T("Choose FTP Server File!!!"));
	}
	_LocalDir = m_LocalDir + _T("\\") + Ftp_File_Name;
	printf(" sadsajdkada : %S\n", _LocalDir);
	m_ftpCon->GetFile(Ftp_File_Name, _LocalDir);
	m_List_Local_File_List.ResetContent();
	GetLocalPCFile(m_LocalDir);

}


void CFTP_Client::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	FreeConsole();
}


void CFTP_Client::OnBnClickedButton1()			// Local PC File Renew Button
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_List_Local_File_List.ResetContent();
	GetLocalPCFile(m_LocalDir);
}


void CFTP_Client::OnBnClickedButton4()			// FTP Server File Renew Button
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_List_Ftp_File_List.ResetContent();
	GetFTPFile();
}


void CFTP_Client::OnLbnDblclkList1()			// Local PC File List Double Click!
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// Local PC ----> FTP Server

	CString _Local_File_Name;
	int _Local_File_Index = m_List_Local_File_List.GetCurSel();
	int LocalDir_Len = m_LocalDir.GetLength();							// Local Directory ��ü ����
	int LocalDir_Find = m_LocalDir.ReverseFind(_T('\\'));				// ���� ������

	if (_Local_File_Index != LB_ERR){
		m_List_Local_File_List.GetText(_Local_File_Index, _Local_File_Name);
	}
	else{
		AfxMessageBox(_T("Choose Local PC File!!!"));
	}

	if (_Local_File_Name == _T("..")){
		m_LocalDir.Delete(LocalDir_Find, (LocalDir_Len - LocalDir_Find));
		m_List_Local_File_List.ResetContent();
		GetLocalPCFile(m_LocalDir);
		printf("After LocalDir : %S\n", m_LocalDir);
	}
	else if(_Local_File_Name.Left(1)=='[')
	{
		//int Len = _Local_File_Name.GetLength();
		int Find = _Local_File_Name.Find(_T(']'));
		_Local_File_Name.Delete(0, Find+1);
		m_LocalDir += _T("\\")+_Local_File_Name;
		m_List_Local_File_List.ResetContent();
		GetLocalPCFile(m_LocalDir);
	}
	else{
		SendToFTPServer(_Local_File_Name);
	}
}


void CFTP_Client::OnLbnDblclkList2()			// FTP Server File List Double Click!
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// FTP Server ----> Local PC
	CString Ftp_File_Name;
	int Ftp_File_Index = m_List_Ftp_File_List.GetCurSel();
	if (Ftp_File_Index != LB_ERR){
		m_List_Ftp_File_List.GetText(Ftp_File_Index, Ftp_File_Name);
	}
	else{
		AfxMessageBox(_T("Choose FTP Server File!!!"));
	}

	if (Ftp_File_Name.Left(1) == '[')
	{
		//int Len = _Local_File_Name.GetLength();
		int Find = Ftp_File_Name.Find(_T(']'));
		Ftp_File_Name.Delete(0, Find + 1);
		m_FtpDir.ReleaseBuffer();
		if (m_FtpDir.Right(1) == _T('\\')){
			m_FtpDir += Ftp_File_Name;
		}
		else{
			m_FtpDir += _T("\\") + Ftp_File_Name;
		}
		m_ftpCon->SetCurrentDirectory(m_FtpDir);
		printf("m_FtpDir : %S\n", m_FtpDir);
		m_List_Ftp_File_List.ResetContent();
		GetFTPFile();
	}
	else{
		SendToLocalPC(Ftp_File_Name);
	}
	//m_ftpCon->GetFile(Ftp_File_Name, m_LocalDir);
}


void CFTP_Client::SendToFTPServer(CString L_FileName)
{
	// Local PC ----> FTP Server
	CString FTP_Server;
	CString Local_PC;
	m_FtpDir.ReleaseBuffer();	// �̰� ���ϸ� m_FtpDir �ڿ� ��θ� ���� ��������.
	// ftp ��� -> m_FtpDir
	// Local ��� -> m_LocalDir
	FTP_Server = m_FtpDir + L_FileName;
	printf("FTP_Server : %S\n", FTP_Server);
	Local_PC = m_LocalDir + _T("\\") + L_FileName;
	printf("Local_PC : %S\n", Local_PC);
	
	try{
		m_ftpCon->PutFile(Local_PC, FTP_Server);
		printf("SendToFTPServer Local : %S\n FTP : %S\n", Local_PC, FTP_Server);
	}
	catch (CInternetException *e){
		e->ReportError();
		e->Delete();
	}
	m_List_Ftp_File_List.ResetContent();
	GetFTPFile();
}


void CFTP_Client::SendToLocalPC(CString F_FileName)
{
	// FTP Server ----> Local PC
	CString _LocalDir;
	_LocalDir = m_LocalDir + _T("\\") + F_FileName;

	try{
		m_ftpCon->GetFile(F_FileName, _LocalDir);
	}
	catch (CInternetException *e){
		e->ReportError();
		e->Delete();
	}
	m_List_Local_File_List.ResetContent();
	GetLocalPCFile(m_LocalDir);
}


void CFTP_Client::OnBnClickedButton5()						// FTP Directory UP Button 
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString CurrentDir;
	int find;
	int len;
	m_ftpCon->GetCurrentDirectory(CurrentDir);
	printf("Before CurrenDir : %S\n", CurrentDir);
	//CurrentDir.ReleaseBuffer();
	len = CurrentDir.GetLength();
	find = CurrentDir.ReverseFind(_T('\/'));
	CurrentDir.Delete(find+1, len - find);
	printf("After CurrentDir : %S\n", CurrentDir);
	m_ftpCon->SetCurrentDirectory(CurrentDir);
	printf("After CurrentDir2 : %S\n", CurrentDir);

	m_List_Ftp_File_List.ResetContent();
	GetFTPFile();


}


void CFTP_Client::OnBnClickedButton6()					  // FTP Directory File Delete
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString Delete_FTP_File;

	int Delete_FTP_File_Index = m_List_Ftp_File_List.GetCurSel();
	if (Delete_FTP_File_Index != LB_ERR){
		m_List_Ftp_File_List.GetText(Delete_FTP_File_Index, Delete_FTP_File);
	}
	else{
		AfxMessageBox(_T("Selecet Delete File What you want"));
	}

	try{
		m_ftpCon->Remove(Delete_FTP_File);
		printf("Delete!!\n");
	}
	catch (CInternetException *e){
		EndWaitCursor();
		e->ReportError();
		e->Delete();
	}
	m_List_Ftp_File_List.ResetContent();
	GetFTPFile();
}


void CFTP_Client::OnBnClickedButton2()					// Local Directory File Delete
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString Delete_Local_File;
	CString Delete_Local_File_Dir;
	CFileFind Dfind;
	BOOL F_Delete;
	int Delete_File_Index = m_List_Local_File_List.GetCurSel();
	if (Delete_File_Index != LB_ERR){
		m_List_Local_File_List.GetText(Delete_File_Index, Delete_Local_File);
	}
	else{
		AfxMessageBox(_T("Choose Local PC File!!!"));
	}
	Delete_Local_File_Dir = m_LocalDir + _T("\\") + Delete_Local_File;
	printf("Delete Local File Dir : %S\n", Delete_Local_File_Dir);
	F_Delete = Dfind.FindFile(Delete_Local_File_Dir);

	if (F_Delete){
		DeleteFile(Delete_Local_File_Dir);
	}
	else{
		printf(" Failed To Delete Local File \n");
	}

	m_List_Local_File_List.ResetContent();
	GetLocalPCFile(m_LocalDir);
}
