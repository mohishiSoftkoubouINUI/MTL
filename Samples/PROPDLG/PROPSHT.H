// propsht.h : CModalShapePropSheet �N���X�̃C���^�[�t�F�[�X
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

class CModalShapePropSheet : public CPropertySheet
{
public:
	DECLARE_DYNAMIC(CModalShapePropSheet)
	CModalShapePropSheet(CWnd* pWndParent = NULL);

// ����
	CStylePage m_stylePage;
	CColorPage m_colorPage;
	CShapePreviewWnd m_wndPreview;

// �I�y���[�V����
	void SetSheetPropsFromShape(CShape* pShape);
	void SetShapePropsFromSheet(CShape* pShape);
	void UpdateShapePreview();

// �I�[�o�[���C�h
	virtual BOOL OnInitDialog();

// ���b�Z�[�W �n���h��
protected:
	//{{AFX_MSG(CModalShapePropSheet)
	afx_msg void OnApplyNow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};