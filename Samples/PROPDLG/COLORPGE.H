// colorpge.h : �w�b�_�[ �t�@�C��
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

/////////////////////////////////////////////////////////////////////////////
// CColorPage �_�C�A���O

class CColorPage : public CPropertyPage
{
// �\�z
public:
	CColorPage();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CColorPage)
	enum { IDD = IDD_COLOR };
	int     m_nColor;
	//}}AFX_DATA


//�I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CColorPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	void OnColorClicked(UINT nCmdID);
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CColorPage)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};