// preview.cpp : CModalShapePropSheet クラスのインプリメンテーション
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
#include "preview.h"
#include "resource.h"
#include "colorpge.h"
#include "stylepge.h"
#include "shapeobj.h"
#include "propsht.h"

BEGIN_MESSAGE_MAP(CShapePreviewWnd, CWnd)
//{{AFX_MSG_MAP(CShapePreviewWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CShapePreviewWnd::CShapePreviewWnd()
{
}

void CShapePreviewWnd::OnPaint()
{
	CPaintDC dc(this);
	CModalShapePropSheet* pShapePropSheet = 
		STATIC_DOWNCAST(CModalShapePropSheet, GetParent());
	CRect rect;
	GetClientRect(rect);
	CShape shape(
		(SHAPE_COLOR_ENUM)pShapePropSheet->m_colorPage.m_nColor,
		(SHAPE_STYLE)pShapePropSheet->m_stylePage.m_nShapeStyle,
		rect);
	shape.Draw(&dc, FALSE);
}

BOOL CShapePreviewWnd::OnEraseBkgnd(CDC* pDC)
{
	// 背景色をダイアログと同じにする
	// （プロパティ シート）

	CWnd* pParentWnd = GetParent();
	HBRUSH hBrush = (HBRUSH)pParentWnd->SendMessage(WM_CTLCOLORDLG,
		(WPARAM)pDC->m_hDC, (LPARAM)pParentWnd->m_hWnd);
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, CBrush::FromHandle(hBrush));
	return TRUE;
}
