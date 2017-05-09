// inputvw.h : ヘッダー ファイル
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
// CInputView フォーム ビュー

class CInputView : public CFormView
{
	DECLARE_DYNCREATE(CInputView)
protected:
	CInputView();           // 動的作成で使用されるプロテクト コンストラクタ

// フォーム データ
public:
	//{{AFX_DATA(CInputView)
	enum { IDD = IDD_INPUTFORM };
	CString m_strData;
	int     m_iColor;
	//}}AFX_DATA

// アトリビュート
public:
	CMainDoc* GetDocument()
			{
				ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMainDoc)));
				return (CMainDoc*) m_pDocument;
			}

// オペレーション
public:

// インプリメンテーション
protected:
	virtual ~CInputView();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CInputView)
	afx_msg void OnDataChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
