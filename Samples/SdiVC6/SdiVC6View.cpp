// SdiVC6View.cpp : CSdiVC6View �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "SdiVC6.h"

#include "SdiVC6Doc.h"
#include "SdiVC6View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSdiVC6View

IMPLEMENT_DYNCREATE(CSdiVC6View, CView)

BEGIN_MESSAGE_MAP(CSdiVC6View, CView)
	//{{AFX_MSG_MAP(CSdiVC6View)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//    ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSdiVC6View �N���X�̍\�z/����

CSdiVC6View::CSdiVC6View()
{
	// TODO: ���̏ꏊ�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B

}

CSdiVC6View::~CSdiVC6View()
{
}

BOOL CSdiVC6View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSdiVC6View �N���X�̕`��

void CSdiVC6View::OnDraw(CDC* pDC)
{
	CSdiVC6Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: ���̏ꏊ�Ƀl�C�e�B�u �f�[�^�p�̕`��R�[�h��ǉ����܂��B
}

/////////////////////////////////////////////////////////////////////////////
// CSdiVC6View �N���X�̐f�f

#ifdef _DEBUG
void CSdiVC6View::AssertValid() const
{
	CView::AssertValid();
}

void CSdiVC6View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSdiVC6Doc* CSdiVC6View::GetDocument() // ��f�o�b�O �o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSdiVC6Doc)));
	return (CSdiVC6Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSdiVC6View �N���X�̃��b�Z�[�W �n���h��
