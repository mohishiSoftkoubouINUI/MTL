// minifrm.h : CShapePropSheetFrame クラスのインターフェース
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

class CShapePropSheetFrame : public CMiniFrameWnd
{
// コンストラクタ
public:
	CShapePropSheetFrame();

// 属性
public:
	CModelessShapePropSheet* m_pModelessShapePropSheet;

// ハンドラ
protected:
	//{{AFX_MSG(CShapePropSheetFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
