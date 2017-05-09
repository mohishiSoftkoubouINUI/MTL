// DlgVC6.h : DLGVC6 アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_DLGVC6_H__DAA79090_8D23_47F9_A5B2_A920C7C45B0B__INCLUDED_)
#define AFX_DLGVC6_H__DAA79090_8D23_47F9_A5B2_A920C7C45B0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CDlgVC6App:
// このクラスの動作の定義に関しては DlgVC6.cpp ファイルを参照してください。
//

class CDlgVC6App : public CWinApp
{
public:
	CDlgVC6App();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgVC6App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CDlgVC6App)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGVC6_H__DAA79090_8D23_47F9_A5B2_A920C7C45B0B__INCLUDED_)
