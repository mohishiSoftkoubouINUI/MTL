
/*
	MTL (MFC on ATL/WTL)  Version 1.0.2.3
		MFC interface with header files only 

	Required :	Microsoft Visual C++ MFC
				WTL Version 9.1
	
	Supported :	CWinApp/CFrameWnd/CView/CDocument and VC6 MFC simple standard UI interface

	Tested :	VC6  MFC ATL (Visual Studio 2000) project create
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

			#ifndef _AFXDLL							// add
			#define _MTLMFC							// add
			#endif // _AFXDLL						// add

			#ifndef _MTLMFC							// add

			#include <afxwin.h>						// exclude MFC header files
			#include <afxext.h>

			#else // _MTLMFC						// add

			#define _MTL_FXN_COMTATIBLE				// add  MESSAGE_MAP classic style member function
			                                        //        eg.  ON_COMMAND(ID_FILE_NEW, OnFileNew)
													//             ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)

			#include <mtlmfc.h>						// add

			#endif // _MTLMFC						// add

		stdafx.cpp
			// This is a part of the Microsoft Foundation Classes C++ library.

			#include "stdafx.h"

			#ifdef _MTLMFC							// add
			#include <mtlmain.h>					// add
			#endif //  _MTLMFC						// add

		finally recompile and run.


		Microsoft Foundation Classes C++ library(MFC) and 
		Active Template Library(ATL) and
		Windows Template Library(WTL) Copyright (C) Microsoft Corporation

		MTL (MFC on ATL/WTL) create and integrated by SoftkoubouINUI May 2017
*/

#ifndef __AFXWIN_H__
#define __AFXWIN_H__
#define __AFXEXT_H__


//#define MTL_REMOVE_CWINAPP_DOCMANAGER
//#define MTL_REMOVE_CWINAPP_PRINT
//#define MTL_REMOVE_CWINAPP_RECENTFILELIST
//#define MTL_REMOVE_CWINAPP_PROFILEREGISTRY

//#define MTL_REMOVE_CFRAMEWND_CONTROLBAR
//#define MTL_REMOVE_CFRAMEWND_DROPFILES
//#define MTL_REMOVE_CFRAMEWND_INDICATOR
//#define MTL_REMOVE_CFRAMEWND_TOOLTIPTEXT
//#define MTL_REMOVE_CFRAMEWND_VIEW

//#define MTL_REMOVE_CVIEW_PRINT

//#define MTL_REMOVE_CDOCMANAGER_MULTITEMPLATE

//#define MTL_REMOVE_CEXCEPTION_DETAIL

//#define MTL_REMOVE_CFILEDIALOG_VISTA

//#define MTL_REMOVE_CASYNCSOCKET_INCLUDE


#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define _SECURE_ATL 0

#define _WTL_TYPES

#if _MSC_VER < 1300	// Visual Studio .NET 2002
#define _MTL_ATL3

#ifndef _WTL_TYPES
#define _WTL_TYPES
#endif

#endif // _MSC_VER



#include <atlbase.h>
#ifndef _WTL_TYPES
#include <atlstr.h>
#include <atltypes.h>
#endif //  _WTL_TYPES
#ifdef _MTL_ATL3
extern CComModule _AtlWinModule;
#define _Module _AtlWinModule
#endif // _MTL_ATL3
#include <atlwin.h>

#ifndef _WTL_TYPES
#define _WTL_NO_CSTRING
#define _WTL_NO_WTYPES
#endif

#include <atlapp.h>
#ifdef ID_EDIT_DELETE
#undef ID_EDIT_DELETE
#endif
#ifdef ID_EDIT_FIND_NEXT
#undef ID_EDIT_FIND_NEXT
#endif
#ifdef ID_EDIT_FIND_PREVIOUS
#undef ID_EDIT_FIND_PREVIOUS
#endif
#ifdef ID_VIEW_REFRESH
#undef ID_VIEW_REFRESH
#endif
#include <atldlgs.h>
#include <atlctrls.h>
#ifdef _WTL_TYPES
#define _ATL_USE_CSTRING_FLOAT
#include <atlmisc.h>
#endif //  _WTL_TYPES

#include <windows.h>
#include <winuser.h>
#include <commctrl.h>

#include <errno.h>
#include <fcntl.h>
#include <float.h>
#include <io.h>
#include <time.h>


//namespace MTL
//{

#if _MSC_VER < 1700	// Visual Studio 2012

__inline void *__cdecl operator new(size_t, void *_P){ return (_P); }

#endif // _MSC_VER

#if _MSC_VER < 1600	// Visual Studio 2010

class CAtlTransactionManager ;

#endif // _MSC_VER


#ifdef _MTL_ATL3

#define ICC_LINK_CLASS                  0x00008000

#ifndef LAYOUT_RTL
#define LAYOUT_LTR                      0x00000000
#define LAYOUT_RTL                      0x00000001
#define NOMIRRORBITMAP                  0x80000000
#endif

#define _O_WTEXT                        0x10000 /* file mode is UTF16 (translated) */

#define PSH_AEROWIZARD                  0x00004000

#define RBBS_USECHEVRON                 0x00000200  // display drop-down button for this band if it's sized smaller than ideal width

#define TVM_SETEXTENDEDSTYLE            (TV_FIRST + 44)
#define TVM_GETEXTENDEDSTYLE            (TV_FIRST + 45)

#ifndef WM_MOUSEWHEEL
#define SM_MOUSEWHEELPRESENT            75
#define SPI_GETWHEELSCROLLLINES         0x0068
#define WHEEL_DELTA                     120
#define WHEEL_PAGESCROLL                (UINT_MAX)
#define WM_MOUSEWHEEL                   0x020A
#endif


typedef LONG LSTATUS;
typedef MSG _ATL_MSG ;
typedef time_t __time64_t ;

#pragma warning(disable : 4200)

struct ATLSTRINGRESOURCEIMAGE
{
	WORD nLength;
	WCHAR achString[];
};

__inline INT clearerr_s(FILE * _File )
{
	clearerr(_File) ;
	return 0 ;
}

__inline __time64_t _mktime64(struct tm *timeptr)
{
	return mktime(timeptr) ;
}

__inline INT _localtime64_s(struct tm* _tm, const __time64_t* time)
{
	*_tm = *localtime(time) ;
	return 0 ;
}

__inline __time64_t _time64(__time64_t* timer)
{
	return time(timer) ;
} 

__inline BOOL WINAPI GetFileSizeEx(HANDLE hFile, PLARGE_INTEGER lpFileSize)
{
	DWORD dwSizeHigh ;
	DWORD dwSize = GetFileSize(hFile, &dwSizeHigh) ;

	lpFileSize->LowPart = dwSize ;
	lpFileSize->HighPart = dwSizeHigh ;
	return TRUE ;
}

#ifndef MTL_REMOVE_CFILEDIALOG_VISTA
#define MTL_REMOVE_CFILEDIALOG_VISTA
#endif

#endif // _MTL_ATL3


#include "mtlapp.h"
#include "mtlappd.h"

#ifdef _MTL_ATL3
#include <atlcom.h>
#include <atlhost.h>
#endif
#include <exdispid.h>
#include "mtlviewht.h"

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "winspool.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "Shlwapi.lib")

#ifndef MTL_REMOVE_CASYNCSOCKET_INCLUDE
#ifndef _WINSOCKAPI_
#include <winsock.h>
#endif

#include "mtlsock.h"

#pragma comment(lib, "wsock32.lib")
#endif // MTL_REMOVE_CASYNCSOCKET_INCLUDE


#ifndef _MTL_ATL3

// not supported

class CDialogEx : public CDialog
{
	DECLARE_DYNAMIC(CDialogEx)

	public:
		CDialogEx(UINT nIDTemplate, CWnd* pParent = NULL) : CDialog(nIDTemplate, pParent)
		{
		}

	public:

};

class CBasePane : public CToolBar
{
	public:

	public:

		//DWORD m_dwStyle ;

	public:
		virtual DWORD GetPaneStyle() const { return m_dwStyle; }

		virtual void SetPaneAlignment(DWORD dwAlignment)
		{
			m_dwStyle &= ~(CBRS_ALIGN_ANY);
			m_dwStyle |= dwAlignment;
		}

		virtual void SetPaneStyle(DWORD dwNewStyle) { m_dwStyle = dwNewStyle; }
};


#define  AFX_DEFAULT_PANE_STYLE  (AFX_CBRS_FLOAT | AFX_CBRS_CLOSE)
#define  AFX_DEFAULT_TOOLBAR_STYLE   (WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_HIDE_INPLACE)


class CMFCToolBar : public CBasePane
{
	public:

	public:
		virtual BOOL Create(CWnd* pParentWnd, DWORD dwStyle = AFX_DEFAULT_TOOLBAR_STYLE, UINT nID = AFX_IDW_TOOLBAR)
		{
			return CreateEx(pParentWnd, TBSTYLE_FLAT, dwStyle, CRect(1, 1, 1, 1), nID);
		}


		static BOOL& GetAltCustomization()
		{
			static BOOL bAltCustomization ;
			return bAltCustomization ;
		}

		//------------------------------------------------------
		// Enable/disable quick customization mode ("Alt+drag"):
		//------------------------------------------------------
		static void __stdcall EnableQuickCustomization(BOOL bEnable = TRUE) { GetAltCustomization() = bEnable; }

		void EnableCustomizeButton(BOOL bEnable, int iCustomizeCmd, const CString& strCustomizeText, BOOL bQuickCustomize = TRUE)
		{
		}

		void EnableCustomizeButton(BOOL bEnable, int iCustomizeCmd, UINT uiCustomizeTextResId, BOOL bQuickCustomize = TRUE)
		{
			CString strCustomizeText;
			ENSURE(strCustomizeText.LoadString(uiCustomizeTextResId));

			EnableCustomizeButton(bEnable, iCustomizeCmd, strCustomizeText, bQuickCustomize);
		}

		_INLINE HBITMAP AfxLoadColorBitmap(HINSTANCE hInst, HRSRC hRsrc)
		{
			HGLOBAL hglb;
			if ((hglb = LoadResource(hInst, hRsrc)) == NULL)
			{
				return NULL;
			}

			LPBITMAPINFOHEADER lpBitmap = (LPBITMAPINFOHEADER)LockResource(hglb);
			if ((lpBitmap == NULL) || (lpBitmap->biBitCount <= 8))
			{
				return NULL;
			}

			// make copy of BITMAPINFOHEADER so we can modify the color table
			UINT nSize = lpBitmap->biSize ;
			int nWidth = (int)lpBitmap->biWidth;
			int nHeight = (int)lpBitmap->biHeight;
			HDC hDCScreen = ::GetDC(NULL);
			HBITMAP hbm = ::CreateCompatibleBitmap(hDCScreen, nWidth, nHeight);

			if (hbm != NULL)
			{
				HDC hDCGlyphs = ::CreateCompatibleDC(hDCScreen);
				HBITMAP hbmOld = (HBITMAP)::SelectObject(hDCGlyphs, hbm);

				LPBYTE lpBits = (LPBYTE)(lpBitmap + 1);

				StretchDIBits(hDCGlyphs, 0, 0, nWidth, nHeight, 0, 0, nWidth, nHeight,
					lpBits, (LPBITMAPINFO)lpBitmap, DIB_RGB_COLORS, SRCCOPY);
				SelectObject(hDCGlyphs, hbmOld);
				::DeleteDC(hDCGlyphs);
			}
			::ReleaseDC(NULL, hDCScreen);

			// free copy of bitmap info struct and resource itself
			::FreeResource(hglb);

			return hbm;
		}

		BOOL LoadBitmap(LPCTSTR lpszResourceName)
		{
			ASSERT_VALID(this);
			ASSERT(lpszResourceName != NULL);

			// determine location of the bitmap in resource fork
			HINSTANCE hInstImageWell = AfxFindResourceHandle(lpszResourceName, RT_BITMAP);
			HRSRC hRsrcImageWell = ::FindResource(hInstImageWell, lpszResourceName, RT_BITMAP);
			if (hRsrcImageWell == NULL)
				return FALSE;

			// load the bitmap
			HBITMAP hbmImageWell = AfxLoadSysColorBitmap(hInstImageWell, hRsrcImageWell);
			if (hbmImageWell == NULL)
				hbmImageWell = AfxLoadColorBitmap(hInstImageWell, hRsrcImageWell);

			// tell common control toolbar about the new bitmap
			if (!AddReplaceBitmap(hbmImageWell))
				return FALSE;

			// remember the resource handles so the bitmap can be recolored if necessary
			m_hInstImageWell = hInstImageWell;
			m_hRsrcImageWell = hRsrcImageWell;
			return TRUE;
		}

		BOOL LoadBitmap(UINT nIDResource) { return LoadBitmap(MAKEINTRESOURCE(nIDResource)); }

		BOOL LoadToolBar(LPCTSTR lpszResourceName)
		{
			ASSERT_VALID(this);
			ASSERT(lpszResourceName != NULL);

			// determine location of the bitmap in resource fork
			HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_TOOLBAR);
			HRSRC hRsrc = ::FindResource(hInst, lpszResourceName, RT_TOOLBAR);
			if (hRsrc == NULL)
				return FALSE;

			HGLOBAL hGlobal = LoadResource(hInst, hRsrc);
			if (hGlobal == NULL)
				return FALSE;

			CToolBarData* pData = (CToolBarData*)LockResource(hGlobal);
			if (pData == NULL)
				return FALSE;
			ASSERT(pData->wVersion == 1);

			UINT* pItems = new UINT[pData->wItemCount];
			for (int i = 0; i < pData->wItemCount; i++)
				pItems[i] = pData->items()[i];
			BOOL bResult = SetButtons(pItems, pData->wItemCount);
			delete[] pItems;

			if (bResult)
			{
				// set new sizes of the buttons
				CSize sizeImage(pData->wWidth, pData->wHeight);
				CSize sizeButton(pData->wWidth + 7, pData->wHeight + 7);
				SetSizes(sizeButton, sizeImage);

				// load bitmap now that sizes are known by the toolbar control
				bResult = LoadBitmap(lpszResourceName);
			}

			UnlockResource(hGlobal);
			FreeResource(hGlobal);

			return bResult;
		}

		BOOL LoadToolBar(UINT nIDResource) { return LoadToolBar(MAKEINTRESOURCE(nIDResource)); }

};

class CMFCStatusBar : public CStatusBar
{
};

class CMFCToolBarImages
{
};

#define  nTextMargin  (3);

class CMFCMenuBar : public CMFCToolBar
{
	public:
		CMFCMenuBar()
		{
			m_hMenu = NULL;
			m_pParentWnd = NULL;

			m_iHighlighted = -1 ;
			m_iLastHighlighted = -1 ;
			m_iMouseHighlighted = -1 ;
			m_bTrackPopupMenu = FALSE ;

			m_bPostTrack = FALSE ;
			m_iPostHighlighted = -1;
		}

	public:
		HMENU m_hMenu;
		CWnd* m_pParentWnd;

		INT	m_iHighlighted ;
		INT	m_iLastHighlighted;
		INT m_iMouseHighlighted;
		BOOL m_bTrackPopupMenu;

		BOOL m_bPostTrack ;
		INT m_iPostHighlighted;
		POINT m_ptPostTrack;

	public:
		virtual LRESULT WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
		{
			if (((nMsg == WM_LBUTTONDOWN) && (m_bTrackPopupMenu == FALSE)) || ((nMsg == WM_MOUSEMOVE) && (m_bTrackPopupMenu)))
			{
				INT xPos = GET_X_LPARAM(lParam);
				INT yPos = GET_Y_LPARAM(lParam);

				CPoint pos = { xPos, yPos } ;

				if (nMsg == WM_MOUSEMOVE)
					ScreenToClient(&pos) ;

				CToolBarCtrl& rCToolBarCtrl = GetToolBarCtrl();

				INT iButton = rCToolBarCtrl.HitTest(&pos);
				if ((0 <= iButton) && (iButton < rCToolBarCtrl.GetButtonCount()))
				{
					//TRACE("WindowProc: hWnd=%x, nMsg=%d, x=%d, y=%d, iButton=%d, iHighlighted=%d, iLastHighlighted=%d, bTrackPopupMenu=%d\n", 
					//	m_hWnd, nMsg, xPos, yPos, iButton, m_iHighlighted, m_iLastHighlighted, m_bTrackPopupMenu);

					INT iHighlighted = (nMsg == WM_LBUTTONDOWN) ? m_iLastHighlighted : m_iMouseHighlighted ;

					if (iHighlighted != iButton)
					{
						TrackPopupMenu(iButton, (nMsg == WM_MOUSEMOVE));
					}
					else if (m_bTrackPopupMenu == FALSE)
					{
						m_iLastHighlighted = -1 ;
					}

					m_iMouseHighlighted = iButton;

					return 0;
				}

				if (nMsg != WM_MOUSEMOVE)
					m_iMouseHighlighted = -1 ;
			}
			else if (nMsg == WM_INITMENUPOPUP)
			{
				//TRACE("WindowProc: WM_INITMENUPOPUP iHighlighted=%d, bTrackPopupMenu=%d\n", m_iHighlighted, m_bTrackPopupMenu);

				m_pParentWnd->WindowProc(nMsg, wParam, lParam);
			}
			else if (nMsg == WM_EXITMENULOOP)
			{
				m_iLastHighlighted = Hilighted(-1);

				//TRACE("WindowProc: WM_EXITMENULOOP iHighlighted=%d, iLastHighlighted=%d, bTrackPopupMenu=%d\n", m_iHighlighted, m_iLastHighlighted, m_bTrackPopupMenu);
			}
			else if (nMsg == WM_NCPAINT)
			{
				DWORD dwStyle = m_dwStyle;
				m_dwStyle &= ~CBRS_ORIENT_VERT ;
				m_dwStyle |= CBRS_ORIENT_HORZ;
				OnNcPaint();
				m_dwStyle = dwStyle;
				return 0;
			}

			//TRACE("WindowProc: hWnd=%x, nMsg=%d, iHighlighted=%d, bTrackPopupMenu=%d\n",
			//		m_hWnd, nMsg, m_iHighlighted, m_bTrackPopupMenu);


			// otherwise, just handle in default way
			LRESULT lResult = CMFCToolBar::WindowProc(nMsg, wParam, lParam);
			return lResult;
		}

		virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode)
		{
			return CalcFixedLayout(dwMode & LM_STRETCH, dwMode & LM_HORZDOCK);
		}

		virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz)
		{
			CToolBarCtrl& rCToolBarCtrl = GetToolBarCtrl();

			CSize size(0, 0) ;

			CString csText ;

			NONCLIENTMETRICS info ;
			info.cbSize = sizeof(NONCLIENTMETRICS) ;

			::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, info.cbSize, &info, 0);

			CFont fontRegular;
			fontRegular.CreateFontIndirect(&info.lfMenuFont);

			CClientDC dc(m_pParentWnd);

			HFONT hFont = (HFONT)::SelectObject(dc.GetSafeHandle(), (HGDIOBJ)fontRegular.GetSafeHandle());

			for (INT i = 0; i < rCToolBarCtrl.GetButtonCount(); i++)
			{
				LPCTSTR pszText = csText.GetBufferSetLength(256) ;

				DefWindowProc(TB_GETSTRING, MAKEWPARAM(256, i), (LPARAM)pszText) ;

				csText.ReleaseBuffer() ;
				csText.Remove(_T('&'));

				CSize siText = dc.GetTextExtent(csText) ;

				siText.cx += 3 * nTextMargin;
				siText.cy += 3 * nTextMargin;

				size.cx += siText.cx ;
				if (size.cy < siText.cy)
					size.cy = siText.cy ;
			}

			::SelectObject(dc.GetSafeHandle(), (HGDIOBJ)hFont);

			CRect cRect ;
			CalcInsideRect(cRect, TRUE) ;

			size.cx -= cRect.Width() - 3 * nTextMargin ;
			size.cy -= cRect.Height();

			return size ;
		}

		BOOL Create(CWnd* pParentWnd, DWORD dwStyle = AFX_DEFAULT_TOOLBAR_STYLE, UINT nID = AFX_IDW_MENUBAR)
		{
			// default image sizes

			m_pParentWnd = pParentWnd;
			BOOL bRet = CMFCToolBar::Create(pParentWnd, dwStyle, nID);

			CreateFromMenu(pParentWnd->GetMenu()->GetSafeHandle()) ;

			SetBorders() ;
			SetHeight(m_sizeButton.cy + afxData.cyBorder2);

			return bRet ;
		}

		void CreateFromMenu(HMENU hMenu, BOOL bDefaultMenu = FALSE, BOOL bForceUpdate = FALSE)
		{
			ASSERT_VALID(this);

			if (GetFocus() == this)
			{
				GetParentFrame()->SetFocus();
			}

			if (m_hMenu != hMenu)
			{
				m_hMenu = hMenu;

				CMenu* pMenu = CMenu::FromHandle(hMenu);
				if (pMenu == NULL)
					return;

				CToolBarCtrl& rCToolBarCtrl = GetToolBarCtrl();

				for (int i = rCToolBarCtrl.GetButtonCount() - 1 ; i >= 0 ; i--)
					rCToolBarCtrl.DeleteButton(i) ;

				//rCToolBarCtrl.SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
				rCToolBarCtrl.SetBitmapSize(CSize(0, 0)) ;

				TBBUTTON button; memset(&button, 0, sizeof(TBBUTTON));

				int iCount = pMenu->GetMenuItemCount();
				for (int i = 0; i < iCount; i++)
				{
					UINT uiID = pMenu->GetMenuItemID(i);

					CString strText;
					pMenu->GetMenuString(i, strText, MF_BYPOSITION);

					INT iIndex = (INT)DefWindowProc(TB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strText);

					button.iBitmap = I_IMAGENONE ;
					button.idCommand = (uiID != 0) ? uiID : 0;
					button.fsState = TBSTATE_ENABLED ;
					//button.fsStyle = (uiID != 0) ? TBSTYLE_DROPDOWN | BTNS_AUTOSIZE : TBSTYLE_SEP | BTNS_AUTOSIZE;
					button.fsStyle = (uiID != 0) ? BTNS_AUTOSIZE : TBSTYLE_SEP | BTNS_AUTOSIZE;
					button.iString = iIndex;

					DefWindowProc(TB_ADDBUTTONS, 1, (LPARAM)&button);
				}

			}
		}

		//
		// Special TrackPopupMenu
		//


		INT Hilighted(INT iButton)
		{
			INT iLastHighlighted = m_iHighlighted ;

			if (iLastHighlighted != iButton)
			{
#if 0
				INT iButtonCount = GetToolBarCtrl().GetButtonCount() ;

				if ((0 <= iLastHighlighted) && (iLastHighlighted < iButtonCount))
					DefWindowProc(TB_PRESSBUTTON, iLastHighlighted, (LPARAM)MAKELONG(FALSE, 0));

				if ((0 <= iButton) && (iButton < iButtonCount))
					DefWindowProc(TB_PRESSBUTTON, iButton, (LPARAM)MAKELONG(TRUE, 0));
#endif
				m_iHighlighted = iButton ;
			}

			return iLastHighlighted ;
		}

		BOOL IsTrackPopupMenu(HMENU hMenu, UINT uKey, UINT uNest = 0)
		{
			INT iCount = GetMenuItemCount(hMenu) ;

			for (INT i = 0 ; i < iCount ; i++)
			{
				UINT uState = GetMenuState(hMenu, i, MF_BYPOSITION) ;

				if (uState & MF_HILITE)
				{
					if (uState & MF_POPUP)
					{
						HMENU hSubMenu = GetSubMenu(hMenu, i) ;
						if (hSubMenu != NULL)
						{
							BOOL bRet = IsTrackPopupMenu(hSubMenu, uKey, ++uNest) ;

							return bRet ;
						}

						if (uKey == VK_RIGHT)
							return TRUE;
					}

					if (uNest != 0)
					{
						if (uKey == VK_LEFT)
							return TRUE;

						if (uKey == VK_RIGHT)
							return FALSE;
					}

					break;
				}
			}

			if ((uKey == VK_RIGHT) && (uNest != 0))
				return TRUE;

			return FALSE;
		}

		typedef struct _CreateWindowParam
		{
			HHOOK	hHookOldWndFilter;
			CMFCMenuBar* pCWnd;
			HWND hMenuWnd;
		} CREATEWINDOWPARAM;

		static CREATEWINDOWPARAM& GetHookOldWndFilter()
		{
			static CREATEWINDOWPARAM cwp = { NULL, NULL, NULL };

			return cwp;
		}

		static LRESULT CALLBACK WndFilterHook(int nCode, WPARAM wParam, LPARAM lParam)
		{
			CREATEWINDOWPARAM& cwp = GetHookOldWndFilter();

			if (nCode == MSGF_MENU)
			{
				MSG* pMsg = (MSG *)lParam;
				if (pMsg->message == WM_KEYDOWN)
				{
					//TRACE("WndFilterHook: CMFCMenuBarWnd=%x, hParentWnd=%x, hWnd=%x, nMsg=%d, wParam=%d, lParam=%d\n", 
					//	cwp.pCWnd->m_hWnd, ::GetParent(pMsg->hwnd), pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam);

					UINT uKey = (UINT)pMsg->wParam ;
					HWND hParentWnd = ::GetParent(pMsg->hwnd) ;

					if ((uKey == VK_LEFT) || (uKey == VK_RIGHT))
					{
						CMFCMenuBar* pCMFCMenuBar = cwp.pCWnd ;

						INT iButton = pCMFCMenuBar->m_iHighlighted;

						HMENU hMenu = GetSubMenu(pCMFCMenuBar->m_hMenu, iButton);

						if (pCMFCMenuBar->IsTrackPopupMenu(hMenu, uKey) == FALSE)
						{
							CToolBarCtrl& rCToolBarCtrl = cwp.pCWnd->GetToolBarCtrl();

							if (uKey == VK_LEFT)
							{
								if (--iButton < 0)
									iButton = rCToolBarCtrl.GetButtonCount() - 1 ;
							
							}
							else
							{
								if (++iButton >= rCToolBarCtrl.GetButtonCount())
									iButton = 0 ;
							}

							pCMFCMenuBar->TrackPopupMenu(iButton, TRUE);
						}
					}
					else if (uKey == VK_ESCAPE)
					{
						//cwp.pCWnd->m_iLastHighlighted = -1 ; //cwp.pCWnd->m_iHighlighted;
						cwp.pCWnd->Hilighted(-1);
					}
				}
				else if (pMsg->message == WM_MOUSEMOVE)
				{
					if (cwp.pCWnd->m_bTrackPopupMenu)
						cwp.pCWnd->WindowProc(pMsg->message, pMsg->wParam, pMsg->lParam);
				}

			}

			return ::CallNextHookEx(cwp.hHookOldWndFilter, nCode, wParam, lParam);
		}

		BOOL TrackPopupMenu(INT iButton, BOOL bPost = FALSE)
		{
			if (m_iHighlighted >= 0)
			{
				//::EndMenu();
				DefWindowProc(WM_CANCELMODE, 0, 0);
			}

			if (iButton < 0)
			{
				Hilighted(iButton);

				return TRUE;
			}

			RECT rRect;
			GetItemRect(iButton, &rRect);

			ClientToScreen(&rRect); // TrackPopupMenu uses screen coords

			if (bPost)
			{
				TRACE("TrackPopupMenu: Post hWnd=%x, x=%d, y=%d, iButton=%d\n", m_hWnd, rRect.left, rRect.bottom, iButton);

				m_bPostTrack = TRUE ;
				m_ptPostTrack = { rRect.left, rRect.bottom };
				m_iPostHighlighted = iButton ;

				return TRUE;
			}

			CMenu* pMenu = CMenu::FromHandle(m_hMenu);
			CMenu* pPopup = pMenu->GetSubMenu(iButton);

			if (NULL != pPopup)
			{
				m_bTrackPopupMenu = TRUE;

				CREATEWINDOWPARAM& cwp = GetHookOldWndFilter();
				cwp.pCWnd = this ;
				cwp.hHookOldWndFilter = ::SetWindowsHookEx(WH_MSGFILTER, WndFilterHook, NULL, ::GetCurrentThreadId());

				POINT ptTrack = { rRect.left, rRect.bottom } ;

				m_iMouseHighlighted = iButton ;

				do
				{
					Hilighted(iButton);

					pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptTrack.x, ptTrack.y, this);

					if (m_bPostTrack == FALSE)
						break ;

					iButton = m_iPostHighlighted;
					ptTrack = m_ptPostTrack;

					m_bPostTrack = FALSE;
					m_iPostHighlighted = -1;

					pPopup = pMenu->GetSubMenu(iButton);

				} while (TRUE) ;

				m_iMouseHighlighted = -1 ;

				MSG msg ;
				if (PeekMessage(&msg, GetSafeHwnd(), WM_LBUTTONDOWN, WM_LBUTTONDOWN, PM_NOREMOVE) == FALSE)
					m_iLastHighlighted = -1;

				::UnhookWindowsHookEx(cwp.hHookOldWndFilter);

				m_bTrackPopupMenu = FALSE;

				return TRUE ;
			}

			m_iMouseHighlighted = -1 ;
			m_iLastHighlighted = -1;
			Hilighted(-1);

			return FALSE;
		}

};


class CMFCPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CMFCPropertyPage)
};




class CMFCPopupMenu : public CMiniFrameWnd
{
	public:
		CMFCPopupMenu()
		{
			m_pMessageWnd = NULL ;

			m_bAutoDestroy = FALSE;
			m_bTrackMode = FALSE ;
			m_bRightAlign = FALSE ;
		}

	public:

		CWnd*   m_pMessageWnd;

		BOOL m_bAutoDestroy;
		BOOL m_bTrackMode;
		BOOL m_bRightAlign;

		CPoint  m_ptLocation;
		CPoint  m_ptLocationInitial;

	public:
		virtual BOOL Create(CWnd* pWndParent, int x, int y, HMENU hMenu, BOOL bLocked = FALSE, BOOL bOwnMessage = FALSE) // Add: Alex Corazzin (2)
		{
			//AFXPlaySystemSound(AFX_SOUND_MENU_POPUP);

			ENSURE(pWndParent != NULL);

			UINT nClassStyle = CS_SAVEBITS;

			CString strClassName = ::AfxRegisterWndClass(nClassStyle, ::LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_BTNFACE + 1), NULL);

			m_hMenu = hMenu;

			if (x == -1 && y == -1) // Undefined position
			{
				if (pWndParent != NULL)
				{
					CRect rectParent;
					pWndParent->GetClientRect(&rectParent);
					pWndParent->ClientToScreen(&rectParent);

					m_ptLocation = CPoint(rectParent.left + 5, rectParent.top + 5);
				}
				else
				{
					m_ptLocation = CPoint(0, 0);
				}
			}
			else
			{
				m_ptLocation = CPoint(x, y);
			}

			m_ptLocationInitial = m_ptLocation;

			DWORD dwStyle = WS_POPUP;
			dwStyle |= (WS_CAPTION | WS_SYSMENU);

			BOOL bIsAnimate = TRUE ;

			m_pMessageWnd = pWndParent;

			CRect rect(x, y, x, y);
			BOOL bCreated = CMiniFrameWnd::CreateEx(pWndParent->GetExStyle() & WS_EX_LAYOUTRTL, strClassName, m_strCaption,
				dwStyle, rect, pWndParent->GetOwner() == NULL ? pWndParent : pWndParent->GetOwner());
			if (!bCreated)
			{
				return FALSE;
			}

			// Update windows covered by menu:
			UpdateBottomWindows();

			SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

			return TRUE;
		}

		static CMFCPopupMenu*& GetActivePopupMenu()
		{
			static CMFCPopupMenu* pActivePopupMenu ;
			return pActivePopupMenu ;
		}

		static CMFCPopupMenu* __stdcall GetActiveMenu() { return GetActivePopupMenu(); }

		//virtual CMFCPopupMenuBar* GetMenuBar() { return &m_wndMenuBar; }

		CMFCPopupMenu* GetParentPopupMenu() const { return NULL; }


		static void SetForceMenuFocus(BOOL bValue);

		void SetAutoDestroy(BOOL bAutoDestroy = TRUE) { m_bAutoDestroy = bAutoDestroy; }
		void SetRightAlign(BOOL bRightAlign = TRUE) { m_bRightAlign = bRightAlign; }

		void UpdateBottomWindows(BOOL bCheckOnly = FALSE)
		{
			CWnd* pWndMain = GetTopLevelParent();
			if (pWndMain == NULL)
			{
				return;
			}

			pWndMain->UpdateWindow();
			return;
		}
};



class CMFCToolTipCtrl : public CToolTipCtrl
{
	DECLARE_DYNAMIC(CMFCToolTipCtrl)

	public:

	public:

	public:


};

class CMFCToolTipInfo
{
	public:
		CMFCToolTipInfo()
		{
			m_bVislManagerTheme = FALSE;
		}

	public:
		BOOL m_bVislManagerTheme;

	public:

};


class CContextMenuManager
{
	public:
		CContextMenuManager()
		{
			m_bTrackMode = FALSE;
			m_bDontCloseActiveMenu = FALSE;

		}

		virtual ~CContextMenuManager() {}

		// Opreations:
	public:
		// Attributes:

		BOOL	m_bTrackMode ;
		BOOL	m_bDontCloseActiveMenu ;

		CMap<UINT, UINT, HMENU, HMENU>       m_Menus;
		CMap<CString, LPCTSTR, HMENU, HMENU> m_MenuNames;

	public:

		BOOL AddMenu(LPCTSTR lpszName, UINT uiMenuResId)
		{
			ENSURE(lpszName != NULL);

			CMenu menu;
			if (!menu.LoadMenu(uiMenuResId))
			{
				ASSERT(FALSE);
				return FALSE;
			}

			HMENU hExMenu;
			if (m_Menus.Lookup(uiMenuResId, hExMenu))
			{
				// Menu with the same name is already exist!
				return FALSE;
			}

			HMENU hMenu = menu.Detach();

			m_Menus.SetAt(uiMenuResId, hMenu);
			m_MenuNames.SetAt(lpszName, hMenu);

			return TRUE;
		}

		BOOL AddMenu(UINT uiMenuNameResId, UINT uiMenuResId)
		{
			CString strMenuName;
			ENSURE(strMenuName.LoadString(uiMenuNameResId));

			return AddMenu(strMenuName, uiMenuResId);
		}

		BOOL ShowPopupMenu(UINT uiMenuResId, int x, int y, CWnd* pWndOwner, BOOL bOwnMessage = FALSE, BOOL bRightAlign = FALSE)
		{
			HMENU hMenu;
			if (!m_Menus.Lookup(uiMenuResId, hMenu) || hMenu == NULL)
			{
				return FALSE;
			}

			if (x == -1 && y == -1 && // Undefined position
				pWndOwner != NULL)
			{
				CRect rectParent;
				pWndOwner->GetClientRect(&rectParent);
				pWndOwner->ClientToScreen(&rectParent);

				x = rectParent.left + 5;
				y = rectParent.top + 5;
			}

			HMENU hmenuPopup = ::GetSubMenu(hMenu, 0);
			if (hmenuPopup == NULL)
			{
				return FALSE;
			}

			return ShowPopupMenu(hmenuPopup, x, y, pWndOwner, bOwnMessage, TRUE, bRightAlign) != NULL;
		}

		CMFCPopupMenu* ShowPopupMenu(HMENU hmenuPopup, int x, int y, CWnd* pWndOwner, BOOL bOwnMessage = FALSE, BOOL bAutoDestroy = TRUE, BOOL bRightAlign = FALSE)
		{
			if (pWndOwner != NULL && pWndOwner->IsKindOf(RUNTIME_CLASS(CDialogEx)) && !bOwnMessage)
			{
				// CDialogEx should own menu messages
				ASSERT(FALSE);
				return NULL;
			}

			if (pWndOwner != NULL && pWndOwner->IsKindOf(RUNTIME_CLASS(CMFCPropertyPage)) && !bOwnMessage)
			{
				// CMFCPropertyPage should own menu messages
				ASSERT(FALSE);
				return NULL;
			}

			if (m_bTrackMode)
			{
				bOwnMessage = TRUE;
			}

			if (!bOwnMessage)
			{
				while (pWndOwner != NULL && pWndOwner->GetStyle() & WS_CHILD)
				{
					pWndOwner = pWndOwner->GetParent();
				}
			}

			CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;
			pPopupMenu->SetAutoDestroy(FALSE);

			pPopupMenu->m_bTrackMode = m_bTrackMode;
			pPopupMenu->SetRightAlign(bRightAlign);

			CMFCPopupMenu* pMenuActive = CMFCPopupMenu::GetActiveMenu();
			if (!m_bDontCloseActiveMenu && pMenuActive != NULL)
			{
				pMenuActive->SendMessage(WM_CLOSE);
			}

			if (!pPopupMenu->Create(pWndOwner, x, y, hmenuPopup, FALSE, bOwnMessage))
			{
				return NULL;
			}

			return pPopupMenu;
		}
};


class CKeyboardManager
{
	public:

	public:

};


class CTooltipManager
{
	public:
		CTooltipManager() ;

		virtual ~CTooltipManager();

	public:
		CMFCToolTipInfo   m_Params[AFX_TOOLTIP_TYPES];
		CRuntimeClass*    m_pRTC[AFX_TOOLTIP_TYPES];
		CList<HWND, HWND> m_lstOwners;

	public:
		void SetTooltipParams(UINT nTypes, CRuntimeClass* pRTC, CMFCToolTipInfo* pParams)
		{
			if (pRTC == NULL || !pRTC->IsDerivedFrom(RUNTIME_CLASS(CMFCToolTipCtrl)))
			{
				if (pParams != NULL)
				{
					// Parameters can be used with CMFCToolTipCtrl class only!
					ASSERT(FALSE);
					pParams = NULL;
				}
			}

			CMFCToolTipInfo defaultParams;

			UINT nType = AFX_TOOLTIP_TYPE_DEFAULT;

			for (int i = 0; i < AFX_TOOLTIP_TYPES; i++)
			{
				if ((nType & nTypes) != 0)
				{
					if (pParams == NULL)
					{
						m_Params[i] = defaultParams;
					}
					else
					{
						m_Params[i] = *pParams;
					}

					m_pRTC[i] = pRTC;
				}

				nType <<= 1;
			}

			for (POSITION pos = m_lstOwners.GetHeadPosition(); pos != NULL;)
			{
				HWND hwndOwner = m_lstOwners.GetNext(pos);

				if (::IsWindow(hwndOwner))
				{
					::SendMessage(hwndOwner, AFX_WM_UPDATETOOLTIPS, (WPARAM)nTypes, 0);
				}
			}
		}

};



_INLINE CString AFXGetRegPath(LPCTSTR lpszPostFix, LPCTSTR lpszProfileName = NULL)
{
	ENSURE(lpszPostFix != NULL);

	CString strReg;

	if (lpszProfileName != NULL && lpszProfileName[0] != 0)
	{
		strReg = lpszProfileName;
	}
	else
	{
		CWinApp* pApp = AfxGetApp();
		ASSERT_VALID(pApp);

		ENSURE(AfxGetApp()->m_pszRegistryKey != NULL);
		ENSURE(AfxGetApp()->m_pszProfileName != NULL);

		strReg = _T("SOFTWARE\\");

		CString strRegKey = pApp->m_pszRegistryKey;
		if (!strRegKey.IsEmpty())
		{
			strReg += strRegKey;
			strReg += _T("\\");
		}

		strReg += pApp->m_pszProfileName;
		strReg += _T("\\");
		strReg += lpszPostFix;
		strReg += _T("\\");
	}

	return strReg;
}

class CWinAppEx : public CWinApp
{
	public:
		CWinAppEx() : CWinApp()
		{
			m_bTooltipManagerAutocreated = FALSE ;
			m_bKeyboardManagerAutocreated = FALSE ;
			m_bContextMenuManagerAutocreated = FALSE ;

			m_afxContextMenuManager = NULL;
			m_afxKeyboardManager = NULL ;
			m_afxTooltipManager = NULL;


		}

	public:
		CString m_strRegSection;

		BOOL m_bContextMenuManagerAutocreated ;
		BOOL m_bKeyboardManagerAutocreated ;
		BOOL m_bTooltipManagerAutocreated ;

		CContextMenuManager*	m_afxContextMenuManager ;
		CKeyboardManager*		m_afxKeyboardManager;
		CTooltipManager*		m_afxTooltipManager;

		// CMFCPopupMenu
		BOOL m_bForceMenuFocus;  // Menu takes a focus when activated

		// CDockingManager
		AFX_DOCK_TYPE m_dockModeGlobal;
		AFX_SMARTDOCK_THEME m_SDTheme;

	public:
		CString GetRegSectionPath(LPCTSTR szSectionAdd = _T(""))
		{
			CString strSectionPath = ::AFXGetRegPath(m_strRegSection);
			if (szSectionAdd != NULL && _tcslen(szSectionAdd) != 0)
			{
				strSectionPath += szSectionAdd;
				strSectionPath += _T("\\");
			}

			return strSectionPath;
		}

		int GetSectionInt(LPCTSTR lpszSubSection, LPCTSTR lpszEntry, int nDefault = 0)
		{
			ENSURE(lpszSubSection != NULL);
			ENSURE(lpszEntry != NULL);

			int nRet = nDefault;

			CString strSection = GetRegSectionPath(lpszSubSection);

			CSettingsStoreSP regSP;
			CSettingsStore& reg = regSP.Create(FALSE, TRUE);

			if (reg.Open(strSection))
			{
				int nReg = 0;
				if (reg.Read(lpszEntry, nReg))
				{
					nRet = nReg;
				}
			}
			return nRet;
		}
		CString GetSectionString(LPCTSTR lpszSubSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault = _T(""))
		{
			ENSURE(lpszSubSection != NULL);
			ENSURE(lpszEntry != NULL);
			ENSURE(lpszDefault != NULL);

			CString strRet = lpszDefault;

			CString strSection = GetRegSectionPath(lpszSubSection);

			CSettingsStoreSP regSP;
			CSettingsStore& reg = regSP.Create(FALSE, TRUE);

			if (reg.Open(strSection))
			{
				CString strReg;
				if (reg.Read(lpszEntry, strReg))
				{
					strRet = strReg;
				}
			}
			return strRet;
		}
		BOOL WriteSectionInt(LPCTSTR lpszSubSection, LPCTSTR lpszEntry, int nValue)
		{
			ENSURE(lpszSubSection != NULL);
			ENSURE(lpszEntry != NULL);

			CString strSection = GetRegSectionPath(lpszSubSection);

			CSettingsStoreSP regSP;
			CSettingsStore& reg = regSP.Create(FALSE, FALSE);

			if (reg.CreateKey(strSection))
			{
				return reg.Write(lpszEntry, nValue);
			}
			return FALSE;
		}
		BOOL WriteSectionString(LPCTSTR lpszSubSection, LPCTSTR lpszEntry, LPCTSTR lpszValue)
		{
			ENSURE(lpszSubSection != NULL);
			ENSURE(lpszEntry != NULL);
			ENSURE(lpszValue != NULL);

			CString strSection = GetRegSectionPath(lpszSubSection);

			CSettingsStoreSP regSP;
			CSettingsStore& reg = regSP.Create(FALSE, FALSE);

			if (reg.CreateKey(strSection))
			{
				return reg.Write(lpszEntry, lpszValue);
			}
			return FALSE;
		}

		int GetInt(LPCTSTR lpszEntry, int nDefault = 0) { return GetSectionInt(_T(""), lpszEntry, nDefault); }
		CString GetString(LPCTSTR lpszEntry, LPCTSTR lpzDefault = _T("")) { return GetSectionString(_T(""), lpszEntry, lpzDefault); }
		BOOL WriteInt(LPCTSTR lpszEntry, int nValue) { return WriteSectionInt(_T(""), lpszEntry, nValue); }
		BOOL WriteString(LPCTSTR lpszEntry, LPCTSTR lpszValue) { return WriteSectionString(_T(""), lpszEntry, lpszValue); }


		BOOL InitContextMenuManager()
		{ 
			if (m_afxContextMenuManager != NULL)
			{
				ASSERT(FALSE);
				return FALSE;
			}

			m_afxContextMenuManager = new CContextMenuManager();
			m_bContextMenuManagerAutocreated = TRUE;

			return TRUE;
		}

		BOOL InitKeyboardManager()
		{
			if (m_afxKeyboardManager != NULL)
			{
				ASSERT(FALSE);
				return FALSE;
			}

			m_afxKeyboardManager = new CKeyboardManager();
			m_bKeyboardManagerAutocreated = TRUE;

			return TRUE;
		}

		BOOL InitTooltipManager()
		{ 
			if (m_afxTooltipManager != NULL)
			{
				ASSERT(FALSE);
				return FALSE;
			}

			m_afxTooltipManager = new CTooltipManager();
			m_bTooltipManagerAutocreated = TRUE;
			return TRUE;
		}

		CContextMenuManager* GetContextMenuManager()
		{
			if (m_afxContextMenuManager == NULL)
			{
				InitContextMenuManager();
			}

			ASSERT_VALID(afxContextMenuManager);
			return m_afxContextMenuManager;
		}

		CKeyboardManager* GetKeyboardManager()
		{
			if (m_afxKeyboardManager == NULL)
			{
				InitKeyboardManager();
			}

			ASSERT_VALID(afxKeyboardManager);
			return m_afxKeyboardManager;
		}

		CTooltipManager* GetTooltipManager()
		{
			if (m_afxTooltipManager == NULL)
			{
				InitTooltipManager();
			}

			ASSERT_VALID(afxTooltipManager);
			return m_afxTooltipManager;
		}


};

_INLINE CWinAppEx* AfxGetAppEx()
{
	return (CWinAppEx*)AfxGetApp();
}

_INLINE void CMFCPopupMenu::SetForceMenuFocus(BOOL bValue) 
{ 
	AfxGetAppEx()->m_bForceMenuFocus = bValue; 
}

_INLINE CTooltipManager::CTooltipManager()
{
	AfxGetAppEx()->m_afxTooltipManager = this;

	for (int i = 0; i < AFX_TOOLTIP_TYPES; i++)
	{
		m_pRTC[i] = NULL;
	}
}

_INLINE CTooltipManager::~CTooltipManager()
{
	AfxGetAppEx()->m_afxTooltipManager = NULL;
}


class CFrameImpl
{
	public:
		CFrameImpl() : m_uiUserToolbarFirst((UINT)-1), m_uiUserToolbarLast((UINT)-1), m_uiControlbarsMenuEntryID(0), m_bViewMenuShowsToolbarsOnly(FALSE)
		{
			

		}


	public:
		UINT m_uiUserToolbarFirst;
		UINT m_uiUserToolbarLast;

		UINT m_uiControlbarsMenuEntryID;

		BOOL m_bViewMenuShowsToolbarsOnly;

		CString            m_strControlBarRegEntry;

	public:
		void InitUserToolbars(LPCTSTR lpszRegEntry, UINT uiUserToolbarFirst, UINT uiUserToolbarLast)
		{
			ASSERT(uiUserToolbarLast >= uiUserToolbarFirst);

			if (uiUserToolbarFirst == (UINT)-1 || uiUserToolbarLast == (UINT)-1)
			{
				ASSERT(FALSE);
				return;
			}

			m_uiUserToolbarFirst = uiUserToolbarFirst;
			m_uiUserToolbarLast = uiUserToolbarLast;

			// Get Path automatically from CWinAppEx if needed
			CWinAppEx* pApp = AfxGetAppEx();

			m_strControlBarRegEntry = (lpszRegEntry == NULL) ? (pApp != NULL ? pApp->GetRegSectionPath() : _T("")) : lpszRegEntry;
		}

		void SetControlbarsMenuId(UINT uiViewMenuEntryID, BOOL bViewMenuShowsToolbarsOnly)
		{
			m_uiControlbarsMenuEntryID = uiViewMenuEntryID;
			m_bViewMenuShowsToolbarsOnly = bViewMenuShowsToolbarsOnly;
		}

};

struct AFX_DOCKSITE_INFO
{
	DWORD          m_dwBarAlignment;
	CRuntimeClass* pDockBarRTC;
};

class CDockingManager
{
	public:
		CDockingManager()
		{
			m_dwEnabledDockBars = 0;
			m_dwEnabledSlideBars = 0;

			m_bControlBarsContextMenu = FALSE;
			m_bControlBarsContextMenuToolbarsOnly = FALSE;
			m_uiCustomizeCmd = 0;
		}


	public:
		DWORD m_dwEnabledDockBars;
		DWORD m_dwEnabledSlideBars;

		// Controlbar context menu attributes:
		BOOL m_bControlBarsContextMenu;
		BOOL m_bControlBarsContextMenuToolbarsOnly;
		UINT m_uiCustomizeCmd;
		CString m_strCustomizeText;

	public:
		void DockPane(CBasePane* pBar, UINT nDockBarID = 0, LPCRECT lpRect = NULL)
		{
		}

		void EnablePaneContextMenu(BOOL bEnable, UINT uiCustomizeCmd, const CString& strCustomizeText, BOOL bToolbarsOnly)
		{
			m_bControlBarsContextMenu = bEnable;
			m_bControlBarsContextMenuToolbarsOnly = bToolbarsOnly;
			m_uiCustomizeCmd = uiCustomizeCmd;
			m_strCustomizeText = strCustomizeText;
		}

		BOOL EnableDocking(DWORD dwStyle)
		{
			//AFX_DOCKSITE_INFO info;
			if (dwStyle & CBRS_ALIGN_TOP && (m_dwEnabledDockBars & CBRS_ALIGN_TOP) == 0)
			{
#if 0
				info.m_dwBarAlignment = CBRS_ALIGN_TOP;
				info.pDockBarRTC = RUNTIME_CLASS(CDockSite);
				if (!AddDockSite(info))
				{
					return FALSE;
				}
#endif
				m_dwEnabledDockBars |= CBRS_ALIGN_TOP;
			}

			if (dwStyle & CBRS_ALIGN_BOTTOM && (m_dwEnabledDockBars & CBRS_ALIGN_BOTTOM) == 0)
			{
#if 0
				info.m_dwBarAlignment = CBRS_ALIGN_BOTTOM;
				info.pDockBarRTC = RUNTIME_CLASS(CDockSite);
				if (!AddDockSite(info))
				{
					return FALSE;
				}
#endif
				m_dwEnabledDockBars |= CBRS_ALIGN_BOTTOM;
			}

			if (dwStyle & CBRS_ALIGN_LEFT && (m_dwEnabledDockBars & CBRS_ALIGN_LEFT) == 0)
			{
#if 0
				info.m_dwBarAlignment = CBRS_ALIGN_LEFT;
				info.pDockBarRTC = RUNTIME_CLASS(CDockSite);
				if (!AddDockSite(info))
				{
					return FALSE;
				}
#endif
				m_dwEnabledDockBars |= CBRS_ALIGN_LEFT;
			}

			if (dwStyle & CBRS_ALIGN_RIGHT && (m_dwEnabledDockBars & CBRS_ALIGN_RIGHT) == 0)
			{
#if 0
				info.m_dwBarAlignment = CBRS_ALIGN_RIGHT;
				info.pDockBarRTC = RUNTIME_CLASS(CDockSite);
				if (!AddDockSite(info))
				{
					return FALSE;
				}
#endif
				m_dwEnabledDockBars |= CBRS_ALIGN_RIGHT;
			}
			//AdjustDockingLayout();

			return TRUE;
		}

		BOOL EnableAutoHidePanes(DWORD dwStyle)
		{
			//AFX_DOCKSITE_INFO info;

			if (dwStyle & CBRS_ALIGN_TOP && (m_dwEnabledSlideBars & CBRS_ALIGN_TOP) == 0)
			{
				if ((m_dwEnabledDockBars & CBRS_ALIGN_TOP) == 0)
				{
					EnableDocking(CBRS_ALIGN_TOP);
				}
#if 0
				info.m_dwBarAlignment = CBRS_ALIGN_TOP;
				info.pDockBarRTC = RUNTIME_CLASS(CAutoHideDockSite);
				if (!AddDockSite(info))
				{
					return FALSE;
				}
#endif
				m_dwEnabledSlideBars |= CBRS_ALIGN_TOP;
			}

			if (dwStyle & CBRS_ALIGN_BOTTOM && (m_dwEnabledSlideBars & CBRS_ALIGN_BOTTOM) == 0)
			{
				if ((m_dwEnabledDockBars & CBRS_ALIGN_BOTTOM) == 0)
				{
					EnableDocking(CBRS_ALIGN_BOTTOM);
				}
#if 0
				info.m_dwBarAlignment = CBRS_ALIGN_BOTTOM;
				info.pDockBarRTC = RUNTIME_CLASS(CAutoHideDockSite);
				if (!AddDockSite(info))
				{
					return FALSE;
				}
#endif
				m_dwEnabledSlideBars |= CBRS_ALIGN_BOTTOM;
			}

			if (dwStyle & CBRS_ALIGN_LEFT && (m_dwEnabledSlideBars & CBRS_ALIGN_LEFT) == 0)
			{
				if ((m_dwEnabledDockBars & CBRS_ALIGN_LEFT) == 0)
				{
					EnableDocking(CBRS_ALIGN_LEFT);
				}
#if 0
				info.m_dwBarAlignment = CBRS_ALIGN_LEFT;
				info.pDockBarRTC = RUNTIME_CLASS(CAutoHideDockSite);
				if (!AddDockSite(info))
				{
					return FALSE;
				}
#endif
				m_dwEnabledSlideBars |= CBRS_ALIGN_LEFT;
			}

			if (dwStyle & CBRS_ALIGN_RIGHT && (m_dwEnabledSlideBars & CBRS_ALIGN_RIGHT) == 0)
			{
				if ((m_dwEnabledDockBars & CBRS_ALIGN_RIGHT) == 0)
				{
					EnableDocking(CBRS_ALIGN_RIGHT);
				}
#if 0
				info.m_dwBarAlignment = CBRS_ALIGN_RIGHT;
				info.pDockBarRTC = RUNTIME_CLASS(CAutoHideDockSite);
				if (!AddDockSite(info))
				{
					return FALSE;
				}
#endif
				m_dwEnabledSlideBars |= CBRS_ALIGN_RIGHT;
			}

			//AdjustDockingLayout();
			return TRUE;
		}


		static void SetDockingMode(AFX_DOCK_TYPE dockMode, AFX_SMARTDOCK_THEME theme = AFX_SDT_DEFAULT)
		{
			CWinAppEx* pCWinAppEx = AfxGetAppEx();

			pCWinAppEx->m_dockModeGlobal = dockMode;

			if (pCWinAppEx->m_dockModeGlobal == DT_SMART)
			{
				// DT_SMART should only be used along with DT_IMMEDIATE
				pCWinAppEx->m_dockModeGlobal = AFX_DOCK_TYPE(DT_SMART | DT_IMMEDIATE);
				pCWinAppEx->m_SDTheme = theme;
			}
		}

};




class CFrameWndEx : public CFrameWnd
{
	DECLARE_DYNAMIC(CFrameWndEx)

	public:
		CDockingManager  m_dockManager; // THE DockManager

		CFrameImpl m_Impl;

	public:
		void DockPane(CBasePane* pBar, UINT nDockBarID = 0, LPCRECT lpRect = NULL)
		{
			CFrameWnd::DockControlBar(pBar) ;

			ASSERT_VALID(this);
			m_dockManager.DockPane(pBar, nDockBarID, lpRect);
		}

		BOOL EnableDocking(DWORD dwDockStyle)
		{
			CFrameWnd::EnableDocking(dwDockStyle);

			return m_dockManager.EnableDocking(dwDockStyle);
		}

		BOOL EnableAutoHidePanes(DWORD dwDockStyle)
		{
			return m_dockManager.EnableAutoHidePanes(dwDockStyle);
		}

		void EnablePaneMenu(BOOL bEnable, UINT uiCustomizeCmd, const CString& strCustomizeLabel, UINT uiViewToolbarsMenuEntryID,
							BOOL bContextMenuShowsToolbarsOnly = FALSE, BOOL bViewMenuShowsToolbarsOnly = FALSE)
		{
			m_dockManager.EnablePaneContextMenu(bEnable, uiCustomizeCmd, strCustomizeLabel, bContextMenuShowsToolbarsOnly);
			m_Impl.SetControlbarsMenuId(uiViewToolbarsMenuEntryID, bViewMenuShowsToolbarsOnly);
		}

		void InitUserToolbars(LPCTSTR lpszRegEntry, UINT uiUserToolbarFirst, UINT uiUserToolbarLast)
		{
			m_Impl.InitUserToolbars(lpszRegEntry, uiUserToolbarFirst, uiUserToolbarLast);
		}

};





class CShellManager
{
	public:

	public:

};

class CMFCVisualManager : public CObject
{
	public:

	public:

		static void __stdcall SetDefaultManager(CRuntimeClass* pRTI)
		{

		}


};

class CMFCVisualManagerWindows : public CMFCVisualManager
{
	DECLARE_DYNAMIC(CMFCVisualManagerWindows)

	public:

	public:

};

#endif // _MTL_ATL3


//} // namespace MTL

#ifndef _MTL_ATL3

// COM

namespace ATL
{

class CAtlMfcModule : public ATL::CAtlModuleT<CAtlMfcModule>
{
	public:

	public:

		// Register/Revoke All Class Factories with the OS (EXE only)
		HRESULT RegisterClassObjects(DWORD dwClsContext, DWORD dwFlags)
		{
			return ATL::AtlComModuleRegisterClassObjects(&ATL::_AtlComModule, dwClsContext, dwFlags);
		}

		HRESULT RevokeClassObjects()
		{
			return ATL::AtlComModuleRevokeClassObjects(&ATL::_AtlComModule);
		}

};

} // namespace ATL

#endif // _MTL_ATL3

// function

#define AfxOleInit()
#define  AfxEnableControlContainer()


#ifdef _MTL_FXN_COMTATIBLE
#if _MSC_VER >= 1400
#ifdef MEMBER_FXN
#undef MEMBER_FXN
#define MEMBER_FXN(f) MEMBER_FXN_COMPATIBLE(f)
#endif
#endif
#endif	// _MTL_FXN_COMTATIBLE

//#include <mtlmain.h>



#endif	// __AFXWIN_H__

