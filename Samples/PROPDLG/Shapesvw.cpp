// shapesvw.cpp : CShapesView クラスのインプリメンテーション
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
#include "shapedoc.h"
#include "shapesvw.h"

#include "minifrm.h"
#include "mainfrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShapesView

IMPLEMENT_DYNCREATE(CShapesView, CView)

BEGIN_MESSAGE_MAP(CShapesView, CView)
	ON_MESSAGE(WM_USER_CHANGE_OBJECT_PROPERTIES, CShapesView::OnChangeObjectProps)
	//{{AFX_MSG_MAP(CShapesView)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_SIMPLE_PROPERTY_SHEET, CShapesView::OnSimplePropertySheet)
	ON_UPDATE_COMMAND_UI(ID_SIMPLE_PROPERTY_SHEET, CShapesView::OnUpdateSimplePropertySheet)
	ON_COMMAND(ID_PROPERTY_SHEET_WITH_PREVIEW, CShapesView::OnPropertySheetWithPreview)
	ON_WM_KEYDOWN()
	ON_UPDATE_COMMAND_UI(ID_PROPERTY_SHEET_WITH_PREVIEW, CShapesView::OnUpdatePropertySheetWithPreview)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShapesView 構築/消滅

CShapesView::CShapesView()
{
	m_pShapeSelected = NULL;
	m_pModalShapePropSheet = NULL;
}

CShapesView::~CShapesView()
{
}


/////////////////////////////////////////////////////////////////////////////
// オペレーション

CModelessShapePropSheet* CShapesView::GetModelessShapePropSheet()
{
	CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	if (pMainFrame->m_pShapePropFrame == NULL)
		return NULL;
	return pMainFrame->m_pShapePropFrame->m_pModelessShapePropSheet;
}

/////////////////////////////////////////////////////////////////////////////
// CShapesView 描画

void CShapesView::OnDraw(CDC* pDC)
{
	CShapesDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRect rectDirty, rectIntersect;
	pDC->GetClipBox(&rectDirty);
	if (rectDirty.IsRectNull())
		GetClientRect(&rectDirty);

	CShape* pShape;
	POSITION pos = pDoc->m_shapeList.GetTailPosition();
	// 奥のものから順に、現在選択されているもの以外のすべての
	// シェイプを描画する
	while (pos != NULL)
	{
		pShape = pDoc->m_shapeList.GetPrev(pos);
		if (rectIntersect.IntersectRect(&pShape->m_rect, &rectDirty) != 0
			&& pShape != m_pShapeSelected)
			pShape->Draw(pDC, FALSE);
	}
	// 選択されているシェイプを一番手前に描画する
	if (m_pShapeSelected != NULL)
		m_pShapeSelected->Draw(pDC, TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CShapesView 印刷

BOOL CShapesView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// デフォルトの準備
	return DoPreparePrinting(pInfo);
}

void CShapesView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CShapesView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CShapesView 診断機能

#ifdef _DEBUG
void CShapesView::AssertValid() const
{
	CView::AssertValid();
}

void CShapesView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CShapesDoc* CShapesView::GetDocument() // デバッグバージョンでないものはインライン
{
	ASSERT_KINDOF(CShapesDoc, m_pDocument);
	return (CShapesDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CShapesView クラスのメッセージハンドラ

void CShapesView::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{
	CShapesDoc* pDoc = GetDocument();
	CModelessShapePropSheet* pModelessShapePropSheet
		= GetModelessShapePropSheet();

	CClientDC dc(this);

	CShape* pShapeHit;
	pShapeHit = pDoc->HitTest(point);
	if (pShapeHit != NULL)
	{   // クリックされたオブジェクトを選択する

		CRect rectInvalid;
		// 以前に選択されていたオブジェクトの表示を無効にする、
		// 選択されていないオブジェクトとして再描画される
		if (m_pShapeSelected != NULL)
			InvalidateRect(&m_pShapeSelected->m_rect);
		m_pShapeSelected = pShapeHit;

		// 新たに選択されたオブジェクトの表示を無効にする、
		// 選択されているオブジェクトとして再描画される
		InvalidateRect(&m_pShapeSelected->m_rect);

		// モードレスプロパティシートが表示されている場合は
		// 新たに選択されたシェイプを反映して設定を更新する
		if (pModelessShapePropSheet != NULL
			&& pModelessShapePropSheet->IsWindowVisible())
		{
		   pModelessShapePropSheet->
			   SetSheetPropsFromShape(m_pShapeSelected);
		}
	}
	else
	{   // 新たにシェイプを追加する

		CShape* pShapeNew = new CShape(
			black,
			rectangle,
			CRect(point.x-50, point.y-50, point.x + 50, point.y + 50));
		pDoc->AddShape(pShapeNew);
		m_pShapeSelected = pShapeNew;
		// モードレスプロパティシートが存在する場合は、現在の設定を
		// 新しいシェイプに適用する
		if (pModelessShapePropSheet != NULL)
		{
		   pModelessShapePropSheet->
			   SetShapePropsFromSheet(m_pShapeSelected);
		}
	}
}

void CShapesView::OnLButtonDblClk(UINT /*nFlags*/, CPoint /*point*/)
{
	// どこでもダブルクリックすると、モードレスプロパティシートが表示される
	CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	pMainFrame->OnMiniFramePropertySheet();
}


void CShapesView::OnSimplePropertySheet()
{
	if (m_pShapeSelected == NULL)
		return;

	CPropertySheet dlgPropertySheet(AFX_IDS_APP_TITLE);
	CStylePage stylePage;
	CColorPage colorPage;

	stylePage.m_nShapeStyle = m_pShapeSelected->m_shapestyle;
	colorPage.m_nColor = m_pShapeSelected->m_shapecolor;

	dlgPropertySheet.AddPage(&stylePage);
	dlgPropertySheet.AddPage(&colorPage);

	if (dlgPropertySheet.DoModal() == IDOK)
	{
		m_pShapeSelected->m_shapecolor = colorPage.m_nColor;
		m_pShapeSelected->m_shapestyle
			= (SHAPE_STYLE)stylePage.m_nShapeStyle;
		GetDocument()->SetModifiedFlag();
		GetDocument()->UpdateAllViews(NULL);
	}
}

void CShapesView::OnUpdateSimplePropertySheet(CCmdUI* pCmdUI)
{
	// プレビュー付きのプロパティシートの機能を利用する
	OnUpdatePropertySheetWithPreview(pCmdUI);
}

void CShapesView::OnPropertySheetWithPreview()
{
	if (m_pShapeSelected == NULL)
		return;

	// モーダルプロパティシートへのポインタを保存するので、
	// ユーザがApply Now を選択し CModalShapePropSheet が
	// WM_USER_CHANGE_OBJECT_PROPERTIES メッセージをビュー
	// に送ったとき、ビューは現在の設定を取得し選択されている
	// オブジェクトに設定を適用するために、簡単にプロパティ
	// シートを参照できる
	m_pModalShapePropSheet = new CModalShapePropSheet;

	// CModalShapePropSheet::SetSheetPropsFromShape は
	// ユーザが Apply Now を選択たときに呼ばれるユーザ
	// 定義メッセージ WM_USER_CHANGE_OBJECT_PROPS のハ
	// ンドラから呼び出されるように設計されているため
	// ここでは使えません。モーダルプロパティシートはこ
	// こではまだ作成されていません。SetSheetPropsFromShape
	// を呼び出すとウィンドウを更新しようとして失敗します。
	m_pModalShapePropSheet->m_stylePage.m_nShapeStyle
		= m_pShapeSelected->m_shapestyle;
	m_pModalShapePropSheet->m_colorPage.m_nColor = m_pShapeSelected->m_shapecolor;

	if (m_pModalShapePropSheet->DoModal() == IDOK)
	{
		m_pModalShapePropSheet->SetShapePropsFromSheet(m_pShapeSelected);
		GetDocument()->SetModifiedFlag();
		GetDocument()->UpdateAllViews(NULL);
	}
	delete m_pModalShapePropSheet;
	m_pModalShapePropSheet = NULL;
}

void CShapesView::OnUpdatePropertySheetWithPreview(CCmdUI* pCmdUI)
{
	BOOL bEnable = TRUE;
	if (m_pShapeSelected == NULL)
	{
		bEnable = FALSE;
	}
	else
	{
		CModelessShapePropSheet* pModelessShapePropSheet
			= GetModelessShapePropSheet();
		if (pModelessShapePropSheet != NULL
			&& pModelessShapePropSheet->IsWindowVisible())
		{
			// モードレスプロパティシートがすでに表示されている場合
			// はモーダルプロパティシートを表示してはいけない
			bEnable = FALSE;
		}
	}
	pCmdUI->Enable(bEnable);
}

LRESULT CShapesView::OnChangeObjectProps(WPARAM, LPARAM)
{
	// ユーザが Apply Now を選択したときはモーダルな CModalShapePropSheet
	// から、また、ユーザがプロパティページで何か設定を変更したときには
	// モードレスな CModelessShapePropSheet から、ユーザ定義メッセージ
	// WM_USER_CHANGE_OBJECT_PROPS が送られます

	if (m_pShapeSelected == NULL)
		return 0;

	CModelessShapePropSheet* pModelessShapePropSheet
		= GetModelessShapePropSheet();
	if (pModelessShapePropSheet != NULL)
		pModelessShapePropSheet->SetShapePropsFromSheet(m_pShapeSelected);

	if (m_pModalShapePropSheet != NULL)
		m_pModalShapePropSheet->SetShapePropsFromSheet(m_pShapeSelected);

	GetDocument()->SetModifiedFlag();
	GetDocument()->UpdateAllViews(NULL);
	return 0;
}

void CShapesView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_ESCAPE)
	{
		CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
		pMainFrame->HideModelessPropSheet();
		return;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
