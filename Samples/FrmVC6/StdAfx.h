// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���A
//            �܂��͎Q�Ɖ񐔂������A�����܂�ύX����Ȃ�
//            �v���W�F�N�g��p�̃C���N���[�h �t�@�C�����L�q���܂��B
//

#if !defined(AFX_STDAFX_H__DB78EE99_872E_46A5_8889_5089B3EC9848__INCLUDED_)
#define AFX_STDAFX_H__DB78EE99_872E_46A5_8889_5089B3EC9848__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Windows �w�b�_�[����w�ǎg�p����Ȃ��X�^�b�t�����O���܂��B

#ifndef _AFXDLL
#define _MTLMFC
#endif

#ifndef _MTLMFC

#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
#include <afxext.h>         // MFC �̊g������
#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �R���� �R���g���[�� �T�|�[�g
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �R���� �R���g���[�� �T�|�[�g
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#else

#define MTL_REMOVE_CWINAPP_DOCMANAGER
#define MTL_REMOVE_CWINAPP_RECENTFILELIST
#define MTL_REMOVE_CWINAPP_PRINT

#define MTL_REMOVE_CFRAMEWND_INDICATOR
#define MTL_REMOVE_CFRAMEWND_VIEW


#define _MTL_FXN_COMTATIBLE

#include <mtlmfc.h>

#endif

#endif // !defined(AFX_STDAFX_H__DB78EE99_872E_46A5_8889_5089B3EC9848__INCLUDED_)
