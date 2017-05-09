// MdiVC6.h : MDIVC6 アプリケーションのメイン ヘッダー ファイル
//

#if !defined(AFX_MDIVC6_H__ED8CB2EF_1EA6_4103_9214_6AF162D2BB97__INCLUDED_)
#define AFX_MDIVC6_H__ED8CB2EF_1EA6_4103_9214_6AF162D2BB97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CMdiVC6App:
// このクラスの動作の定義に関しては MdiVC6.cpp ファイルを参照してください。
//

class CMdiVC6App : public CWinApp
{
public:
	CMdiVC6App();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMdiVC6App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション
	//{{AFX_MSG(CMdiVC6App)
	afx_msg void OnAppAbout();
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MDIVC6_H__ED8CB2EF_1EA6_4103_9214_6AF162D2BB97__INCLUDED_)
