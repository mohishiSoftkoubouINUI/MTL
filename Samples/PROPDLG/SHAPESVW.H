// shapesvw.h : CShapesView �N���X�̃C���^�[�t�F�[�X
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

class CShapesView : public CView
{
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CShapesView();
	DECLARE_DYNCREATE(CShapesView)

// �A�g���r���[�g
public:
	CShapesDoc* GetDocument();
	CShape* m_pShapeSelected;
protected:
	CModalShapePropSheet* m_pModalShapePropSheet;


// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CShapesView)
	public:
	virtual void OnDraw(CDC* pDC);  // �r���[�̕`��̂��߂ɃI�[�o�[���C�h
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	CModelessShapePropSheet* GetModelessShapePropSheet();

	virtual ~CShapesView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CShapesView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSimplePropertySheet();
	afx_msg void OnUpdateSimplePropertySheet(CCmdUI* pCmdUI);
	afx_msg void OnPropertySheetWithPreview();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnUpdatePropertySheetWithPreview(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
   afx_msg LRESULT OnChangeObjectProps(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // �f�o�b�O�o�[�W������ shapesvw.cpp �Œ�`����Ă���
inline CShapesDoc* CShapesView::GetDocument()
   { return (CShapesDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////