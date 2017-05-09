// DlgVC6Dlg.h : ヘッダー ファイル
//

#if !defined(AFX_DLGVC6DLG_H__3602826F_338F_4287_91A7_35D86C36818E__INCLUDED_)
#define AFX_DLGVC6DLG_H__3602826F_338F_4287_91A7_35D86C36818E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDlgVC6Dlg ダイアログ

class CDlgVC6Dlg : public CDialog
{
// 構築
public:
	CDlgVC6Dlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgVC6Dlg)
	enum { IDD = IDD_DLGVC6_DIALOG };
		// メモ: この位置に ClassWizard によってデータ メンバが追加されます。
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgVC6Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgVC6Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGVC6DLG_H__3602826F_338F_4287_91A7_35D86C36818E__INCLUDED_)
