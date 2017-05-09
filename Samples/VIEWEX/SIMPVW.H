// simpvw.h : シンプルなビュー クラスのインターフェイス
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

// CTextView - テキスト出力
// CColorView - カラー出力

/////////////////////////////////////////////////////////////////////////////

class CTextView : public CView
{
protected: // シリアライズ機能のみから作成します。
	CTextView();
	DECLARE_DYNCREATE(CTextView)

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
public:
	virtual ~CTextView();
	virtual void OnDraw(CDC* pDC);      // このビューを描画する際にオーバーライドされます。

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CTextView)
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

class CColorView : public CView
{
protected: // シリアライズ機能のみから作成します。
	CColorView();
	DECLARE_DYNCREATE(CColorView)

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
public:
	virtual ~CColorView();
	virtual void OnDraw(CDC* pDC);      // このビューを描画する際にオーバーライドされます。
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView,
					CView* pDeactiveView);

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CColorView)
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
