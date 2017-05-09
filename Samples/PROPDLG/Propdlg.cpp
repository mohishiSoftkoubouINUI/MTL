// propdlg.cpp : アプリケーション用クラスの機能定義を行います。
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// このソース コードは Microsoft Foundation Class リファレンスおよび、
// 関連するオンライン ドキュメントの補足のみを目的としています。
// Microsoft Foundation Class 製品に関する詳しい情報はこれらのドキュメント
// を参照してください。
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
// CPropDlgApp クラスの構築

CPropDlgApp::CPropDlgApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CPropDlgApp オブジェクト

CPropDlgApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPropDlgApp クラスの初期化

BOOL CPropDlgApp::InitInstance()
{
	// このアプリケーションではダイアログは必ずグレーでなくてはなら
	// ないので、CTL3D32.DLL がロードされていない場合には立体表示で
	// ないグレーのダイアログを変わりに使います。

	if (!Enable3dControls())
		SetDialogBkColor();

	LoadStdProfileSettings();  // 標準の INI ファイルのオプションをロードします(MRU を含む)

	// ドキュメント テンプレートを登録

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_SHAPESTYPE,
		RUNTIME_CLASS(CShapesDoc),
		RUNTIME_CLASS(CMDIChildWnd),          // 標準の MDI 子フレーム
		RUNTIME_CLASS(CShapesView));
	AddDocTemplate(pDocTemplate);

	// メイン MDI フレーム ウィンドウを作成
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// ファイルマネジャのドラッグアンドドロップと DDE Execute のオープンを許可します
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// DDE、ファイルのオープンなど標準のシェル コマンドのコマンドラインを解析します。
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// コマンドラインでディスパッチ コマンドを指定します。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	m_pMainWnd->DragAcceptFiles();
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
	//{{AFX_MSG(CAboutDlg)
		// メッセージ ハンドラはありません。
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
		// メッセージ ハンドラはありません。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ダイアログを実行するためのアプリケーション コマンド
void CPropDlgApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CPropDlgApp コマンド
