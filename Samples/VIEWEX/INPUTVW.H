// inputvw.h : �w�b�_�[ �t�@�C��
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
// CInputView �t�H�[�� �r���[

class CInputView : public CFormView
{
	DECLARE_DYNCREATE(CInputView)
protected:
	CInputView();           // ���I�쐬�Ŏg�p�����v���e�N�g �R���X�g���N�^

// �t�H�[�� �f�[�^
public:
	//{{AFX_DATA(CInputView)
	enum { IDD = IDD_INPUTFORM };
	CString m_strData;
	int     m_iColor;
	//}}AFX_DATA

// �A�g���r���[�g
public:
	CMainDoc* GetDocument()
			{
				ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMainDoc)));
				return (CMainDoc*) m_pDocument;
			}

// �I�y���[�V����
public:

// �C���v�������e�[�V����
protected:
	virtual ~CInputView();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CInputView)
	afx_msg void OnDataChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////