// maindoc.cpp : CMainDoc クラスのインプリメンテーション
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

#include "enterdlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainDoc

IMPLEMENT_SERIAL(CMainDoc, CDocument, 0 /* schema number*/ )

BEGIN_MESSAGE_MAP(CMainDoc, CDocument)
	//{{AFX_MSG_MAP(CMainDoc)
	ON_COMMAND(IDM_CHANGEDATA, CMainDoc::OnChangeData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDoc クラスの構築/消滅

CMainDoc::CMainDoc()
{
}

CMainDoc::~CMainDoc()
{
}

BOOL CMainDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	m_strData = "Sample Data String";
	m_colorData = RGB(0, 0, 0);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainDoc シリアライゼーション

void CMainDoc::Serialize(CArchive&)
{
	ASSERT(FALSE);      // このプログラム例ではデータを保存しません。
}

/////////////////////////////////////////////////////////////////////////////
// CMainDoc コマンド

void CMainDoc::OnChangeData()
{
	CEnterDlg dlg;
	dlg.m_strInput = m_strData;
	if (dlg.DoModal() != IDOK)
		return;
	m_strData = dlg.m_strInput;
	// このドキュメントがデータを保存した場合、この位置で SetModifiedFlag を呼び出します。
	UpdateAllViews(NULL);   // 全体の更新
}

/////////////////////////////////////////////////////////////////////////////
