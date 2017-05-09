// shapesdoc.h : CShapesDoc クラスのインターフェース
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

#ifndef _MTLMFC
#include <afxtempl.h>
#endif

class CShapesDoc : public CDocument
{
protected: // シリアライズ機能のみから作成します。
	CShapesDoc();
	DECLARE_DYNCREATE(CShapesDoc)

// アトリビュート
public:
	CTypedPtrList<CObList, CShape*> m_shapeList;

// オペレーション
public:
	void AddShape(CShape* pShapeNew);
	CShape* HitTest(CPoint pt);

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CShapesDoc)
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CShapesDoc();
	virtual void Serialize(CArchive& ar);   // ドキュメントの入出力のためにオーバーライド
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CShapesDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
