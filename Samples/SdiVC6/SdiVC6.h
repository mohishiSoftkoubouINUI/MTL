// SdiVC6.h : SDIVC6 �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//

#if !defined(AFX_SDIVC6_H__55787D5C_6349_4F3F_93BE_87D32B219635__INCLUDED_)
#define AFX_SDIVC6_H__55787D5C_6349_4F3F_93BE_87D32B219635__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CSdiVC6App:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� SdiVC6.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CSdiVC6App : public CWinApp
{
public:
	CSdiVC6App();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSdiVC6App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
	//{{AFX_MSG(CSdiVC6App)
	afx_msg void OnAppAbout();
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SDIVC6_H__55787D5C_6349_4F3F_93BE_87D32B219635__INCLUDED_)
