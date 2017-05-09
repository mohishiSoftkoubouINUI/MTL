// viewex.h : VIEWEX アプリケーション用メイン ヘッダー ファイル
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

#if !defined(__AFXWIN_H__) || !defined(__AFXEXT_H__)
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル

#include "maindoc.h"        // メイン ドキュメント データ
#include "simpvw.h"         // 単純な読み取り専用ビュー
#include "inputvw.h"        // 編集可能なビュー

/////////////////////////////////////////////////////////////////////////////
// CViewExApp:
// このクラスのインプリメンテーションについては viewex.cpp を参照してください。
//

class CViewExApp : public CWinApp
{
public:
	CViewExApp();

// オーバーライド
	virtual BOOL InitInstance();

// インプリメンテーション

	//{{AFX_MSG(CViewExApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
