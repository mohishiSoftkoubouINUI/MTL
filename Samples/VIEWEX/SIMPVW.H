// simpvw.h : �V���v���ȃr���[ �N���X�̃C���^�[�t�F�C�X
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

// CTextView - �e�L�X�g�o��
// CColorView - �J���[�o��

/////////////////////////////////////////////////////////////////////////////

class CTextView : public CView
{
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CTextView();
	DECLARE_DYNCREATE(CTextView)

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
public:
	virtual ~CTextView();
	virtual void OnDraw(CDC* pDC);      // ���̃r���[��`�悷��ۂɃI�[�o�[���C�h����܂��B

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CTextView)
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

class CColorView : public CView
{
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CColorView();
	DECLARE_DYNCREATE(CColorView)

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
public:
	virtual ~CColorView();
	virtual void OnDraw(CDC* pDC);      // ���̃r���[��`�悷��ۂɃI�[�o�[���C�h����܂��B
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView,
					CView* pDeactiveView);

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CColorView)
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////