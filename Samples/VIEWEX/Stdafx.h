// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// このソース コードは Microsoft Foundation Class リファレンスおよび、
// 関連するオンライン ドキュメントの補足のみを目的としています。
// Microsoft Foundation Class 製品に関する詳しい情報はこれらのドキュメント
// を参照してください。
//

#define VC_EXTRALEAN

#ifndef _AFXDLL
#define _MTLMFC
#endif

#ifndef _MTLMFC


#include <afxwin.h>
#include <afxext.h>         // MFC の拡張部分 例、CFormView、CSplitterWnd

#else

#define MTL_REMOVE_CWINAPP_PRINT
#define MTL_REMOVE_CWINAPP_RECENTFILELIST

#define MTL_REMOVE_CFRAMEWND_CONTROLBAR
#define MTL_REMOVE_CFRAMEWND_TOOLTIPTEXT
#define MTL_REMOVE_CFRAMEWND_INDICATOR

#define MTL_REMOVE_CVIEW_PRINT


#define _MTL_FXN_COMTATIBLE

#include <mtlmfc.h>

#endif
