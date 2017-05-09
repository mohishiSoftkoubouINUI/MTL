// propsht2.h : CModelessShapePropSheet �N���X�̃C���^�[�t�F�[�X
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

class CModelessShapePropSheet;

class CModelessShapePropSheet : public CPropertySheet
{
public:
	DECLARE_DYNAMIC(CModelessShapePropSheet)
	CModelessShapePropSheet(CWnd* pWndParent);

// ����
	CStylePage m_stylePage;
	CColorPage m_colorPage;

// �I�y���[�V����
	void SetSheetPropsFromShape(CShape* pShape);
	void SetShapePropsFromSheet(CShape* pShape);

// �I�[�o�[���C�h
	virtual void PostNcDestroy();

// ���b�Z�[�W �n���h��
protected:
	//{{AFX_MSG(CModelessShapePropSheet)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};