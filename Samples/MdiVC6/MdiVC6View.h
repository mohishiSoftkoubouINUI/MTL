// MdiVC6View.h : CMdiVC6View クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MDIVC6VIEW_H__0B172281_FCDD_440F_A718_1031744E3964__INCLUDED_)
#define AFX_MDIVC6VIEW_H__0B172281_FCDD_440F_A718_1031744E3964__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMdiVC6View : public CView
{
protected: // シリアライズ機能のみから作成します。
	CMdiVC6View();
	DECLARE_DYNCREATE(CMdiVC6View)

// アトリビュート
public:
	CMdiVC6Doc* GetDocument();

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMdiVC6View)
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CMdiVC6View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMdiVC6View)
		// メモ -  ClassWizard はこの位置にメンバ関数を追加または削除します。
		//         この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // MdiVC6View.cpp ファイルがデバッグ環境の時使用されます。
inline CMdiVC6Doc* CMdiVC6View::GetDocument()
   { return (CMdiVC6Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MDIVC6VIEW_H__0B172281_FCDD_440F_A718_1031744E3964__INCLUDED_)
