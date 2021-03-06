// shapeobj.h : CShape クラスのインターフェース
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

enum SHAPE_STYLE
{
	rectangle,
	rounded_rectangle,
	ellipse,
};

enum SHAPE_COLOR_ENUM
{
	black,
	red,
	green,
	blue,
};

struct SHAPE_COLOR
{
	SHAPE_COLOR_ENUM e;
	operator COLORREF() const;
	const SHAPE_COLOR& operator = (COLORREF colorref);
	operator int() const;
	const SHAPE_COLOR& operator = (int n);
};

class CShape : public CObject
{
public:
	CShape();
	CShape(SHAPE_COLOR_ENUM colorenum, SHAPE_STYLE shapestyle, CRect& rect);
	DECLARE_SERIAL(CShape)

	SHAPE_COLOR m_shapecolor;
	SHAPE_STYLE m_shapestyle;
	CRect       m_rect;

	void Draw(CDC* pDC, BOOL bSelected);
	virtual void Serialize(CArchive& ar);
};
