// shapedoc.cpp : CShapesDoc �N���X�̃C���v�������e�[�V����
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
#include "shapedoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShapesDoc

IMPLEMENT_DYNCREATE(CShapesDoc, CDocument)

BEGIN_MESSAGE_MAP(CShapesDoc, CDocument)
	//{{AFX_MSG_MAP(CShapesDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShapesDoc �\�z/����

CShapesDoc::CShapesDoc()
{
}

CShapesDoc::~CShapesDoc()
{
	POSITION pos = m_shapeList.GetHeadPosition();
	while (pos != NULL)
	{
		delete m_shapeList.GetNext(pos);
	}
}

BOOL CShapesDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	CShape* pShapeNew = new CShape(
		black,
		rectangle,
		CRect(20,20,120,120));
	AddShape(pShapeNew);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CShapesDoc �I�y���[�V����

void CShapesDoc::AddShape(CShape* pShapeNew)
{
	m_shapeList.AddHead(pShapeNew);
	UpdateAllViews(NULL);
	SetModifiedFlag();
}

CShape* CShapesDoc::HitTest(CPoint pt)
{
	CShape* pShapeHit;
	POSITION pos = m_shapeList.GetHeadPosition();
	while (pos != NULL)
	{
		pShapeHit = m_shapeList.GetNext(pos);
		if (pShapeHit->m_rect.PtInRect(pt))
			return pShapeHit;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CShapesDoc �V���A���C�Y�@�\

void CShapesDoc::Serialize(CArchive& ar)
{
	m_shapeList.Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CShapesDoc �f�f�@�\

#ifdef _DEBUG
void CShapesDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CShapesDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CShapesDoc �R�}���h