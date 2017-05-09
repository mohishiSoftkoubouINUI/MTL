// mainfrm.cpp : CMainFrame クラスのインプリメンテーション
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
#include "colorpge.h"
#include "stylepge.h"
#include "shapeobj.h"
#include "preview.h"
#include "propsht.h"
#include "propsht2.h"
#include "minifrm.h"
#include "resource.h"
#include "mainfrm.h"
#include "shapedoc.h"
#include "shapesvw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_MINI_FRAME_PROPERTY_SHEET, CMainFrame::OnMiniFramePropertySheet)
	ON_UPDATE_COMMAND_UI(ID_MINI_FRAME_PROPERTY_SHEET, CMainFrame::OnUpdateMiniFramePropertySheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// コントロール バーの初期化に使われる ID の配列

// ツールボタン - ID はコマンドボタン
static UINT BASED_CODE buttons[] =
{
	// 'toolbar.bmp' ビットマップの中と同じ順番
	ID_FILE_NEW,
	ID_FILE_OPEN,
	ID_FILE_SAVE,
		ID_SEPARATOR,
	ID_EDIT_CUT,
	ID_EDIT_COPY,
	ID_EDIT_PASTE,
		ID_SEPARATOR,
	ID_FILE_PRINT,
	ID_APP_ABOUT,
};

static UINT BASED_CODE indicators[] =
{
	ID_SEPARATOR,           // ステータスライン インジケータ
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame 構築/消滅

CMainFrame::CMainFrame()
{
	m_pShapePropFrame = NULL;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadBitmap(IDR_MAINFRAME) ||
		!m_wndToolBar.SetButtons(buttons,
		  sizeof(buttons)/sizeof(UINT)))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // 作成に失敗
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // 作成に失敗
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame 診断機能

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メッセージ ハンドラ

void CMainFrame::OnMiniFramePropertySheet()
{
	// ミニ フレームが存在しない場合は、新規に作成する
	// それ以外の場合は、可視にする

	if (m_pShapePropFrame == NULL)
	{
		m_pShapePropFrame = new CShapePropSheetFrame;
		CRect rect(0, 0, 0, 0);
		CString strTitle;
		strTitle.LoadString(IDS_OBJECT_PROPERTIES);
		if (!m_pShapePropFrame->Create(NULL, strTitle,
			WS_POPUP | WS_CAPTION | WS_SYSMENU, rect, this))
		{
			m_pShapePropFrame = NULL;
			return;
		}
		m_pShapePropFrame->CenterWindow();
	}

	// モードレス プロパティ シートを可視にする前に、現在の選択され
	// ているシェイプを反映するために設定を更新する
	CShapesView* pView = 
		STATIC_DOWNCAST(CShapesView, MDIGetActive()->GetActiveView());
	ASSERT_VALID(pView);
	if (pView->m_pShapeSelected != NULL)
	{
	   m_pShapePropFrame->m_pModelessShapePropSheet->
		   SetSheetPropsFromShape(pView->m_pShapeSelected);
	}

	if (m_pShapePropFrame != NULL && !m_pShapePropFrame->IsWindowVisible())
		m_pShapePropFrame->ShowWindow(SW_SHOW);
}

void CMainFrame::OnUpdateMiniFramePropertySheet(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pShapePropFrame == NULL
		|| !m_pShapePropFrame->IsWindowVisible());
}

void CMainFrame::HideModelessPropSheet()
{
	if (m_pShapePropFrame != NULL)
		m_pShapePropFrame->ShowWindow(SW_HIDE);
}
