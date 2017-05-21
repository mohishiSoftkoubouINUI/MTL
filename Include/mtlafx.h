






// special AFX window class name mangling

#ifndef _UNICODE
#define _UNICODE_SUFFIX
#else
#define _UNICODE_SUFFIX _T("u")
#endif

#ifndef _DEBUG
#define _DEBUG_SUFFIX
#else
#define _DEBUG_SUFFIX _T("d")
#endif

#define _STATIC_SUFFIX _T("s")

#define _MFC_FILENAME_VER _T("Mtl")

#define AFX_WNDCLASS(s) _T("Afx") _T(s) _MFC_FILENAME_VER _STATIC_SUFFIX _UNICODE_SUFFIX _DEBUG_SUFFIX

#define AFX_WND             AFX_WNDCLASS("Wnd")
#define AFX_WNDCONTROLBAR   AFX_WNDCLASS("ControlBar")
#define AFX_WNDMDIFRAME     AFX_WNDCLASS("MDIFrame")
#define AFX_WNDFRAMEORVIEW  AFX_WNDCLASS("FrameOrView")
#define AFX_WNDOLECONTROL   AFX_WNDCLASS("OleControl")

#define _afxWnd				AFX_WND
#define _afxWndControlBar	AFX_WNDCONTROLBAR	
#define _afxWndMDIFrame		AFX_WNDMDIFRAME
#define _afxWndFrameOrView	AFX_WNDFRAMEORVIEW
#define _afxWndOleControl	AFX_WNDOLECONTROL


enum {
	traceAppMsg = 0,
	traceDumpContext = 1,
	traceWinMsg = 2,
	traceMemory = 3,
	traceCmdRouting = 4,
	traceHtml = 5,
	traceSocket = 6,
	traceOle = 7,
	traceDatabase = 8,
	traceInternet = 9,
};


struct _AUX_DATA
{
	// system metrics
	int cxVScroll, cyHScroll;
	int cxBorder2, cyBorder2;

	// device metrics for screen
	int cxPixelsPerInch, cyPixelsPerInch;

	COLORREF clrBtnFace, clrBtnShadow, clrBtnHilite;
	COLORREF clrBtnText, clrWindowFrame;

	HCURSOR hcurWait;
	HCURSOR hcurArrow;

	_AUX_DATA()
	{
		// System metrics which depend on subsystem version
		cxVScroll = ::GetSystemMetrics(SM_CXVSCROLL) + AFX_CX_BORDER;
		cyHScroll = ::GetSystemMetrics(SM_CYHSCROLL) + AFX_CY_BORDER;


		cxBorder2 = (AFX_CX_BORDER * 2);
		cyBorder2 = (AFX_CY_BORDER * 2);

		// Device metrics for screen
		HDC hDCScreen = GetDC(NULL);
		ASSERT(hDCScreen != NULL);
		cxPixelsPerInch = GetDeviceCaps(hDCScreen, LOGPIXELSX);
		cyPixelsPerInch = GetDeviceCaps(hDCScreen, LOGPIXELSY);
		ReleaseDC(NULL, hDCScreen);

		clrBtnFace = ::GetSysColor(COLOR_BTNFACE);
		clrBtnShadow = ::GetSysColor(COLOR_BTNSHADOW);
		clrBtnHilite = ::GetSysColor(COLOR_BTNHIGHLIGHT);
		clrBtnText = ::GetSysColor(COLOR_BTNTEXT);
		clrWindowFrame = ::GetSysColor(COLOR_WINDOWFRAME);

		// Standard cursors
		hcurWait = ::LoadCursor(NULL, IDC_WAIT);
		hcurArrow = ::LoadCursor(NULL, IDC_ARROW);
	}

};


#ifdef _UNICODE
#define _CHAR_INC(p) ((p) + 1)
#else
#define _CHAR_INC(p) CharNext(p)
#endif


class CAfx
{
	public:

	public:
		static void Init()
		{
		}

		static void Term()
		{
			HalftoneBrush(TRUE) ;

			//AfxUnregisterWndClasses() ;
		}


		static CException& GetCException(INT iException)
		{
			static CInvalidArgException _simpleInvalidArgException(FALSE, AFX_IDS_INVALID_ARG_EXCEPTION);
			static CMemoryException _simpleMemoryException(FALSE, AFX_IDS_MEMORY_EXCEPTION);
			static CNotSupportedException _simpleNotSupportedException(FALSE, AFX_IDS_NOT_SUPPORTED_EXCEPTION);
			static CResourceException _simpleResourceException(FALSE, AFX_IDS_RESOURCE_EXCEPTION);

			switch (iException)
			{
				case AFX_IDS_INVALID_ARG_EXCEPTION:
					return _simpleInvalidArgException;
					break;
				case AFX_IDS_MEMORY_EXCEPTION:
					return _simpleMemoryException ;
					break ;
				case AFX_IDS_NOT_SUPPORTED_EXCEPTION :
					return _simpleNotSupportedException;
					break;
				case AFX_IDS_RESOURCE_EXCEPTION:
					return _simpleResourceException;
					break;

			}

			return _simpleNotSupportedException ;
		}

		static void _CDECL _TraceNop(LPCSTR exp, ...) {}
		static void _CDECL _TraceNop(DWORD category, UINT level, LPCSTR lpszFormat, ...) {}

#ifdef _DEBUG
		static void _CDECL _Trace2(LPCSTR exp, ...)
		{
			va_list argptr;
			va_start(argptr, exp);

			CHAR szOutput[1024] ;
			wvsprintfA(szOutput, exp, argptr) ;

			va_end(argptr) ;

			OutputDebugStringA(szOutput) ;
		}

		static void _CDECL _Trace2(DWORD category, UINT level, LPCSTR lpszFormat, ...)
		{
			static LPCSTR szaCategory[] = { 
								"traceAppMsg",		// 汎用、MFC メッセージ。 常に推奨されます。 
								"traceDumpContext",	// [CDumpContext]からメッセージ。 
								"traceWinMsg",		// MFC のメッセージ処理コードからメッセージ。 
								"traceMemory",		// MFC のメモリ管理コードからメッセージ。 
								"traceCmdRouting",	// MFC の Windows コマンドのルート コードからメッセージ。 
								"traceHtml",		// MFC の DHTML ダイアログのサポートのメッセージ。 
								"traceSocket",		// MFC ソケットのサポートのメッセージ。 
								"traceOle",			// MFC OLE サポートのメッセージ。 
								"traceDatabase",	// MFC データベース サポートのメッセージ。 
								"traceInternet",	// MFC インターネットのサポートのメッセージ。 
			};

			CHAR szOutput[1024];
			wsprintfA(szOutput, "%s(%d): ", szaCategory[category], level);

			OutputDebugStringA(szOutput);

			va_list argptr;
			va_start(argptr, lpszFormat);

			wvsprintfA(szOutput, lpszFormat, argptr);

			va_end(argptr);

			OutputDebugStringA(szOutput);
		}
#endif


		/////////////////////////////////////////////////////////////////////////////
		// lpszCanon = C:\MYAPP\DEBUGS\C\TESWIN.C
		//
		// cchMax   b   Result
		// ------   -   ---------
		//  1- 7    F   <empty>
		//  1- 7    T   TESWIN.C
		//  8-14    x   TESWIN.C
		// 15-16    x   C:\...\TESWIN.C
		// 17-23    x   C:\...\C\TESWIN.C
		// 24-25    x   C:\...\DEBUGS\C\TESWIN.C
		// 26+      x   C:\MYAPP\DEBUGS\C\TESWIN.C

		static void _AfxAbbreviateName(LPTSTR lpszCanon, int cchMax, BOOL bAtLeastName)
		{
			ENSURE_ARG(AfxIsValidString(lpszCanon));

			const TCHAR*lpszBase = lpszCanon;
			int cchFullPath = lstrlen(lpszCanon);

			int cchFileName = AfxGetFileName(lpszCanon, NULL, 0) - 1;
			const TCHAR* lpszFileName = lpszBase + (cchFullPath - cchFileName);

			// If cchMax is more than enough to hold the full path name, we're done.
			// This is probably a pretty common case, so we'll put it first.
			if (cchMax >= cchFullPath)
				return;

			// If cchMax isn't enough to hold at least the basename, we're done
			if (cchMax < cchFileName)
			{
				if (!bAtLeastName)
					lpszCanon[0] = _T('\0');
				else
					lstrcpy(lpszCanon, lpszFileName);
				return;
			}

			// Calculate the length of the volume name.  Normally, this is two characters
			// (e.g., "C:", "D:", etc.), but for a UNC name, it could be more (e.g.,
			// "\\server\share").
			//
			// If cchMax isn't enough to hold at least <volume_name>\...\<base_name>, the
			// result is the base filename.

			const TCHAR* lpszCur = lpszBase + 2;                 // Skip "C:" or leading "\\"

			if (lpszBase[0] == '\\' && lpszBase[1] == '\\') // UNC pathname
			{
				// First skip to the '\' between the server name and the share name,
				while (*lpszCur != '\\')
				{
					lpszCur = _CHAR_INC(lpszCur);
					ASSERT(*lpszCur != '\0');
				}
			}
			// if a UNC get the share name, if a drive get at least one directory
			ASSERT(*lpszCur == '\\');
			// make sure there is another directory, not just c:\filename.ext
			if (cchFullPath - cchFileName > 3)
			{
				lpszCur = _CHAR_INC(lpszCur);
				while (*lpszCur != '\\')
				{
					lpszCur = _CHAR_INC(lpszCur);
					ASSERT(*lpszCur != '\0');
				}
			}
			ASSERT(*lpszCur == '\\');

			int cchVolName = int(lpszCur - lpszBase);
			if (cchMax < cchVolName + 5 + cchFileName)
			{
				lstrcpy(lpszCanon, lpszFileName);
				return;
			}

			// Now loop through the remaining directory components until something
			// of the form <volume_name>\...\<one_or_more_dirs>\<base_name> fits.
			//
			// Assert that the whole filename doesn't fit -- this should have been
			// handled earlier.

			ASSERT(cchVolName + lstrlen(lpszCur) > cchMax);
			while (cchVolName + 4 + lstrlen(lpszCur) > cchMax)
			{
				do
				{
					lpszCur = _CHAR_INC(lpszCur);
					ASSERT(*lpszCur != '\0');
				} while (*lpszCur != '\\');
			}

			// Form the resultant string and we're done.
			int cch;
			if (cchVolName >= 0 && cchVolName < cchMax)
				cch = cchVolName;
			else cch = cchMax;
			memcpy(lpszCanon + cch, _T("\\..."), sizeof(_T("\\...")));
			lstrcat(lpszCanon, lpszCur);
		}


		static BOOL AfxComparePath(LPCTSTR lpszPath1, LPCTSTR lpszPath2)
		{
#pragma warning(push)
#pragma warning(disable:4068)
#pragma prefast(push)
#pragma prefast(disable:400, "lstrcmpi is ok here as we are backing it up with further comparison")
			// use case insensitive compare as a starter
			if (lstrcmpi(lpszPath1, lpszPath2) != 0)
			{
				return FALSE;
			}
#pragma prefast(pop)
#pragma warning(pop)

			// on non-DBCS systems, we are done
			if (!GetSystemMetrics(SM_DBCSENABLED))
				return TRUE;

			// on DBCS systems, the file name may not actually be the same
			// in particular, the file system is case sensitive with respect to
			// "full width" roman characters.
			// (ie. fullwidth-R is different from fullwidth-r).
			int nLen = lstrlen(lpszPath1);
			if (nLen != lstrlen(lpszPath2))
				return FALSE;
			ASSERT(nLen < _MAX_PATH);

			// need to get both CT_CTYPE1 and CT_CTYPE3 for each filename
			LCID lcid = GetThreadLocale();
			WORD aCharType11[_MAX_PATH];
			VERIFY(GetStringTypeEx(lcid, CT_CTYPE1, lpszPath1, -1, aCharType11));
			WORD aCharType13[_MAX_PATH];
			VERIFY(GetStringTypeEx(lcid, CT_CTYPE3, lpszPath1, -1, aCharType13));
			WORD aCharType21[_MAX_PATH];
			VERIFY(GetStringTypeEx(lcid, CT_CTYPE1, lpszPath2, -1, aCharType21));
#ifdef _DEBUG
			WORD aCharType23[_MAX_PATH];
			VERIFY(GetStringTypeEx(lcid, CT_CTYPE3, lpszPath2, -1, aCharType23));
#endif

			// for every C3_FULLWIDTH character, make sure it has same C1 value
			int i = 0;
			for (LPCTSTR lpsz = lpszPath1; *lpsz != 0; lpsz = _CHAR_INC(lpsz))
			{
				// check for C3_FULLWIDTH characters only
				if (aCharType13[i] & C3_FULLWIDTH)
				{
#ifdef _DEBUG
					ASSERT(aCharType23[i] & C3_FULLWIDTH); // should always match!
#endif

					// if CT_CTYPE1 is different then file system considers these
					// file names different.
					if (aCharType11[i] != aCharType21[i])
						return FALSE;
				}
				++i; // look at next character type
			}
			return TRUE; // otherwise file name is truly the same
		}



		static void AfxDeleteObject(HGDIOBJ* pObject)
		{
			if (*pObject != NULL)
			{
				DeleteObject(*pObject);
				*pObject = NULL;
			}
		}

		static LONG AfxDelRegTreeHelper(HKEY hParentKey, const CString& strKeyName, CAtlTransactionManager* pTM = NULL)
		{
			TCHAR   szSubKeyName[MAX_PATH + 1];
			HKEY    hCurrentKey;

			CString strRedirectedKeyName = strKeyName;

			DWORD dwResult = ::RegOpenKeyEx(hParentKey, strRedirectedKeyName, 0, KEY_WRITE | KEY_READ, &hCurrentKey);
			if (dwResult == ERROR_SUCCESS)
			{
				// Remove all subkeys of the key to delete
				while ((dwResult = RegEnumKey(hCurrentKey, 0, szSubKeyName, MAX_PATH)) == ERROR_SUCCESS)
				{
					// temp CString constructed from szSubKeyName can throw in Low Memory condition.
					if ((dwResult = AfxDelRegTreeHelper(hCurrentKey, szSubKeyName, pTM)) != ERROR_SUCCESS)
						break;
				}

				// If all went well, we should now be able to delete the requested key
				if ((dwResult == ERROR_NO_MORE_ITEMS) || (dwResult == ERROR_BADKEY))
				{
					dwResult = ::RegDeleteKey(hParentKey, strRedirectedKeyName);
				}
				RegCloseKey(hCurrentKey);
			}

			return dwResult;
		}

		static CObject* AfxDynamicDownCast(CRuntimeClass* pClass, CObject* pObject)
		{
			if (pObject != NULL && pObject->IsKindOf(pClass))
				return pObject;

			return NULL;
		}

		static BOOL AfxEndDeferRegisterClass(LONG fToRegister)
		{
			// mask off all classes that are already registered
			AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
			fToRegister &= ~pModuleState->m_fRegisteredClasses;
			if (fToRegister == 0)
				return TRUE;

			LONG fRegisteredClasses = 0;

			// common initialization
			WNDCLASS wndcls;
			memset(&wndcls, 0, sizeof(WNDCLASS));   // start with NULL defaults
			wndcls.lpfnWndProc = DefWindowProc;
			wndcls.hInstance = AfxGetInstanceHandle();
			wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);//afxData.hcurArrow;

			INITCOMMONCONTROLSEX init;
			init.dwSize = sizeof(init);

			if ((fToRegister & (AFX_WND_REG | 
								AFX_WNDOLECONTROL_REG | 
								AFX_WNDCONTROLBAR_REG | 
								AFX_WNDMDIFRAME_REG | 
								AFX_WNDFRAMEORVIEW_REG)) != 0)
			{
				// work to register classes as specified by fToRegister, populate fRegisteredClasses as we go
				if (fToRegister & AFX_WND_REG)
				{
					// Child windows - no brush, no icon, safest default class styles
					wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
					wndcls.lpszClassName = _afxWnd;
					if (AfxRegisterClass(&wndcls))
						fRegisteredClasses |= AFX_WND_REG;
				}
				if (fToRegister & AFX_WNDOLECONTROL_REG)
				{
					// OLE Control windows - use parent DC for speed
					wndcls.style |= CS_PARENTDC | CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
					wndcls.lpszClassName = _afxWndOleControl;
					if (AfxRegisterClass(&wndcls))
						fRegisteredClasses |= AFX_WNDOLECONTROL_REG;
				}
				if (fToRegister & AFX_WNDCONTROLBAR_REG)
				{
					// Control bar windows
					wndcls.style = 0;   // control bars don't handle double click
					wndcls.lpszClassName = _afxWndControlBar;
					wndcls.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
					if (AfxRegisterClass(&wndcls))
						fRegisteredClasses |= AFX_WNDCONTROLBAR_REG;
				}
				if (fToRegister & AFX_WNDMDIFRAME_REG)
				{
					// MDI Frame window (also used for splitter window)
					wndcls.style = CS_DBLCLKS;
					wndcls.hbrBackground = NULL;
					if (_AfxRegisterWithIcon(&wndcls, _afxWndMDIFrame, AFX_IDI_STD_MDIFRAME))
						fRegisteredClasses |= AFX_WNDMDIFRAME_REG;
				}
				if (fToRegister & AFX_WNDFRAMEORVIEW_REG)
				{
					// SDI Frame or MDI Child windows or views - normal colors
					wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
					wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
					if (_AfxRegisterWithIcon(&wndcls, _afxWndFrameOrView, AFX_IDI_STD_FRAME))
						fRegisteredClasses |= AFX_WNDFRAMEORVIEW_REG;
				}
			}

			if ((fToRegister & (AFX_WNDCOMMCTLS_REG |
								AFX_WNDOLECONTROL_REG |
								AFX_WNDCOMMCTL_UPDOWN_REG |
								AFX_WNDCOMMCTL_TREEVIEW_REG |
								AFX_WNDCOMMCTL_TAB_REG |
								AFX_WNDCOMMCTL_PROGRESS_REG |
								AFX_WNDCOMMCTL_LISTVIEW_REG |
								AFX_WNDCOMMCTL_HOTKEY_REG |
								AFX_WNDCOMMCTL_BAR_REG |
								AFX_WNDCOMMCTL_ANIMATE_REG |
								AFX_WNDCOMMCTL_INTERNET_REG |
								AFX_WNDCOMMCTL_COOL_REG |
								AFX_WNDCOMMCTL_USEREX_REG |
								AFX_WNDCOMMCTL_DATE_REG |
								AFX_WNDCOMMCTL_LINK_REG |
								AFX_WNDCOMMCTL_PAGER_REG)) != 0)
			{
				DWORD dwICC = 0;
				LONG fToReq = 0;

				if (fToRegister & AFX_WNDCOMMCTL_ANIMATE_REG)
				{
					dwICC |= ICC_ANIMATE_CLASS;
					fToReq |= AFX_WNDCOMMCTL_ANIMATE_REG;
				}
				if (fToRegister & AFX_WNDCOMMCTL_BAR_REG)
				{
					dwICC |= ICC_BAR_CLASSES;
					fToReq |= AFX_WNDCOMMCTL_BAR_REG;
				}
				if (fToRegister & AFX_WNDCOMMCTL_COOL_REG)
				{
					dwICC |= ICC_COOL_CLASSES;
					fToReq |= AFX_WNDCOMMCTL_COOL_REG;
				}
				if (fToRegister & AFX_WNDCOMMCTL_DATE_REG)
				{
					dwICC |= ICC_DATE_CLASSES;
					fToReq |= AFX_WNDCOMMCTL_DATE_REG;
				}
				if (fToRegister & AFX_WNDCOMMCTL_HOTKEY_REG)
				{
					dwICC |= ICC_HOTKEY_CLASS;
					fToReq |= AFX_WNDCOMMCTL_HOTKEY_REG;
				}
				if (fToRegister & AFX_WNDCOMMCTL_INTERNET_REG)
				{
					dwICC |= ICC_INTERNET_CLASSES;
					fToReq |= AFX_WNDCOMMCTL_INTERNET_REG;
				}
				if (fToRegister & AFX_WNDCOMMCTL_LINK_REG)
				{
					dwICC |= ICC_LINK_CLASS;
					fToReq |= AFX_WNDCOMMCTL_LINK_REG;
				}
				if (fToRegister & AFX_WNDCOMMCTL_LISTVIEW_REG)
				{
					dwICC |= ICC_LISTVIEW_CLASSES;
					fToReq |= AFX_WNDCOMMCTL_LISTVIEW_REG;
				}
				if (fToRegister & AFX_WNDCOMMCTL_PAGER_REG)
				{
					dwICC |= ICC_PAGESCROLLER_CLASS;
					fToReq |= AFX_WNDCOMMCTL_PAGER_REG;
				}
				if (fToRegister & AFX_WNDCOMMCTL_PROGRESS_REG)
				{
					dwICC |= ICC_PROGRESS_CLASS;
					fToReq |= AFX_WNDCOMMCTL_PROGRESS_REG;
				}
				if (fToRegister & AFX_WNDCOMMCTL_TAB_REG)
				{
					dwICC |= ICC_TAB_CLASSES;
					fToReq |= AFX_WNDCOMMCTL_TAB_REG;
				}
				if (fToRegister & AFX_WNDCOMMCTL_TREEVIEW_REG)
				{
					dwICC |= ICC_TREEVIEW_CLASSES;
					fToReq |= AFX_WNDCOMMCTL_TREEVIEW_REG;
				}
				if (fToRegister & AFX_WNDCOMMCTL_UPDOWN_REG)
				{
					dwICC |= ICC_UPDOWN_CLASS;
					fToReq |= AFX_WNDCOMMCTL_UPDOWN_REG;
				}
				if (fToRegister & AFX_WNDCOMMCTL_USEREX_REG)
				{
					dwICC |= ICC_USEREX_CLASSES;
					fToReq |= AFX_WNDCOMMCTL_USEREX_REG;
				}
				if (fToRegister & AFX_WNDCOMMCTLS_REG)
				{
					// this flag is compatible with the old InitCommonControls() API
					dwICC |= ICC_WIN95_CLASSES;
					fToReq |= AFX_WIN95CTLS_MASK ;
				}
				if (fToReq != 0)
				{
					init.dwICC = dwICC ;
					fRegisteredClasses |= _AfxInitCommonControls(&init, fToReq);
				}
			}

			// save new state of registered controls
			pModuleState->m_fRegisteredClasses |= fRegisteredClasses;

			// special case for all common controls registered, turn on AFX_WNDCOMMCTLS_REG
			if ((pModuleState->m_fRegisteredClasses & AFX_WIN95CTLS_MASK) == AFX_WIN95CTLS_MASK)
			{
				pModuleState->m_fRegisteredClasses |= AFX_WNDCOMMCTLS_REG;
				fRegisteredClasses |= AFX_WNDCOMMCTLS_REG;
			}

			// must have registered at least as mamy classes as requested
			return (fToRegister & fRegisteredClasses) == fToRegister;
		}

		static BOOL AfxExtractSubString(CString& rString, LPCTSTR lpszFullString, int iSubString, TCHAR chSep = '\n')
		{
			if (lpszFullString == NULL)
				return FALSE;

			while (iSubString--)
			{
				lpszFullString = _tcschr(lpszFullString, chSep);
				if (lpszFullString == NULL)
				{
					rString.Empty();        // return empty string as well
					return FALSE;
				}
				lpszFullString++;       // point past the separator
			}
			LPCTSTR lpchEnd = _tcschr(lpszFullString, chSep);
			int nLen = (lpchEnd == NULL) ?
						(int)(lstrlen(lpszFullString)) : (int)(lpchEnd - lpszFullString);
			ASSERT(nLen >= 0);
			memcpy(rString.GetBufferSetLength(nLen), lpszFullString, nLen*sizeof(TCHAR));
			rString.ReleaseBuffer();	// Need to call ReleaseBuffer 
			// after calling GetBufferSetLength
			return TRUE;
		}

		// Strings in format ".....%1 .... %2 ...." etc.

		static void AfxFormatStrings(CString& rString, LPCTSTR lpszFormat, LPCTSTR const* rglpsz, int nString)
		{
			ENSURE_ARG(lpszFormat != NULL);
			ENSURE_ARG(rglpsz != NULL);
			// determine length of destination string, not including null terminator
			int nTotalLen = 0;
			LPCTSTR pchSrc = lpszFormat;
			while (*pchSrc != '\0')
			{
				if (pchSrc[0] == '%' &&
					((pchSrc[1] >= '1' && pchSrc[1] <= '9') ||
					(pchSrc[1] >= 'A' && pchSrc[1] <= 'Z')))
				{
					// %A comes after %9 -- we'll need it someday
					int i;
					if (pchSrc[1] > '9')
						i = 9 + (pchSrc[1] - 'A');
					else
						i = pchSrc[1] - '1';
					pchSrc += 2;
					if (i >= nString)
						++nTotalLen;
					else if (rglpsz[i] != NULL)
						nTotalLen += (int)(lstrlen(rglpsz[i]));
				}
				else
				{
					if (_istlead(*pchSrc))
						++nTotalLen, ++pchSrc;
					++pchSrc;
					++nTotalLen;
				}
			}

			pchSrc = lpszFormat;
			LPTSTR pchDest = rString.GetBuffer(nTotalLen);
			while (*pchSrc != '\0')
			{
				if (pchSrc[0] == '%' &&
					((pchSrc[1] >= '1' && pchSrc[1] <= '9') ||
					(pchSrc[1] >= 'A' && pchSrc[1] <= 'Z')))
				{
					// %A comes after %9 -- we'll need it someday
					int i;
					if (pchSrc[1] > '9')
						i = 9 + (pchSrc[1] - 'A');
					else
						i = pchSrc[1] - '1';
					pchSrc += 2;
					if (i >= nString)
					{
						TRACE(traceAppMsg, 0, "Error: illegal string index requested %d.\n", i);
						*pchDest++ = '?';
						nTotalLen--;
					}
					else if (rglpsz[i] != NULL)
					{
						int nLen = (int)(lstrlen(rglpsz[i]));
						lstrcpy(pchDest, rglpsz[i]);
						nTotalLen -= nLen;
						pchDest += nLen;
					}
				}
				else
				{
					if (_istlead(*pchSrc))
						*pchDest++ = *pchSrc++, nTotalLen--; // copy first of 2 bytes
					*pchDest++ = *pchSrc++;
					nTotalLen--;
				}
			}
			rString.ReleaseBuffer((int)((LPCTSTR)pchDest - (LPCTSTR)rString));
			// ReleaseBuffer will assert if we went too far
		}

		static void AfxFormatStrings(CString& rString, UINT nIDS, LPCTSTR const* rglpsz, int nString)
		{
			CString strFormat;
			if (!strFormat.LoadString(nIDS) != 0)
			{
				TRACE(traceAppMsg, 0, "Error: failed to load AfxFormatString string 0x%04x.\n", nIDS);
				ASSERT(FALSE);
				return;
			}
			AfxFormatStrings(rString, strFormat, rglpsz, nString);
		}


		static UINT AfxGetFileName(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax)
		{
			ENSURE_ARG(lpszPathName != NULL);

			// always capture the complete file name including extension (if present)
			LPTSTR lpszTemp = ::PathFindFileName(lpszPathName);

			// lpszTitle can be NULL which just returns the number of bytes
			if (lpszTitle == NULL)
				return (UINT)(lstrlen(lpszTemp)) + 1;

			// otherwise copy it into the buffer provided
			lstrcpyn(lpszTitle, lpszTemp, nMax);
			return 0;
		}


		static UINT AfxGetFileTitle(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax)
		{
			// use a temporary to avoid bugs in ::GetFileTitle when lpszTitle is NULL
			TCHAR szTemp[_MAX_PATH];
			LPTSTR lpszTemp = lpszTitle;
			if (lpszTemp == NULL)
			{
				lpszTemp = szTemp;
				nMax = _countof(szTemp);
			}
			if (::GetFileTitle(lpszPathName, lpszTemp, (WORD)nMax) != 0)
			{
				// when ::GetFileTitle fails, use cheap imitation
				return AfxGetFileName(lpszPathName, lpszTitle, nMax);
			}
			return lpszTitle == NULL ? (UINT)(lstrlen(lpszTemp)) + 1 : 0;
		}



		static void AfxGetRoot(LPCTSTR lpszPath, CString& strRoot)
		{
			ASSERT(lpszPath != NULL);

			LPTSTR lpszRoot = strRoot.GetBuffer(_MAX_PATH);
			memset(lpszRoot, 0, _MAX_PATH);
			lstrcpyn(lpszRoot, lpszPath, _MAX_PATH);
			PathStripToRoot(lpszRoot);
			strRoot.ReleaseBuffer();
		}

		// turn a file, relative path or other into an absolute path
		static BOOL _AfxFullPath2(LPTSTR lpszPathOut, LPCTSTR lpszFileIn, CFileException* pException = NULL)
			// lpszPathOut = buffer of _MAX_PATH
			// lpszFileIn = file, relative path or absolute path
			// (both in ANSI character set)
			// pException - pointer to exception object - can be NULL.
		{
			// first, fully qualify the path name
			LPTSTR lpszFilePart;
			DWORD dwRet = GetFullPathName(lpszFileIn, _MAX_PATH, lpszPathOut, &lpszFilePart);
			if (dwRet == 0)
			{
				lstrcpyn(lpszPathOut, lpszFileIn, _MAX_PATH); // take it literally
				return FALSE;
			}
			else if (dwRet >= _MAX_PATH)
			{
				return FALSE; // long path won't fit in buffer
			}

			CString strRoot;
			// determine the root name of the volume
			AfxGetRoot(lpszPathOut, strRoot);

			if (!::PathIsUNC(strRoot))
			{
				// get file system information for the volume
				DWORD dwFlags, dwDummy;
				if (!GetVolumeInformation(strRoot, NULL, 0, NULL, &dwDummy, &dwFlags, NULL, 0))
				{
					TRACE(traceAppMsg, 0, "Warning: could not get volume information '%s'.\n", (LPCTSTR)strRoot);
					return FALSE;   // preserving case may not be correct
				}

				// not all characters have complete uppercase/lowercase
				if (!(dwFlags & FS_CASE_IS_PRESERVED))
					CharUpper(lpszPathOut);

				// assume non-UNICODE file systems, use OEM character set
				if (!(dwFlags & FS_UNICODE_STORED_ON_DISK))
				{
					WIN32_FIND_DATA data;
					HANDLE h = FindFirstFile(lpszFileIn, &data);
					if (h != INVALID_HANDLE_VALUE)
					{
						FindClose(h);
						if (lpszFilePart != NULL && lpszFilePart > lpszPathOut)
						{
							int nFileNameLen = lstrlen(data.cFileName);
							int nIndexOfPart = (int)(lpszFilePart - lpszPathOut);
							if ((nFileNameLen + nIndexOfPart) < _MAX_PATH)
							{
								lstrcpyn(lpszFilePart, data.cFileName, _MAX_PATH - nIndexOfPart);
							}
							else
							{
								// the path+filename of the file is too long
								return FALSE; // Path doesn't fit in the buffer.
							}
						}
						else
						{
							return FALSE;
						}
					}
				}
			}

			return TRUE;
		}

		static LONG _AfxInitCommonControls(LPINITCOMMONCONTROLSEX lpInitCtrls, LONG fToRegister)
		{
			ASSERT(fToRegister != 0);
			LONG lResult = 0;

			if (InitCommonControlsEx(lpInitCtrls))
			{
				// InitCommonControlsEx was successful so return the full mask
				lResult = fToRegister;
			}

			return lResult;
		}

		// Common Control Versions:
		//   WinNT 4.0          maj=4 min=00
		//   IE 3.x             maj=4 min=70
		//   IE 4.0             maj=4 min=71
		//   IE 5.0             maj=5 min=80
		//   Win2000            maj=5 min=81
		static HRESULT GetCommCtrlVersion(LPDWORD pdwMajor, LPDWORD pdwMinor)
		{
			*pdwMajor = 0;
			*pdwMinor = 0;

			HINSTANCE hInstDLL = ::LoadLibraryW(L"comctl32.dll");
			if (hInstDLL == NULL)
			{
				*pdwMajor = 4;
				*pdwMinor = 0;
				return 0;
				//return AtlHresultFromLastError();
			}

			// We must get this function explicitly because some DLLs don't implement it.
			DLLGETVERSIONPROC pfnDllGetVersion = (DLLGETVERSIONPROC)::GetProcAddress(hInstDLL, "DllGetVersion");
			HRESULT hRet;

			if (pfnDllGetVersion != NULL)
			{
				DLLVERSIONINFO dvi;
				memset(&dvi, 0, sizeof(dvi));
				dvi.cbSize = sizeof(dvi);

				hRet = (*pfnDllGetVersion)(&dvi);
				if (SUCCEEDED(hRet))
				{
					*pdwMajor = dvi.dwMajorVersion;
					*pdwMinor = dvi.dwMinorVersion;
				}
			}
			else
			{
				hRet = E_NOTIMPL;
			}

			::FreeLibrary(hInstDLL);
			return hRet;
		}

		static DWORD _AfxGetComCtlVersion()
		{
			static int _afxComCtlVersion = -1;

			if (_afxComCtlVersion == -1)
			{
				DWORD dwMajor = 0, dwMinor = 0;
				GetCommCtrlVersion(&dwMajor, &dwMinor);

				_afxComCtlVersion = MAKELONG(dwMinor, dwMajor);
			}

			return _afxComCtlVersion;
		}


		static const ATLSTRINGRESOURCEIMAGE* _AtlGetStringResourceImage(HINSTANCE hInstance, HRSRC hResource, UINT id) throw()
		{
			HGLOBAL hGlobal = ::LoadResource(hInstance, hResource);
			if (hGlobal == NULL)
			{
				return(NULL);
			}

			const ATLSTRINGRESOURCEIMAGE* pImage = (const ATLSTRINGRESOURCEIMAGE*)::LockResource(hGlobal);
			if (pImage == NULL)
			{
				return(NULL);
			}

			ULONG nResourceSize = ::SizeofResource(hInstance, hResource);
			const ATLSTRINGRESOURCEIMAGE* pImageEnd = (const ATLSTRINGRESOURCEIMAGE*)(LPBYTE(pImage) + nResourceSize);
			UINT iIndex = id & 0x000f;

			while ((iIndex > 0) && (pImage < pImageEnd))
			{
				pImage = (const ATLSTRINGRESOURCEIMAGE*)(LPBYTE(pImage) + (sizeof(ATLSTRINGRESOURCEIMAGE) + (pImage->nLength * sizeof(WCHAR))));
				iIndex--;
			}
			if (pImage >= pImageEnd)
			{
				return(NULL);
			}
			if (pImage->nLength == 0)
			{
				return(NULL);
			}

			return(pImage);
		}

		static const ATLSTRINGRESOURCEIMAGE* AtlGetStringResourceImage(HINSTANCE hInstance, UINT id) throw()
		{
			/*
			The and operation (& static_cast<WORD>(~0)) protects the expression from being greater
			than WORD - this would cause a runtime error when the application is compiled with /RTCc flag.
			*/
			HRSRC hResource = ::FindResourceW(hInstance, MAKEINTRESOURCEW((((id >> 4) + 1) & (WORD)(~0))), (LPWSTR)RT_STRING);
			if (hResource == NULL)
			{
				return(NULL);
			}

			return _AtlGetStringResourceImage(hInstance, hResource, id);
		}

		static void AfxGlobalFree(HGLOBAL hGlobal)
		{
			if (hGlobal == NULL)
				return;

			// avoid bogus warning error messages from various debugging tools
			ASSERT(GlobalFlags(hGlobal) != GMEM_INVALID_HANDLE);
			UINT nCount = GlobalFlags(hGlobal) & GMEM_LOCKCOUNT;
			while (nCount--)
				GlobalUnlock(hGlobal);

			// finally, really free the handle
			GlobalFree(hGlobal);
		}

		static int AfxLoadString(UINT nID, LPSTR lpszBuf, UINT nMaxBuf = 256)
		{
			const ATLSTRINGRESOURCEIMAGE* pImage = AtlGetStringResourceImage(AfxGetResourceHandle(), nID);
			if (pImage == NULL)
			{
				lpszBuf[0] = '\0';
				return 0;
			}
			ASSERT(pImage->nLength != 0);
			int nBytes = ::WideCharToMultiByte(CP_ACP, 0, pImage->achString, pImage->nLength, lpszBuf, nMaxBuf - 1, NULL, NULL);
			lpszBuf[nBytes] = '\0';

			return nBytes;
		}

		static int AfxLoadString(UINT nID, LPWSTR lpszBuf, UINT nMaxBuf = 256)
		{
			const ATLSTRINGRESOURCEIMAGE* pImage = AtlGetStringResourceImage(AfxGetResourceHandle(), nID);
			if (pImage == NULL)
			{
				lpszBuf[0] = L'\0';
				return 0;
			}
			ASSERT(pImage->nLength != 0);
			int nCharsToCopy = min(nMaxBuf - 1, pImage->nLength);
			memcpy(lpszBuf, pImage->achString, nCharsToCopy*sizeof(WCHAR));
			lpszBuf[nCharsToCopy] = L'\0';

			return nCharsToCopy;
		}

		static void AfxGetModuleFileName(HINSTANCE hInst, CString& strFileName)
		{
			LPTSTR lpsz = strFileName.GetBuffer(_MAX_PATH);
			DWORD dwLen = ::GetModuleFileName(hInst, lpsz, _MAX_PATH);
			strFileName.ReleaseBuffer(dwLen) ;
		}

		static void AfxPostQuitMessage(int nExitCode)
		{
			::PostQuitMessage(nExitCode);
		}


		static BOOL _AfxRegisterWithIcon(WNDCLASS* pWndCls, LPCTSTR lpszClassName, UINT nIDIcon)
		{
			pWndCls->lpszClassName = lpszClassName;
			HINSTANCE hInst = AfxFindResourceHandle(ATL_MAKEINTRESOURCE(nIDIcon), ATL_RT_GROUP_ICON);
			if ((pWndCls->hIcon = ::LoadIconW(hInst, ATL_MAKEINTRESOURCEW(nIDIcon))) == NULL)
			{
				// use default icon
				pWndCls->hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
			}
			return AfxRegisterClass(pWndCls);
		}


		// like RegisterClass, except will automatically call UnregisterClass
		static BOOL AfxRegisterClass(WNDCLASS* lpWndClass)
		{
			WNDCLASS wndcls;
			if (GetClassInfo(lpWndClass->hInstance, lpWndClass->lpszClassName, &wndcls))
			{
				// class already registered
				return TRUE;
			}

			if (!RegisterClass(lpWndClass))
			{
				TRACE(traceAppMsg, 0, "Can't register window class named %s\n", lpWndClass->lpszClassName);
				return FALSE;
			}

			BOOL bRet = TRUE;

			AFX_MODULE_STATE* pState = AfxGetModuleState();
			if (pState->m_afxContextIsDLL)
			{
				// class registered successfully, add to registered list
				pState->m_strUnregisterList += lpWndClass->lpszClassName;
				pState->m_strUnregisterList += '\n';
			}

			return bRet;
		}

		static LPCTSTR AfxRegisterWndClass(UINT nClassStyle, HCURSOR hCursor = NULL, HBRUSH hbrBackground = NULL, HICON hIcon = NULL)
		{
			// Returns a temporary string name for the class
			//  Save in a CString if you want to use it for a long time
			_AFX_THREAD_STATE* pState = AfxGetThreadState();
			LPTSTR lpszName = pState->m_szTempClassName;

			// generate a synthetic name for this class
			HINSTANCE hInst = AfxGetInstanceHandle();

			if (hCursor == NULL && hbrBackground == NULL && hIcon == NULL)
			{
				wsprintf(lpszName, _T("Afx:%p:%x"), hInst, nClassStyle);
			}
			else
			{
				wsprintf(lpszName, _T("Afx:%p:%x:%p:%p:%p"), hInst, nClassStyle, hCursor, hbrBackground, hIcon);
			}

			// see if the class already exists
			WNDCLASS wndcls;
			if (GetClassInfo(hInst, lpszName, &wndcls))
			{
				// already registered, assert everything is good
				ASSERT(wndcls.style == nClassStyle);

				// NOTE: We have to trust that the hIcon, hbrBackground, and the
				//  hCursor are semantically the same, because sometimes Windows does
				//  some internal translation or copying of those handles before
				//  storing them in the internal WNDCLASS retrieved by GetClassInfo.
				return lpszName;
			}

			// otherwise we need to register a new class
			wndcls.style = nClassStyle;
			wndcls.lpfnWndProc = DefWindowProc;
			wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
			wndcls.hInstance = hInst;
			wndcls.hIcon = hIcon;
			wndcls.hCursor = hCursor;
			wndcls.hbrBackground = hbrBackground;
			wndcls.lpszMenuName = NULL;
			wndcls.lpszClassName = lpszName;
			AfxRegisterClass(&wndcls);

			// return thread-local pointer
			return lpszName;
		}

		static void AfxRepositionWindow(AFX_SIZEPARENTPARAMS* lpLayout, HWND hWnd, LPCRECT lpRect)
		{
			ASSERT(hWnd != NULL);
			ASSERT(lpRect != NULL);
			HWND hWndParent = ::GetParent(hWnd);
			ASSERT(hWndParent != NULL);

			if (lpLayout != NULL && lpLayout->hDWP == NULL)
				return;

			// first check if the new rectangle is the same as the current
			CRect rectOld;
			::GetWindowRect(hWnd, rectOld);
			::ScreenToClient(hWndParent, &rectOld.TopLeft());
			::ScreenToClient(hWndParent, &rectOld.BottomRight());
			if (::EqualRect(rectOld, lpRect))
				return;     // nothing to do

			// try to use DeferWindowPos for speed, otherwise use SetWindowPos
			if (lpLayout != NULL)
			{
				lpLayout->hDWP = ::DeferWindowPos(lpLayout->hDWP, hWnd, NULL,
													lpRect->left, lpRect->top, lpRect->right - lpRect->left,
													lpRect->bottom - lpRect->top, SWP_NOACTIVATE | SWP_NOZORDER);
			}
			else
			{
				::SetWindowPos(hWnd, NULL, lpRect->left, lpRect->top,
								lpRect->right - lpRect->left, lpRect->bottom - lpRect->top,
								SWP_NOACTIVATE | SWP_NOZORDER);
			}
		}


		static void AfxSetWindowText(HWND hWndCtrl, LPCTSTR lpszNew)
		{
			size_t nNewLen = lstrlen(lpszNew);
			TCHAR szOld[256] = { _T('\0') }; // _T("");
			// fast check to see if text really changes (reduces flash in controls)
			if (nNewLen > _countof(szOld) ||
				::GetWindowText(hWndCtrl, szOld, _countof(szOld)) != (int)(nNewLen) ||
				lstrcmp(szOld, lpszNew) != 0)
			{
				// change it
				::SetWindowText(hWndCtrl, lpszNew);
			}
		}

		static void AfxUnregisterWndClasses()
		{
			// unregister Window classes
			AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
			int start = 0;
			CString className = Tokenize(pModuleState->m_strUnregisterList, _T("\n"), start);
			while (!className.IsEmpty())
			{
				UnregisterClass((LPCTSTR)(className), AfxGetInstanceHandle());
				className = Tokenize(pModuleState->m_strUnregisterList, _T("\n"), start);
			}
			pModuleState->m_strUnregisterList.Empty();

		}




		static CString AfxStringFromCLSID(REFCLSID rclsid)
		{
			TCHAR szCLSID[256];
			wsprintf(szCLSID, 
				_T("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"),
				rclsid.Data1, rclsid.Data2, rclsid.Data3,
				rclsid.Data4[0], rclsid.Data4[1], rclsid.Data4[2], rclsid.Data4[3],
				rclsid.Data4[4], rclsid.Data4[5], rclsid.Data4[6], rclsid.Data4[7]);
			return szCLSID;
		}

		static BOOL AfxGetInProcServer(LPCTSTR lpszCLSID, CString& str)
		{
			HKEY hKey = NULL;
			BOOL b = FALSE;
			LPTSTR lpsz = str.GetBuffer(_MAX_PATH);
			DWORD dwSize = _MAX_PATH * sizeof(TCHAR);
			DWORD dwType = REG_NONE;
			LONG lRes = ~ERROR_SUCCESS;

			if (RegOpenKeyExW(HKEY_CLASSES_ROOT, L"CLSID", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
			{
				HKEY hKeyCLSID = NULL;
				if (RegOpenKeyEx(hKey, lpszCLSID, 0, KEY_READ, &hKeyCLSID) == ERROR_SUCCESS)
				{
					HKEY hKeyInProc = NULL;
					if (RegOpenKeyExW(hKeyCLSID, L"InProcServer32", 0, KEY_QUERY_VALUE, &hKeyInProc) == ERROR_SUCCESS)
					{
						lRes = ::RegQueryValueEx(hKeyInProc, _T(""), NULL, &dwType, (BYTE*)lpsz, &dwSize);
						b = (lRes == ERROR_SUCCESS);
						RegCloseKey(hKeyInProc);
					}
					RegCloseKey(hKeyCLSID);
				}
				RegCloseKey(hKey);
			}
			str.ReleaseBuffer();
			return b;
		}


		// CString

		static CString& GetCStringNil()
		{
			static CString csNil ;

			return csNil ;
		}

		static int _mbstowcsz(wchar_t* wcstr, const char* mbstr, ULONG count)
		{
			// count is number of wchar_t's
			if (count == 0 && wcstr != NULL)
				return 0;

			int result = ::MultiByteToWideChar(CP_ACP, 0, mbstr, -1, wcstr, count);
			ENSURE(wcstr == NULL || result <= (int)count);
			if ((result > 0) && (wcstr != NULL))
			{
				wcstr[result - 1] = 0;
			}

			return result;
		}

		static CString Tokenize(CString& rString, LPTSTR pszTokens, int& iStart)
		{
			if (iStart < 0)
				return CString();

			if ((pszTokens == NULL) || (*pszTokens == (TCHAR)0))
			{
				if (iStart < rString.GetLength())
				{
					return (CString((LPCTSTR)rString + iStart));
				}
			}
			else
			{
				LPCTSTR pszPlace = (LPCTSTR)rString + iStart;
				LPCTSTR pszEnd = (LPCTSTR)rString + rString.GetLength();
				if (pszPlace < pszEnd)
				{
					int nIncluding = (int)_tcsspn(pszPlace, pszTokens);

					if ((pszPlace + nIncluding) < pszEnd)
					{
						pszPlace += nIncluding;
						int nExcluding = (int)_tcscspn(pszPlace, pszTokens);

						int iFrom = iStart + nIncluding;
						int nUntil = nExcluding;
						iStart = iFrom + nUntil + 1;

						return (rString.Mid(iFrom, nUntil));
					}
				}
			}

			// return empty string, done tokenizing
			iStart = -1;

			return (CString());
		}

		static int _wcstombsz(char* mbstr, const wchar_t* wcstr, ULONG count)
		{
			// count is number of bytes
			if (count == 0 && mbstr != NULL)
				return 0;

			int result = ::WideCharToMultiByte(CP_ACP, 0, wcstr, -1, mbstr, count, NULL, NULL);
			ATLASSERT(mbstr == NULL || result <= (int)count);
			if ((mbstr != NULL) && (result > 0))
				mbstr[result - 1] = 0;
			return result;
		}

		// gdi
		static HBRUSH HalftoneBrush(BOOL bDelete = FALSE)
		{
			static HBRUSH hHalftoneBrush = NULL ;

			if (bDelete == FALSE)
			{
				if (hHalftoneBrush == NULL)
				{
					WORD grayPattern[8];
					for (int i = 0; i < 8; i++)
						grayPattern[i] = (WORD)(0x5555 << (i & 1));
					HBITMAP grayBitmap = CreateBitmap(8, 8, 1, 1, grayPattern);
					if (grayBitmap != NULL)
					{
						hHalftoneBrush = ::CreatePatternBrush(grayBitmap);
						DeleteObject(grayBitmap);
					}
				}

				return hHalftoneBrush ;
			}

			if (hHalftoneBrush != NULL)
			{
				::DeleteObject(hHalftoneBrush) ;
				hHalftoneBrush = NULL ;
			}

			return NULL;
		}

		static const struct _AUX_DATA& GetAfxData()
		{
			static struct _AUX_DATA _afxdata;

			return _afxdata;
		}



};

#define AfxDeferRegisterClass(fClass) AfxEndDeferRegisterClass(fClass)

#define DYNAMIC_DOWNCAST(class_name, object) \
			(class_name*)CAfx::AfxDynamicDownCast(RUNTIME_CLASS(class_name), object)

#define STATIC_DOWNCAST(class_name, object) (class_name*)(object)



_INLINE void _AfxAbbreviateName(LPTSTR lpszCanon, int cchMax, BOOL bAtLeastName) { CAfx::_AfxAbbreviateName(lpszCanon, cchMax, bAtLeastName) ; }
_INLINE BOOL AfxComparePath(LPCTSTR lpszPath1, LPCTSTR lpszPath2) { return CAfx::AfxComparePath(lpszPath1, lpszPath2); }
_INLINE void AfxDeleteObject(HGDIOBJ* pObject) { CAfx::AfxDeleteObject(pObject); }
_INLINE LONG AfxDelRegTreeHelper(HKEY hParentKey, const CString& strKeyName, CAtlTransactionManager* pTM = NULL) { return CAfx::AfxDelRegTreeHelper(hParentKey, strKeyName, pTM); }
_INLINE BOOL AfxEndDeferRegisterClass(LONG fToRegister) { return CAfx::AfxEndDeferRegisterClass(fToRegister); }
_INLINE BOOL AfxExtractSubString(CString& rString, LPCTSTR lpszFullString, int iSubString, TCHAR chSep = '\n') { return CAfx::AfxExtractSubString(rString, lpszFullString, iSubString, chSep); }
_INLINE void AfxFormatStrings(CString& rString, LPCTSTR lpszFormat, LPCTSTR const* rglpsz, int nString) { CAfx::AfxFormatStrings(rString, lpszFormat, rglpsz, nString); }
_INLINE void AfxFormatStrings(CString& rString, UINT nIDS, LPCTSTR const* rglpsz, int nString) { CAfx::AfxFormatStrings(rString, nIDS, rglpsz, nString); }
_INLINE void AfxFormatString1(CString& rString, UINT nIDS, LPCTSTR lpsz1) { CAfx::AfxFormatStrings(rString, nIDS, &lpsz1, 1); }
_INLINE BOOL _AfxFullPath2(LPTSTR lpszPathOut, LPCTSTR lpszFileIn, CFileException* pException = NULL) { return CAfx::_AfxFullPath2(lpszPathOut, lpszFileIn, pException); }
_INLINE BOOL AfxFullPath(LPTSTR lpszPathOut, LPCTSTR lpszFileIn) { return CAfx::_AfxFullPath2(lpszPathOut, lpszFileIn); }
_INLINE DWORD _AfxGetComCtlVersion() { return CAfx::_AfxGetComCtlVersion() ; }
_INLINE UINT AfxGetFileName(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax) { return CAfx::AfxGetFileName(lpszPathName, lpszTitle, nMax); }
_INLINE UINT AfxGetFileTitle(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax) { return CAfx::AfxGetFileTitle(lpszPathName, lpszTitle, nMax); }
_INLINE void AfxGetRoot(LPCTSTR lpszPath, CString& strRoot) { CAfx::AfxGetRoot(lpszPath, strRoot); }
_INLINE void AfxGlobalFree(HGLOBAL hGlobal) { CAfx::AfxGlobalFree(hGlobal) ; }
_INLINE LONG _AfxInitCommonControls(LPINITCOMMONCONTROLSEX lpInitCtrls, LONG fToRegister) { return CAfx::_AfxInitCommonControls(lpInitCtrls, fToRegister); }
_INLINE int AfxLoadString(UINT nID, LPSTR lpszBuf, UINT nMaxBuf = 256) { return CAfx::AfxLoadString(nID, lpszBuf, nMaxBuf) ; }
_INLINE int AfxLoadString(UINT nID, LPWSTR lpszBuf, UINT nMaxBuf = 256) { return CAfx::AfxLoadString(nID, lpszBuf, nMaxBuf) ; }
_INLINE void AfxGetModuleFileName(HINSTANCE hInst, CString& strFileName) { CAfx::AfxGetModuleFileName(hInst, strFileName) ; }
_INLINE void AfxPostQuitMessage(int nExitCode) { CAfx::AfxPostQuitMessage(nExitCode); }
_INLINE BOOL AfxRegisterClass(WNDCLASS* lpWndClass) { return CAfx::AfxRegisterClass(lpWndClass); }
_INLINE BOOL _AfxRegisterWithIcon(WNDCLASS* pWndCls, LPCTSTR lpszClassName, UINT nIDIcon) { return CAfx::_AfxRegisterWithIcon(pWndCls, lpszClassName, nIDIcon); }
_INLINE LPCTSTR AfxRegisterWndClass(UINT nClassStyle, HCURSOR hCursor = NULL, HBRUSH hbrBackground = NULL, HICON hIcon = NULL) { return CAfx::AfxRegisterWndClass(nClassStyle, hCursor, hbrBackground, hIcon); }
_INLINE void AfxRepositionWindow(AFX_SIZEPARENTPARAMS* lpLayout, HWND hWnd, LPCRECT lpRect) { CAfx::AfxRepositionWindow(lpLayout, hWnd, lpRect) ; }
_INLINE void AfxSetWindowText(HWND hWndCtrl, LPCTSTR lpszNew) { CAfx::AfxSetWindowText(hWndCtrl, lpszNew); }


_INLINE CString AfxStringFromCLSID(REFCLSID rclsid) { return CAfx::AfxStringFromCLSID(rclsid) ; }
_INLINE BOOL AfxGetInProcServer(LPCTSTR lpszCLSID, CString& str) { return CAfx::AfxGetInProcServer(lpszCLSID, str); }

#ifdef _WTL_TYPES
_INLINE int _mbstowcsz(wchar_t* wcstr, const char* mbstr, ULONG count) { return CAfx::_mbstowcsz(wcstr, mbstr, count) ; }
#endif
_INLINE CString Tokenize(CString& rString, LPTSTR pszTokens, int& iStart) { return CAfx::Tokenize(rString, pszTokens, iStart); }
_INLINE int _wcstombsz(char* mbstr, const wchar_t* wcstr, ULONG count) { return CAfx::_wcstombsz(mbstr, wcstr, count) ; }


/*============================================================================*/
// Standard exception throws

_INLINE void AfxThrowInvalidArgException() { THROW(&CAfx::GetCException(AFX_IDS_INVALID_ARG_EXCEPTION)); }
_INLINE void AfxThrowMemoryException() 	{ THROW(&CAfx::GetCException(AFX_IDS_MEMORY_EXCEPTION)); }
_INLINE void AfxThrowNotSupportedException() { THROW(&CAfx::GetCException(AFX_IDS_NOT_SUPPORTED_EXCEPTION)); }
_INLINE void AfxThrowResourceException() { THROW(&CAfx::GetCException(AFX_IDS_RESOURCE_EXCEPTION)); }

//_INLINE void AfxThrowArchiveException(int cause, LPCTSTR lpszArchiveName = NULL);
//_INLINE void AfxThrowFileException(int cause, LONG lOsError = -1, LPCTSTR lpszFileName = NULL);
//void AfxThrowOleException(LONG sc);



class CStringW : public CString
{
	public:
		CStringW() : CString() {}

#ifdef _UNICODE
		CStringW(LPCSTR lpsz) : CString(lpsz) {}
#else // _UNICODE
		CStringW(LPCSTR lpsz)
		{
			Init();
			int nSrcLen = (lpsz != NULL) ? lstrlenA(lpsz) : 0;
			if (nSrcLen != 0)
			{
				int nDstlen = nSrcLen * sizeof(WCHAR) ;

				if (AllocBuffer(nDstlen + 1))
				{
					_mbstowcsz((wchar_t*)m_pchData, lpsz, nSrcLen + 1);
					ReleaseBuffer(nDstlen);
				}
			}
		}

#endif // _UNICODE
		CStringW(LPCWSTR lpsz) : CString(lpsz) {}


	public:
#ifndef _UNICODE
		CString& operator = (LPCSTR lpsz)
		{
			int nSrcLen = (lpsz != NULL) ? lstrlenA(lpsz) : 0;

			int nDstlen = nSrcLen * sizeof(WCHAR);

			if (AllocBeforeWrite(nDstlen + 1))
			{
				_mbstowcsz((wchar_t*)m_pchData, lpsz, nSrcLen + 1);
				ReleaseBuffer(nDstlen);
			}
			return *this;
		}

		int GetLength() const { return CString::GetLength() / sizeof(WCHAR); }   // as an array of characters
#endif // _UNICODE
		LPWSTR GetBuffer() { return (LPWSTR)CString::GetBuffer(0) ; }
		LPCWSTR GetString() { return (LPCWSTR)m_pchData; }


};




// generate static object constructor for class registration

struct AFX_CLASSINIT
{
	AFX_CLASSINIT(CRuntimeClass* pNewClass) { AfxClassInit(pNewClass); }
};


struct CRuntimeClass
{
	LPCSTR m_lpszClassName;
	int m_nObjectSize;
	UINT m_wSchema; // schema number of the loaded class
	CObject* (*m_pfnCreateObject)(); // NULL => abstract class

	CRuntimeClass* m_pBaseClass;

	CRuntimeClass* m_pNextClass;       // linked list of registered classes
	const AFX_CLASSINIT* m_pClassInit;

	CObject* CreateObject()
	{
		ENSURE(this);

		if (m_pfnCreateObject == NULL)
		{
			TRACE(traceAppMsg, 0,
				"Error: Trying to create object which is not "
				"DECLARE_DYNCREATE \nor DECLARE_SERIAL: %hs.\n", m_lpszClassName);
			return NULL;
		}

		CObject* pObject = (*m_pfnCreateObject)();

		return pObject;
	}

	static CRuntimeClass* FromName(LPCSTR lpszClassName);

	BOOL IsDerivedFrom(const CRuntimeClass* pBaseClass) const
	{
		ENSURE(this != NULL);
		ENSURE(pBaseClass != NULL);

		// simple SI case
		const CRuntimeClass* pClassThis = this;
		while (pClassThis != NULL)
		{
			if (pClassThis == pBaseClass)
				return TRUE;
			pClassThis = pClassThis->m_pBaseClass;
		}
		return FALSE;       // walked to the top, no match
	}

	static CRuntimeClass* Load(CArchive& ar, UINT* pwSchemaNum);
	void Store(CArchive& ar) const;

};



class CCommandLineInfo
{
	public:
		CCommandLineInfo()
		{
			m_bShowSplash = TRUE;
			m_bRunEmbedded = FALSE;
			m_bRunAutomated = FALSE;
			m_nShellCommand = FileNew;
		}

	public:
		BOOL m_bRunAutomated;
		BOOL m_bRunEmbedded;
		BOOL m_bShowSplash;
		enum {
			FileNew, FileOpen, FilePrint, FilePrintTo, FileDDE, FileDDENoShow, AppRegister,
			AppUnregister, RestartByRestartManager, FileNothing = -1
		} m_nShellCommand;

		// not valid for FileNew
		CString m_strFileName;

		// valid only for FilePrintTo
		CString m_strPrinterName;
		CString m_strDriverName;
		CString m_strPortName;

	public:
		void CCommandLineInfo::ParseParamFlag(const TCHAR* pszParam)
		{
			// OLE command switches are case insensitive, while
			// shell command switches are case sensitive

			if (/* lstrcmpi((LPCTSTR)pszParam, _T("Register")) == 0 || */
				lstrcmpi((LPCTSTR)pszParam, _T("Regserver")) == 0)
				m_nShellCommand = AppRegister;
			else if (/* lstrcmpi((LPCTSTR)pszParam, _T("Unregister")) == 0 || */
				lstrcmpi((LPCTSTR)pszParam, _T("Unregserver")) == 0)
				m_nShellCommand = AppUnregister;
		}

		void CCommandLineInfo::ParseParamNotFlag(const TCHAR* pszParam)
		{
			if (m_strFileName.IsEmpty())
				m_strFileName = pszParam;
		}

		void CCommandLineInfo::ParseLast(BOOL bLast)
		{
			if (bLast)
			{
				if (m_nShellCommand == FileNew && !m_strFileName.IsEmpty())
					m_nShellCommand = FileOpen;
				m_bShowSplash = !m_bRunEmbedded && !m_bRunAutomated;
			}
		}

		// plain char* version on UNICODE for source-code backwards compatibility
		virtual void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast)
		{
			if (bFlag)
			{
				ParseParamFlag(pszParam);
			}
			else
				ParseParamNotFlag(pszParam);

			ParseLast(bLast);
		}

};




struct CCreateContext   // Creation information structure
	// All fields are optional and may be NULL
{
	// for creating new views
	CRuntimeClass* m_pNewViewClass; // runtime class of view to create or NULL
	CDocument* m_pCurrentDoc;

	// for creating MDI children (CMDIChildWnd::LoadFrame)
	CDocTemplate* m_pNewDocTemplate;

	// for sharing view/frame state from the original view/frame
	CView* m_pLastView;
	CFrameWnd* m_pCurrentFrame;

	// Implementation
	CCreateContext() { memset(this, 0, sizeof(*this)); }
};






class CRecentFileList
{
	public:
		CRecentFileList(UINT nStart, LPCTSTR lpszSection, LPCTSTR lpszEntryFormat, int nSize, int nMaxDispLen = AFX_ABBREV_FILENAME_LEN)
		{
			m_arrNames = new CString[nSize];

			m_nSize = nSize;
			m_nStart = nStart;

			m_strSectionName = lpszSection;
			m_strEntryFormat = lpszEntryFormat;

			m_nMaxDisplayLength = nMaxDispLen;
		}

		virtual ~CRecentFileList()
		{
			delete[] m_arrNames;
		}

	public:
		int m_nSize;                // contents of the MRU list
		CString* m_arrNames;
		CString m_strSectionName;   // for saving
		CString m_strEntryFormat;
		UINT m_nStart;              // for displaying
		int m_nMaxDisplayLength;
		CString m_strOriginal;      // original menu item contents

	public:
		CString& operator[](int nIndex) { ENSURE_ARG(nIndex >= 0 && nIndex < m_nSize); return m_arrNames[nIndex]; }

		virtual void Add(LPCTSTR lpszPathName)
		{
			ASSERT(m_arrNames != NULL);

			// fully qualify the path name
			TCHAR szTemp[_MAX_PATH];

			AfxFullPath(szTemp, lpszPathName);

			// update the MRU list, if an existing MRU string matches file name
			int iMRU;
			for (iMRU = 0; iMRU < m_nSize - 1; iMRU++)
			{
				if (AfxComparePath(m_arrNames[iMRU], szTemp))
					break;      // iMRU will point to matching entry
			}
			// move MRU strings before this one down
			for (; iMRU > 0; iMRU--)
			{
				ASSERT(iMRU > 0);
				ASSERT(iMRU < m_nSize);
				m_arrNames[iMRU] = m_arrNames[iMRU - 1];
			}
			// place this one at the beginning
			m_arrNames[0] = szTemp;

		}

		void Add(LPCTSTR lpszPathName, LPCTSTR lpszAppID)
		{
			Add(lpszPathName);
		}

		virtual BOOL GetDisplayName(CString& strName, int nIndex, LPCTSTR lpszCurDir, int nCurDir, BOOL bAtLeastName = TRUE) const
		{
			ENSURE_ARG(lpszCurDir == NULL || AfxIsValidString(lpszCurDir, nCurDir));

			ASSERT(m_arrNames != NULL);
			ENSURE_ARG(nIndex < m_nSize);
			if (lpszCurDir == NULL || m_arrNames[nIndex].IsEmpty())
				return FALSE;

			int nLenName = m_arrNames[nIndex].GetLength();
			LPTSTR lpch = strName.GetBuffer(nLenName + 1);
			if (lpch == NULL)
			{
				return FALSE;
			}
			lstrcpyn(lpch, m_arrNames[nIndex], nLenName + 1);
			// nLenDir is the length of the directory part of the full path
			int nLenDir = nLenName - (AfxGetFileName(lpch, NULL, 0) - 1);
			BOOL bSameDir = FALSE;
			if (nLenDir == nCurDir)
			{
				TCHAR chSave = lpch[nLenDir];
				lpch[nCurDir] = 0;  // terminate at same location as current dir
				bSameDir = AfxComparePath(lpszCurDir, lpch);
				lpch[nLenDir] = chSave;
			}
			// copy the full path, otherwise abbreviate the name
			if (bSameDir)
			{
				// copy file name only since directories are same
				TCHAR szTemp[_MAX_PATH];
				AfxGetFileTitle(lpch + nCurDir, szTemp, _countof(szTemp));
				lstrcpyn(lpch, szTemp, nLenName + 1);
			}
			else if (m_nMaxDisplayLength != -1)
			{
				// strip the extension if the system calls for it
				TCHAR szTemp[_MAX_PATH];
				AfxGetFileTitle(lpch + nLenDir, szTemp, _countof(szTemp));
				lstrcpyn(lpch + nLenDir, szTemp, nLenName + 1 - nLenDir);

				// abbreviate name based on what will fit in limited space
				_AfxAbbreviateName(lpch, m_nMaxDisplayLength, bAtLeastName);
			}
			strName.ReleaseBuffer();
			return TRUE;
		}

		int GetSize() const { return m_nSize; }

		virtual void ReadList();    // reads from registry or ini file

		virtual void Remove(int nIndex)
		{
			ENSURE_ARG(nIndex >= 0 && nIndex < m_nSize);

			m_arrNames[nIndex].Empty();
			int iMRU;
			for (iMRU = nIndex; iMRU < m_nSize - 1; iMRU++)
				m_arrNames[iMRU] = m_arrNames[iMRU + 1];

			ASSERT(iMRU < m_nSize);
			m_arrNames[iMRU].Empty();
		}

		virtual void UpdateMenu(CCmdUI* pCmdUI);

		virtual void WriteList();

};







class AFX_COM
{
	public:
		HRESULT CreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter, REFIID riid, LPVOID* ppv)
		{
			// find the object's class factory
			LPCLASSFACTORY pf = NULL;

			if (ppv == NULL)
				return E_INVALIDARG;
			*ppv = NULL;

			HRESULT hRes = GetClassObject(rclsid, IID_IClassFactory, (LPVOID*)&pf);
			if (FAILED(hRes))
				return hRes;

			if (pf == NULL)
				return E_POINTER;

			// call it to create the instance	
			hRes = pf->CreateInstance(pUnkOuter, riid, ppv);

			// let go of the factory
			pf->Release();
			return hRes;
		}

		HRESULT GetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
		{
			*ppv = NULL;
			HINSTANCE hInst = NULL;

			// find server name for this class ID

			CString strCLSID = AfxStringFromCLSID(rclsid);
			CString strServer;
			if (!AfxGetInProcServer(strCLSID, strServer))
				return REGDB_E_CLASSNOTREG;

			// try to load it
			hInst = LoadLibraryEx(strServer, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
			if (hInst == NULL)
			{
				return HRESULT_FROM_WIN32(::GetLastError());
			}

	#pragma warning(disable:4191)
			// get its entry point
			HRESULT(STDAPICALLTYPE* pfn)(REFCLSID rclsid, REFIID riid, LPVOID* ppv);
			pfn = (HRESULT(STDAPICALLTYPE*)(REFCLSID rclsid, REFIID riid, LPVOID* ppv))
				GetProcAddress(hInst, "DllGetClassObject");
	#pragma warning(default:4191)

			// call it, if it worked
			if (pfn != NULL)
				return pfn(rclsid, riid, ppv);
			return CO_E_ERRORINDLL;
		}
};


#ifndef _MTL_ATL3

// not supported

class CSettingsStore
{
		// Construction
	public:
		CSettingsStore() : m_bReadOnly(FALSE), m_bAdmin(FALSE), m_dwUserData(0) {}
		CSettingsStore(BOOL bAdmin, BOOL bReadOnly) : m_bReadOnly(bReadOnly), m_bAdmin(bAdmin), m_dwUserData(0)
		{
			m_reg.m_hKey = bAdmin ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;
		}

		virtual ~CSettingsStore() { Close(); }

	protected:
		CRegKey m_reg;
		CString m_strPath;
		BOOL    m_bReadOnly;
		BOOL    m_bAdmin;
		DWORD_PTR   m_dwUserData;

		// Operations
	public:
		virtual BOOL CreateKey(LPCTSTR lpszPath)
		{
			if (m_bReadOnly)
			{
				ASSERT(FALSE);
				return FALSE;
			}

			return m_reg.Create(m_reg.m_hKey, PreparePath(lpszPath)) == ERROR_SUCCESS;
		}

		virtual BOOL Open(LPCTSTR lpszPath)
		{
			return m_reg.Open(m_reg.m_hKey, PreparePath(lpszPath),
				m_bReadOnly ? (KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS | KEY_NOTIFY) : KEY_ALL_ACCESS) == ERROR_SUCCESS;
		}

		virtual void Close() { m_reg.Close(); }
		virtual BOOL DeleteValue(LPCTSTR lpszValue) { return m_reg.DeleteValue(lpszValue) == ERROR_SUCCESS; }

		virtual BOOL DeleteKey(LPCTSTR lpszPath, BOOL bAdmin = FALSE)
		{
			if (m_bReadOnly)
			{
				return FALSE;
			}

			m_reg.Close();
			m_reg.m_hKey = bAdmin ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;

			return m_reg.RecurseDeleteKey(PreparePath(lpszPath)) == ERROR_SUCCESS;
		}

		CString PreparePath(LPCTSTR lpszPath)
		{
			ENSURE(lpszPath != NULL);

			int iPathLen = lstrlen(lpszPath);
			if (iPathLen > 0 && lpszPath[iPathLen - 1] == _T('\\'))
			{
				iPathLen--;
			}

			return CString(lpszPath, iPathLen);
		}

		virtual BOOL Read(LPCTSTR lpszValueName, int& nValue) { return Read(lpszValueName, (DWORD&)nValue); }
		virtual BOOL Read(LPCTSTR lpszValueName, DWORD& dwValue) { return m_reg.QueryDWORDValue(lpszValueName, dwValue) == ERROR_SUCCESS; }

		virtual BOOL Read(LPCTSTR lpszValueName, CString& strValue)
		{
			ENSURE(lpszValueName != NULL);

			strValue.Empty();

			DWORD dwCount = 0;
			if (m_reg.QueryStringValue(lpszValueName, NULL, &dwCount) != ERROR_SUCCESS)
			{
				return FALSE;
			}

			if (dwCount == 0)
			{
				return TRUE;
			}

			LPTSTR szValue = new TCHAR[dwCount + 1];

			BOOL bRes = m_reg.QueryStringValue(lpszValueName, szValue, &dwCount) == ERROR_SUCCESS;
			if (bRes)
			{
				strValue = szValue;
			}

			delete[] szValue;
			return bRes;
		}

		virtual BOOL Write(LPCTSTR lpszValueName, int nValue) { return Write(lpszValueName, (DWORD)nValue); }

		virtual BOOL Write(LPCTSTR lpszValueName, DWORD dwVal)
		{
			if (m_bReadOnly)
			{
				ASSERT(FALSE);
				return FALSE;
			}

			return m_reg.SetDWORDValue(lpszValueName, dwVal) == ERROR_SUCCESS;
		}

		virtual BOOL Write(LPCTSTR lpszValueName, LPCTSTR lpszVal)
		{
			if (m_bReadOnly)
			{
				ASSERT(FALSE);
				return FALSE;
			}

			return m_reg.SetStringValue(lpszValueName, lpszVal) == ERROR_SUCCESS;
		}

};

class CSettingsStoreSP
{
	public:
		CSettingsStoreSP(DWORD_PTR dwUserData = 0) : m_pRegistry(NULL), m_dwUserData(dwUserData)
		{
		}

		~CSettingsStoreSP()
		{
			if (m_pRegistry != NULL)
			{
				ASSERT_VALID(m_pRegistry);
				delete m_pRegistry;
			}
		}

	public:
		CSettingsStore* m_pRegistry;
		DWORD_PTR      m_dwUserData;

	public:
		CSettingsStore& Create(BOOL bAdmin, BOOL bReadOnly);

};


#endif // _MTL_ATL3

