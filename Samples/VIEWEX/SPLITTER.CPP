// splitter.cpp : インプリメンテーション ファイル
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
// CSplitterFrame

// 出力テキストビューおよび入力ビューを分割するスプリッター ウィンドウの作成
//                           |
//    TEXT VIEW (CTextView)  | INPUT VIEW (CInputView)
//                           |

IMPLEMENT_DYNCREATE(CSplitterFrame, CMDIChildWnd)

CSplitterFrame::CSplitterFrame()
{
}

CSplitterFrame::~CSplitterFrame()
{
}


BOOL CSplitterFrame::OnCreateClient(LPCREATESTRUCT,
	 CCreateContext* pContext)
{
	// 1 列、2 行のスプリッターを作成
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}

	// 1 番目のスプリッター ペインを追加 - 0 行目のデフォルト ビュー 
	if (!m_wndSplitter.CreateView(0, 0,
		pContext->m_pNewViewClass, CSize(130, 50), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}

	// 2 番目のスプリッター ペインを追加 - 1 行目の入力ビュー
	if (!m_wndSplitter.CreateView(0, 1,
		RUNTIME_CLASS(CInputView), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}

	// 入力ビューをアクティブにします。
	SetActiveView((CView*)m_wndSplitter.GetPane(0,1));

	return TRUE;
}

BEGIN_MESSAGE_MAP(CSplitterFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CSplitterFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3WaySplitterFrame - CSplitterFrame のように一番目のペインの入力ビュー
// を除いて、2 つの出力ビューがあります。

//                             |  Text View (CTextView)
//    INPUT VIEW (CInputView)  |------------------------
//                             |  Color View (CColorView)

IMPLEMENT_DYNCREATE(C3WaySplitterFrame, CMDIChildWnd)

C3WaySplitterFrame::C3WaySplitterFrame()
{
}

C3WaySplitterFrame::~C3WaySplitterFrame()
{
}


BOOL C3WaySplitterFrame::OnCreateClient(LPCREATESTRUCT lpcs,
	 CCreateContext* pContext)
{
	// 1 列、2 行のスプリッターを作成
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}

	// 1 番目のスプリッター ペインを追加 - 0 行目のデフォルト ビュー
	if (!m_wndSplitter.CreateView(0, 0,
		pContext->m_pNewViewClass, CSize(200, 50), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}

	// 2 番目のスプリッター ペインを追加 - 2 行にネストしたスプリッター
	if (!m_wndSplitter2.CreateStatic(
		&m_wndSplitter,     // 親ウィンドウは 1 番め目のスプリッター
		2, 1,               // 新しいスプリッターは 2 列、1 行
		WS_CHILD | WS_VISIBLE | WS_BORDER,  // スタイル WS_BORDER が必要です。
		m_wndSplitter.IdFromRowCol(0, 1)
			// 新しいスプリッターは 1 番目の列にあります、 1 番目のスプリッターの 2 行目
	   ))
	{
		TRACE0("Failed to create nested splitter\n");
		return FALSE;
	}

	// ネストされたスプリッター内に 2 つのビューを作成します。
	int cyText = max(lpcs->cy - 70, 20);    // テキスト ペインの高さ

	if (!m_wndSplitter2.CreateView(0, 0,
		RUNTIME_CLASS(CTextView), CSize(0, cyText), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}
	if (!m_wndSplitter2.CreateView(1, 0,
		RUNTIME_CLASS(CColorView), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create third pane\n");
		return FALSE;
	}

	// すべて動作した場合、3 つの異なるビューを含む 2 つのスプリッター ウィンドウ
	// ができます。
	return TRUE;
}

BEGIN_MESSAGE_MAP(C3WaySplitterFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(C3WaySplitterFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// 

IMPLEMENT_DYNAMIC(CViewExSplitWnd, CSplitterWnd)

CViewExSplitWnd::CViewExSplitWnd()
{
}

CViewExSplitWnd::~CViewExSplitWnd()
{
}

CWnd* CViewExSplitWnd::GetActivePane(int* pRow, int* pCol)
{
	ASSERT_VALID(this);

	// フレーム ウィンドウのアクティブ ビューの使用を試みます。
	CWnd* pView = NULL;
 	CFrameWnd* pFrameWnd = GetParentFrame();
	ASSERT_VALID(pFrameWnd);
	pView = pFrameWnd->GetActiveView();

	// 失敗した場合、現在のフォーカスを使用します。
	if (pView == NULL)
		pView = GetFocus();

	return pView;
}


