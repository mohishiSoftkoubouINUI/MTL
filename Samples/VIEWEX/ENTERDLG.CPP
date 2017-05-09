// enterdlg.cpp : �C���v�������e�[�V���� �t�@�C��
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
// CEnterDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CEnterDlg, CDialog)

CEnterDlg::CEnterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEnterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEnterDlg)
	m_strInput = "";
	//}}AFX_DATA_INIT
}

void CEnterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEnterDlg)
	DDX_Text(pDX, IDC_EDIT1, m_strInput);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEnterDlg, CDialog)
	//{{AFX_MSG_MAP(CEnterDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnterDlg ���b�Z�[�W �n���h��