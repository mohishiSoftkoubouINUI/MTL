// maindoc.h : CMainDoc �N���X�̃C���^�[�t�F�C�X
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

class CMainDoc : public CDocument
{
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CMainDoc();
	DECLARE_SERIAL(CMainDoc)

// �A�g���r���[�g
public:
	// �h�L�������g���L�̃f�[�^�̗�
	CString m_strData;          // �X�g�����O �e�L�X�g
	COLORREF m_colorData;       // �X�g�����O �e�L�X�g�̐F

// �C���v�������e�[�V����
public:
	virtual ~CMainDoc();
	virtual void Serialize(CArchive& ar);   // �h�L�������g I/O �ɑ΂��ăI�[�o�[���C�h����܂��B
protected:
	virtual BOOL    OnNewDocument();

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CMainDoc)
	afx_msg void OnChangeData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////