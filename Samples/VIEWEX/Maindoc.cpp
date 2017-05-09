// maindoc.cpp : CMainDoc �N���X�̃C���v�������e�[�V����
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

#include "stdafx.h"
#include "viewex.h"

#include "enterdlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainDoc

IMPLEMENT_SERIAL(CMainDoc, CDocument, 0 /* schema number*/ )

BEGIN_MESSAGE_MAP(CMainDoc, CDocument)
	//{{AFX_MSG_MAP(CMainDoc)
	ON_COMMAND(IDM_CHANGEDATA, CMainDoc::OnChangeData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDoc �N���X�̍\�z/����

CMainDoc::CMainDoc()
{
}

CMainDoc::~CMainDoc()
{
}

BOOL CMainDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	m_strData = "Sample Data String";
	m_colorData = RGB(0, 0, 0);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainDoc �V���A���C�[�[�V����

void CMainDoc::Serialize(CArchive&)
{
	ASSERT(FALSE);      // ���̃v���O������ł̓f�[�^��ۑ����܂���B
}

/////////////////////////////////////////////////////////////////////////////
// CMainDoc �R�}���h

void CMainDoc::OnChangeData()
{
	CEnterDlg dlg;
	dlg.m_strInput = m_strData;
	if (dlg.DoModal() != IDOK)
		return;
	m_strData = dlg.m_strInput;
	// ���̃h�L�������g���f�[�^��ۑ������ꍇ�A���̈ʒu�� SetModifiedFlag ���Ăяo���܂��B
	UpdateAllViews(NULL);   // �S�̂̍X�V
}

/////////////////////////////////////////////////////////////////////////////
