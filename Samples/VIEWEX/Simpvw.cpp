// simpvw.cpp : �V���v���ȃr���[ �N���X�̃C���v�������e�[�V����
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

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextView

IMPLEMENT_DYNCREATE(CTextView, CView)

BEGIN_MESSAGE_MAP(CTextView, CView)
	//{{AFX_MSG_MAP(CTextView)
	ON_WM_MOUSEACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextView �N���X�̍\�z/����

CTextView::CTextView()
{
}

CTextView::~CTextView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CTextView �N���X�̕`��

void CTextView::OnDraw(CDC* pDC)
{
	CMainDoc* pDoc = GetDocument();

	CRect rect;
	GetClientRect(rect);
	pDC->SetTextAlign(TA_BASELINE | TA_CENTER);
	pDC->SetTextColor(pDoc->m_colorData);
	pDC->SetBkMode(TRANSPARENT);
	// �E�B���h�E�̒���
	pDC->TextOut(rect.Width() / 2, rect.Height() / 2, 
		pDoc->m_strData, pDoc->m_strData.GetLength());
}


int CTextView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// ���̃r���[���A�N�e�B�u�ɂ������Ȃ��̂ŁACView �̃C���v�������e�[�V������
	// ������܂��B
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

/////////////////////////////////////////////////////////////////////////////
// CColorView

IMPLEMENT_DYNCREATE(CColorView, CView)

BEGIN_MESSAGE_MAP(CColorView, CView)
	//{{AFX_MSG_MAP(CColorView)
	ON_WM_MOUSEACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorView �N���X�̍\�z/����

CColorView::CColorView()
{
}

CColorView::~CColorView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CColorView �N���X�̕`��

void CColorView::OnDraw(CDC* pDC)
{
	CMainDoc* pDoc = GetDocument();

	CRect rect;
	GetClientRect(rect);

	// �w��̐F�Ńr���[��h��Ԃ��܂��B
	CBrush br(pDoc->m_colorData);
	pDC->FillRect(rect, &br);
}

int CColorView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// ���̃r���[���A�N�e�B�u�ɂ������Ȃ��̂ŁACView �̃C���v�������e�[�V������
	// ������܂��B
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CColorView::OnActivateView(BOOL, CView*, CView*)
{
	ASSERT(FALSE);      // �o�݂͂̂̃r���[ - �����ăA�N�e�B�u�ɂ��܂���B
}


/////////////////////////////////////////////////////////////////////////////
