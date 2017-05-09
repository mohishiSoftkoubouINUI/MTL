// SdiVC6.h : SDIVC6 アプリケーションのメイン ヘッダー ファイル
//

#if !defined(AFX_SDIVC6_H__55787D5C_6349_4F3F_93BE_87D32B219635__INCLUDED_)
#define AFX_SDIVC6_H__55787D5C_6349_4F3F_93BE_87D32B219635__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CSdiVC6App:
// このクラスの動作の定義に関しては SdiVC6.cpp ファイルを参照してください。
//

class CSdiVC6App : public CWinApp
{
public:
	CSdiVC6App();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSdiVC6App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション
	//{{AFX_MSG(CSdiVC6App)
	afx_msg void OnAppAbout();
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SDIVC6_H__55787D5C_6349_4F3F_93BE_87D32B219635__INCLUDED_)
