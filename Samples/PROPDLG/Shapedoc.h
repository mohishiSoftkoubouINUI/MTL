// shapesdoc.h : CShapesDoc �N���X�̃C���^�[�t�F�[�X
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

#ifndef _MTLMFC
#include <afxtempl.h>
#endif

class CShapesDoc : public CDocument
{
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CShapesDoc();
	DECLARE_DYNCREATE(CShapesDoc)

// �A�g���r���[�g
public:
	CTypedPtrList<CObList, CShape*> m_shapeList;

// �I�y���[�V����
public:
	void AddShape(CShape* pShapeNew);
	CShape* HitTest(CPoint pt);

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CShapesDoc)
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CShapesDoc();
	virtual void Serialize(CArchive& ar);   // �h�L�������g�̓��o�͂̂��߂ɃI�[�o�[���C�h
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CShapesDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
