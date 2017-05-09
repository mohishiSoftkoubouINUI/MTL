// viewex.cpp : アプリケーション用にクラスの動作の定義を行います。
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
#include "viewex.h"

#include "splitter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewExApp

BEGIN_MESSAGE_MAP(CViewExApp, CWinApp)
	//{{AFX_MSG_MAP(CViewExApp)
	ON_COMMAND(ID_APP_ABOUT, CViewExApp::OnAppAbout)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewExApp クラスの構築
// この位置に InitInstance 中の重要な初期化処理をすべて記述してください。

CViewExApp::CViewExApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CViewExApp オブジェクト

CViewExApp NEAR theApp;

/////////////////////////////////////////////////////////////////////////////
// CViewExApp クラスの初期化

BOOL CViewExApp::InitInstance()
{
	// 標準的な初期化処理
	Enable3dControls();

	// この VIEWEX サンプルでは CView 派生クラスに対するいくつかのテクニックを
	// 紹介しています。  このサンプル内の全てのビューは同じ CMainDoc クラスを
	// 使用しています。  簡単に説明すると CMainDoc クラスはシリアライゼーション
	// をサポートしていませんが、シングル ストリングおよび RGB カラーの非常に
	// 単純なデータ モデルを持っています。異なるドキュメント テンプレートを
	// 各興味深いビュー タイプに対して登録します。

	// 簡単なテキスト出力ビュー
	AddDocTemplate(new CMultiDocTemplate(IDR_TEXTTYPE,
			RUNTIME_CLASS(CMainDoc),
			RUNTIME_CLASS(CMDIChildWnd),
			RUNTIME_CLASS(CTextView)));

	// 簡単なカラー出力ビュー
	//  'fileNewName' なしのドキュメント テンプレートなので FileNew の選択はありません。
	AddDocTemplate(new CMultiDocTemplate(IDR_COLORTYPE,
			RUNTIME_CLASS(CMainDoc),
			RUNTIME_CLASS(CMDIChildWnd),
			RUNTIME_CLASS(CColorView)));

	// 入力付きのフォーム ビュー
	AddDocTemplate(new CMultiDocTemplate(IDR_INPUTTYPE,
			RUNTIME_CLASS(CMainDoc),
			RUNTIME_CLASS(CMDIChildWnd),
			RUNTIME_CLASS(CInputView)));

	// 簡単なテキスト出力およびフォーム入力ビュー付きスプリッター フレーム
	AddDocTemplate(new CMultiDocTemplate(IDR_SPLIT2TYPE,
			RUNTIME_CLASS(CMainDoc),
			RUNTIME_CLASS(CSplitterFrame),
			RUNTIME_CLASS(CTextView)));

	// フォーム入力、テキスト出力、カラー出力ビューの 3 通りのスプリッター フレーム
	AddDocTemplate(new CMultiDocTemplate(IDR_SPLIT3TYPE,
			RUNTIME_CLASS(CMainDoc),
			RUNTIME_CLASS(C3WaySplitterFrame),
			RUNTIME_CLASS(CInputView)));

	// メイン MDI フレーム ウィンドウの作成
	// ツールバー、ステータス バーまたはその他特別な機能を持たない簡単な MDI
	// フレーム ウィンドウに対して、CMDIFrameWnd クラスは、CMDIChildWnd が
	// ドキュメント フレーム ウィンドウに対して使用されるように、メイン フレーム
	// ウィンドウに対して直接使用されます。

	CMDIFrameWnd* pMainFrame = new CMDIFrameWnd;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	// この例ではまた、全てのビューで共有されるメニューバーが 1 つだけあります。
	// MFC の自動メニューサポートは、現在アクティブなビューに基づき適用しない
	// メニュー アイテムを使用不能にします。
	// 1 つだけのメニューバーは、開いているドキュメントが無い場合も含めて、すべ
	// てのドキュメント タイプに対して使用されます。

	// メイン メニューを最終的に表示します
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	m_pMainWnd = pMainFrame;

	// コマンド ライン引数は無視されます、新しい(空の)ドキュメントを作成します。
	OnFileNew();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CViewExApp コマンド

void CViewExApp::OnAppAbout()
{
	CDialog(IDD_ABOUTBOX).DoModal();
}

/////////////////////////////////////////////////////////////////////////////
