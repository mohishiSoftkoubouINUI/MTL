// SdiVC6Doc.cpp : CSdiVC6Doc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "SdiVC6.h"

#include "SdiVC6Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSdiVC6Doc

IMPLEMENT_DYNCREATE(CSdiVC6Doc, CDocument)

BEGIN_MESSAGE_MAP(CSdiVC6Doc, CDocument)
	//{{AFX_MSG_MAP(CSdiVC6Doc)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSdiVC6Doc �N���X�̍\�z/����

CSdiVC6Doc::CSdiVC6Doc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B

}

CSdiVC6Doc::~CSdiVC6Doc()
{
}

BOOL CSdiVC6Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSdiVC6Doc �V���A���C�[�[�V����

void CSdiVC6Doc::Serialize(CArchive& ar)
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
// CSdiVC6Doc �N���X�̐f�f

#ifdef _DEBUG
void CSdiVC6Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSdiVC6Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSdiVC6Doc �R�}���h
