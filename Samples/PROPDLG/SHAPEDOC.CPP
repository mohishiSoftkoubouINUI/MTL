// shapedoc.cpp : CShapesDoc クラスのインプリメンテーション
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
#include "shapedoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShapesDoc

IMPLEMENT_DYNCREATE(CShapesDoc, CDocument)

BEGIN_MESSAGE_MAP(CShapesDoc, CDocument)
	//{{AFX_MSG_MAP(CShapesDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShapesDoc 構築/消滅

CShapesDoc::CShapesDoc()
{
}

CShapesDoc::~CShapesDoc()
{
	POSITION pos = m_shapeList.GetHeadPosition();
	while (pos != NULL)
	{
		delete m_shapeList.GetNext(pos);
	}
}

BOOL CShapesDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	CShape* pShapeNew = new CShape(
		black,
		rectangle,
		CRect(20,20,120,120));
	AddShape(pShapeNew);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CShapesDoc オペレーション

void CShapesDoc::AddShape(CShape* pShapeNew)
{
	m_shapeList.AddHead(pShapeNew);
	UpdateAllViews(NULL);
	SetModifiedFlag();
}

CShape* CShapesDoc::HitTest(CPoint pt)
{
	CShape* pShapeHit;
	POSITION pos = m_shapeList.GetHeadPosition();
	while (pos != NULL)
	{
		pShapeHit = m_shapeList.GetNext(pos);
		if (pShapeHit->m_rect.PtInRect(pt))
			return pShapeHit;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CShapesDoc シリアライズ機能

void CShapesDoc::Serialize(CArchive& ar)
{
	m_shapeList.Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CShapesDoc 診断機能

#ifdef _DEBUG
void CShapesDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CShapesDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CShapesDoc コマンド
