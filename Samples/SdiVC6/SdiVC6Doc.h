// SdiVC6Doc.h : CSdiVC6Doc クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SDIVC6DOC_H__D485EB53_DCE7_4C88_9BF8_690D6095C5DC__INCLUDED_)
#define AFX_SDIVC6DOC_H__D485EB53_DCE7_4C88_9BF8_690D6095C5DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSdiVC6Doc : public CDocument
{
protected: // シリアライズ機能のみから作成します。
	CSdiVC6Doc();
	DECLARE_DYNCREATE(CSdiVC6Doc)

// アトリビュート
public:

// オペレーション
public:

//オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSdiVC6Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CSdiVC6Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CSdiVC6Doc)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SDIVC6DOC_H__D485EB53_DCE7_4C88_9BF8_690D6095C5DC__INCLUDED_)
