// MdiVC6Doc.h : CMdiVC6Doc クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MDIVC6DOC_H__119C1417_B630_4364_AE02_9612562525B2__INCLUDED_)
#define AFX_MDIVC6DOC_H__119C1417_B630_4364_AE02_9612562525B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMdiVC6Doc : public CDocument
{
protected: // シリアライズ機能のみから作成します。
	CMdiVC6Doc();
	DECLARE_DYNCREATE(CMdiVC6Doc)

// アトリビュート
public:

// オペレーション
public:

//オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMdiVC6Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CMdiVC6Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMdiVC6Doc)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MDIVC6DOC_H__119C1417_B630_4364_AE02_9612562525B2__INCLUDED_)
