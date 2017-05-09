// splitter.h : �J�X�^�� �X�v���b�^�[ �R���g���[������т�����܂ރt���[��
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
// CSplitterFrame �X�v���b�^�[/���C�p�[ �t���t���[��

class CSplitterFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CSplitterFrame)
protected:
	CSplitterFrame();   // ���I�쐬�Ŏg�p�����v���e�N�g �R���X�g���N�^

// �A�g���r���[�g
protected:
	CSplitterWnd m_wndSplitter;

// �C���v�������e�[�V����
public:
	virtual ~CSplitterFrame();
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CSplitterFrame)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// 

class CViewExSplitWnd : public CSplitterWnd
{
	DECLARE_DYNAMIC(CViewExSplitWnd)

// �C���v�������e�[�V����
public:
	CViewExSplitWnd();
	~CViewExSplitWnd();
	CWnd* GetActivePane(int* pRow = NULL, int* pCol = NULL);
};


/////////////////////////////////////////////////////////////////////////////
//

class C3WaySplitterFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(C3WaySplitterFrame)
protected:
	C3WaySplitterFrame();   // ���I�쐬�Ŏg�p�����v���e�N�g �R���X�g���N�^

// �A�g���r���[�g
protected:
	CViewExSplitWnd m_wndSplitter;
	CViewExSplitWnd m_wndSplitter2;        // 1 �Ԗڂɖ��ߍ��܂�܂�

// �C���v�������e�[�V����
public:
	virtual ~C3WaySplitterFrame();
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(C3WaySplitterFrame)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////