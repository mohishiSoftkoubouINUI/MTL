// FrmVC6.h : FRMVC6 �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//

#if !defined(AFX_FRMVC6_H__B6E9F609_8A4D_4F57_B536_7FC85DEDC66C__INCLUDED_)
#define AFX_FRMVC6_H__B6E9F609_8A4D_4F57_B536_7FC85DEDC66C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CFrmVC6App:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� FrmVC6.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CFrmVC6App : public CWinApp
{
public:
	CFrmVC6App();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CFrmVC6App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

public:
	//{{AFX_MSG(CFrmVC6App)
	afx_msg void OnAppAbout();
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_FRMVC6_H__B6E9F609_8A4D_4F57_B536_7FC85DEDC66C__INCLUDED_)
