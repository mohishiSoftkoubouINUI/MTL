// enterdlg.h : �w�b�_�[ �t�@�C��
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
// CEnterDlg �_�C�A���O

class CEnterDlg : public CDialog
{
	DECLARE_DYNAMIC(CEnterDlg)
// �\�z
public:
	CEnterDlg(CWnd* pParent = NULL);    // �W���̃R���X�g���^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEnterDlg)
	enum { IDD = IDD_CHANGEDATA };
	CString m_strInput;
	//}}AFX_DATA

// �C���v�������e�[�V����
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEnterDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};