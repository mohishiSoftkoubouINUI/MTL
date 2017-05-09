
MTL (MFC on ATL/WTL) https://github.com/mohishiSoftkoubouINUI/MTL
    ATL/WTL を使用したヘッダファイルのみの MFC インターフェースです。

必須環境
    Microsoft Visual C++ MFC
        静的ライブラリでMFC プロジェクトを作成できる VC 環境です。
    WTL Version 9.1
        VC 環境にインクルードパスを設定します。

機能
    CWinApp/CFrameWnd/CView/CDocument など VC6 MFC で作成する標準機能のユーザーインターフェースです。

テスト環境
    VC6  MFC ATL (Visual Studio 2000) プロジェクト作成
    VC12 MFC ATL (Visual Studio 2013) プロジェクトアップグレード

mtlmfc フォルダファイル構成
    mtlmfc
        Include
            mtlmfc.h など MTL に必要なヘッダファイルです。
        Samples
            VC6 32bit、VC12 64bit でコンパイルしたファイルです。
                DlgVC6        ダイアログプロジェクト
                FrmVC6        フレームプロジェクト
                SdiVC6        シングルドキュメントプロジェクト
                MdiVC6        マルチドキュメントプロジェクト
                VIEWEX        スプリッターウィンドウサンプル
                PROPDLG       プロパティサンプル UNICODE
        wtl
            Include
                WTL9.1 のヘッダファイルです。

MFC 静的ライブラリプロジェクトの基本的な MTL 対応手順
1.  MTL と WTL の インクルードパスを VC プロジェクトに追加します。
2.  プロジェクトのランタイムライブラリを静的ライブラリに変更します。
3.  stdafx.h ファイをプリコンパイルヘッダに指定します(プロジェクトのデフォルト)。
4.  stdafx.h ファイル と stdafx.cpp ファイルを以下の手順で書き換えます。
        stdafx.h
            // This is a part of the Microsoft Foundation Classes C++ library.
            ...

            #define VC_EXTRALEAN

            #ifndef _AFXDLL                        // 追加
            #define _MTLMFC                        // 追加
            #endif // _AFXDLL                      // 追加

            #ifndef _MTLMFC                        // 追加

            #include <afxwin.h>                    // MFC ヘッダファイルは除外します。
            #include <afxext.h>

            #else // _MTLMFC                       // 追加

            #define _MTL_FXN_COMTATIBLE            // 追加  MESSAGE_MAP 従来型メンバー関数呼び出し指定
                                                   //        例.  ON_COMMAND(ID_FILE_NEW, OnFileNew)
                                                   //             ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)

            #include <mtlmfc.h>                    // 追加

            #endif // _MTLMFC                      // 追加

        stdafx.cpp
            // This is a part of the Microsoft Foundation Classes C++ library.
            ...

            #include "stdafx.h"

            #ifdef _MTLMFC                         // 追加
            #include <mtlmain.h>                   // 追加
            #endif //  _MTLMFC                     // 追加
5. 変更したファイルを保存し、プロジェクトをビルドし実行します。

備考
    Samples フォルダには、stdafx.h、stdafx.cpp を修正済みのファイルが有ります。
    VC6 から VC12 などプロジェクトアップグレード後静的ライブラリリンクサイズが大きく感じられた場合、
    MTL を使用する事でリンクサイズを小さくする事ができます。
    MTL には、よりリンクサイズを小さくするための不必要な機能をリンク対象から削除するマクロが定義してあります。
    Samples フォルダの stdafx.h には、プロジェクト毎に適切なマクロを定義して有ります。
    MTL はヘッダファイルですので、プロジェクトに相応しい機能にファイル修正して使用することが可能です。
    MTL 対応修正後の MFC プロジェクトは、MFC DLL 使用プロジェクトに変更する事で 
    MFC DLL使用 としてコンパイルする事が可能です。
    また以下の箇所をコメントアウトする事で MFC 静的ライブラリプロジェクトとしてコンパイルする事が可能です。
        stdafx.h
            #ifndef _AFXDLL
            //#define _MTLMFC                      // MFC 静的ライブラリ使用の場合はコメントアウト
            #endif // _AFXDLL
    各クラスに未実装の機能、例えば OLE 機能がありますので、状況によってはコンパイル、リンク時にエラーが発生する事があります。

コード修正必須
    BEGIN_MESSAGE_MAP 内 ON_MESSAGE_VOID(WM_WINDOWPOSCHANGED, OnWindowPosChanged) は以下に変更します。
        ON_WM_WINDOWPOSCHANGED()

    CGdiobj 継承クラスの m_hObject メンバ変数参照は以下に変更します。
        GetSafeHandle()




1.0.0.1 主な追加変更
新規作成





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
