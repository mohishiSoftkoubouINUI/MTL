// inputvw.cpp : インプリメンテーション ファイル
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
// CInputView

IMPLEMENT_DYNCREATE(CInputView, CFormView)

CInputView::CInputView()
	: CFormView(CInputView::IDD)
{
	//{{AFX_DATA_INIT(CInputView)
	m_strData = "";
	m_iColor = -1;
	//}}AFX_DATA_INIT
}

CInputView::~CInputView()
{
}

void CInputView::OnUpdate(CView*, LPARAM, CObject*)
{
	CMainDoc* pDoc = GetDocument();
	m_strData = pDoc->m_strData;

	if (pDoc->m_colorData == RGB(255, 0, 0))
		m_iColor = 0;
	else if (pDoc->m_colorData == RGB(0, 255, 0))
		m_iColor = 1;
	else if (pDoc->m_colorData == RGB(0, 0, 255))
		m_iColor = 2;
	else
		m_iColor = -1;

TRACE2("OnUpdate: m_iColor = %d ($%lx)\n", m_iColor, pDoc->m_colorData);

	UpdateData(FALSE);  // データをコントロールへ設定
}

void CInputView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputView)
	DDX_Text(pDX, IDC_EDIT1, m_strData);
	DDX_Radio(pDX, IDC_RADIO1, m_iColor);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInputView, CFormView)
	//{{AFX_MSG_MAP(CInputView)
	ON_EN_CHANGE(IDC_EDIT1, CInputView::OnDataChange)
	ON_BN_CLICKED(IDC_RADIO1, CInputView::OnDataChange)
	ON_BN_CLICKED(IDC_RADIO2, CInputView::OnDataChange)
	ON_BN_CLICKED(IDC_RADIO3, CInputView::OnDataChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputView メッセージ ハンドラ

void CInputView::OnDataChange()
{
	if (!UpdateData())
		return;

	CMainDoc* pDoc = GetDocument();
	COLORREF color = RGB(255 * (m_iColor == 0),
						255 * (m_iColor == 1),
						255 * (m_iColor == 2));

	BOOL bUpdate = FALSE;
	if (m_strData != pDoc->m_strData)
	{
		pDoc->m_strData = m_strData;
		bUpdate = TRUE;
	}
	if (color != pDoc->m_colorData)
	{
		pDoc->m_colorData = color;
		bUpdate = TRUE;
	}
	if (bUpdate)
	{
		// ドキュメントがデータを保存した場合、この位置で SetModifiedFlag を呼び出します。
		pDoc->UpdateAllViews(this);
	}
}

/////////////////////////////////////////////////////////////////////////////
