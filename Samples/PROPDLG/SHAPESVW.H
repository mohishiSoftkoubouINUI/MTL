// shapesvw.h : CShapesView クラスのインターフェース
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

class CShapesView : public CView
{
protected: // シリアライズ機能のみから作成します。
	CShapesView();
	DECLARE_DYNCREATE(CShapesView)

// アトリビュート
public:
	CShapesDoc* GetDocument();
	CShape* m_pShapeSelected;
protected:
	CModalShapePropSheet* m_pModalShapePropSheet;


// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CShapesView)
	public:
	virtual void OnDraw(CDC* pDC);  // ビューの描画のためにオーバーライド
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	CModelessShapePropSheet* GetModelessShapePropSheet();

	virtual ~CShapesView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CShapesView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSimplePropertySheet();
	afx_msg void OnUpdateSimplePropertySheet(CCmdUI* pCmdUI);
	afx_msg void OnPropertySheetWithPreview();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnUpdatePropertySheetWithPreview(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
   afx_msg LRESULT OnChangeObjectProps(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // デバッグバージョンは shapesvw.cpp で定義されている
inline CShapesDoc* CShapesView::GetDocument()
   { return (CShapesDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
