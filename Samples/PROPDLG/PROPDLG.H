// propdlg.h : PROPDLG �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// ���̃\�[�X �R�[�h�� Microsoft Foundation Class ���t�@�����X����сA
// �֘A����I�����C�� �h�L�������g�̕⑫�݂̂�ړI�Ƃ��Ă��܂��B
// Microsoft Foundation Class ���i�Ɋւ���ڂ������͂����̃h�L�������g
// ���Q�Ƃ��Ă��������B
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // ���C�� �V���{��

#define WM_USER_CHANGE_OBJECT_PROPERTIES (WM_USER + 1)

/////////////////////////////////////////////////////////////////////////////
// CPropDlgApp:
// ���̃N���X�̃C���v�������e�[�V������ propdlg.cpp ���Q�Ƃ��Ă�������
//

class CPropDlgApp : public CWinApp
{
public:
	CPropDlgApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CPropDlgApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CPropDlgApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////