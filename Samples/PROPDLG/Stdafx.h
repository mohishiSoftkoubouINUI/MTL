// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// このソース コードは Microsoft Foundation Class リファレンスおよび、
// 関連するオンライン ドキュメントの補足のみを目的としています。
// Microsoft Foundation Class 製品に関する詳しい情報はこれらのドキュメント
// を参照してください。
//

// stdafx.h : 標準のシステム インクルード ファイル、
//            または参照回数が多く、かつあまり変更されない
//            プロジェクト専用のインクルード ファイルを記述します。

#define VC_EXTRALEAN

#ifndef _AFXDLL
#define _MTLMFC
#endif

#ifndef _MTLMFC

#include <afxwin.h>		 // MFC の標準コンポーネント
#include <afxext.h>         // MFC の拡張部分

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC の Windows 95 コモン コントロール サポート
#endif // _AFX_NO_AFXCMN_SUPPORT

#else

#define MTL_REMOVE_CWINAPP_PROFILEREGISTRY

#define _MTL_FXN_COMTATIBLE

#include <mtlmfc.h>

#endif
