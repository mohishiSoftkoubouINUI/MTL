// minifrm.h : CShapePropSheetFrame �N���X�̃C���^�[�t�F�[�X
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

class CShapePropSheetFrame : public CMiniFrameWnd
{
// �R���X�g���N�^
public:
	CShapePropSheetFrame();

// ����
public:
	CModelessShapePropSheet* m_pModelessShapePropSheet;

// �n���h��
protected:
	//{{AFX_MSG(CShapePropSheetFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};