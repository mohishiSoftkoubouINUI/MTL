// propdlg.cpp : �A�v���P�[�V�����p�N���X�̋@�\��`���s���܂��B
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// ���̃\�[�X �R�[�h�� Microsoft Foundation Class ���t�@�����X����сA
// �֘A����I�����C�� �h�L�������g�̕⑫�݂̂�ړI�Ƃ��Ă��܂��B
// Microsoft Foundation Class ���i�Ɋւ���ڂ������͂����̃h�L�������g
// ���Q�Ƃ��Ă��������B
//

#include "stdafx.h"
#include "propdlg.h"

#include "shapeobj.h"
#include "colorpge.h"
#include "stylepge.h"
#include "preview.h"
#include "propsht.h"
#include "propsht2.h"
#include "minifrm.h"
#include "mainfrm.h"
#include "shapedoc.h"
#include "shapesvw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropDlgApp

BEGIN_MESSAGE_MAP(CPropDlgApp, CWinApp)
	//{{AFX_MSG_MAP(CPropDlgApp)
	ON_COMMAND(ID_APP_ABOUT, CPropDlgApp::OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropDlgApp �N���X�̍\�z

CPropDlgApp::CPropDlgApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CPropDlgApp �I�u�W�F�N�g

CPropDlgApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPropDlgApp �N���X�̏�����

BOOL CPropDlgApp::InitInstance()
{
	// ���̃A�v���P�[�V�����ł̓_�C�A���O�͕K���O���[�łȂ��Ă͂Ȃ�
	// �Ȃ��̂ŁACTL3D32.DLL �����[�h����Ă��Ȃ��ꍇ�ɂ͗��̕\����
	// �Ȃ��O���[�̃_�C�A���O��ς��Ɏg���܂��B

	if (!Enable3dControls())
		SetDialogBkColor();

	LoadStdProfileSettings();  // �W���� INI �t�@�C���̃I�v�V���������[�h���܂�(MRU ���܂�)

	// �h�L�������g �e���v���[�g��o�^

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_SHAPESTYPE,
		RUNTIME_CLASS(CShapesDoc),
		RUNTIME_CLASS(CMDIChildWnd),          // �W���� MDI �q�t���[��
		RUNTIME_CLASS(CShapesView));
	AddDocTemplate(pDocTemplate);

	// ���C�� MDI �t���[�� �E�B���h�E���쐬
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// �t�@�C���}�l�W���̃h���b�O�A���h�h���b�v�� DDE Execute �̃I�[�v���������܂�
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// DDE�A�t�@�C���̃I�[�v���ȂǕW���̃V�F�� �R�}���h�̃R�}���h���C������͂��܂��B
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// �R�}���h���C���Ńf�B�X�p�b�` �R�}���h���w�肵�܂��B
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	m_pMainWnd->DragAcceptFiles();
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// �A�v���P�[�V�����̃o�[�W�������Ŏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
	//{{AFX_MSG(CAboutDlg)
		// ���b�Z�[�W �n���h���͂���܂���B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// ���b�Z�[�W �n���h���͂���܂���B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// �_�C�A���O�����s���邽�߂̃A�v���P�[�V���� �R�}���h
void CPropDlgApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CPropDlgApp �R�}���h
