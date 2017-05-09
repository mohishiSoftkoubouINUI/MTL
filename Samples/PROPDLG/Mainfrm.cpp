// mainfrm.cpp : CMainFrame �N���X�̃C���v�������e�[�V����
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
#include "propdlg.h"
#include "colorpge.h"
#include "stylepge.h"
#include "shapeobj.h"
#include "preview.h"
#include "propsht.h"
#include "propsht2.h"
#include "minifrm.h"
#include "resource.h"
#include "mainfrm.h"
#include "shapedoc.h"
#include "shapesvw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_MINI_FRAME_PROPERTY_SHEET, CMainFrame::OnMiniFramePropertySheet)
	ON_UPDATE_COMMAND_UI(ID_MINI_FRAME_PROPERTY_SHEET, CMainFrame::OnUpdateMiniFramePropertySheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// �R���g���[�� �o�[�̏������Ɏg���� ID �̔z��

// �c�[���{�^�� - ID �̓R�}���h�{�^��
static UINT BASED_CODE buttons[] =
{
	// 'toolbar.bmp' �r�b�g�}�b�v�̒��Ɠ�������
	ID_FILE_NEW,
	ID_FILE_OPEN,
	ID_FILE_SAVE,
		ID_SEPARATOR,
	ID_EDIT_CUT,
	ID_EDIT_COPY,
	ID_EDIT_PASTE,
		ID_SEPARATOR,
	ID_FILE_PRINT,
	ID_APP_ABOUT,
};

static UINT BASED_CODE indicators[] =
{
	ID_SEPARATOR,           // �X�e�[�^�X���C�� �C���W�P�[�^
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �\�z/����

CMainFrame::CMainFrame()
{
	m_pShapePropFrame = NULL;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadBitmap(IDR_MAINFRAME) ||
		!m_wndToolBar.SetButtons(buttons,
		  sizeof(buttons)/sizeof(UINT)))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // �쐬�Ɏ��s
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // �쐬�Ɏ��s
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �f�f�@�\

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���b�Z�[�W �n���h��

void CMainFrame::OnMiniFramePropertySheet()
{
	// �~�j �t���[�������݂��Ȃ��ꍇ�́A�V�K�ɍ쐬����
	// ����ȊO�̏ꍇ�́A���ɂ���

	if (m_pShapePropFrame == NULL)
	{
		m_pShapePropFrame = new CShapePropSheetFrame;
		CRect rect(0, 0, 0, 0);
		CString strTitle;
		strTitle.LoadString(IDS_OBJECT_PROPERTIES);
		if (!m_pShapePropFrame->Create(NULL, strTitle,
			WS_POPUP | WS_CAPTION | WS_SYSMENU, rect, this))
		{
			m_pShapePropFrame = NULL;
			return;
		}
		m_pShapePropFrame->CenterWindow();
	}

	// ���[�h���X �v���p�e�B �V�[�g�����ɂ���O�ɁA���݂̑I������
	// �Ă���V�F�C�v�𔽉f���邽�߂ɐݒ���X�V����
	CShapesView* pView = 
		STATIC_DOWNCAST(CShapesView, MDIGetActive()->GetActiveView());
	ASSERT_VALID(pView);
	if (pView->m_pShapeSelected != NULL)
	{
	   m_pShapePropFrame->m_pModelessShapePropSheet->
		   SetSheetPropsFromShape(pView->m_pShapeSelected);
	}

	if (m_pShapePropFrame != NULL && !m_pShapePropFrame->IsWindowVisible())
		m_pShapePropFrame->ShowWindow(SW_SHOW);
}

void CMainFrame::OnUpdateMiniFramePropertySheet(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pShapePropFrame == NULL
		|| !m_pShapePropFrame->IsWindowVisible());
}

void CMainFrame::HideModelessPropSheet()
{
	if (m_pShapePropFrame != NULL)
		m_pShapePropFrame->ShowWindow(SW_HIDE);
}
