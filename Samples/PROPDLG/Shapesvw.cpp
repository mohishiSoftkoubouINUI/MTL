// shapesvw.cpp : CShapesView �N���X�̃C���v�������e�[�V����
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

#include "shapeobj.h"
#include "colorpge.h"
#include "stylepge.h"
#include "preview.h"
#include "propsht.h"
#include "propsht2.h"
#include "shapedoc.h"
#include "shapesvw.h"

#include "minifrm.h"
#include "mainfrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShapesView

IMPLEMENT_DYNCREATE(CShapesView, CView)

BEGIN_MESSAGE_MAP(CShapesView, CView)
	ON_MESSAGE(WM_USER_CHANGE_OBJECT_PROPERTIES, CShapesView::OnChangeObjectProps)
	//{{AFX_MSG_MAP(CShapesView)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_SIMPLE_PROPERTY_SHEET, CShapesView::OnSimplePropertySheet)
	ON_UPDATE_COMMAND_UI(ID_SIMPLE_PROPERTY_SHEET, CShapesView::OnUpdateSimplePropertySheet)
	ON_COMMAND(ID_PROPERTY_SHEET_WITH_PREVIEW, CShapesView::OnPropertySheetWithPreview)
	ON_WM_KEYDOWN()
	ON_UPDATE_COMMAND_UI(ID_PROPERTY_SHEET_WITH_PREVIEW, CShapesView::OnUpdatePropertySheetWithPreview)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShapesView �\�z/����

CShapesView::CShapesView()
{
	m_pShapeSelected = NULL;
	m_pModalShapePropSheet = NULL;
}

CShapesView::~CShapesView()
{
}


/////////////////////////////////////////////////////////////////////////////
// �I�y���[�V����

CModelessShapePropSheet* CShapesView::GetModelessShapePropSheet()
{
	CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	if (pMainFrame->m_pShapePropFrame == NULL)
		return NULL;
	return pMainFrame->m_pShapePropFrame->m_pModelessShapePropSheet;
}

/////////////////////////////////////////////////////////////////////////////
// CShapesView �`��

void CShapesView::OnDraw(CDC* pDC)
{
	CShapesDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRect rectDirty, rectIntersect;
	pDC->GetClipBox(&rectDirty);
	if (rectDirty.IsRectNull())
		GetClientRect(&rectDirty);

	CShape* pShape;
	POSITION pos = pDoc->m_shapeList.GetTailPosition();
	// ���̂��̂��珇�ɁA���ݑI������Ă�����̈ȊO�̂��ׂĂ�
	// �V�F�C�v��`�悷��
	while (pos != NULL)
	{
		pShape = pDoc->m_shapeList.GetPrev(pos);
		if (rectIntersect.IntersectRect(&pShape->m_rect, &rectDirty) != 0
			&& pShape != m_pShapeSelected)
			pShape->Draw(pDC, FALSE);
	}
	// �I������Ă���V�F�C�v����Ԏ�O�ɕ`�悷��
	if (m_pShapeSelected != NULL)
		m_pShapeSelected->Draw(pDC, TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CShapesView ���

BOOL CShapesView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �f�t�H���g�̏���
	return DoPreparePrinting(pInfo);
}

void CShapesView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CShapesView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CShapesView �f�f�@�\

#ifdef _DEBUG
void CShapesView::AssertValid() const
{
	CView::AssertValid();
}

void CShapesView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CShapesDoc* CShapesView::GetDocument() // �f�o�b�O�o�[�W�����łȂ����̂̓C�����C��
{
	ASSERT_KINDOF(CShapesDoc, m_pDocument);
	return (CShapesDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CShapesView �N���X�̃��b�Z�[�W�n���h��

void CShapesView::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{
	CShapesDoc* pDoc = GetDocument();
	CModelessShapePropSheet* pModelessShapePropSheet
		= GetModelessShapePropSheet();

	CClientDC dc(this);

	CShape* pShapeHit;
	pShapeHit = pDoc->HitTest(point);
	if (pShapeHit != NULL)
	{   // �N���b�N���ꂽ�I�u�W�F�N�g��I������

		CRect rectInvalid;
		// �ȑO�ɑI������Ă����I�u�W�F�N�g�̕\���𖳌��ɂ���A
		// �I������Ă��Ȃ��I�u�W�F�N�g�Ƃ��čĕ`�悳���
		if (m_pShapeSelected != NULL)
			InvalidateRect(&m_pShapeSelected->m_rect);
		m_pShapeSelected = pShapeHit;

		// �V���ɑI�����ꂽ�I�u�W�F�N�g�̕\���𖳌��ɂ���A
		// �I������Ă���I�u�W�F�N�g�Ƃ��čĕ`�悳���
		InvalidateRect(&m_pShapeSelected->m_rect);

		// ���[�h���X�v���p�e�B�V�[�g���\������Ă���ꍇ��
		// �V���ɑI�����ꂽ�V�F�C�v�𔽉f���Đݒ���X�V����
		if (pModelessShapePropSheet != NULL
			&& pModelessShapePropSheet->IsWindowVisible())
		{
		   pModelessShapePropSheet->
			   SetSheetPropsFromShape(m_pShapeSelected);
		}
	}
	else
	{   // �V���ɃV�F�C�v��ǉ�����

		CShape* pShapeNew = new CShape(
			black,
			rectangle,
			CRect(point.x-50, point.y-50, point.x + 50, point.y + 50));
		pDoc->AddShape(pShapeNew);
		m_pShapeSelected = pShapeNew;
		// ���[�h���X�v���p�e�B�V�[�g�����݂���ꍇ�́A���݂̐ݒ��
		// �V�����V�F�C�v�ɓK�p����
		if (pModelessShapePropSheet != NULL)
		{
		   pModelessShapePropSheet->
			   SetShapePropsFromSheet(m_pShapeSelected);
		}
	}
}

void CShapesView::OnLButtonDblClk(UINT /*nFlags*/, CPoint /*point*/)
{
	// �ǂ��ł��_�u���N���b�N����ƁA���[�h���X�v���p�e�B�V�[�g���\�������
	CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	pMainFrame->OnMiniFramePropertySheet();
}


void CShapesView::OnSimplePropertySheet()
{
	if (m_pShapeSelected == NULL)
		return;

	CPropertySheet dlgPropertySheet(AFX_IDS_APP_TITLE);
	CStylePage stylePage;
	CColorPage colorPage;

	stylePage.m_nShapeStyle = m_pShapeSelected->m_shapestyle;
	colorPage.m_nColor = m_pShapeSelected->m_shapecolor;

	dlgPropertySheet.AddPage(&stylePage);
	dlgPropertySheet.AddPage(&colorPage);

	if (dlgPropertySheet.DoModal() == IDOK)
	{
		m_pShapeSelected->m_shapecolor = colorPage.m_nColor;
		m_pShapeSelected->m_shapestyle
			= (SHAPE_STYLE)stylePage.m_nShapeStyle;
		GetDocument()->SetModifiedFlag();
		GetDocument()->UpdateAllViews(NULL);
	}
}

void CShapesView::OnUpdateSimplePropertySheet(CCmdUI* pCmdUI)
{
	// �v���r���[�t���̃v���p�e�B�V�[�g�̋@�\�𗘗p����
	OnUpdatePropertySheetWithPreview(pCmdUI);
}

void CShapesView::OnPropertySheetWithPreview()
{
	if (m_pShapeSelected == NULL)
		return;

	// ���[�_���v���p�e�B�V�[�g�ւ̃|�C���^��ۑ�����̂ŁA
	// ���[�U��Apply Now ��I���� CModalShapePropSheet ��
	// WM_USER_CHANGE_OBJECT_PROPERTIES ���b�Z�[�W���r���[
	// �ɑ������Ƃ��A�r���[�͌��݂̐ݒ���擾���I������Ă���
	// �I�u�W�F�N�g�ɐݒ��K�p���邽�߂ɁA�ȒP�Ƀv���p�e�B
	// �V�[�g���Q�Ƃł���
	m_pModalShapePropSheet = new CModalShapePropSheet;

	// CModalShapePropSheet::SetSheetPropsFromShape ��
	// ���[�U�� Apply Now ��I�����Ƃ��ɌĂ΂�郆�[�U
	// ��`���b�Z�[�W WM_USER_CHANGE_OBJECT_PROPS �̃n
	// ���h������Ăяo�����悤�ɐ݌v����Ă��邽��
	// �����ł͎g���܂���B���[�_���v���p�e�B�V�[�g�͂�
	// ���ł͂܂��쐬����Ă��܂���BSetSheetPropsFromShape
	// ���Ăяo���ƃE�B���h�E���X�V���悤�Ƃ��Ď��s���܂��B
	m_pModalShapePropSheet->m_stylePage.m_nShapeStyle
		= m_pShapeSelected->m_shapestyle;
	m_pModalShapePropSheet->m_colorPage.m_nColor = m_pShapeSelected->m_shapecolor;

	if (m_pModalShapePropSheet->DoModal() == IDOK)
	{
		m_pModalShapePropSheet->SetShapePropsFromSheet(m_pShapeSelected);
		GetDocument()->SetModifiedFlag();
		GetDocument()->UpdateAllViews(NULL);
	}
	delete m_pModalShapePropSheet;
	m_pModalShapePropSheet = NULL;
}

void CShapesView::OnUpdatePropertySheetWithPreview(CCmdUI* pCmdUI)
{
	BOOL bEnable = TRUE;
	if (m_pShapeSelected == NULL)
	{
		bEnable = FALSE;
	}
	else
	{
		CModelessShapePropSheet* pModelessShapePropSheet
			= GetModelessShapePropSheet();
		if (pModelessShapePropSheet != NULL
			&& pModelessShapePropSheet->IsWindowVisible())
		{
			// ���[�h���X�v���p�e�B�V�[�g�����łɕ\������Ă���ꍇ
			// �̓��[�_���v���p�e�B�V�[�g��\�����Ă͂����Ȃ�
			bEnable = FALSE;
		}
	}
	pCmdUI->Enable(bEnable);
}

LRESULT CShapesView::OnChangeObjectProps(WPARAM, LPARAM)
{
	// ���[�U�� Apply Now ��I�������Ƃ��̓��[�_���� CModalShapePropSheet
	// ����A�܂��A���[�U���v���p�e�B�y�[�W�ŉ����ݒ��ύX�����Ƃ��ɂ�
	// ���[�h���X�� CModelessShapePropSheet ����A���[�U��`���b�Z�[�W
	// WM_USER_CHANGE_OBJECT_PROPS �������܂�

	if (m_pShapeSelected == NULL)
		return 0;

	CModelessShapePropSheet* pModelessShapePropSheet
		= GetModelessShapePropSheet();
	if (pModelessShapePropSheet != NULL)
		pModelessShapePropSheet->SetShapePropsFromSheet(m_pShapeSelected);

	if (m_pModalShapePropSheet != NULL)
		m_pModalShapePropSheet->SetShapePropsFromSheet(m_pShapeSelected);

	GetDocument()->SetModifiedFlag();
	GetDocument()->UpdateAllViews(NULL);
	return 0;
}

void CShapesView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_ESCAPE)
	{
		CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
		pMainFrame->HideModelessPropSheet();
		return;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
