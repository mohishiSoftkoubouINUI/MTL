// stdafx.h : 標準のシステム インクルード ファイル、
//            または参照回数が多く、かつあまり変更されない
//            プロジェクト専用のインクルード ファイルを記述します。
//

#if !defined(AFX_STDAFX_H__0011D1B6_F5FC_4CCA_99C6_B040484BD461__INCLUDED_)
#define AFX_STDAFX_H__0011D1B6_F5FC_4CCA_99C6_B040484BD461__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します。


#ifndef _AFXDLL
#define _MTLMFC
#endif

#ifndef _MTLMFC

#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分
#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サポート
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#else

#define MTL_REMOVE_CWINAPP_PRINT
#define MTL_REMOVE_CWINAPP_PROFILEREGISTRY

#define MTL_REMOVE_CVIEW_PRINT

#define MTL_REMOVE_CDOCMANAGER_MULTITEMPLATE


#define _MTL_FXN_COMTATIBLE

#include <mtlmfc.h>

#endif

#endif // !defined(AFX_STDAFX_H__0011D1B6_F5FC_4CCA_99C6_B040484BD461__INCLUDED_)
