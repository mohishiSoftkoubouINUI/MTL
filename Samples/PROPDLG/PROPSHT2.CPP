// shapesht.cpp : CModelessShapePropSheet �N���X�̃C���v�������e�[�V����
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
#include "resource.h"
#include "shapeobj.h"
#include "colorpge.h"
#include "stylepge.h"
#include "propsht2.h"

IMPLEMENT_DYNAMIC(CModelessShapePropSheet, CPropertySheet)

BEGIN_MESSAGE_MAP(CModelessShapePropSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CModelessShapePropSheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CModelessShapePropSheet::CModelessShapePropSheet(CWnd* pWndParent)
	: CPropertySheet(AFX_IDS_APP_TITLE, pWndParent)
{
	AddPage(&m_stylePage);
	AddPage(&m_colorPage);
}

void CModelessShapePropSheet::PostNcDestroy()
{
	CPropertySheet::PostNcDestroy();
	delete this;
}

void CModelessShapePropSheet::SetSheetPropsFromShape(CShape* pShape)
{
	m_stylePage.m_nShapeStyle = pShape->m_shapestyle;
	m_stylePage.SetModified(FALSE);

	m_colorPage.m_nColor = pShape->m_shapecolor;
	m_colorPage.SetModified(FALSE);

	// ���݃A�N�e�B�u�ȃv���p�e�B�y�[�W�̃R���g���[���ɐݒ肳���
	// ����V�����V�F�[�v�v���p�e�B�𔽉f����
	GetActivePage()->UpdateData(FALSE);
}

void CModelessShapePropSheet::SetShapePropsFromSheet(CShape* pShape)
{
	pShape->m_shapecolor = m_colorPage.m_nColor;
	pShape->m_shapestyle = (SHAPE_STYLE)m_stylePage.m_nShapeStyle;

	m_colorPage.SetModified(FALSE);
	m_stylePage.SetModified(FALSE);
}