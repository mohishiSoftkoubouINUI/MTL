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

#include "shapeobj.h"
#include "resource.h"
#include "colorpge.h"
#include "stylepge.h"
#include "propsht2.h"
#include "minifrm.h"
#include "mainfrm.h"

BEGIN_MESSAGE_MAP(CShapePropSheetFrame, CMiniFrameWnd)
	//{{AFX_MSG_MAP(CShapePropSheetFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_SETFOCUS()
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CShapePropSheetFrame::CShapePropSheetFrame()
{
	m_pModelessShapePropSheet = NULL;
}

int CShapePropSheetFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMiniFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pModelessShapePropSheet = new CModelessShapePropSheet(this);
	if (!m_pModelessShapePropSheet->Create(this, 
		WS_CHILD | WS_VISIBLE, 0))
	{
		delete m_pModelessShapePropSheet;
		m_pModelessShapePropSheet = NULL;
		return -1;
	}

	// �`���C���h �v���p�e�B�V�[�g�̃T�C�Y�ɍ��킹�ă~�j �t���[����
	// �T�C�Y��ύX����
	CRect rectClient, rectWindow;
	m_pModelessShapePropSheet->GetWindowRect(rectClient);
	rectWindow = rectClient;

	// CMiniFrameWnd::CalcWindowRect �̓~�j �t���[���̕\�����邽�߂�
	// ���ƍ����𒲐߂���
	CalcWindowRect(rectWindow);
	SetWindowPos(NULL, rectWindow.left, rectWindow.top,
		rectWindow.Width(), rectWindow.Height(),
		SWP_NOZORDER | SWP_NOACTIVATE);
	m_pModelessShapePropSheet->SetWindowPos(NULL, 0, 0,
		rectClient.Width(), rectClient.Height(),
		SWP_NOZORDER | SWP_NOACTIVATE);

	return 0;
}

void CShapePropSheetFrame::OnClose()
{
	// ���[�h���X �v���p�e�B�V�[�g��������ɁA�s���ɂ���
	CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	pMainFrame->HideModelessPropSheet();
}

void CShapePropSheetFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// ���ߍ��܂�Ă���v���p�e�B�V�[�g�Ƀt�H�[�J�X���b�Z�[�W��]������
	ASSERT_VALID(m_pModelessShapePropSheet);
	m_pModelessShapePropSheet->SetFocus();
}

void CShapePropSheetFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CMiniFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	// ���ׂĂ� WM_ACTIVATE ���b�Z�[�W���v���p�e�B�V�[�g�ɓ]������
	// �_�C�A���O �}�l�W�����g�̂悤�ɁA�t�H�[�J�X��ۑ�/���A����
	ASSERT_VALID(m_pModelessShapePropSheet);

	// GetCurrentMessage �ŕύX����Ă��Ȃ����b�Z�[�W �f�[�^���擾
	const MSG* pMsg = GetCurrentMessage();
	ASSERT(pMsg->message == WM_ACTIVATE);
	m_pModelessShapePropSheet->SendMessage(pMsg->message, pMsg->wParam,
		pMsg->lParam);
}