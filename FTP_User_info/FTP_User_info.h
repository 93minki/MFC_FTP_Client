
// FTP_User_info.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CFTP_User_infoApp:
// �� Ŭ������ ������ ���ؼ��� FTP_User_info.cpp�� �����Ͻʽÿ�.
//

class CFTP_User_infoApp : public CWinApp
{
public:
	CFTP_User_infoApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CFTP_User_infoApp theApp;