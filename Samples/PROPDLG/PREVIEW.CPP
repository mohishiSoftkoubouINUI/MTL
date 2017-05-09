// preview.cpp : CModalShapePropSheet �N���X�̃C���v�������e�[�V����
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
#include "preview.h"
#include "resource.h"
#include "colorpge.h"
#include "stylepge.h"
#include "shapeobj.h"
#include "propsht.h"

BEGIN_MESSAGE_MAP(CShapePreviewWnd, CWnd)
//{{AFX_MSG_MAP(CShapePreviewWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CShapePreviewWnd::CShapePreviewWnd()
{
}

void CShapePreviewWnd::OnPaint()
{
	CPaintDC dc(this);
	CModalShapePropSheet* pShapePropSheet = 
		STATIC_DOWNCAST(CModalShapePropSheet, GetParent());
	CRect rect;
	GetClientRect(rect);
	CShape shape(
		(SHAPE_COLOR_ENUM)pShapePropSheet->m_colorPage.m_nColor,
		(SHAPE_STYLE)pShapePropSheet->m_stylePage.m_nShapeStyle,
		rect);
	shape.Draw(&dc, FALSE);
}

BOOL CShapePreviewWnd::OnEraseBkgnd(CDC* pDC)
{
	// �w�i�F���_�C�A���O�Ɠ����ɂ���
	// �i�v���p�e�B �V�[�g�j

	CWnd* pParentWnd = GetParent();
	HBRUSH hBrush = (HBRUSH)pParentWnd->SendMessage(WM_CTLCOLORDLG,
		(WPARAM)pDC->m_hDC, (LPARAM)pParentWnd->m_hWnd);
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, CBrush::FromHandle(hBrush));
	return TRUE;
}