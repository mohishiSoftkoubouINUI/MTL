// FrmVC6.h : FRMVC6 アプリケーションのメイン ヘッダー ファイル
//

#if !defined(AFX_FRMVC6_H__B6E9F609_8A4D_4F57_B536_7FC85DEDC66C__INCLUDED_)
#define AFX_FRMVC6_H__B6E9F609_8A4D_4F57_B536_7FC85DEDC66C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CFrmVC6App:
// このクラスの動作の定義に関しては FrmVC6.cpp ファイルを参照してください。
//

class CFrmVC6App : public CWinApp
{
public:
	CFrmVC6App();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CFrmVC6App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

public:
	//{{AFX_MSG(CFrmVC6App)
	afx_msg void OnAppAbout();
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_FRMVC6_H__B6E9F609_8A4D_4F57_B536_7FC85DEDC66C__INCLUDED_)
