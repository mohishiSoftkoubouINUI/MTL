// propsht.h : CModalShapePropSheet クラスのインターフェース
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

class CModalShapePropSheet : public CPropertySheet
{
public:
	DECLARE_DYNAMIC(CModalShapePropSheet)
	CModalShapePropSheet(CWnd* pWndParent = NULL);

// 属性
	CStylePage m_stylePage;
	CColorPage m_colorPage;
	CShapePreviewWnd m_wndPreview;

// オペレーション
	void SetSheetPropsFromShape(CShape* pShape);
	void SetShapePropsFromSheet(CShape* pShape);
	void UpdateShapePreview();

// オーバーライド
	virtual BOOL OnInitDialog();

// メッセージ ハンドラ
protected:
	//{{AFX_MSG(CModalShapePropSheet)
	afx_msg void OnApplyNow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
