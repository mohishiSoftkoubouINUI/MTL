





/*============================================================================*/
// CCommonDialog - base class for all common dialogs

class CCommonDialog : public CDialog
{
	//DECLARE_DYNAMIC(CCommonDialog)

	public:
		CCommonDialog(CWnd* pParentWnd) : CDialog((UINT)0, pParentWnd) { }

	public:
		DECLARE_MESSAGE_MAP()

	public:
		static UINT_PTR CALLBACK _AfxCommDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		virtual void OnOK()
		{
			ASSERT_VALID(this);

			if (!UpdateData(TRUE))
			{
				TRACE(traceAppMsg, 0, "UpdateData failed during dialog termination.\n");
				// the UpdateData routine will set focus to correct item
				return;
			}

			// Common dialogs do not require ::EndDialog
			Default();
		}

		virtual void OnCancel()
		{
			ASSERT_VALID(this);

			// Common dialogs do not require ::EndDialog
			Default();
		}

};

BEGIN_MESSAGE_MAP(CCommonDialog, CDialog)
	//TRACE("CCommonDialog::hWnd=%d, uMsg=%d, wParam=%08x, lParam=%08x\n", hWnd, uMsg, wParam, lParam);
END_MESSAGE_MAP()

/*============================================================================*/
// CFileDialog - used for FileOpen... or FileSaveAs...

#ifndef _MTL_ATL3
typedef UINT_PTR(CALLBACK* COMMDLGPROC)(HWND, UINT, WPARAM, LPARAM);
#else //  _MTL_ATL3
typedef UINT (CALLBACK* COMMDLGPROC)(HWND, UINT, UINT, LONG);
#endif //  _MTL_ATL3

// Use this macro for loading a local cached function from a DLL that is known to be loaded (e.g. KERNEL32)
#define IFDYNAMICGETCACHEDFUNCTIONFORMFC(libraryname, functionname, functionpointer) \
	static volatile auto functionpointer##_cache = (decltype(::functionname)*)(NULL); \
	auto functionpointer = (decltype(::functionname)*)(functionpointer##_cache); \
	if (functionpointer == (decltype(::functionname)*)(NULL)) \
	{ \
		HINSTANCE hLibrary = GetModuleHandleW(libraryname); \
		if (hLibrary != NULL) \
		{ \
			functionpointer = (decltype(::functionname)*)(::GetProcAddress(hLibrary, #functionname)); \
			functionpointer##_cache = (decltype(::functionname)*)(::EncodePointer((PVOID)functionpointer)); \
		} \
	} \
	else \
	{ \
		functionpointer = (decltype(::functionname)*)(::DecodePointer((PVOID)functionpointer)); \
	} \
	if (functionpointer != (decltype(::functionname)*)(NULL))



class CFileDialogMTL : public CCommonDialog
{
	DECLARE_DYNAMIC(CFileDialogMTL)

	public:
		// Constructors
		CFileDialogMTL(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
					LPCTSTR lpszDefExt = NULL, LPCTSTR lpszFileName = NULL, DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					LPCTSTR lpszFilter = NULL, CWnd* pParentWnd = NULL, DWORD dwSize = 0, BOOL bVistaStyle = TRUE) : CCommonDialog(pParentWnd)
		{
#ifndef MTL_REMOVE_CFILEDIALOG_VISTA
			OSVERSIONINFO vi;
			ZeroMemory(&vi, sizeof(OSVERSIONINFO));
			vi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

			// Fix for warnings when building against WinBlue build 9444.0.130614-1739
			// warning C4996: 'GetVersionExW': was declared deprecated
			// externalapis\windows\winblue\sdk\inc\sysinfoapi.h(442)
			// Deprecated. Use VerifyVersionInfo* or IsWindows* macros from VersionHelpers.
#pragma warning( disable : 4996 )
			::GetVersionEx(&vi);
#pragma warning( default : 4996 )

			// if running under Vista
			if (vi.dwMajorVersion >= 6)
			{
				m_bVistaStyle = bVistaStyle;
			}
			else
#endif // MTL_REMOVE_CFILEDIALOG_VISTA
			{
				m_bVistaStyle = FALSE;
			}

			m_bPickFoldersMode = FALSE;
			m_bPickNonFileSysFoldersMode = FALSE;
#if 0
			// determine size of OPENFILENAME struct if dwSize is zero
			if (dwSize == 0)
			{
				dwSize = sizeof(OPENFILENAME);
			}

			// size of OPENFILENAME must be at least version 5
			ASSERT(dwSize >= sizeof(OPENFILENAME));
			// allocate memory for OPENFILENAME struct based on size passed in
			m_pOFN = (LPOPENFILENAME)(malloc(dwSize));
			ASSERT(m_pOFN != NULL);
			if (m_pOFN == NULL)
				AfxThrowMemoryException();
#else
			dwSize = sizeof(OPENFILENAME);
#endif
			memset(&m_ofn, 0, dwSize); // initialize structure to 0/NULL
			m_szFileName[0] = '\0';
			m_szFileTitle[0] = '\0';
			m_pofnTemp = NULL;

			m_bOpenFileDialog = bOpenFileDialog;
			m_bFileTypesSet = FALSE;
			m_nIDHelp = bOpenFileDialog ? AFX_IDD_FILEOPEN : AFX_IDD_FILESAVE;

			m_ofn.lStructSize = dwSize;
			m_ofn.lpstrFile = m_szFileName;
			m_ofn.nMaxFile = _countof(m_szFileName);
			m_ofn.lpstrDefExt = lpszDefExt;
			m_ofn.lpstrFileTitle = (LPTSTR)m_szFileTitle;
			m_ofn.nMaxFileTitle = _countof(m_szFileTitle);
			m_ofn.Flags |= dwFlags | OFN_ENABLEHOOK | OFN_EXPLORER;
			if (dwFlags & OFN_ENABLETEMPLATE)
				m_ofn.Flags &= ~OFN_ENABLESIZING;
			m_ofn.hInstance = AfxGetResourceHandle();
			m_ofn.lpfnHook = (COMMDLGPROC)_AfxCommDlgProc;

			// setup initial file name
			if (lpszFileName != NULL)
				lstrcpyn(m_szFileName, lpszFileName, _countof(m_szFileName));

			// Translate filter into commdlg format (lots of \0)
			if (lpszFilter != NULL)
			{
				m_strFilter = lpszFilter;
				LPTSTR pch = m_strFilter.GetBuffer(0); // modify the buffer in place
				// MFC delimits with '|' not '\0'
				while ((pch = _tcschr(pch, '|')) != NULL)
					*pch++ = '\0';
				m_ofn.lpstrFilter = m_strFilter;
				// do not call ReleaseBuffer() since the string contains '\0' characters
			}

#ifndef MTL_REMOVE_CFILEDIALOG_VISTA
			if (m_bVistaStyle == TRUE)
			{
				if (SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
				{ // multi-threaded is not supported
					IFileDialog* pIFileDialog;
					IFileDialogCustomize* pIFileDialogCustomize;

					HRESULT hr;
#ifdef _SHELLDIALOG
					USE_INTERFACE_PART_STD(FileDialogEvents);
					USE_INTERFACE_PART_STD(FileDialogControlEvents);
#endif // _SHELLDIALOG
					if (m_bOpenFileDialog)
					{
						hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER,
												IID_PPV_ARGS(&pIFileDialog));
					}
					else
					{
						hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER,
												IID_PPV_ARGS(&pIFileDialog));
					}
					if (FAILED(hr))
					{
						m_bVistaStyle = FALSE;
						return;
					}

					hr = pIFileDialog->QueryInterface(IID_PPV_ARGS(&pIFileDialogCustomize));
					ENSURE(SUCCEEDED(hr));

#ifdef _SHELLDIALOG
					hr = pIFileDialog->Advise((IFileDialogEvents*)(&m_xFileDialogEvents), &m_dwCookie);
					ENSURE(SUCCEEDED(hr));
#endif // _SHELLDIALOG
					m_pIFileDialog = (void*)(pIFileDialog);
					m_pIFileDialogCustomize = (void*)(pIFileDialogCustomize);
				}
				else
				{
					m_bVistaStyle = FALSE;
				}
			}
#endif // MTL_REMOVE_CFILEDIALOG_VISTA
		}

		virtual ~CFileDialogMTL()
		{
#ifdef _SHELLDIALOG
			free(m_pOFN);
#endif // _SHELLDIALOG

#ifndef MTL_REMOVE_CFILEDIALOG_VISTA
			if (m_bVistaStyle == TRUE)
			{
#ifdef _SHELLDIALOG
				HRESULT hr;
				hr = ((IFileDialog*)(m_pIFileDialog))->Unadvise(m_dwCookie);
				ENSURE(SUCCEEDED(hr));
#endif // _SHELLDIALOG

				((IFileDialogCustomize*)(m_pIFileDialogCustomize))->Release();
				((IFileDialog*)(m_pIFileDialog))->Release();

				m_hWnd = NULL ;

				CoUninitialize();
			}
#endif // MTL_REMOVE_CFILEDIALOG_VISTA
		}

	public:
		//DECLARE_MESSAGE_MAP()


		// Attributes
		OPENFILENAME m_ofn;
#if 0
		LPOPENFILENAME m_pOFN;

		const OPENFILENAME& GetOFN() const { return *m_pOFN; }
		OPENFILENAME& GetOFN() { return *m_pOFN; }
#endif

		BOOL m_bVistaStyle;
		BOOL m_bPickFoldersMode;
		BOOL m_bPickNonFileSysFoldersMode;
		DWORD m_dwCookie;
		void* m_pIFileDialog;
		void* m_pIFileDialogCustomize;

		BOOL m_bOpenFileDialog;       // TRUE for file open, FALSE for file save
		CString m_strFilter;          // filter string
		// separate fields with '|', terminate with '||\0'
		TCHAR m_szFileTitle[64];       // contains file title after return
		TCHAR m_szFileName[_MAX_PATH]; // contains full path name after return

		OPENFILENAME*  m_pofnTemp;

		BOOL m_bFileTypesSet;   // have file types already been set for Vista style dialog?


	public:


#ifdef _SHELLDIALOG
		virtual void OnButtonClicked(DWORD dwIDCtl)
		{
			ASSERT_VALID(this);

			// Do not call Default() if you override
			// no default processing needed
		}

		virtual void OnCheckButtonToggled(DWORD dwIDCtl, BOOL bChecked)
		{
			ASSERT_VALID(this);

			// Do not call Default() if you override
			// no default processing needed
		}

		virtual void OnControlActivating(DWORD dwIDCtl)
		{
			ASSERT_VALID(this);

			// Do not call Default() if you override
			// no default processing needed
		}
#endif // _SHELLDIALOG
		virtual void OnFileNameChange()
		{
			ASSERT_VALID(this);

			// Do not call Default() if you override
			// no default processing needed
		}

		virtual BOOL OnFileNameOK()
		{
			ASSERT_VALID(this);

			// Do not call Default() if you override
			return FALSE;
		}

		virtual void OnFolderChange()
		{
			ASSERT_VALID(this);

			// Do not call Default() if you override
			// no default processing needed
		}

		virtual void OnInitDone()
		{
			ASSERT_VALID(this);
			GetParent()->CenterWindow();

			// Do not call Default() if you override
			// no default processing needed
		}
#ifdef _SHELLDIALOG
		virtual void OnItemSelected(DWORD dwIDCtl, DWORD dwIDItem)
		{
			ASSERT_VALID(this);

			// Do not call Default() if you override
			// no default processing needed
		}
#endif // _SHELLDIALOG
		virtual void OnLBSelChangedNotify(UINT nIDBox, UINT iCurSel, UINT nCode)
		{
			ASSERT_VALID(this);

			// Do not call Default() if you override
			// no default processing needed
		}

		virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
		{
			ASSERT(pResult != NULL);

			// allow message map to override
			if (CCommonDialog::OnNotify(wParam, lParam, pResult))
				return TRUE;

			OFNOTIFY* pNotify = (OFNOTIFY*)lParam;
			switch (pNotify->hdr.code)
			{
				case CDN_INITDONE:
					OnInitDone();
					return TRUE;
				case CDN_SELCHANGE:
					OnFileNameChange();
					return TRUE;
				case CDN_FOLDERCHANGE:
					OnFolderChange();
					return TRUE;
				case CDN_SHAREVIOLATION:
					*pResult = OnShareViolation(pNotify->pszFile);
					return TRUE;
				case CDN_HELP:
					if (!SendMessage(WM_COMMAND, ID_HELP))
						SendMessage(WM_COMMANDHELP, 0, 0);
					return TRUE;
				case CDN_FILEOK:
					*pResult = OnFileNameOK();
					return TRUE;
				case CDN_TYPECHANGE:
					OnTypeChange();
					return TRUE;
			}

			return FALSE;   // not handled
		}

		virtual UINT OnShareViolation(LPCTSTR lpszPathName)
		{
			ASSERT_VALID(this);

			// Do not call Default() if you override
			return OFN_SHAREWARN; // default
		}

		virtual void OnTypeChange()
		{
			ASSERT_VALID(this);

			// Do not call Default() if you override
			// no default processing needed
		}



#ifndef  MTL_REMOVE_CFILEDIALOG_VISTA
		static HRESULT _AfxSHCreateItemFromParsingName(PCWSTR pszPath, IBindCtx *pbc, REFIID riid, void **ppv)
		{
#if _MFC_NTDDI_MIN >= NTDDI_VISTA
			return SHCreateItemFromParsingName(pszPath, pbc, riid, ppv);
#else
			// use SHCreateItemFromParsingName if it is available (only on Vista+)...
			IFDYNAMICGETCACHEDFUNCTIONFORMFC(L"shell32.dll", SHCreateItemFromParsingName, pfSHCreateItemFromParsingName)
			{
				return (*pfSHCreateItemFromParsingName)(pszPath, pbc, riid, ppv);
			}

			// ...otherwise return failure because the API is only needed for Vista+ feature support.
			return E_FAIL;
#endif
		}

		void ApplyOFNToShellDialog()
		{
			ASSERT(m_bVistaStyle == TRUE);
			if (m_bVistaStyle == TRUE)
			{
				HRESULT hr;
				// m_ofn.lpstrTitle
				if (m_ofn.lpstrTitle != NULL)
				{
#ifdef UNICODE
					hr = ((IFileDialog*)(m_pIFileDialog))->SetTitle(m_ofn.lpstrTitle);
					ENSURE(SUCCEEDED(hr));
#else
					CStringW strTitle(m_ofn.lpstrTitle);
					hr = ((IFileDialog*)(m_pIFileDialog))->SetTitle(strTitle.GetString());
					ENSURE(SUCCEEDED(hr));
#endif
				}
				// m_ofn.lpstrDefExt
				if (m_ofn.lpstrDefExt != NULL)
				{
					CStringW strDefExt(m_ofn.lpstrDefExt);
					hr = ((IFileDialog*)(m_pIFileDialog))->SetDefaultExtension(strDefExt.GetString());
					ENSURE(SUCCEEDED(hr));
				}
				// m_ofn.lpstrFilter
				if (m_ofn.lpstrFilter != NULL)
				{
					UINT nFilterCount = 0;
					LPCTSTR lpstrFilter = m_ofn.lpstrFilter;
					while (lpstrFilter[0])
					{
						lpstrFilter += lstrlen(lpstrFilter) + 1;
						lpstrFilter += lstrlen(lpstrFilter) + 1;
						nFilterCount++;
					}
					if (nFilterCount > 0)
					{
						COMDLG_FILTERSPEC* pFilter = NULL;
						pFilter = new COMDLG_FILTERSPEC[nFilterCount];
						ASSERT(pFilter != NULL);
						if (pFilter == NULL)
						{
							AfxThrowMemoryException();
						}
						lpstrFilter = m_ofn.lpstrFilter;
						size_t nFilterIndex = 0;
						size_t filterSize;
						LPWSTR lpwstrFilter;
						while (nFilterIndex < nFilterCount)
						{
							CStringW strTemp;

							filterSize = lstrlen(lpstrFilter) + 1;
							lpwstrFilter = (LPWSTR)(new WCHAR[filterSize]);
							ASSERT(lpwstrFilter != NULL);
							if (lpwstrFilter == NULL)
							{
								AfxThrowMemoryException();
							}
							strTemp = lpstrFilter;
							memcpy(lpwstrFilter, strTemp.GetString(), (strTemp.GetLength() + 1)*sizeof(WCHAR));
							pFilter[nFilterIndex].pszName = lpwstrFilter;
							lpstrFilter += filterSize;

							filterSize = lstrlen(lpstrFilter) + 1;
							lpwstrFilter = (LPWSTR)(new WCHAR[filterSize]);
							ASSERT(lpwstrFilter != NULL);
							if (lpwstrFilter == NULL)
							{
								AfxThrowMemoryException();
							}
							strTemp = lpstrFilter;
							memcpy(lpwstrFilter, strTemp.GetString(), (strTemp.GetLength() + 1)*sizeof(WCHAR));
							pFilter[nFilterIndex].pszSpec = lpwstrFilter;
							lpstrFilter += filterSize;

							nFilterIndex++;
						}

						if (!m_bFileTypesSet)  // file types can only be set once per dialog instance
						{
							hr = ((IFileDialog*)(m_pIFileDialog))->SetFileTypes(nFilterCount, pFilter);
							ENSURE(SUCCEEDED(hr));
							m_bFileTypesSet = TRUE;
						}

						for (nFilterIndex = 0; nFilterIndex < nFilterCount; nFilterIndex++)
						{
							delete[] pFilter[nFilterIndex].pszName;
							delete[] pFilter[nFilterIndex].pszSpec;
						}
						delete[] pFilter;

						hr = ((IFileDialog*)(m_pIFileDialog))->SetFileTypeIndex(m_ofn.nFilterIndex > 1 ? m_ofn.nFilterIndex : 1);
						ENSURE(SUCCEEDED(hr));
					}
				}
				// m_ofn.lpstrFile and m_ofn.lpstrInitialDir
				if ((m_ofn.lpstrFile != NULL) || (m_ofn.lpstrInitialDir != NULL))
				{
					CStringW strInitialDir;
					if ((m_ofn.lpstrFile != NULL) && (m_ofn.lpstrFile[0] != '\0'))
					{
						CStringW strFile(m_ofn.lpstrFile);
						strInitialDir = strFile;
						::PathRemoveFileSpecW(strInitialDir.GetBuffer());
						strInitialDir.ReleaseBuffer();
						int offset = strInitialDir.GetLength();
						if (strFile[offset] == L'\\')
						{
							offset++;
						}
						hr = ((IFileDialog*)(m_pIFileDialog))->SetFileName(strFile.GetString() + offset);
						ENSURE(SUCCEEDED(hr));
					}
					if ((m_ofn.lpstrInitialDir != NULL) && strInitialDir.IsEmpty())
					{
						strInitialDir = m_ofn.lpstrInitialDir;
					}
					if (!strInitialDir.IsEmpty())
					{
						IShellItem *psiInitialDir = NULL;

						hr = _AfxSHCreateItemFromParsingName(strInitialDir.GetString(), NULL, IID_PPV_ARGS(&psiInitialDir));
						if (SUCCEEDED(hr))
						{
							hr = ((IFileDialog*)(m_pIFileDialog))->SetFolder(psiInitialDir);
							ENSURE(SUCCEEDED(hr));
							psiInitialDir->Release();
						}
					}
				}
				// m_ofn.Flags
				DWORD dwFlags = 0;
				hr = ((IFileDialog*)(m_pIFileDialog))->GetOptions(&dwFlags);
				ENSURE(SUCCEEDED(hr));

#ifndef VISTA_FILE_DIALOG_FLAG_MAPPING
#define VISTA_FILE_DIALOG_FLAG_MAPPING(OLD,NEW) \
	((m_ofn.Flags & (OLD)) ? (dwFlags |= (NEW)) : (dwFlags &= ~(NEW)))
#ifndef VISTA_FILE_DIALOG_FLAG_DIRECT_MAPPING
#define VISTA_FILE_DIALOG_FLAG_DIRECT_MAPPING(FLAG) \
	VISTA_FILE_DIALOG_FLAG_MAPPING(OFN_##FLAG, FOS_##FLAG)

				VISTA_FILE_DIALOG_FLAG_DIRECT_MAPPING(ALLOWMULTISELECT);
				VISTA_FILE_DIALOG_FLAG_DIRECT_MAPPING(CREATEPROMPT);
				VISTA_FILE_DIALOG_FLAG_DIRECT_MAPPING(DONTADDTORECENT);
				VISTA_FILE_DIALOG_FLAG_DIRECT_MAPPING(FILEMUSTEXIST);
				VISTA_FILE_DIALOG_FLAG_DIRECT_MAPPING(FORCESHOWHIDDEN);
				VISTA_FILE_DIALOG_FLAG_DIRECT_MAPPING(NOCHANGEDIR);
				VISTA_FILE_DIALOG_FLAG_DIRECT_MAPPING(NODEREFERENCELINKS);
				VISTA_FILE_DIALOG_FLAG_DIRECT_MAPPING(NOREADONLYRETURN);
				VISTA_FILE_DIALOG_FLAG_DIRECT_MAPPING(NOTESTFILECREATE);
				VISTA_FILE_DIALOG_FLAG_DIRECT_MAPPING(NOVALIDATE);
				VISTA_FILE_DIALOG_FLAG_DIRECT_MAPPING(OVERWRITEPROMPT);
				VISTA_FILE_DIALOG_FLAG_DIRECT_MAPPING(PATHMUSTEXIST);
				VISTA_FILE_DIALOG_FLAG_DIRECT_MAPPING(SHAREAWARE);

#undef VISTA_FILE_DIALOG_FLAG_DIRECT_MAPPING
#undef VISTA_FILE_DIALOG_FLAG_MAPPING
#endif
#endif

				dwFlags &= ~FOS_HIDEPINNEDPLACES;
				if ((m_ofn.FlagsEx & OFN_EX_NOPLACESBAR) == OFN_EX_NOPLACESBAR)
				{
					dwFlags |= FOS_HIDEPINNEDPLACES;
				}

				dwFlags &= ~FOS_PICKFOLDERS;
				if (m_bPickFoldersMode || m_bPickNonFileSysFoldersMode)
				{
					dwFlags |= FOS_PICKFOLDERS;
				}

				// We only expect and handle file system paths (for compatibility with GetOpenFileName functionality), so set the
				// "force file system" flag which enables GetOpenFileName-like download behavior for non file system paths, unless
				// the m_bPickNonFileSysFoldersMode is set to allow picking non-file system folders (like libraries in Windows 7).
				dwFlags |= FOS_FORCEFILESYSTEM;
				if (m_bPickNonFileSysFoldersMode)
				{
					dwFlags &= ~FOS_FORCEFILESYSTEM;
				}

				hr = ((IFileDialog*)(m_pIFileDialog))->SetOptions(dwFlags);
				ENSURE(SUCCEEDED(hr));
			}
		}
#endif MTL_REMOVE_CFILEDIALOG_VISTA

		// Operations
		virtual INT_PTR DoModal()
		{
			ASSERT_VALID(this);
			ASSERT(m_ofn.Flags & OFN_ENABLEHOOK);
			ASSERT(m_ofn.lpfnHook != NULL); // can still be a user hook

			// zero out the file buffer for consistent parsing later
			ASSERT(AfxIsValidAddress(m_ofn.lpstrFile, m_ofn.nMaxFile));
			DWORD nOffset = (DWORD)(lstrlen(m_ofn.lpstrFile)) + 1;
			ASSERT(nOffset <= m_ofn.nMaxFile);
			memset(m_ofn.lpstrFile + nOffset, 0, (m_ofn.nMaxFile - nOffset)*sizeof(TCHAR));

			//  This is a special case for the file open/save dialog,
			//  which sometimes pumps while it is coming up but before it has
			//  disabled the main window.
			HWND hWndFocus = ::GetFocus();
			BOOL bEnableParent = FALSE;
			m_ofn.hwndOwner = CWnd::GetSafeOwner_(m_pParentWnd->GetSafeHwnd(), NULL);
			if (m_ofn.hwndOwner != NULL && ::IsWindowEnabled(m_ofn.hwndOwner))
			{
				bEnableParent = TRUE;
				::EnableWindow(m_ofn.hwndOwner, FALSE);
			}

			//HookWindowCreate(this);
			CREATEWINDOWPARAM& cwp = GetHookOldCbtFilter();
			cwp.pCWnd = this;

			m_nFlags |= WF_CONTINUEMODAL;

			INT_PTR nResult = 0;

			if (m_bVistaStyle == TRUE)
			{
#ifndef MTL_REMOVE_CFILEDIALOG_VISTA
				ApplyOFNToShellDialog();
				HRESULT hr = ((IFileDialog*)(m_pIFileDialog))->Show(m_ofn.hwndOwner);
				nResult = (hr == S_OK) ? IDOK : IDCANCEL;
				if (nResult == IDOK)
					UpdateOFNFromShellDialog() ;
#endif // MTL_REMOVE_CFILEDIALOG_VISTA
			}
			else if (m_bOpenFileDialog)
				nResult = GetOpenFileName(&m_ofn);
			else
				nResult = GetSaveFileName(&m_ofn);

			//UnhookWindowCreate();

			// Second part of special case for file open/save dialog.
			if (bEnableParent)
				::EnableWindow(m_ofn.hwndOwner, TRUE);
			if (::IsWindow(hWndFocus))
				::SetFocus(hWndFocus);

			return nResult ? nResult : IDCANCEL;
		}

		// Helpers for parsing file name after successful return
		// or during Overridable callbacks if OFN_EXPLORER is set
		CString GetFileExt()   // return only ext
		{
#ifndef MTL_REMOVE_CFILEDIALOG_VISTA
			if (m_bVistaStyle == TRUE)
			{
				CString strResult;
				if (m_hWnd != NULL)
				{
					strResult = GetFileName();
				}
				else
				{
					strResult = GetPathName();
				}
				strResult.ReleaseBuffer();
				LPTSTR pszExtension = ::PathFindExtension(strResult);
				if (pszExtension != NULL && *pszExtension == _T('.'))
				{
					return pszExtension + 1;
				}

				strResult.Empty();
				return strResult;
			}
#endif // MTL_REMOVE_CFILEDIALOG_VISTA
			if ((m_ofn.Flags & OFN_EXPLORER) && m_hWnd != NULL)
			{
				ASSERT(::IsWindow(m_hWnd));
				CString strResult;
				LPTSTR pszResult = strResult.GetBuffer(MAX_PATH);
				LRESULT nResult = GetParent()->SendMessage(CDM_GETSPEC, MAX_PATH, (LPARAM)(pszResult));
				strResult.ReleaseBuffer();
				if (nResult >= 0)
				{
					LPTSTR pszExtension = ::PathFindExtension(strResult);
					if (pszExtension != NULL && *pszExtension == _T('.'))
					{
						return pszExtension + 1;
					}
				}

				strResult.Empty();
				return strResult;
			}

			if (m_pofnTemp != NULL)
				if (m_pofnTemp->nFileExtension == 0)
					return _T("");
				else
					return m_pofnTemp->lpstrFile + m_pofnTemp->nFileExtension;

			if (m_ofn.nFileExtension == 0)
				return _T("");
			else
				return m_ofn.lpstrFile + m_ofn.nFileExtension;
		}

		CString GetFileName()  // return only filename
		{
#ifndef MTL_REMOVE_CFILEDIALOG_VISTA
			if (m_bVistaStyle == TRUE)
			{
				if (m_hWnd != NULL)
				{
					LPWSTR wcFileName;
					HRESULT hr = ((IFileDialog*)(m_pIFileDialog))->GetFileName(&wcFileName);
					CString strResult(wcFileName);

					if (SUCCEEDED(hr))
					{
						CoTaskMemFree(wcFileName);
					}
					strResult.ReleaseBuffer();
					return strResult;
				}

				return m_ofn.lpstrFileTitle;
			}
#endif // MTL_REMOVE_CFILEDIALOG_VISTA

			if ((m_ofn.Flags & OFN_EXPLORER) && m_hWnd != NULL)
			{
				ASSERT(::IsWindow(m_hWnd));
				CString strResult;
				if (GetParent()->SendMessage(CDM_GETSPEC, (WPARAM)MAX_PATH, (LPARAM)strResult.GetBuffer(MAX_PATH)) < 0)
				{
					strResult.Empty();
				}
				else
				{
					strResult.ReleaseBuffer();
					return strResult;
				}
			}
			return m_ofn.lpstrFileTitle;
		}

		CString GetFileTitle() // return file title
		{
			CString strResult = GetFileName();
			LPTSTR pszBuffer = strResult.GetBuffer(0);
			::PathRemoveExtension(pszBuffer);
			strResult.ReleaseBuffer();
			return strResult;
		}

		CString GetFolderPath()  // return full path
		{
			CString strResult;
#ifndef MTL_REMOVE_CFILEDIALOG_VISTA
			if (m_bVistaStyle == TRUE)
			{
				IShellItem *psiResult;
				HRESULT hr = ((IFileDialog*)(m_pIFileDialog))->GetFolder(&psiResult);
				if (SUCCEEDED(hr))
				{
					LPWSTR wcFolderPath = NULL;
					hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &wcFolderPath);
					if (SUCCEEDED(hr))
					{
						strResult = wcFolderPath;
						CoTaskMemFree(wcFolderPath);
					}
					psiResult->Release();
				}
			}
			else
#endif // MTL_REMOVE_CFILEDIALOG_VISTA
			{
				ASSERT(::IsWindow(m_hWnd));
				ASSERT(m_ofn.Flags & OFN_EXPLORER);

				if (GetParent()->SendMessage(CDM_GETFOLDERPATH, (WPARAM)MAX_PATH, (LPARAM)strResult.GetBuffer(MAX_PATH)) < 0)
					strResult.Empty();
				else
					strResult.ReleaseBuffer();
			}
			return strResult;
		}



#ifndef MTL_REMOVE_CFILEDIALOG_VISTA
		IFileDialogCustomize* GetIFileDialogCustomize() throw()
		{
			ASSERT(m_bVistaStyle == TRUE);
			IFileDialogCustomize* pIFileDialogCustomize = NULL;
			if (m_bVistaStyle == TRUE)
			{
				((IFileDialog*)(m_pIFileDialog))->QueryInterface(IID_PPV_ARGS(&pIFileDialogCustomize));
			}
			return pIFileDialogCustomize;
		}

		IFileOpenDialog* GetIFileOpenDialog() throw()
		{
			ASSERT(m_bVistaStyle == TRUE);
			IFileOpenDialog* pIFileOpenDialog = NULL;
			if (m_bVistaStyle == TRUE)
			{
				((IFileDialog*)(m_pIFileDialog))->QueryInterface(IID_PPV_ARGS(&pIFileOpenDialog));
			}
			return pIFileOpenDialog;
		}

		IFileSaveDialog* GetIFileSaveDialog() throw()
		{
			ASSERT(m_bVistaStyle == TRUE);
			IFileSaveDialog* pIFileSaveDialog = NULL;
			if (m_bVistaStyle == TRUE)
			{
				((IFileDialog*)(m_pIFileDialog))->QueryInterface(IID_PPV_ARGS(&pIFileSaveDialog));
			}
			return pIFileSaveDialog;
		}
#endif // MTL_REMOVE_CFILEDIALOG_VISTA

		CString GetNextPathName(POSITION& pos) const
		{
			BOOL bExplorer = m_ofn.Flags & OFN_EXPLORER;
			TCHAR chDelimiter;
			if (bExplorer)
				chDelimiter = '\0';
			else
				chDelimiter = ' ';

			LPTSTR lpsz = (LPTSTR)pos;
			if (lpsz == m_ofn.lpstrFile) // first time
			{
				if ((m_ofn.Flags & OFN_ALLOWMULTISELECT) == 0)
				{
					pos = NULL;
					return m_ofn.lpstrFile;
				}

				// find char pos after first Delimiter
				while (*lpsz != chDelimiter && *lpsz != '\0')
					lpsz = _CHAR_INC(lpsz);
				lpsz = _CHAR_INC(lpsz);

				// if single selection then return only selection
				if (*lpsz == 0)
				{
					pos = NULL;
					return m_ofn.lpstrFile;
				}
			}

			CString strBasePath = m_ofn.lpstrFile;
			if (!bExplorer)
			{
				LPTSTR lpszPath = m_ofn.lpstrFile;
				while (*lpszPath != chDelimiter)
					lpszPath = _CHAR_INC(lpszPath);
				strBasePath = strBasePath.Left(int(lpszPath - m_ofn.lpstrFile));
			}

			LPTSTR lpszFileName = lpsz;
			CString strFileName = lpsz;

			// find char pos at next Delimiter
			while (*lpsz != chDelimiter && *lpsz != '\0')
				lpsz = _CHAR_INC(lpsz);

			if (!bExplorer && *lpsz == '\0')
				pos = NULL;
			else
			{
				if (!bExplorer)
					strFileName = strFileName.Left(int(lpsz - lpszFileName));

				lpsz = _CHAR_INC(lpsz);
				if (*lpsz == '\0') // if double terminated then done
					pos = NULL;
				else
					pos = (POSITION)lpsz;
			}

			TCHAR strDrive[_MAX_DRIVE], strDir[_MAX_DIR], strName[_MAX_FNAME], strExt[_MAX_EXT];
			_tsplitpath(strFileName, strDrive, strDir, strName, strExt);
			TCHAR strPath[_MAX_PATH];
			if (*strDrive || *strDir)
			{
				lstrcpy(strPath, strFileName);
			}
			else
			{
				if ((strBasePath.GetLength() != 3) || (strBasePath[1] != ':') || (strBasePath[2] != '\\'))
				{
					strBasePath += _T("\\");
				}
				_tsplitpath(strBasePath, strDrive, strDir, NULL, NULL);
				_tmakepath(strPath, strDrive, strDir, strName, strExt);
			}

			return strPath;
		}

		CString GetPathName() // return full path and filename
		{
#ifndef MTL_REMOVE_CFILEDIALOG_VISTA
			if (m_bVistaStyle == TRUE)
			{
				if (m_hWnd != NULL)
				{
					CString strResult;
					IShellItem *psiResult;
					HRESULT hr = (static_cast<IFileDialog*>(m_pIFileDialog))->GetCurrentSelection(&psiResult);
					if (SUCCEEDED(hr))
					{
						SFGAOF sfgaoAttribs;
						if ((psiResult->GetAttributes(SFGAO_STREAM, &sfgaoAttribs) == S_FALSE)
							&& (psiResult->GetAttributes(SFGAO_FOLDER, &sfgaoAttribs) == S_OK))
						{
							;
						}
						else
						{
							LPWSTR wcPathName = NULL;
							hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &wcPathName);
							if (SUCCEEDED(hr))
							{
								strResult = wcPathName;
								strResult.ReleaseBuffer();
								CoTaskMemFree(wcPathName);
							}
						}
						psiResult->Release();
					}
					return strResult;
				}

				return m_ofn.lpstrFile;
			}
#endif // MTL_REMOVE_CFILEDIALOG_VISTA
			if ((m_ofn.Flags & OFN_EXPLORER) && m_hWnd != NULL)
			{
				ASSERT(::IsWindow(m_hWnd));
				CString strResult;
				if (GetParent()->SendMessage(CDM_GETSPEC, (WPARAM)MAX_PATH,
											(LPARAM)strResult.GetBuffer(MAX_PATH)) < 0)
				{
					strResult.Empty();
				}
				else
				{
					strResult.ReleaseBuffer();
				}

				if (!strResult.IsEmpty())
				{
					if (GetParent()->SendMessage(CDM_GETFILEPATH, (WPARAM)MAX_PATH,
												(LPARAM)strResult.GetBuffer(MAX_PATH)) < 0)
						strResult.Empty();
					else
					{
						strResult.ReleaseBuffer();
						return strResult;
					}
				}
			}
			return m_ofn.lpstrFile;
		}

		BOOL GetReadOnlyPref() const { return m_ofn.Flags & OFN_READONLY ? TRUE : FALSE; }// return TRUE if readonly checked

		POSITION GetStartPosition() const { return (POSITION)m_ofn.lpstrFile; }

		void HideControl(int nID)
		{
			ASSERT(::IsWindow(m_hWnd));
#ifndef MTL_REMOVE_CFILEDIALOG_VISTA
			if (m_bVistaStyle == TRUE)
			{
				HRESULT hr = ((IFileDialogCustomize*)(m_pIFileDialogCustomize))->SetControlState(nID, CDCS_INACTIVE);
				ENSURE(SUCCEEDED(hr));
			}
			else
#endif // MTL_REMOVE_CFILEDIALOG_VISTA
			{
				ASSERT(m_ofn.Flags & OFN_EXPLORER);
				GetParent()->SendMessage(CDM_HIDECONTROL, (WPARAM)nID, 0);
			}
		}


		BOOL IsPickFoldersMode() const { return m_bPickFoldersMode; }
		BOOL IsPickNonFileSysFoldersMode() const { return m_bPickNonFileSysFoldersMode; }


		void SetControlText(int nID, LPCTSTR lpsz)
		{
			ASSERT(::IsWindow(m_hWnd));

#ifndef MTL_REMOVE_CFILEDIALOG_VISTA
			if (m_bVistaStyle == TRUE)
			{
#ifdef UNICODE
				HRESULT hr = (static_cast<IFileDialogCustomize*>(m_pIFileDialogCustomize))->SetControlLabel(nID, lpsz);
#else
				CStringW dest(lpsz);
				HRESULT hr = (static_cast<IFileDialogCustomize*>(m_pIFileDialogCustomize))->SetControlLabel(nID, dest.GetString());
#endif
				ENSURE(SUCCEEDED(hr));
			}
			else
#endif // MTL_REMOVE_CFILEDIALOG_VISTA
			{
				ASSERT(m_ofn.Flags & OFN_EXPLORER);
				GetParent()->SendMessage(CDM_SETCONTROLTEXT, (WPARAM)nID, (LPARAM)lpsz);
			}
		}

		void SetDefExt(LPCTSTR lpsz)
		{
			ASSERT(::IsWindow(m_hWnd));
#ifndef MTL_REMOVE_CFILEDIALOG_VISTA
			if (m_bVistaStyle == TRUE)
			{
#ifdef UNICODE
				HRESULT hr = (static_cast<IFileDialog*>(m_pIFileDialog))->SetDefaultExtension(lpsz);
#else
				CStringW strExt(lpsz);
				HRESULT hr = (static_cast<IFileDialog*>(m_pIFileDialog))->SetDefaultExtension(strExt.GetString());
#endif
				ENSURE(SUCCEEDED(hr));
			}
			else
#endif // MTL_REMOVE_CFILEDIALOG_VISTA
			{
				ASSERT(m_ofn.Flags & OFN_EXPLORER);
				GetParent()->SendMessage(CDM_SETDEFEXT, 0, (LPARAM)lpsz);
			}
		}

		void SetTemplate(UINT nWin3ID, UINT nWin4ID) { SetTemplate(MAKEINTRESOURCE(nWin3ID), MAKEINTRESOURCE(nWin4ID)); }

		void SetTemplate(LPCTSTR lpWin3ID, LPCTSTR lpWin4ID)
		{
#ifndef MTL_REMOVE_CFILEDIALOG_VISTA
			if (m_bVistaStyle == TRUE)
			{
				AfxThrowNotSupportedException();
			}
#endif // MTL_REMOVE_CFILEDIALOG_VISTA
			if (m_ofn.Flags & OFN_EXPLORER)
				m_ofn.lpTemplateName = lpWin4ID;
			else
				m_ofn.lpTemplateName = lpWin3ID;
			m_ofn.Flags |= OFN_ENABLETEMPLATE;
		}

		virtual void UpdateOFNFromShellDialog()
		{
#ifndef MTL_REMOVE_CFILEDIALOG_VISTA
			ASSERT(m_bVistaStyle == TRUE);
			if (m_bVistaStyle == TRUE)
			{
				IShellItem *psiResult;
				HRESULT hr = ((IFileDialog*)(m_pIFileDialog))->GetResult(&psiResult);
				if (SUCCEEDED(hr))
				{
					// Save properties:
					IFileSaveDialog* pSaveFile = GetIFileSaveDialog();
					if (pSaveFile != NULL)
					{
						IPropertyStore* pStore = NULL;
						HRESULT hr = pSaveFile->GetProperties(&pStore);
						if (SUCCEEDED(hr))
						{
							pSaveFile->ApplyProperties(psiResult, pStore, GetSafeHwnd (), NULL);
							pStore->Release();
						}

						pSaveFile->Release();
					}	

					LPWSTR wcPathName = NULL;
					hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &wcPathName);
					if (SUCCEEDED(hr))
					{
						CStringW strTmp(wcPathName);
						::PathRemoveFileSpecW(strTmp.GetBuffer());
						strTmp.ReleaseBuffer();
						size_t offset = strTmp.GetLength();
						if (wcPathName[offset] == L'\\')
						{
							offset++;
						}
#ifdef UNICODE
						lstrcpyn(m_ofn.lpstrFile, wcPathName, m_ofn.nMaxFile);
						lstrcpyn(m_ofn.lpstrFileTitle, wcPathName + offset, m_ofn.nMaxFileTitle);
#else
						::WideCharToMultiByte(CP_ACP, 0, wcPathName + offset,
							-1, m_ofn.lpstrFileTitle, m_ofn.nMaxFileTitle, NULL, NULL);
						m_ofn.lpstrFileTitle[m_ofn.nMaxFileTitle - 1] = _T('\0');
						::WideCharToMultiByte(CP_ACP, 0, wcPathName, -1, m_ofn.lpstrFile,
							m_ofn.nMaxFile - 1, NULL, NULL);
						m_ofn.lpstrFile[m_ofn.nMaxFile - 2] = _T('\0');
#endif
						m_ofn.lpstrFile[lstrlen(m_ofn.lpstrFile) + 1] = _T('\0');
						CoTaskMemFree(wcPathName);
					}
					psiResult->Release();
				}
				else if (m_ofn.Flags & OFN_ALLOWMULTISELECT)
				{
					IFileOpenDialog *pfod = NULL;
					HRESULT hr = ((IFileDialog*)(m_pIFileDialog))->QueryInterface(IID_PPV_ARGS(&pfod));
					if (SUCCEEDED(hr))
					{
						IShellItemArray *ppenum = NULL;
						HRESULT hr = pfod->GetResults(&ppenum);
						if (SUCCEEDED(hr))
						{
							IEnumShellItems *ppenumShellItems;
							hr = ppenum->EnumItems(&ppenumShellItems);
							if (SUCCEEDED(hr))
							{
								IShellItem *rgelt[1];
								ULONG celtFetched = 0;
								if (ppenumShellItems->Next(1, rgelt, &celtFetched) == S_OK)
								{
									CStringW strTmp;
									LPTSTR pszFileName = m_ofn.lpstrFile;
									LPWSTR wcPathName = NULL;
									hr = rgelt[0]->GetDisplayName(SIGDN_FILESYSPATH, &wcPathName);
									if (SUCCEEDED(hr))
									{
										::PathRemoveFileSpecW(wcPathName);
#ifdef UNICODE
										lstrcpyn(pszFileName, wcPathName, m_ofn.nMaxFile);
										pszFileName += lstrlen(wcPathName) + 1;
#else
										pszFileName += ::WideCharToMultiByte(CP_ACP, 0, wcPathName, -1,
																pszFileName, m_ofn.nMaxFile - 1, NULL, NULL);
#endif
										CoTaskMemFree(wcPathName);
									}
									do
									{
										wcPathName = NULL;
										hr = rgelt[0]->GetDisplayName(SIGDN_FILESYSPATH, &wcPathName);
										if (SUCCEEDED(hr))
										{
											INT iPathNameLen = lstrlenW(wcPathName);
											strTmp = wcPathName;
											::PathRemoveFileSpecW((LPWSTR)strTmp.GetBufferSetLength(iPathNameLen));
											strTmp.ReleaseBuffer();
											size_t offset = strTmp.GetLength();
											if (wcPathName[offset] == L'\\')
											{
												offset++;
											}
#ifdef UNICODE
											lstrcpyn(pszFileName, wcPathName + offset, (INT)(m_ofn.nMaxFile - (pszFileName - m_ofn.lpstrFile) - 1));
											pszFileName += lstrlen(wcPathName + offset) + 1;
#else
											pszFileName += ::WideCharToMultiByte(CP_ACP, 0, wcPathName + offset, -1, pszFileName,
												m_ofn.nMaxFile - static_cast<int>(pszFileName - m_ofn.lpstrFile) - 1,
												NULL, NULL);
#endif
											CoTaskMemFree(wcPathName);
										}
										hr = rgelt[0]->Release();
									} while ((pszFileName < m_ofn.lpstrFile + m_ofn.nMaxFile - 1)
										&& (ppenumShellItems->Next(1, rgelt, &celtFetched) == S_OK));
									if (pszFileName < m_ofn.lpstrFile + m_ofn.nMaxFile - 1)
									{
										pszFileName[0] = _T('\0');
									}
									else
									{
										m_ofn.lpstrFile[m_ofn.nMaxFile - 2] = _T('\0');
										m_ofn.lpstrFile[m_ofn.nMaxFile - 1] = _T('\0');
									}
								}
								ppenumShellItems->Release();
							}
							ppenum->Release();
						}
						pfod->Release();
					}
				}

				CString strPathName = GetPathName();

				CString strFileName;
				LPTSTR pszFilename = ::PathFindFileName(strPathName);
				if (pszFilename != NULL)
				{
					strFileName = pszFilename;
				}

				CString strExtension;
				LPTSTR pszExtension = ::PathFindExtension(strPathName);
				if (pszExtension != NULL && *pszExtension == _T('.'))
				{
					strExtension = pszExtension + 1;
				}

				m_ofn.nFileOffset = (WORD)(strPathName.GetLength() - strFileName.GetLength());
				m_ofn.nFileExtension = (WORD)(strPathName.GetLength() - strExtension.GetLength());
			}
#endif // MTL_REMOVE_CFILEDIALOG_VISTA
		}


#ifndef MTL_REMOVE_CFILEDIALOG_VISTA
#ifdef _SHELLDIALOG
		HRESULT AddCheckButton(DWORD dwIDCtl, const CString& strLabel, BOOL bChecked)
		{
			if (!m_bVistaStyle) { return E_NOTIMPL; }

			IFileDialogCustomize* pFileDialogCustomize = GetIFileDialogCustomize();

#ifndef UNICODE
			CStringW strLabelW = strLabel;
			HRESULT hr = pFileDialogCustomize->AddCheckButton(dwIDCtl, (LPCWSTR)strLabelW.GetString(), bChecked);
#else
			HRESULT hr = pFileDialogCustomize->AddCheckButton(dwIDCtl, (LPCWSTR)strLabel.GetString(), bChecked);
#endif

			pFileDialogCustomize->Release();
			return hr;
		}

		HRESULT AddComboBox(DWORD dwIDCtl)
		{
			if (!m_bVistaStyle) { return E_NOTIMPL; }

			IFileDialogCustomize* pFileDialogCustomize = GetIFileDialogCustomize();

			HRESULT hr = pFileDialogCustomize->AddComboBox(dwIDCtl);

			pFileDialogCustomize->Release();
			return hr;
		}

		HRESULT AddControlItem(DWORD dwIDCtl, DWORD dwIDItem, const CString& strLabel)
		{
			if (!m_bVistaStyle) { return E_NOTIMPL; }

			IFileDialogCustomize* pFileDialogCustomize = GetIFileDialogCustomize();

#ifndef UNICODE
			CStringW strLabelW = strLabel;
			HRESULT hr = pFileDialogCustomize->AddControlItem(dwIDCtl, dwIDItem, (LPCWSTR)strLabelW.GetString());
#else
			HRESULT hr = pFileDialogCustomize->AddControlItem(dwIDCtl, dwIDItem, (LPCWSTR)strLabel.GetString());
#endif

			pFileDialogCustomize->Release();
			return hr;
		}

		HRESULT AddEditBox(DWORD dwIDCtl, const CString& strText)
		{
			if (!m_bVistaStyle) { return E_NOTIMPL; }

			IFileDialogCustomize* pFileDialogCustomize = GetIFileDialogCustomize();

#ifndef UNICODE
			CStringW strTextW = strText;
			HRESULT hr = pFileDialogCustomize->AddEditBox(dwIDCtl, (LPCWSTR)strTextW.GetString());
#else
			HRESULT hr = pFileDialogCustomize->AddEditBox(dwIDCtl, (LPCWSTR)strText.GetString());
#endif

			pFileDialogCustomize->Release();
			return hr;
		}

		HRESULT AddMenu(DWORD dwIDCtl, const CString& strLabel)
		{
			if (!m_bVistaStyle) { return E_NOTIMPL; }

			IFileDialogCustomize* pFileDialogCustomize = GetIFileDialogCustomize();

#ifndef UNICODE
			CStringW strLabelW = strLabel;
			HRESULT hr = pFileDialogCustomize->AddMenu(dwIDCtl, (LPCWSTR)strLabelW.GetString());
#else
			HRESULT hr = pFileDialogCustomize->AddMenu(dwIDCtl, (LPCWSTR)strLabel.GetString());
#endif

			pFileDialogCustomize->Release();
			return hr;
		}

		HRESULT AddSeparator(DWORD dwIDCtl)
		{
			if (!m_bVistaStyle) { return E_NOTIMPL; }

			IFileDialogCustomize* pFileDialogCustomize = GetIFileDialogCustomize();

			HRESULT hr = pFileDialogCustomize->AddSeparator(dwIDCtl);

			pFileDialogCustomize->Release();
			return hr;
		}

		HRESULT AddText(DWORD dwIDCtl, const CString& strText)
		{
			if (!m_bVistaStyle) { return E_NOTIMPL; }

			IFileDialogCustomize* pFileDialogCustomize = GetIFileDialogCustomize();

#ifndef UNICODE
			CStringW strTextW = strText;
			HRESULT hr = pFileDialogCustomize->AddText(dwIDCtl, (LPCWSTR)strTextW.GetString());
#else
			HRESULT hr = pFileDialogCustomize->AddText(dwIDCtl, (LPCWSTR)strText.GetString());
#endif

			pFileDialogCustomize->Release();
			return hr;
		}

		void AddPlace(LPCWSTR lpszFolder, FDAP fdap = FDAP_TOP) throw()
		{
			if (!m_bVistaStyle) { return; }

			ASSERT(lpszFolder != NULL);

			CComPtr<IShellItem> shellItem;

			HRESULT hr = _AfxSHCreateItemFromParsingName(lpszFolder, 0, IID_IShellItem, (void**)(&shellItem));
			ENSURE(SUCCEEDED(hr));

			AddPlace(shellItem, fdap);
		}

		void AddPlace(IShellItem* psi, FDAP fdap = FDAP_TOP) throw()
		{
			if (!m_bVistaStyle) { return; }

			HRESULT hr = ((IFileDialog*)(m_pIFileDialog))->AddPlace(psi, fdap);

			ENSURE(SUCCEEDED(hr));
		}

		HRESULT AddPushButton(DWORD dwIDCtl, const CString& strLabel)
		{
			if (!m_bVistaStyle) { return E_NOTIMPL; }

			IFileDialogCustomize* pFileDialogCustomize = GetIFileDialogCustomize();

#ifndef UNICODE
			CStringW strLabelW = strLabel;
			HRESULT hr = pFileDialogCustomize->AddPushButton(dwIDCtl, (LPCWSTR)strLabelW.GetString());
#else
			HRESULT hr = pFileDialogCustomize->AddPushButton(dwIDCtl, (LPCWSTR)strLabel.GetString());
#endif

			pFileDialogCustomize->Release();
			return hr;
}

		HRESULT AddRadioButtonList(DWORD dwIDCtl)
		{
			if (!m_bVistaStyle) { return E_NOTIMPL; }

			IFileDialogCustomize* pFileDialogCustomize = GetIFileDialogCustomize();

			HRESULT hr = pFileDialogCustomize->AddRadioButtonList(dwIDCtl);

			pFileDialogCustomize->Release();
			return hr;
		}


		HRESULT CDialogEventHandler_CreateInstance(REFIID riid, void **ppv);

		HRESULT EnableOpenDropDown(DWORD dwIDCtl)
		{
			if (!m_bVistaStyle) { return E_NOTIMPL; }

			IFileDialogCustomize* pFileDialogCustomize = GetIFileDialogCustomize();
			HRESULT hr = pFileDialogCustomize->EnableOpenDropDown(dwIDCtl);

			pFileDialogCustomize->Release();
			return hr;
		}

		HRESULT EndVisualGroup()
		{
			if (!m_bVistaStyle) { return E_NOTIMPL; }

			IFileDialogCustomize* pFileDialogCustomize = GetIFileDialogCustomize();

			HRESULT hr = pFileDialogCustomize->EndVisualGroup();

			pFileDialogCustomize->Release();
			return hr;
		}

		HRESULT GetCheckButtonState(DWORD dwIDCtl, BOOL& bChecked)
		{
			if (!m_bVistaStyle) { return E_NOTIMPL; }

			IFileDialogCustomize* pFileDialogCustomize = GetIFileDialogCustomize();

			HRESULT hr = pFileDialogCustomize->GetCheckButtonState(dwIDCtl, &bChecked);

			pFileDialogCustomize->Release();
			return hr;
		}

		HRESULT GetControlItemState(DWORD dwIDCtl, DWORD dwIDItem, CDCONTROLSTATEF& dwState)
		{
			if (!m_bVistaStyle) { return E_NOTIMPL; }

			IFileDialogCustomize* pFileDialogCustomize = GetIFileDialogCustomize();

			HRESULT hr = pFileDialogCustomize->GetControlItemState(dwIDCtl, dwIDItem, &dwState);

			pFileDialogCustomize->Release();
			return hr;
		}

		HRESULT GetControlState(DWORD dwIDCtl, CDCONTROLSTATEF& dwState)
		{
			if (!m_bVistaStyle) { return E_NOTIMPL; }

			IFileDialogCustomize* pFileDialogCustomize = GetIFileDialogCustomize();

			HRESULT hr = pFileDialogCustomize->GetControlState(dwIDCtl, &dwState);

			pFileDialogCustomize->Release();
			return hr;
		}

		HRESULT GetEditBoxText(DWORD dwIDCtl, CString& strText)
		{
			if (!m_bVistaStyle) { return E_NOTIMPL; }

			IFileDialogCustomize* pFileDialogCustomize = GetIFileDialogCustomize();

			strText.Empty();

			LPWSTR wcText = NULL;
			HRESULT hr = pFileDialogCustomize->GetEditBoxText(dwIDCtl, &wcText);

			if (SUCCEEDED(hr))
			{
				strText = wcText;
				CoTaskMemFree(wcText);
			}

			pFileDialogCustomize->Release();
			return hr;
		}

		IShellItem* GetResult() throw();
		HRESULT GetSelectedControlItem(DWORD dwIDCtl, DWORD& dwIDItem);

		IShellItemArray* GetResults() throw()
		{
			IShellItem *pItem = NULL;
			if (!m_bVistaStyle) { return pItem; }

			HRESULT hr = (static_cast<IFileDialog*>(m_pIFileDialog))->GetResult(&pItem);
			if (SUCCEEDED(hr))
			{
				return pItem;
			}

#ifndef UNICODE
			CStringW strPathNameW = GetPathName().GetString();
			hr = _AfxSHCreateItemFromParsingName((PCWSTR)strPathNameW, 0, IID_IShellItem, (void**)&pItem);
#else
			hr = _AfxSHCreateItemFromParsingName((PCWSTR)GetPathName().GetString(), 0, IID_IShellItem, (void**)&pItem);
#endif
			ENSURE(SUCCEEDED(hr));

			return pItem;
		}

		HRESULT MakeProminent(DWORD dwIDCtl)
		{
			if (!m_bVistaStyle) { return E_NOTIMPL; }

			IFileDialogCustomize* pFileDialogCustomize = GetIFileDialogCustomize();

			HRESULT hr = pFileDialogCustomize->MakeProminent(dwIDCtl);

			pFileDialogCustomize->Release();
			return hr;
		}

		HRESULT RemoveControlItem(DWORD dwIDCtl, DWORD dwIDItem)
		{
			if (!m_bVistaStyle) { return E_NOTIMPL; }

			IFileDialogCustomize* pFileDialogCustomize = GetIFileDialogCustomize();

			HRESULT hr = pFileDialogCustomize->RemoveControlItem(dwIDCtl, dwIDItem);

			pFileDialogCustomize->Release();
			return hr;
}

		HRESULT SetCheckButtonState(DWORD dwIDCtl, BOOL bChecked)
		{
			if (!m_bVistaStyle) { return E_NOTIMPL; }

			IFileDialogCustomize* pFileDialogCustomize = GetIFileDialogCustomize();

			HRESULT hr = pFileDialogCustomize->SetCheckButtonState(dwIDCtl, bChecked);

			pFileDialogCustomize->Release();
			return hr;
}

		HRESULT SetControlItemState(DWORD dwIDCtl, DWORD dwIDItem, CDCONTROLSTATEF dwState)
		{
			if (!m_bVistaStyle) { return E_NOTIMPL; }

			IFileDialogCustomize* pFileDialogCustomize = GetIFileDialogCustomize();

			HRESULT hr = pFileDialogCustomize->SetControlItemState(dwIDCtl, dwIDItem, dwState);

			pFileDialogCustomize->Release();
			return hr;
		}

		HRESULT SetControlItemText(DWORD dwIDCtl, DWORD dwIDItem, const CString& strLabel)
		{
			if (!m_bVistaStyle) { return E_NOTIMPL; }

			IFileDialogCustomize* pFileDialogCustomize = GetIFileDialogCustomize();

#ifndef UNICODE
			CStringW strLabelW = strLabel;
			HRESULT hr = pFileDialogCustomize->SetControlItemText(dwIDCtl, dwIDItem, (LPCWSTR)strLabelW.GetString());
#else
			HRESULT hr = pFileDialogCustomize->SetControlItemText(dwIDCtl, dwIDItem, (LPCWSTR)strLabel.GetString());
#endif

			pFileDialogCustomize->Release();
			return hr;
		}

		HRESULT SetControlLabel(DWORD dwIDCtl, const CString& strLabel)
		{
			if (!m_bVistaStyle) { return E_NOTIMPL; }

			IFileDialogCustomize* pFileDialogCustomize = GetIFileDialogCustomize();

#ifndef UNICODE
			CStringW strLabelW = strLabel;
			HRESULT hr = pFileDialogCustomize->SetControlLabel(dwIDCtl, (LPCWSTR)strLabelW.GetString());
#else
			HRESULT hr = pFileDialogCustomize->SetControlLabel(dwIDCtl, (LPCWSTR)strLabel.GetString());
#endif

			pFileDialogCustomize->Release();
			return hr;
		}

		HRESULT SetControlState(DWORD dwIDCtl, CDCONTROLSTATEF dwState)
		{
			if (!m_bVistaStyle) { return E_NOTIMPL; }

			IFileDialogCustomize* pFileDialogCustomize = GetIFileDialogCustomize();

			HRESULT hr = pFileDialogCustomize->SetControlState(dwIDCtl, dwState);

			pFileDialogCustomize->Release();
			return hr;
		}

		HRESULT SetEditBoxText(DWORD dwIDCtl, const CString& strText)
		{
			if (!m_bVistaStyle) { return E_NOTIMPL; }

			IFileDialogCustomize* pFileDialogCustomize = GetIFileDialogCustomize();

#ifndef UNICODE
			CStringW strTextW = strText;
			HRESULT hr = pFileDialogCustomize->SetEditBoxText(dwIDCtl, (LPCWSTR)strTextW.GetString());
#else
			HRESULT hr = pFileDialogCustomize->SetEditBoxText(dwIDCtl, (LPCWSTR)strText.GetString());
#endif

			pFileDialogCustomize->Release();
			return hr;
		}

		BOOL SetProperties(LPCWSTR lpszPropList)
		{
			ASSERT(m_bVistaStyle == TRUE);
			ASSERT(lpszPropList != NULL);
			ASSERT(GetSafeHwnd() == NULL);

			BOOL bRes = FALSE;

			if (!m_bVistaStyle)
			{
				return bRes;
			}

			IFileSaveDialog* pSaveFile = GetIFileSaveDialog();
			if (pSaveFile == NULL)
			{
				return bRes;
			}

			IPropertyDescriptionList* pPropertyDescriptionList = NULL;
			HRESULT hr = _AfxPSGetPropertyDescriptionListFromString(lpszPropList, IID_PPV_ARGS(&pPropertyDescriptionList));

			if (SUCCEEDED(hr))
			{
				hr = pSaveFile->SetCollectedProperties(pPropertyDescriptionList, TRUE);
				if (SUCCEEDED(hr))
				{
					IShellItem2* pItem = NULL;
#ifndef UNICODE
					CStringW strPathNameW = GetPathName().GetString();
					hr = _AfxSHCreateItemFromParsingName((PCWSTR)strPathNameW, 0, IID_IShellItem2, (void**)(&pItem));
#else
					hr = _AfxSHCreateItemFromParsingName((PCWSTR)GetPathName().GetString(), 0, IID_IShellItem2, (void**)(&pItem));
#endif
					if (SUCCEEDED(hr) && pItem != NULL)
					{
						IPropertyStore* pStore = NULL;
						hr = pItem->GetPropertyStore(GPS_HANDLERPROPERTIESONLY, IID_IPropertyStore, (LPVOID*)&pStore);
						if (SUCCEEDED(hr))
						{
							pSaveFile->SetProperties(pStore);
							pStore->Release();
						}

						pItem->Release();
					}
					bRes = TRUE;
				}

				pPropertyDescriptionList->Release();
			}

			pSaveFile->Release();
			return bRes;
		}

		HRESULT SetSelectedControlItem(DWORD dwIDCtl, DWORD dwIDItem)
		{
			if (!m_bVistaStyle) { return E_NOTIMPL; }

			IFileDialogCustomize* pFileDialogCustomize = GetIFileDialogCustomize();

			HRESULT hr = pFileDialogCustomize->SetSelectedControlItem(dwIDCtl, dwIDItem);

			pFileDialogCustomize->Release();
			return hr;
		}

		HRESULT StartVisualGroup(DWORD dwIDCtl, const CString& strLabel)
		{
			if (!m_bVistaStyle) { return E_NOTIMPL; }

			IFileDialogCustomize* pFileDialogCustomize = GetIFileDialogCustomize();

#ifndef UNICODE
			CStringW strLabelW = strLabel;
			HRESULT hr = pFileDialogCustomize->StartVisualGroup(dwIDCtl, (LPCWSTR)strLabelW.GetString());
#else
			HRESULT hr = pFileDialogCustomize->StartVisualGroup(dwIDCtl, (LPCWSTR)strLabel.GetString());
#endif

			pFileDialogCustomize->Release();
			return hr;
		}

#endif // _SHELLDIALOG
#endif // MTL_REMOVE_CFILEDIALOG_VISTA


#ifdef _SHELLDIALOG
		DECLARE_INTERFACE_MAP()

#ifndef MTL_REMOVE_CFILEDIALOG_VISTA
		BEGIN_INTERFACE_PART(FileDialogEvents, IFileDialogEvents)
			STDMETHOD(OnFileOk)(IFileDialog *);
			STDMETHOD(OnFolderChange)(IFileDialog *);
			STDMETHOD(OnFolderChanging)(IFileDialog *, IShellItem *);
			STDMETHOD(OnHelp)(IFileDialog *);
			STDMETHOD(OnSelectionChange)(IFileDialog *);
			STDMETHOD(OnShareViolation)(IFileDialog *pfd, IShellItem *psi, FDE_SHAREVIOLATION_RESPONSE *pResponse);
			STDMETHOD(OnTypeChange)(IFileDialog *);
			STDMETHOD(OnOverwrite)(IFileDialog *, IShellItem *, FDE_OVERWRITE_RESPONSE *);
		END_INTERFACE_PART_OPTIONAL(FileDialogEvents)

		BEGIN_INTERFACE_PART(FileDialogControlEvents, IFileDialogControlEvents)
			STDMETHOD(OnButtonClicked)(IFileDialogCustomize *, DWORD);
			STDMETHOD(OnCheckButtonToggled)(IFileDialogCustomize *, DWORD, BOOL);
			STDMETHOD(OnControlActivating)(IFileDialogCustomize *, DWORD);
			STDMETHOD(OnItemSelected)(IFileDialogCustomize *, DWORD, DWORD);
			END_INTERFACE_PART_OPTIONAL(FileDialogControlEvents)
#else // MTL_REMOVE_CFILEDIALOG_VISTA
		BEGIN_INTERFACE_PART(FileDialogEvents, IUnknown)
		END_INTERFACE_PART_OPTIONAL(FileDialogEvents)

		BEGIN_INTERFACE_PART(FileDialogControlEvents, IUnknown)
		END_INTERFACE_PART_OPTIONAL(FileDialogControlEvents)
#endif // _SHELLDIALOG
#endif // MTL_REMOVE_CFILEDIALOG_VISTA
};

//BEGIN_MESSAGE_MAP(CFileDialogMTL, CCommonDialog)
//END_MESSAGE_MAP()



class _NO_VTABLE CCommonWnd : public CCmdTarget
{
	public:
	public:
	public:
		virtual BOOL _WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
		{
			if (message == WM_INITDIALOG)
			{
				CWindow cWnd(hWnd) ;

				cWnd.CenterWindow();
			}

			return _ProcessWindowMessage(hWnd, message, wParam, lParam, lResult);
		}

};



class  CColorDialogMTL : public CCommonWnd, public CColorDialog
{
	public:
	public:
		// メッセージマップ
		DECLARE_MESSAGE_MAP()

	public:
};

BEGIN_MESSAGE_MAP(CColorDialogMTL, CCommonWnd)
END_MESSAGE_MAP()



class  CFindReplaceDialogMTL : public CCommonWnd, public CFindReplaceDialog
{
	public:
	public:
		// メッセージマップ
		DECLARE_MESSAGE_MAP()

	public:
};

BEGIN_MESSAGE_MAP(CFindReplaceDialogMTL, CCommonWnd)
END_MESSAGE_MAP()



class  CFolderDialogMTL : public CCommonWnd, public CFolderDialog
{
	public:
	public:
		// メッセージマップ
		DECLARE_MESSAGE_MAP()

	public:
};

BEGIN_MESSAGE_MAP(CFolderDialogMTL, CCommonWnd)
END_MESSAGE_MAP()



class  CFontDialogMTL : public CCommonWnd, public CFontDialog
{
	public:
	public:
		// メッセージマップ
		DECLARE_MESSAGE_MAP()

	public:
};

BEGIN_MESSAGE_MAP(CFontDialogMTL, CCommonWnd)
END_MESSAGE_MAP()



class  CPageSetupDialogMTL : public CCommonWnd, public CPageSetupDialog
{
	public:
	public:
		// メッセージマップ
		DECLARE_MESSAGE_MAP()

	public:
};

BEGIN_MESSAGE_MAP(CPageSetupDialogMTL, CCommonWnd)
END_MESSAGE_MAP()



class CPrintDialogMTL : public CCommonWnd, public CPrintDialog
{
	public:
		CPrintDialogMTL(BOOL bPrintSetupOnly,
						DWORD dwFlags = PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE | PD_NOSELECTION,
						CWnd* pParentWnd = NULL)
			: CPrintDialog(bPrintSetupOnly, dwFlags, pParentWnd->GetSafeHwnd())
		{

		}

	public:
		// メッセージマップ
		DECLARE_MESSAGE_MAP()

	public:

		CString GetDeviceName() const
		{
			if (m_pd.hDevNames == NULL)
				return (LPCTSTR)NULL;

			LPDEVNAMES lpDev = (LPDEVNAMES)GlobalLock(m_pd.hDevNames);

			// If this asserts it is because m_pd.hDevNames is no longer
			// valid.  This can occur if you call GetPrinterDeviceDefaults()
			// while in OnBeginPrinting().  Continuing will cause an
			// access violation.
			ASSERT(lpDev);

			return (LPCTSTR)lpDev + lpDev->wDeviceOffset;
		}

		CString GetDriverName() const
		{
			if (m_pd.hDevNames == NULL)
				return (LPCTSTR)NULL;

			LPDEVNAMES lpDev = (LPDEVNAMES)GlobalLock(m_pd.hDevNames);

			// If this asserts it is because m_pd.hDevNames is no longer
			// valid.  This can occur if you call GetPrinterDeviceDefaults()
			// while in OnBeginPrinting().  Continuing will cause an
			// access violation.
			ASSERT(lpDev);

			return (LPCTSTR)lpDev + lpDev->wDriverOffset;
		}

		CString GetPortName() const
		{
			if (m_pd.hDevNames == NULL)
				return (LPCTSTR)NULL;

			LPDEVNAMES lpDev = (LPDEVNAMES)GlobalLock(m_pd.hDevNames);

			// If this asserts it is because m_pd.hDevNames is no longer
			// valid.  This can occur if you call GetPrinterDeviceDefaults()
			// while in OnBeginPrinting().  Continuing will cause an
			// access violation.
			ASSERT(lpDev);

			return (LPCTSTR)lpDev + lpDev->wOutputOffset;
		}


};

BEGIN_MESSAGE_MAP(CPrintDialogMTL, CCommonWnd)
END_MESSAGE_MAP()


#ifndef _MTL_ATL3
class  CPrintDialogExMTL : public CCommonWnd, public CPrintDialogEx
{
	public:
	public:
		// メッセージマップ
		DECLARE_MESSAGE_MAP()

	public:
};

BEGIN_MESSAGE_MAP(CPrintDialogExMTL, CCommonWnd)
END_MESSAGE_MAP()
#endif // _MTL_ATL3





#if 1
#define CColorDialog CColorDialogMTL
#define CFileDialog CFileDialogMTL
#define CFindReplaceDialog CFindReplaceDialogMTL
#define CFolderDialog CFolderDialogMTL
#define CFontDialog CFontDialogMTL
#define CPageSetupDialog CPageSetupDialogMTL
#define CPrintDialog CPrintDialogMTL
#define CPrintDialogEx CPrintDialogExMTL
#endif


/*============================================================================*/
// Informational data structures

struct CPrintInfo // Printing information structure
{
	CPrintInfo()
	{
		m_pPD = new CPrintDialog(FALSE, PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOSELECTION);

		ASSERT(m_pPD->m_pd.hDC == NULL);

		SetMinPage(1);              // one based page numbers
		SetMaxPage(0xffff);         // unknown how many pages

		m_nCurPage = 1;
		m_nJobNumber = SP_ERROR;

		m_lpUserData = NULL;        // Initialize to no user data
		m_bPreview = FALSE;         // initialize to not preview
		m_bDirect = FALSE;          // initialize to not direct
		m_bDocObject = FALSE;       // initialize to not IPrint
		m_bContinuePrinting = TRUE; // Assume it is OK to print

		m_dwFlags = 0;
		m_nOffsetPage = 0;
	}

	~CPrintInfo()
	{
		if (m_pPD != NULL && m_pPD->m_pd.hDC != NULL)
		{
			::DeleteDC(m_pPD->m_pd.hDC);
			m_pPD->m_pd.hDC = NULL;
		}
		delete m_pPD;
	}

	CPrintDialog* m_pPD;     // pointer to print dialog

	BOOL m_bDocObject;       // TRUE if printing by IPrint interface
	BOOL m_bPreview;         // TRUE if in preview mode
	BOOL m_bDirect;          // TRUE if bypassing Print Dialog
	BOOL m_bContinuePrinting;// set to FALSE to prematurely end printing
	UINT m_nCurPage;         // Current page
	UINT m_nNumPreviewPages; // Desired number of preview pages
	CString m_strPageDesc;   // Format string for page number display
	LPVOID m_lpUserData;     // pointer to user created struct
	CRect m_rectDraw;        // rectangle defining current usable page area
	int m_nJobNumber;			 // job number (after StartDoc)

	// these only valid if m_bDocObject
	UINT m_nOffsetPage;      // offset of first page in combined IPrint job
	DWORD m_dwFlags;         // flags passed to IPrint::Print


	void SetMinPage(UINT nMinPage) { m_pPD->m_pd.nMinPage = (WORD)nMinPage; }
	void SetMaxPage(UINT nMaxPage) { m_pPD->m_pd.nMaxPage = (WORD)nMaxPage; }
	UINT GetMinPage() const { return m_pPD->m_pd.nMinPage; }
	UINT GetMaxPage() const { return m_pPD->m_pd.nMaxPage; }
	UINT GetFromPage() const { return m_pPD->m_pd.nFromPage; }
	UINT GetToPage() const { return m_pPD->m_pd.nToPage; }

	UINT GetOffsetPage() const;

};

struct CPrintPreviewState   // Print Preview context/state
{
	CPrintPreviewState::CPrintPreviewState()
	{
		// set defaults
		nIDMainPane = AFX_IDW_PANE_FIRST;
		dwStates = AFX_CONTROLBAR_MASK(AFX_IDW_STATUS_BAR);
		// status bar visible if available
		lpfnCloseProc = _AfxPreviewCloseProc;
		// set frame hook so closing the frame window
		//  when in preview state will just end the mode
		hMenu = NULL;
		pViewActiveOld = NULL;
		hAccelTable = NULL;
	}

	UINT nIDMainPane;          // main pane ID to hide
	HMENU hMenu;               // saved hMenu
	DWORD dwStates;            // Control Bar Visible states (bit map)
	CView* pViewActiveOld;     // save old active view during preview
	BOOL(CALLBACK* lpfnCloseProc)(CFrameWnd* pFrameWnd);
	HACCEL hAccelTable;       // saved accelerator table

	static BOOL CALLBACK _AfxPreviewCloseProc(CFrameWnd* pFrameWnd) ;

};

_INLINE void CFrameWnd::OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState)
{
	ENSURE_ARG(pState != NULL);
	// default implementation changes control bars, menu and main pane window

	CFrameWnd* pActiveFrame = GetActiveFrame();
	ENSURE_VALID(pActiveFrame);

	// Set visibility of standard ControlBars (only the first 32)
	DWORD dwOldStates = 0;
	POSITION pos = m_listControlBars.GetHeadPosition();
	while (pos != NULL)
	{
		CControlBar* pBar = (CControlBar*)m_listControlBars.GetNext(pos);
		ENSURE_VALID(pBar);
		UINT_PTR nID = _AfxGetDlgCtrlID(pBar->m_hWnd);
		if (nID >= AFX_IDW_CONTROLBAR_FIRST && nID <= AFX_IDW_CONTROLBAR_FIRST + 31)
		{
			DWORD dwMask = 1L << (UINT)(nID - AFX_IDW_CONTROLBAR_FIRST);
			if (pBar->IsVisible())
				dwOldStates |= dwMask;      // save if previously visible
			if (!pBar->IsDockBar() || nID != AFX_IDW_DOCKBAR_FLOAT)
				ShowControlBar(pBar, (pState->dwStates & dwMask), TRUE);
		}
	}
	pState->dwStates = dwOldStates; // save for restore

	if (bPreview)
	{
		// Entering Print Preview

		ASSERT(m_lpfnCloseProc == NULL);    // no chaining
		m_lpfnCloseProc = pState->lpfnCloseProc;

		// show any modeless dialogs, popup windows, float tools, etc
		ShowOwnedWindows(FALSE);

		// Hide the main pane
		HWND hWnd = ::GetDlgItem(m_hWnd, pState->nIDMainPane);
		ASSERT(hWnd != NULL);       // must be one that we are hiding!
		::ShowWindow(hWnd, SW_HIDE);

		// Get rid of the menu first (will resize the window)
		pState->hMenu = m_dwMenuBarState == AFX_MBS_VISIBLE ? ::GetMenu(m_hWnd) : m_hMenu;

		if (pState->hMenu != NULL)
		{
			// Invalidate before SetMenu since we are going to replace
			//  the frame's client area anyway
			Invalidate();
			SetMenu(NULL);
			m_nIdleFlags &= ~idleMenu;  // avoid any idle menu processing
		}

		// Save the accelerator table and remove it.
		pState->hAccelTable = m_hAccelTable;
		m_hAccelTable = NULL;
		LoadAccelTable(ATL_MAKEINTRESOURCE(AFX_IDR_PREVIEW_ACCEL));

		// Make room for the PreviewView by changing AFX_IDW_PANE_FIRST's ID
		//  to AFX_IDW_PREVIEW_FIRST
		if (pState->nIDMainPane != AFX_IDW_PANE_FIRST)
			hWnd = ::GetDlgItem(m_hWnd, AFX_IDW_PANE_FIRST);
		if (hWnd != NULL)
			_AfxSetDlgCtrlID(hWnd, AFX_IDW_PANE_SAVE);

#ifdef _DEBUG
		if ((::GetWindowLong(m_hWnd, GWL_STYLE) & (WS_HSCROLL | WS_VSCROLL)) != 0)
			TRACE(traceAppMsg, 0, "Warning: scroll bars in frame windows may cause unusual behaviour.\n");
#endif
	}
	else
	{
		// Leaving Preview
		m_lpfnCloseProc = NULL;

		// shift original AFX_IDW_PANE_FIRST back to its rightful ID
		HWND hWnd = ::GetDlgItem(m_hWnd, AFX_IDW_PANE_SAVE);
		if (hWnd != NULL)
		{
			HWND hWndTemp = ::GetDlgItem(m_hWnd, AFX_IDW_PANE_FIRST);
			if (hWndTemp != NULL)
				_AfxSetDlgCtrlID(hWndTemp, AFX_IDW_PANE_SAVE);
			_AfxSetDlgCtrlID(hWnd, AFX_IDW_PANE_FIRST);
		}

		// put the menu back in place if it was removed before
		if (pState->hMenu != NULL)
		{
			// Invalidate before SetMenu since we are going to replace
			//  the frame's client area anyway
			Invalidate();
			if (m_dwMenuBarState == AFX_MBS_VISIBLE)
			{
				::SetMenu(m_hWnd, pState->hMenu);
			}
			else if (m_dwMenuBarState == AFX_MBS_HIDDEN)
			{
				m_hMenu = pState->hMenu;
			}
		}

		RecalcLayout();

		// now show main pane that was hidden
		if (pState->nIDMainPane != AFX_IDW_PANE_FIRST)
			hWnd = ::GetDlgItem(m_hWnd, pState->nIDMainPane);
		ASSERT(hWnd != NULL);
		::ShowWindow(hWnd, SW_SHOW);

		// Restore the Accelerator table
		m_hAccelTable = pState->hAccelTable;

		// show any modeless dialogs, popup windows, float tools, etc
		ShowOwnedWindows(TRUE);
	}
}


