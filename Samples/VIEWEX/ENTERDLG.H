// enterdlg.h : ヘッダー ファイル
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

/////////////////////////////////////////////////////////////////////////////
// CEnterDlg ダイアログ

class CEnterDlg : public CDialog
{
	DECLARE_DYNAMIC(CEnterDlg)
// 構築
public:
	CEnterDlg(CWnd* pParent = NULL);    // 標準のコンストラタ

// ダイアログ データ
	//{{AFX_DATA(CEnterDlg)
	enum { IDD = IDD_CHANGEDATA };
	CString m_strInput;
	//}}AFX_DATA

// インプリメンテーション
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEnterDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
