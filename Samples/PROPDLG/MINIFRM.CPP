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

#include "shapeobj.h"
#include "resource.h"
#include "colorpge.h"
#include "stylepge.h"
#include "propsht2.h"
#include "minifrm.h"
#include "mainfrm.h"

BEGIN_MESSAGE_MAP(CShapePropSheetFrame, CMiniFrameWnd)
	//{{AFX_MSG_MAP(CShapePropSheetFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_SETFOCUS()
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CShapePropSheetFrame::CShapePropSheetFrame()
{
	m_pModelessShapePropSheet = NULL;
}

int CShapePropSheetFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMiniFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pModelessShapePropSheet = new CModelessShapePropSheet(this);
	if (!m_pModelessShapePropSheet->Create(this, 
		WS_CHILD | WS_VISIBLE, 0))
	{
		delete m_pModelessShapePropSheet;
		m_pModelessShapePropSheet = NULL;
		return -1;
	}

	// チャイルド プロパティシートのサイズに合わせてミニ フレームの
	// サイズを変更する
	CRect rectClient, rectWindow;
	m_pModelessShapePropSheet->GetWindowRect(rectClient);
	rectWindow = rectClient;

	// CMiniFrameWnd::CalcWindowRect はミニ フレームの表示するために
	// 幅と高さを調節する
	CalcWindowRect(rectWindow);
	SetWindowPos(NULL, rectWindow.left, rectWindow.top,
		rectWindow.Width(), rectWindow.Height(),
		SWP_NOZORDER | SWP_NOACTIVATE);
	m_pModelessShapePropSheet->SetWindowPos(NULL, 0, 0,
		rectClient.Width(), rectClient.Height(),
		SWP_NOZORDER | SWP_NOACTIVATE);

	return 0;
}

void CShapePropSheetFrame::OnClose()
{
	// モードレス プロパティシートを閉じる代わりに、不可視にする
	CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	pMainFrame->HideModelessPropSheet();
}

void CShapePropSheetFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 埋め込まれているプロパティシートにフォーカスメッセージを転送する
	ASSERT_VALID(m_pModelessShapePropSheet);
	m_pModelessShapePropSheet->SetFocus();
}

void CShapePropSheetFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CMiniFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	// すべての WM_ACTIVATE メッセージをプロパティシートに転送する
	// ダイアログ マネジャ自身のように、フォーカスを保存/復帰する
	ASSERT_VALID(m_pModelessShapePropSheet);

	// GetCurrentMessage で変更されていないメッセージ データを取得
	const MSG* pMsg = GetCurrentMessage();
	ASSERT(pMsg->message == WM_ACTIVATE);
	m_pModelessShapePropSheet->SendMessage(pMsg->message, pMsg->wParam,
		pMsg->lParam);
}
