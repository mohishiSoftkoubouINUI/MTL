// SdiVC6View.h : CSdiVC6View クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SDIVC6VIEW_H__8F5E4407_7295_41F4_AAAB_DCCB4E00A88B__INCLUDED_)
#define AFX_SDIVC6VIEW_H__8F5E4407_7295_41F4_AAAB_DCCB4E00A88B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSdiVC6View : public CView
{
protected: // シリアライズ機能のみから作成します。
	CSdiVC6View();
	DECLARE_DYNCREATE(CSdiVC6View)

// アトリビュート
public:
	CSdiVC6Doc* GetDocument();

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSdiVC6View)
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CSdiVC6View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CSdiVC6View)
		// メモ -  ClassWizard はこの位置にメンバ関数を追加または削除します。
		//         この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // SdiVC6View.cpp ファイルがデバッグ環境の時使用されます。
inline CSdiVC6Doc* CSdiVC6View::GetDocument()
   { return (CSdiVC6Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SDIVC6VIEW_H__8F5E4407_7295_41F4_AAAB_DCCB4E00A88B__INCLUDED_)
