// MdiVC6Doc.cpp : CMdiVC6Doc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "MdiVC6.h"

#include "MdiVC6Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMdiVC6Doc

IMPLEMENT_DYNCREATE(CMdiVC6Doc, CDocument)

BEGIN_MESSAGE_MAP(CMdiVC6Doc, CDocument)
	//{{AFX_MSG_MAP(CMdiVC6Doc)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMdiVC6Doc �N���X�̍\�z/����

CMdiVC6Doc::CMdiVC6Doc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B

}

CMdiVC6Doc::~CMdiVC6Doc()
{
}

BOOL CMdiVC6Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMdiVC6Doc �V���A���C�[�[�V����

void CMdiVC6Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���̈ʒu�ɕۑ��p�̃R�[�h��ǉ����Ă��������B
	}
	else
	{
		// TODO: ���̈ʒu�ɓǂݍ��ݗp�̃R�[�h��ǉ����Ă��������B
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMdiVC6Doc �N���X�̐f�f

#ifdef _DEBUG
void CMdiVC6Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMdiVC6Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMdiVC6Doc �R�}���h
