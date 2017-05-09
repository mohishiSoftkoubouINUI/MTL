// propsht2.h : CModelessShapePropSheet クラスのインターフェース
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

class CModelessShapePropSheet;

class CModelessShapePropSheet : public CPropertySheet
{
public:
	DECLARE_DYNAMIC(CModelessShapePropSheet)
	CModelessShapePropSheet(CWnd* pWndParent);

// 属性
	CStylePage m_stylePage;
	CColorPage m_colorPage;

// オペレーション
	void SetSheetPropsFromShape(CShape* pShape);
	void SetShapePropsFromSheet(CShape* pShape);

// オーバーライド
	virtual void PostNcDestroy();

// メッセージ ハンドラ
protected:
	//{{AFX_MSG(CModelessShapePropSheet)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
