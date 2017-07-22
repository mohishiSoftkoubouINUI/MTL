
MTL (MFC on ATL/WTL) https://github.com/mohishiSoftkoubouINUI/MTL
    ATL/WTL ���g�p�����w�b�_�t�@�C���݂̂� MFC �C���^�[�t�F�[�X�ł��B

�K�{��
    Microsoft Visual C++ MFC
        �ÓI���C�u������MFC �v���W�F�N�g���쐬�ł��� VC ���ł��B
    WTL Version 9.1
        VC ���ɃC���N���[�h�p�X��ݒ肵�܂��B

�@�\
    CWinApp/CFrameWnd/CView/CDocument �Ȃ� VC6 MFC �ō쐬����W���@�\�̃��[�U�[�C���^�[�t�F�[�X�ł��B

�e�X�g��
    VC6  MFC ATL (Visual Studio 2000) �v���W�F�N�g�쐬
    VC12 MFC ATL (Visual Studio 2013) �v���W�F�N�g�A�b�v�O���[�h

mtlmfc �t�H���_�t�@�C���\��
    mtlmfc
        Include
            mtlmfc.h �Ȃ� MTL �ɕK�v�ȃw�b�_�t�@�C���ł��B
        Samples
            VC6 32bit�AVC12 64bit �ŃR���p�C�������t�@�C���ł��B
                DlgVC6        �_�C�A���O�v���W�F�N�g
                FrmVC6        �t���[���v���W�F�N�g
                SdiVC6        �V���O���h�L�������g�v���W�F�N�g
                MdiVC6        �}���`�h�L�������g�v���W�F�N�g
                VIEWEX        �X�v���b�^�[�E�B���h�E�T���v��
                PROPDLG       �v���p�e�B�T���v�� UNICODE
        wtl
            Include
                WTL9.1 �̃w�b�_�t�@�C���ł��B

MFC �ÓI���C�u�����v���W�F�N�g�̊�{�I�� MTL �Ή��菇
1.  MTL �� WTL �� �C���N���[�h�p�X�� VC �v���W�F�N�g�ɒǉ����܂��B
2.  �v���W�F�N�g�̃����^�C�����C�u������ÓI���C�u�����ɕύX���܂��B
3.  stdafx.h �t�@�C���v���R���p�C���w�b�_�Ɏw�肵�܂�(�v���W�F�N�g�̃f�t�H���g)�B
4.  stdafx.h �t�@�C�� �� stdafx.cpp �t�@�C�����ȉ��̎菇�ŏ��������܂��B
        stdafx.h
            // This is a part of the Microsoft Foundation Classes C++ library.
            ...

            #define VC_EXTRALEAN

            #ifndef _AFXDLL                        // �ǉ�
            #define _MTLMFC                        // �ǉ�
            #endif // _AFXDLL                      // �ǉ�

            #ifndef _MTLMFC                        // �ǉ�

            #include <afxwin.h>                    // MFC �w�b�_�t�@�C���͏��O���܂��B
            #include <afxext.h>

            #else // _MTLMFC                       // �ǉ�

            #define _MTL_FXN_COMTATIBLE            // �ǉ�  MESSAGE_MAP �]���^�����o�[�֐��Ăяo���w��
                                                   //        ��.  ON_COMMAND(ID_FILE_NEW, OnFileNew)
                                                   //             ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)

            #include <mtlmfc.h>                    // �ǉ�

            #endif // _MTLMFC                      // �ǉ�

        stdafx.cpp
            // This is a part of the Microsoft Foundation Classes C++ library.
            ...

            #include "stdafx.h"

            #ifdef _MTLMFC                         // �ǉ�
            #include <mtlmain.h>                   // �ǉ�
            #endif //  _MTLMFC                     // �ǉ�
5. �ύX�����t�@�C����ۑ����A�v���W�F�N�g���r���h�����s���܂��B

���l
    Samples �t�H���_�ɂ́Astdafx.h�Astdafx.cpp ���C���ς݂̃t�@�C�����L��܂��B
    VC6 ���� VC12 �Ȃǃv���W�F�N�g�A�b�v�O���[�h��ÓI���C�u���������N�T�C�Y���傫��������ꂽ�ꍇ�A
    MTL ���g�p���鎖�Ń����N�T�C�Y�����������鎖���ł��܂��B
    MTL �ɂ́A��胊���N�T�C�Y�����������邽�߂̕s�K�v�ȋ@�\�������N�Ώۂ���폜����}�N������`���Ă���܂��B
    Samples �t�H���_�� stdafx.h �ɂ́A�v���W�F�N�g���ɓK�؂ȃ}�N�����`���ėL��܂��B
    MTL �̓w�b�_�t�@�C���ł��̂ŁA�v���W�F�N�g�ɑ��������@�\�Ƀt�@�C���C�����Ďg�p���邱�Ƃ��\�ł��B
    MTL �Ή��C����� MFC �v���W�F�N�g�́AMFC DLL �g�p�v���W�F�N�g�ɕύX���鎖�� 
    MFC DLL�g�p �Ƃ��ăR���p�C�����鎖���\�ł��B
    �܂��ȉ��̉ӏ����R�����g�A�E�g���鎖�� MFC �ÓI���C�u�����v���W�F�N�g�Ƃ��ăR���p�C�����鎖���\�ł��B
        stdafx.h
            #ifndef _AFXDLL
            //#define _MTLMFC                      // MFC �ÓI���C�u�����g�p�̏ꍇ�̓R�����g�A�E�g
            #endif // _AFXDLL
    �e�N���X�ɖ������̋@�\�A�Ⴆ�� OLE �@�\������܂��̂ŁA�󋵂ɂ���Ă̓R���p�C���A�����N���ɃG���[���������鎖������܂��B

�R�[�h�C���K�{
    BEGIN_MESSAGE_MAP �� ON_MESSAGE_VOID(WM_WINDOWPOSCHANGED, OnWindowPosChanged) �͈ȉ��ɕύX���܂��B
        ON_WM_WINDOWPOSCHANGED()

    CGdiobj �p���N���X�� m_hObject �����o�ϐ��Q�Ƃ͈ȉ��ɕύX���܂��B
        GetSafeHandle()




1.0.0.1 ��Ȓǉ��ύX
�V�K�쐬





/*
    MTL (MFC on ATL/WTL)  Version 1.0.0.1
        MFC interface with header files only 

    Required :  Microsoft Visual C++ MFC
                WTL Version 9.1
    
    Supported : CWinApp/CFrameWnd/CView/CDocument and VC6 MFC simple standard UI interface

    Tested :    VC6  MFC ATL (Visual Studio 2000) project create
                VC12 MFC ATL (Visual Studio 2013) project upgrade and recompile

    Basic instructions :
        General Property 
            Use of MFC   MFC project will statically
        C/C++
            Code Generation  Runtime Library /MT(/MTd)
            Precompiled Header  stdafx.h(default)

        stdafx.h
            // This is a part of the Microsoft Foundation Classes C++ library.

            #define VC_EXTRALEAN

            #ifndef _AFXDLL                        // add
            #define _MTLMFC                        // add
            #endif // _AFXDLL                      // add

            #ifndef _MTLMFC                        // add

            #include <afxwin.h>                    // exclude MFC header files
            #include <afxext.h>

            #else // _MTLMFC                       // add

            #define _MTL_FXN_COMTATIBLE            // add  MESSAGE_MAP classic style member function
                                                   //        eg.  ON_COMMAND(ID_FILE_NEW, OnFileNew)
                                                   //             ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)

            #include <mtlmfc.h>                    // add

            #endif // _MTLMFC                      // add

        stdafx.cpp
            // This is a part of the Microsoft Foundation Classes C++ library.

            #include "stdafx.h"

            #ifdef _MTLMFC                         // add
            #include <mtlmain.h>                   // add
            #endif //  _MTLMFC                     // add

        finally recompile and run.


        Microsoft Foundation Classes C++ library(MFC) and 
        Active Template Library(ATL) and
        Windows Template Library(WTL) Copyright (C) Microsoft Corporation

        MTL (MFC on ATL/WTL) create and integrated by SoftkoubouINUI May 2017

        Free for personal use only.
        Please contact us for commercial use.
*/
