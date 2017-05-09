// maindoc.h : CMainDoc クラスのインターフェイス
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

class CMainDoc : public CDocument
{
protected: // シリアライズ機能のみから作成します。
	CMainDoc();
	DECLARE_SERIAL(CMainDoc)

// アトリビュート
public:
	// ドキュメント特有のデータの例
	CString m_strData;          // ストリング テキスト
	COLORREF m_colorData;       // ストリング テキストの色

// インプリメンテーション
public:
	virtual ~CMainDoc();
	virtual void Serialize(CArchive& ar);   // ドキュメント I/O に対してオーバーライドされます。
protected:
	virtual BOOL    OnNewDocument();

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMainDoc)
	afx_msg void OnChangeData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
