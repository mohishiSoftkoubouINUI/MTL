// viewex.h : VIEWEX �A�v���P�[�V�����p���C�� �w�b�_�[ �t�@�C��
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

#if !defined(__AFXWIN_H__) || !defined(__AFXEXT_H__)
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // ���C�� �V���{��

#include "maindoc.h"        // ���C�� �h�L�������g �f�[�^
#include "simpvw.h"         // �P���ȓǂݎ���p�r���[
#include "inputvw.h"        // �ҏW�\�ȃr���[

/////////////////////////////////////////////////////////////////////////////
// CViewExApp:
// ���̃N���X�̃C���v�������e�[�V�����ɂ��Ă� viewex.cpp ���Q�Ƃ��Ă��������B
//

class CViewExApp : public CWinApp
{
public:
	CViewExApp();

// �I�[�o�[���C�h
	virtual BOOL InitInstance();

// �C���v�������e�[�V����

	//{{AFX_MSG(CViewExApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////