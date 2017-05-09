// splitter.cpp : �C���v�������e�[�V���� �t�@�C��
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
// CSplitterFrame

// �o�̓e�L�X�g�r���[����ѓ��̓r���[�𕪊�����X�v���b�^�[ �E�B���h�E�̍쐬
//                           |
//    TEXT VIEW (CTextView)  | INPUT VIEW (CInputView)
//                           |

IMPLEMENT_DYNCREATE(CSplitterFrame, CMDIChildWnd)

CSplitterFrame::CSplitterFrame()
{
}

CSplitterFrame::~CSplitterFrame()
{
}


BOOL CSplitterFrame::OnCreateClient(LPCREATESTRUCT,
	 CCreateContext* pContext)
{
	// 1 ��A2 �s�̃X�v���b�^�[���쐬
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}

	// 1 �Ԗڂ̃X�v���b�^�[ �y�C����ǉ� - 0 �s�ڂ̃f�t�H���g �r���[ 
	if (!m_wndSplitter.CreateView(0, 0,
		pContext->m_pNewViewClass, CSize(130, 50), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}

	// 2 �Ԗڂ̃X�v���b�^�[ �y�C����ǉ� - 1 �s�ڂ̓��̓r���[
	if (!m_wndSplitter.CreateView(0, 1,
		RUNTIME_CLASS(CInputView), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}

	// ���̓r���[���A�N�e�B�u�ɂ��܂��B
	SetActiveView((CView*)m_wndSplitter.GetPane(0,1));

	return TRUE;
}

BEGIN_MESSAGE_MAP(CSplitterFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CSplitterFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3WaySplitterFrame - CSplitterFrame �̂悤�Ɉ�Ԗڂ̃y�C���̓��̓r���[
// �������āA2 �̏o�̓r���[������܂��B

//                             |  Text View (CTextView)
//    INPUT VIEW (CInputView)  |------------------------
//                             |  Color View (CColorView)

IMPLEMENT_DYNCREATE(C3WaySplitterFrame, CMDIChildWnd)

C3WaySplitterFrame::C3WaySplitterFrame()
{
}

C3WaySplitterFrame::~C3WaySplitterFrame()
{
}


BOOL C3WaySplitterFrame::OnCreateClient(LPCREATESTRUCT lpcs,
	 CCreateContext* pContext)
{
	// 1 ��A2 �s�̃X�v���b�^�[���쐬
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}

	// 1 �Ԗڂ̃X�v���b�^�[ �y�C����ǉ� - 0 �s�ڂ̃f�t�H���g �r���[
	if (!m_wndSplitter.CreateView(0, 0,
		pContext->m_pNewViewClass, CSize(200, 50), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}

	// 2 �Ԗڂ̃X�v���b�^�[ �y�C����ǉ� - 2 �s�Ƀl�X�g�����X�v���b�^�[
	if (!m_wndSplitter2.CreateStatic(
		&m_wndSplitter,     // �e�E�B���h�E�� 1 �Ԃߖڂ̃X�v���b�^�[
		2, 1,               // �V�����X�v���b�^�[�� 2 ��A1 �s
		WS_CHILD | WS_VISIBLE | WS_BORDER,  // �X�^�C�� WS_BORDER ���K�v�ł��B
		m_wndSplitter.IdFromRowCol(0, 1)
			// �V�����X�v���b�^�[�� 1 �Ԗڂ̗�ɂ���܂��A 1 �Ԗڂ̃X�v���b�^�[�� 2 �s��
	   ))
	{
		TRACE0("Failed to create nested splitter\n");
		return FALSE;
	}

	// �l�X�g���ꂽ�X�v���b�^�[���� 2 �̃r���[���쐬���܂��B
	int cyText = max(lpcs->cy - 70, 20);    // �e�L�X�g �y�C���̍���

	if (!m_wndSplitter2.CreateView(0, 0,
		RUNTIME_CLASS(CTextView), CSize(0, cyText), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}
	if (!m_wndSplitter2.CreateView(1, 0,
		RUNTIME_CLASS(CColorView), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create third pane\n");
		return FALSE;
	}

	// ���ׂē��삵���ꍇ�A3 �̈قȂ�r���[���܂� 2 �̃X�v���b�^�[ �E�B���h�E
	// ���ł��܂��B
	return TRUE;
}

BEGIN_MESSAGE_MAP(C3WaySplitterFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(C3WaySplitterFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// 

IMPLEMENT_DYNAMIC(CViewExSplitWnd, CSplitterWnd)

CViewExSplitWnd::CViewExSplitWnd()
{
}

CViewExSplitWnd::~CViewExSplitWnd()
{
}

CWnd* CViewExSplitWnd::GetActivePane(int* pRow, int* pCol)
{
	ASSERT_VALID(this);

	// �t���[�� �E�B���h�E�̃A�N�e�B�u �r���[�̎g�p�����݂܂��B
	CWnd* pView = NULL;
 	CFrameWnd* pFrameWnd = GetParentFrame();
	ASSERT_VALID(pFrameWnd);
	pView = pFrameWnd->GetActiveView();

	// ���s�����ꍇ�A���݂̃t�H�[�J�X���g�p���܂��B
	if (pView == NULL)
		pView = GetFocus();

	return pView;
}

