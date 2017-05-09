// colorpge.h : ヘッダー ファイル
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
// CColorPage ダイアログ

class CColorPage : public CPropertyPage
{
// 構築
public:
	CColorPage();

// ダイアログ データ
	//{{AFX_DATA(CColorPage)
	enum { IDD = IDD_COLOR };
	int     m_nColor;
	//}}AFX_DATA


//オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CColorPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	void OnColorClicked(UINT nCmdID);
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CColorPage)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
