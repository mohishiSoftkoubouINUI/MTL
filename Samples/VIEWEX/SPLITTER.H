// splitter.h : カスタム スプリッター コントロールおよびそれを含むフレーム
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
// CSplitterFrame スプリッター/ワイパー 付きフレーム

class CSplitterFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CSplitterFrame)
protected:
	CSplitterFrame();   // 動的作成で使用されるプロテクト コンストラクタ

// アトリビュート
protected:
	CSplitterWnd m_wndSplitter;

// インプリメンテーション
public:
	virtual ~CSplitterFrame();
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSplitterFrame)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// 

class CViewExSplitWnd : public CSplitterWnd
{
	DECLARE_DYNAMIC(CViewExSplitWnd)

// インプリメンテーション
public:
	CViewExSplitWnd();
	~CViewExSplitWnd();
	CWnd* GetActivePane(int* pRow = NULL, int* pCol = NULL);
};


/////////////////////////////////////////////////////////////////////////////
//

class C3WaySplitterFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(C3WaySplitterFrame)
protected:
	C3WaySplitterFrame();   // 動的作成で使用されるプロテクト コンストラクタ

// アトリビュート
protected:
	CViewExSplitWnd m_wndSplitter;
	CViewExSplitWnd m_wndSplitter2;        // 1 番目に埋め込まれます

// インプリメンテーション
public:
	virtual ~C3WaySplitterFrame();
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(C3WaySplitterFrame)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
