// viewex.cpp : �A�v���P�[�V�����p�ɃN���X�̓���̒�`���s���܂��B
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

#include "stdafx.h"
#include "viewex.h"

#include "splitter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewExApp

BEGIN_MESSAGE_MAP(CViewExApp, CWinApp)
	//{{AFX_MSG_MAP(CViewExApp)
	ON_COMMAND(ID_APP_ABOUT, CViewExApp::OnAppAbout)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewExApp �N���X�̍\�z
// ���̈ʒu�� InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B

CViewExApp::CViewExApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CViewExApp �I�u�W�F�N�g

CViewExApp NEAR theApp;

/////////////////////////////////////////////////////////////////////////////
// CViewExApp �N���X�̏�����

BOOL CViewExApp::InitInstance()
{
	// �W���I�ȏ���������
	Enable3dControls();

	// ���� VIEWEX �T���v���ł� CView �h���N���X�ɑ΂��邢�����̃e�N�j�b�N��
	// �Љ�Ă��܂��B  ���̃T���v�����̑S�Ẵr���[�͓��� CMainDoc �N���X��
	// �g�p���Ă��܂��B  �ȒP�ɐ�������� CMainDoc �N���X�̓V���A���C�[�[�V����
	// ���T�|�[�g���Ă��܂��񂪁A�V���O�� �X�g�����O����� RGB �J���[�̔���
	// �P���ȃf�[�^ ���f���������Ă��܂��B�قȂ�h�L�������g �e���v���[�g��
	// �e�����[���r���[ �^�C�v�ɑ΂��ēo�^���܂��B

	// �ȒP�ȃe�L�X�g�o�̓r���[
	AddDocTemplate(new CMultiDocTemplate(IDR_TEXTTYPE,
			RUNTIME_CLASS(CMainDoc),
			RUNTIME_CLASS(CMDIChildWnd),
			RUNTIME_CLASS(CTextView)));

	// �ȒP�ȃJ���[�o�̓r���[
	//  'fileNewName' �Ȃ��̃h�L�������g �e���v���[�g�Ȃ̂� FileNew �̑I���͂���܂���B
	AddDocTemplate(new CMultiDocTemplate(IDR_COLORTYPE,
			RUNTIME_CLASS(CMainDoc),
			RUNTIME_CLASS(CMDIChildWnd),
			RUNTIME_CLASS(CColorView)));

	// ���͕t���̃t�H�[�� �r���[
	AddDocTemplate(new CMultiDocTemplate(IDR_INPUTTYPE,
			RUNTIME_CLASS(CMainDoc),
			RUNTIME_CLASS(CMDIChildWnd),
			RUNTIME_CLASS(CInputView)));

	// �ȒP�ȃe�L�X�g�o�͂���уt�H�[�����̓r���[�t���X�v���b�^�[ �t���[��
	AddDocTemplate(new CMultiDocTemplate(IDR_SPLIT2TYPE,
			RUNTIME_CLASS(CMainDoc),
			RUNTIME_CLASS(CSplitterFrame),
			RUNTIME_CLASS(CTextView)));

	// �t�H�[�����́A�e�L�X�g�o�́A�J���[�o�̓r���[�� 3 �ʂ�̃X�v���b�^�[ �t���[��
	AddDocTemplate(new CMultiDocTemplate(IDR_SPLIT3TYPE,
			RUNTIME_CLASS(CMainDoc),
			RUNTIME_CLASS(C3WaySplitterFrame),
			RUNTIME_CLASS(CInputView)));

	// ���C�� MDI �t���[�� �E�B���h�E�̍쐬
	// �c�[���o�[�A�X�e�[�^�X �o�[�܂��͂��̑����ʂȋ@�\�������Ȃ��ȒP�� MDI
	// �t���[�� �E�B���h�E�ɑ΂��āACMDIFrameWnd �N���X�́ACMDIChildWnd ��
	// �h�L�������g �t���[�� �E�B���h�E�ɑ΂��Ďg�p�����悤�ɁA���C�� �t���[��
	// �E�B���h�E�ɑ΂��Ē��ڎg�p����܂��B

	CMDIFrameWnd* pMainFrame = new CMDIFrameWnd;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	// ���̗�ł͂܂��A�S�Ẵr���[�ŋ��L����郁�j���[�o�[�� 1 ��������܂��B
	// MFC �̎������j���[�T�|�[�g�́A���݃A�N�e�B�u�ȃr���[�Ɋ�Â��K�p���Ȃ�
	// ���j���[ �A�C�e�����g�p�s�\�ɂ��܂��B
	// 1 �����̃��j���[�o�[�́A�J���Ă���h�L�������g�������ꍇ���܂߂āA����
	// �Ẵh�L�������g �^�C�v�ɑ΂��Ďg�p����܂��B

	// ���C�� ���j���[���ŏI�I�ɕ\�����܂�
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	m_pMainWnd = pMainFrame;

	// �R�}���h ���C�������͖�������܂��A�V����(���)�h�L�������g���쐬���܂��B
	OnFileNew();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CViewExApp �R�}���h

void CViewExApp::OnAppAbout()
{
	CDialog(IDD_ABOUTBOX).DoModal();
}

/////////////////////////////////////////////////////////////////////////////
