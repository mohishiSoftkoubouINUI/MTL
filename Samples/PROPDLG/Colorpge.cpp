// colorpge.cpp : �C���v�������e�[�V���� �t�@�C��
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
#include "stylepge.h"
#include "colorpge.h"
#include "shapeobj.h"
#include "preview.h"
#include "propsht.h"
#include "propsht2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorPage �v���p�e�B �y�[�W

CColorPage::CColorPage()
	: CPropertyPage(CColorPage::IDD)
{
	//{{AFX_DATA_INIT(CColorPage)
	//}}AFX_DATA_INIT
	m_nColor = black;
}

void CColorPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorPage)
	DDX_Radio(pDX, IDC_BLACK, m_nColor);
	//}}AFX_DATA_MAP
}

void CColorPage::OnColorClicked(UINT /*nCmdID*/)
{
	// CColorPage �v���p�e�B�y�[�W �� CModalShapePropSheet ��
	// CModelessShapePropSheet �̗����Ɏg���Ă��܂��B�ǂ����
	// �v���p�e�B�V�[�g�̏ꍇ�������ɃV�F�C�v��ύX����Ƃ������ʂ�
	// �����������Ă��܂��BCModalShapePropSheet �̏ꍇ�A���݂̃V�F
	//�C�v���v���r���[�E�B���h�E�ɕ\������ACModelessShapePropSheet
	// �̏ꍇ�ɂ̓r���[�̒��őI������Ă���̂����݂̃V�F�C�v�ł��B
	//

	CPropertySheet* pSheet = STATIC_DOWNCAST(CPropertySheet, GetParent());

	CModalShapePropSheet* pModalSheet = 
		DYNAMIC_DOWNCAST(CModalShapePropSheet, pSheet);
	if (pModalSheet != NULL)
	{
		UpdateData();
		pModalSheet->UpdateShapePreview();
		SetModified(); // Apply Now �{�^����L���ɂ���
	}

	CModelessShapePropSheet* pModelessSheet = 
		DYNAMIC_DOWNCAST(CModelessShapePropSheet, pSheet);
	if (pModelessSheet != NULL)
	{
		UpdateData();
		CFrameWnd* pFrameWnd = STATIC_DOWNCAST(CFrameWnd, AfxGetMainWnd());
		CView* pView = pFrameWnd->GetActiveFrame()->GetActiveView();
		pView->SendMessage(WM_USER_CHANGE_OBJECT_PROPERTIES, 0, 0);
	}
}

BEGIN_MESSAGE_MAP(CColorPage, CPropertyPage)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BLACK, IDC_BLUE, CColorPage::OnColorClicked)
	//{{AFX_MSG_MAP(CColorPage)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ����܂��B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorPage ���b�Z�[�W �n���h��
