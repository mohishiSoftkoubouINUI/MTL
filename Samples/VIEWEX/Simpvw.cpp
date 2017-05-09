// simpvw.cpp : シンプルなビュー クラスのインプリメンテーション
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

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextView

IMPLEMENT_DYNCREATE(CTextView, CView)

BEGIN_MESSAGE_MAP(CTextView, CView)
	//{{AFX_MSG_MAP(CTextView)
	ON_WM_MOUSEACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextView クラスの構築/消滅

CTextView::CTextView()
{
}

CTextView::~CTextView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CTextView クラスの描画

void CTextView::OnDraw(CDC* pDC)
{
	CMainDoc* pDoc = GetDocument();

	CRect rect;
	GetClientRect(rect);
	pDC->SetTextAlign(TA_BASELINE | TA_CENTER);
	pDC->SetTextColor(pDoc->m_colorData);
	pDC->SetBkMode(TRANSPARENT);
	// ウィンドウの中央
	pDC->TextOut(rect.Width() / 2, rect.Height() / 2, 
		pDoc->m_strData, pDoc->m_strData.GetLength());
}


int CTextView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// このビューをアクティブにしたくないので、CView のインプリメンテーションを
	// 回避します。
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

/////////////////////////////////////////////////////////////////////////////
// CColorView

IMPLEMENT_DYNCREATE(CColorView, CView)

BEGIN_MESSAGE_MAP(CColorView, CView)
	//{{AFX_MSG_MAP(CColorView)
	ON_WM_MOUSEACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorView クラスの構築/消滅

CColorView::CColorView()
{
}

CColorView::~CColorView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CColorView クラスの描画

void CColorView::OnDraw(CDC* pDC)
{
	CMainDoc* pDoc = GetDocument();

	CRect rect;
	GetClientRect(rect);

	// 指定の色でビューを塗りつぶします。
	CBrush br(pDoc->m_colorData);
	pDC->FillRect(rect, &br);
}

int CColorView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// このビューをアクティブにしたくないので、CView のインプリメンテーションを
	// 回避します。
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CColorView::OnActivateView(BOOL, CView*, CView*)
{
	ASSERT(FALSE);      // 出力のみのビュー - 決してアクティブにしません。
}


/////////////////////////////////////////////////////////////////////////////
