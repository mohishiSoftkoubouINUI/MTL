// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// ���̃\�[�X �R�[�h�� Microsoft Foundation Class ���t�@�����X����сA
// �֘A����I�����C�� �h�L�������g�̕⑫�݂̂�ړI�Ƃ��Ă��܂��B
// Microsoft Foundation Class ���i�Ɋւ���ڂ������͂����̃h�L�������g
// ���Q�Ƃ��Ă��������B
//

#define VC_EXTRALEAN

#ifndef _AFXDLL
#define _MTLMFC
#endif

#ifndef _MTLMFC


#include <afxwin.h>
#include <afxext.h>         // MFC �̊g������ ��ACFormView�ACSplitterWnd

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
