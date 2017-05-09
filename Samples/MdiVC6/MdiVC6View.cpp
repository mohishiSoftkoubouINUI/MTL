// MdiVC6View.cpp : CMdiVC6View �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "MdiVC6.h"

#include "MdiVC6Doc.h"
#include "MdiVC6View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMdiVC6View

IMPLEMENT_DYNCREATE(CMdiVC6View, CView)

BEGIN_MESSAGE_MAP(CMdiVC6View, CView)
	//{{AFX_MSG_MAP(CMdiVC6View)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//    ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
	// �W������R�}���h
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMdiVC6View �N���X�̍\�z/����

CMdiVC6View::CMdiVC6View()
{
	// TODO: ���̏ꏊ�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B

}

CMdiVC6View::~CMdiVC6View()
{
}

BOOL CMdiVC6View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMdiVC6View �N���X�̕`��

void CMdiVC6View::OnDraw(CDC* pDC)
{
	CMdiVC6Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: ���̏ꏊ�Ƀl�C�e�B�u �f�[�^�p�̕`��R�[�h��ǉ����܂��B
}

/////////////////////////////////////////////////////////////////////////////
// CMdiVC6View �N���X�̈��

BOOL CMdiVC6View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �f�t�H���g�̈������
	return DoPreparePrinting(pInfo);
}

void CMdiVC6View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ����O�̓��ʂȏ�����������ǉ����Ă��������B
}

void CMdiVC6View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �����̌㏈����ǉ����Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// CMdiVC6View �N���X�̐f�f

#ifdef _DEBUG
void CMdiVC6View::AssertValid() const
{
	CView::AssertValid();
}

void CMdiVC6View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMdiVC6Doc* CMdiVC6View::GetDocument() // ��f�o�b�O �o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMdiVC6Doc)));
	return (CMdiVC6Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMdiVC6View �N���X�̃��b�Z�[�W �n���h��
