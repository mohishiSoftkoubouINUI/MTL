// DlgVC6.h : DLGVC6 �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_DLGVC6_H__DAA79090_8D23_47F9_A5B2_A920C7C45B0B__INCLUDED_)
#define AFX_DLGVC6_H__DAA79090_8D23_47F9_A5B2_A920C7C45B0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CDlgVC6App:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� DlgVC6.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CDlgVC6App : public CWinApp
{
public:
	CDlgVC6App();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDlgVC6App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CDlgVC6App)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DLGVC6_H__DAA79090_8D23_47F9_A5B2_A920C7C45B0B__INCLUDED_)
