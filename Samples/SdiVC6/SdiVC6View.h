// SdiVC6View.h : CSdiVC6View �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SDIVC6VIEW_H__8F5E4407_7295_41F4_AAAB_DCCB4E00A88B__INCLUDED_)
#define AFX_SDIVC6VIEW_H__8F5E4407_7295_41F4_AAAB_DCCB4E00A88B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSdiVC6View : public CView
{
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CSdiVC6View();
	DECLARE_DYNCREATE(CSdiVC6View)

// �A�g���r���[�g
public:
	CSdiVC6Doc* GetDocument();

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSdiVC6View)
	public:
	virtual void OnDraw(CDC* pDC);  // ���̃r���[��`�悷��ۂɃI�[�o�[���C�h����܂��B
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CSdiVC6View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CSdiVC6View)
		// ���� -  ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//         ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // SdiVC6View.cpp �t�@�C�����f�o�b�O���̎��g�p����܂��B
inline CSdiVC6Doc* CSdiVC6View::GetDocument()
   { return (CSdiVC6Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SDIVC6VIEW_H__8F5E4407_7295_41F4_AAAB_DCCB4E00A88B__INCLUDED_)
