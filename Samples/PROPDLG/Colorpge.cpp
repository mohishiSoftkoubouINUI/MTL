// colorpge.cpp : インプリメンテーション ファイル
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
#include "stylepge.h"
#include "colorpge.h"
#include "shapeobj.h"
#include "preview.h"
#include "propsht.h"
#include "propsht2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorPage プロパティ ページ

CColorPage::CColorPage()
	: CPropertyPage(CColorPage::IDD)
{
	//{{AFX_DATA_INIT(CColorPage)
	//}}AFX_DATA_INIT
	m_nColor = black;
}

void CColorPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorPage)
	DDX_Radio(pDX, IDC_BLACK, m_nColor);
	//}}AFX_DATA_MAP
}

void CColorPage::OnColorClicked(UINT /*nCmdID*/)
{
	// CColorPage プロパティページ は CModalShapePropSheet と
	// CModelessShapePropSheet の両方に使われています。どちらの
	// プロパティシートの場合もすぐにシェイプを変更するという共通の
	// 特徴を持っています。CModalShapePropSheet の場合、現在のシェ
	//イプがプレビューウィンドウに表示され、CModelessShapePropSheet
	// の場合にはビューの中で選択されているのが現在のシェイプです。
	//

	CPropertySheet* pSheet = STATIC_DOWNCAST(CPropertySheet, GetParent());

	CModalShapePropSheet* pModalSheet = 
		DYNAMIC_DOWNCAST(CModalShapePropSheet, pSheet);
	if (pModalSheet != NULL)
	{
		UpdateData();
		pModalSheet->UpdateShapePreview();
		SetModified(); // Apply Now ボタンを有効にする
	}

	CModelessShapePropSheet* pModelessSheet = 
		DYNAMIC_DOWNCAST(CModelessShapePropSheet, pSheet);
	if (pModelessSheet != NULL)
	{
		UpdateData();
		CFrameWnd* pFrameWnd = STATIC_DOWNCAST(CFrameWnd, AfxGetMainWnd());
		CView* pView = pFrameWnd->GetActiveFrame()->GetActiveView();
		pView->SendMessage(WM_USER_CHANGE_OBJECT_PROPERTIES, 0, 0);
	}
}

BEGIN_MESSAGE_MAP(CColorPage, CPropertyPage)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BLACK, IDC_BLUE, CColorPage::OnColorClicked)
	//{{AFX_MSG_MAP(CColorPage)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorPage メッセージ ハンドラ
