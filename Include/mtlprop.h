



class CPropertySheetMTL ;

#define IDD_PROPSHEET   1006
#define IDD_WIZARD      1020

#define PROP_CLOSEPENDING_NAME _T("AfxClosePending")

struct _AFX_PROPPAGEFONTINFO
{
	LPTSTR m_pszFaceName;
	WORD m_wSize;

	_AFX_PROPPAGEFONTINFO() : m_pszFaceName(NULL), m_wSize(0) {}
	~_AFX_PROPPAGEFONTINFO() { GlobalFree(m_pszFaceName); }

	_AFX_PROPPAGEFONTINFO* GetData() { return this ; }
};

/*============================================================================*/
// CPropertyPage -- one page of a tabbed dialog

class CPropertyPageMTL : public CDialog
{
	DECLARE_DYNAMIC(CPropertyPageMTL)

	// Construction
	public:
		// simple construction
		CPropertyPageMTL() : m_psp(AllocPSP(0))
		{
			CommonConstruct(NULL, 0);
		}

		CPropertyPageMTL(UINT nIDTemplate, UINT nIDCaption = 0, DWORD dwSize = sizeof(PROPSHEETPAGE)) : m_psp(AllocPSP(dwSize))
		{
			ASSERT(nIDTemplate != 0);
			CommonConstruct(MAKEINTRESOURCE(nIDTemplate), nIDCaption);
		}

		CPropertyPageMTL(LPCTSTR lpszTemplateName, UINT nIDCaption = 0, DWORD dwSize = sizeof(PROPSHEETPAGE)) : m_psp(AllocPSP(dwSize))
		{
			ENSURE_ARG(AfxIsValidString(lpszTemplateName));
			CommonConstruct(lpszTemplateName, nIDCaption);
		}

		// extended construction
		CPropertyPageMTL(UINT nIDTemplate, UINT nIDCaption, UINT nIDHeaderTitle, UINT nIDHeaderSubTitle = 0, DWORD dwSize = sizeof(PROPSHEETPAGE)) : m_psp(AllocPSP(dwSize))
		{
			ASSERT(nIDTemplate != 0);
			CommonConstruct(MAKEINTRESOURCE(nIDTemplate), nIDCaption, nIDHeaderTitle, nIDHeaderSubTitle);
		}

		CPropertyPageMTL(LPCTSTR lpszTemplateName, UINT nIDCaption, UINT nIDHeaderTitle, UINT nIDHeaderSubTitle = 0, DWORD dwSize = sizeof(PROPSHEETPAGE)) : m_psp(AllocPSP(dwSize))
		{
			ASSERT(AfxIsValidString(lpszTemplateName));
			CommonConstruct(lpszTemplateName, nIDCaption, nIDHeaderTitle, nIDHeaderSubTitle);
		}

		virtual ~CPropertyPageMTL()
		{
			free(m_pPSP);
			Cleanup();

			if (m_hDialogTemplate != NULL)
				GlobalFree(m_hDialogTemplate);
		}

	public:
		DECLARE_MESSAGE_MAP()
	
		PROPSHEETPAGE& m_psp;
		LPPROPSHEETPAGE m_pPSP;

		CString m_strCaption;
		BOOL m_bFirstSetActive;
		CString m_strHeaderTitle;    // this is displayed in the header
		CString m_strHeaderSubTitle; //

	public:
		static UINT CALLBACK AfxPropPageCallback(HWND hWnd, UINT message, PROPSHEETPAGE* pPropPage)
		{
			switch (message)
			{
				case PSPCB_CREATE:
				{
					ASSERT(AfxIsValidAddress(pPropPage, pPropPage->dwSize));
					CPropertyPageMTL* pPage = (CPropertyPageMTL*)pPropPage->lParam;
					ASSERT_VALID(pPage);
					HookWindowCreate(pPage);
#if 0
					TRY
					{
						AfxHookWindowCreate(pPage);
					}
					CATCH_ALL(e)
					{
						// Note: DELETE_EXCEPTION(e) not necessary
						return FALSE;
					}
					END_CATCH_ALL
#endif
				}
				return TRUE;

				case PSPCB_RELEASE:
					//AfxUnhookWindowCreate();
					UnhookWindowCreate();
					break;

			}

			return 0;
		}

		static int CALLBACK FontEnumProc(const LOGFONT*, const TEXTMETRIC*, DWORD, LPARAM lParam)
		{
			if (lParam != NULL)
			{
				*(BOOL*)lParam = TRUE;
			}
			return 0;
		}

		static _AFX_PROPPAGEFONTINFO& _GetAfxPropPageFontInfo()
		{
			static _AFX_PROPPAGEFONTINFO _afxPropPageFontInfo ;

			return _afxPropPageFontInfo ;
		}
		
		static BOOL IsFontInstalled(LPCTSTR pszFace)
		{
			LOGFONT lf;

			memset(&lf, 0, sizeof(lf));

			size_t nLenFace = lstrlen(pszFace);
			ENSURE(nLenFace<LF_FACESIZE);

			lstrcpy(lf.lfFaceName, pszFace);
			lf.lfCharSet = DEFAULT_CHARSET;

			BOOL bInstalled = FALSE;
			HDC hDC = ::GetDC(NULL);
			if (hDC != NULL)
			{
				::EnumFontFamiliesEx(hDC, &lf, FontEnumProc, (LPARAM)&bInstalled, 0);
				::ReleaseDC(NULL, hDC);
			}

			return bInstalled;
		}

		static BOOL AfxGetPropSheetFont(CString& strFace, WORD& wSize, BOOL bWizard)
		{
			_AFX_PROPPAGEFONTINFO* pFontInfo = _GetAfxPropPageFontInfo().GetData();

			// determine which font property sheet will use
			if (pFontInfo->m_wSize == 0)
			{
				ASSERT(pFontInfo->m_pszFaceName == NULL);

				HINSTANCE hInst = GetModuleHandleW(L"comctl32.dll");
				if (hInst != NULL)
				{
					HRSRC hResource = NULL;
					WORD wLang = 0;
#ifndef _MTL_ATL3
					LANGID langid;
					langid = GetUserDefaultUILanguage();
					if ((PRIMARYLANGID(langid) == LANG_JAPANESE) && IsFontInstalled(_T("MS UI Gothic")))
						wLang = MAKELANGID(LANG_JAPANESE, 0x3f);
#else
					typedef LANGID (WINAPI* PFNGETUSERDEFAULTUILANGUAGE)();

					HMODULE hKernel32 = ::GetModuleHandleA("KERNEL32.DLL");
					PFNGETUSERDEFAULTUILANGUAGE pfnGetUserDefaultUILanguage;
					pfnGetUserDefaultUILanguage = (PFNGETUSERDEFAULTUILANGUAGE)::GetProcAddress(
						hKernel32, "GetUserDefaultUILanguage");
					if (pfnGetUserDefaultUILanguage != NULL)
					{
						LANGID langid;
						langid = pfnGetUserDefaultUILanguage();
						if ((PRIMARYLANGID(langid) == LANG_JAPANESE) && 
							IsFontInstalled(_T("MS UI Gothic")))
							wLang = MAKELANGID(LANG_JAPANESE, 0x3f);
					}
#endif
					if (wLang != 0)
					{
						hResource = ::FindResourceExW(hInst, (LPWSTR)RT_DIALOG,
							MAKEINTRESOURCEW(bWizard ? IDD_WIZARD : IDD_PROPSHEET), wLang);
					}
					if (hResource == NULL)
					{
						hResource = ::FindResourceW(hInst,
							MAKEINTRESOURCEW(bWizard ? IDD_WIZARD : IDD_PROPSHEET),
							(LPWSTR)RT_DIALOG);
					}
					if (hResource != NULL)
					{
						HGLOBAL hTemplate = LoadResource(hInst, hResource);
						if (hTemplate != NULL)
						{
							CDialogTemplate::GetFont((DLGTEMPLATE*)hTemplate, strFace,
								wSize);
						}
					}
				}
				//pFontInfo->m_pszFaceName = (LPTSTR)GlobalAlloc(GPTR, static_cast<UINT>(::ATL::AtlMultiplyThrow(static_cast<UINT>((strFace.GetLength() + 1)), static_cast<UINT>(sizeof(TCHAR)))));
				//ENSURE_THROW(pFontInfo->m_pszFaceName != NULL, ::AfxThrowMemoryException());
				//Checked::tcscpy_s(pFontInfo->m_pszFaceName, strFace.GetLength() + 1, strFace);
				pFontInfo->m_pszFaceName = (LPTSTR)GlobalAlloc(GPTR, sizeof(TCHAR) * (strFace.GetLength() + 1));
				lstrcpy(pFontInfo->m_pszFaceName, strFace);
				pFontInfo->m_wSize = wSize;
			}

			strFace = pFontInfo->m_pszFaceName;
			wSize = pFontInfo->m_wSize;

			return (wSize != 0xFFFF);
		}

		static DLGTEMPLATE* _AfxChangePropPageFont(const DLGTEMPLATE* pTemplate, BOOL bWizard)
		{
			CString strFaceDefault;
			WORD wSizeDefault;

			if (!AfxGetPropSheetFont(strFaceDefault, wSizeDefault, bWizard))
				return NULL;

			// set font of property page to same font used by property sheet
			CString strFace;
			WORD wSize;
			if ((!CDialogTemplate::GetFont(pTemplate, strFace, wSize)) ||
				(strFace != strFaceDefault) || (wSize != wSizeDefault))
			{
				CDialogTemplate dlgTemplate(pTemplate);
				dlgTemplate.SetFont(strFaceDefault, wSizeDefault);
				return (DLGTEMPLATE*)dlgTemplate.Detach();
			}

			return NULL;
		}


		// Overridables
		virtual BOOL OnApply() { ASSERT_VALID(this); OnOK(); return TRUE; }

		virtual void OnCancel() { ASSERT_VALID(this); }

		HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
		{
			LRESULT lResult;
			if (pWnd->SendChildNotifyLastMsg(&lResult))
				return (HBRUSH)lResult;

			return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		}


		virtual BOOL OnKillActive()
		{
			ASSERT_VALID(this);

			if (!UpdateData())
			{
				TRACE(traceAppMsg, 0, "UpdateData failed during page deactivation\n");
				return FALSE;
			}
			return TRUE;
		}

		virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) ;
		virtual void OnOK() { ASSERT_VALID(this); }

		virtual BOOL OnQueryCancel() { return TRUE; }    // ok to cancel

		virtual void OnReset() { ASSERT_VALID(this); OnCancel(); }

		virtual BOOL OnSetActive()
		{
			ASSERT_VALID(this);

			if (m_bFirstSetActive)
				m_bFirstSetActive = FALSE;
			else
				UpdateData(FALSE);
			return TRUE;
		}

		virtual LRESULT OnWizardBack() { return 0; }

		virtual BOOL OnWizardFinish() ;

		virtual HWND OnWizardFinishEx(){ return OnWizardFinish() ? (HWND)FALSE : (HWND)TRUE; } //Reversing the return values from OnWizardFinish.

		virtual LRESULT OnWizardNext() { return 0; }

		// Operations
		void CancelToClose() ;

		void Cleanup() { }

		PROPSHEETPAGE& AllocPSP(DWORD dwSize)
		{
			if (dwSize == 0)
			{
				dwSize = sizeof(PROPSHEETPAGE);
			}

			// size of PROPSHEETPAGE must be at least version 4
			//ASSERT(dwSize >= PROPSHEETPAGE_V2_SIZE);
			// allocate memory for PROPSHEETPAGE struct based on size passed in
			m_pPSP = (LPPROPSHEETPAGE)(malloc(dwSize));
			ASSERT(m_pPSP != NULL);
			if (m_pPSP == NULL)
				AfxThrowMemoryException();

			memset(m_pPSP, 0, dwSize);
			m_pPSP->dwSize = dwSize;

			return GetPSP();
		}

		void CommonConstruct(LPCTSTR lpszTemplateName, UINT nIDCaption)
		{
			m_psp.dwFlags = PSP_USECALLBACK;
			if (lpszTemplateName != NULL)
				m_psp.hInstance = AfxFindResourceHandle(lpszTemplateName, RT_DIALOG);
			m_psp.pszTemplate = lpszTemplateName;
			m_psp.pfnDlgProc = (DLGPROC)AfxDlgProc ;
			m_psp.lParam = (LPARAM)this;
			m_psp.pfnCallback = AfxPropPageCallback;
			if (nIDCaption != 0)
			{
				ENSURE(m_strCaption.LoadString(nIDCaption));
				m_psp.pszTitle = m_strCaption;
				m_psp.dwFlags |= PSP_USETITLE;
			}
			if (AfxHelpEnabled())
				m_psp.dwFlags |= PSP_HASHELP;
			if (IS_INTRESOURCE(lpszTemplateName))
				m_nIDHelp = LOWORD((DWORD_PTR)lpszTemplateName);
			m_lpszTemplateName = m_psp.pszTemplate;
			m_bFirstSetActive = TRUE;

			//AfxRegisterMFCCtrlClasses();
		}

		void CommonConstruct(LPCTSTR lpszTemplateName, UINT nIDCaption, UINT nIDHeaderTitle, UINT nIDHeaderSubTitle)
		{
			CommonConstruct(lpszTemplateName, nIDCaption);

			if (nIDHeaderTitle != 0)
			{
				ENSURE(m_strHeaderTitle.LoadString(nIDHeaderTitle));
			}

			if (nIDHeaderSubTitle != 0)
			{
				ENSURE(m_strHeaderSubTitle.LoadString(nIDHeaderSubTitle));
			}
		}

		void EndDialog(int nID) ; // called for error scenarios
			
		virtual CPropertySheetMTL* GetParentSheet()
		{
			for (CWnd *pWnd = GetParent(); pWnd != NULL; pWnd = pWnd->GetParent())
			{
				CPropertySheetMTL *pSheet = (CPropertySheetMTL*)pWnd;
				if (pSheet != NULL)
				{
					return pSheet;
				}
			}
			ASSERT(FALSE); // Could not find the CPropertySheet
			return NULL;
		}

		const PROPSHEETPAGE& GetPSP() const { return *m_pPSP; }
		PROPSHEETPAGE& GetPSP() { return *m_pPSP; }


		BOOL IsButtonEnabled(int iButton)
		{
			HWND hWnd = ::GetDlgItem(::GetParent(m_hWnd), iButton);
			if (hWnd == NULL)
				return FALSE;
			return ::IsWindowEnabled(hWnd);
		}

		LRESULT MapWizardResult(LRESULT lToMap) ;

		void PreProcessPageTemplate(PROPSHEETPAGE& psp, BOOL bWizard)
		{
			const DLGTEMPLATE* pTemplate;

			if (psp.dwFlags & PSP_DLGINDIRECT)
			{
				pTemplate = psp.pResource;
			}
			else
			{
				HRSRC hResource = ::FindResource(psp.hInstance, psp.pszTemplate, RT_DIALOG);
				if (hResource == NULL)
				{
					AfxThrowResourceException();
				}
				HGLOBAL hTemplate = LoadResource(psp.hInstance, hResource);
				if (hTemplate == NULL)
				{
					AfxThrowResourceException();
				}
				pTemplate = (LPCDLGTEMPLATE)LockResource(hTemplate);
				if (pTemplate == NULL)
				{
					AfxThrowResourceException();
				}
			}

			// if the dialog could contain OLE controls, deal with them now
			//if (afxOccManager != NULL)
			//	pTemplate = InitDialogInfo(pTemplate);

			// set font of property page to same font used by property sheet
			HGLOBAL hTemplate = _AfxChangePropPageFont(pTemplate, bWizard);

			if (m_hDialogTemplate != NULL)
			{
				GlobalFree(m_hDialogTemplate);
				m_hDialogTemplate = NULL;
			}

			if (hTemplate != NULL)
			{
				pTemplate = (LPCDLGTEMPLATE)hTemplate;
				m_hDialogTemplate = hTemplate;
			}
			psp.pResource = pTemplate;
			psp.dwFlags |= PSP_DLGINDIRECT;
		}

		void SetModified(BOOL bChanged = TRUE)
		{
			if (m_hWnd == NULL) // allowed for backward compatibility
				return;

			ASSERT(::IsWindow(m_hWnd));
			ASSERT(GetParentSheet() != NULL);

			CWnd* pParentWnd = (CWnd*)GetParentSheet();
			if (bChanged)
				pParentWnd->SendMessage(PSM_CHANGED, (WPARAM)m_hWnd);
			else
				pParentWnd->SendMessage(PSM_UNCHANGED, (WPARAM)m_hWnd);
		}

		virtual BOOL PreTranslateMessage(MSG* pMsg)
		{
			VERIFY(!CWnd::PreTranslateMessage(pMsg));

			return FALSE;   // handled by CPropertySheet::PreTranslateMessage
		}


		LRESULT QuerySiblings(WPARAM wParam, LPARAM lParam) ;

};

#define CPropertyPageEx CPropertyPageMTL


BEGIN_MESSAGE_MAP(CPropertyPageMTL, CDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()










/*============================================================================*/
// CPropertySheet -- a tabbed "dialog" (really a popup-window)

class CPropertySheetMTL : public CWnd
{
	DECLARE_DYNAMIC(CPropertySheetMTL)

	// Construction
	public:
		// simple construction
		CPropertySheetMTL()
		{
			CommonConstruct(NULL, 0);
		}

		CPropertySheetMTL(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0)
		{
			ASSERT(nIDCaption != 0);

			ENSURE(m_strCaption.LoadString(nIDCaption));
			CommonConstruct(pParentWnd, iSelectPage);
		}

		CPropertySheetMTL(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0)
		{
			ENSURE_ARG(AfxIsValidString(pszCaption));

			m_strCaption = pszCaption;
			CommonConstruct(pParentWnd, iSelectPage);
		}

		// extended construction
		CPropertySheetMTL(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage, HBITMAP hbmWatermark,
						HPALETTE hpalWatermark = NULL, HBITMAP hbmHeader = NULL)
		{
			ASSERT(nIDCaption != 0);

			ENSURE(m_strCaption.LoadString(nIDCaption));
			CommonConstruct(pParentWnd, iSelectPage, hbmWatermark, hpalWatermark, hbmHeader);
		}

		CPropertySheetMTL(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage, HBITMAP hbmWatermark,
						HPALETTE hpalWatermark = NULL, HBITMAP hbmHeader = NULL)
		{
			ENSURE_ARG(AfxIsValidString(pszCaption));

			m_strCaption = pszCaption;
			CommonConstruct(pParentWnd, iSelectPage, hbmWatermark, hpalWatermark, hbmHeader);
		}

		// for modeless creation
		virtual BOOL Create(CWnd* pParentWnd = NULL, DWORD dwStyle = (DWORD)-1, DWORD dwExStyle = 0);
		// the default style, expressed by passing -1 as dwStyle, is actually:
		// WS_SYSMENU | WS_POPUP | WS_CAPTION | DS_MODALFRAME | DS_CONTEXT_HELP | WS_VISIBLE

		virtual ~CPropertySheetMTL()
		{
			free((void*)m_psh.ppsp);
		}

	// Attributes
	public:
		DECLARE_MESSAGE_MAP()
	
		PROPSHEETHEADER m_psh;

		CPtrArray m_pages;      // array of CPropertyPage pointers
		CString m_strCaption;   // caption of the pseudo-dialog
		CWnd* m_pParentWnd;     // parent window of property sheet
		BOOL m_bStacked;        // EnableStackedTabs sets this
		BOOL m_bModeless;       // TRUE when Create called instead of DoModal

	public:
		static int CALLBACK AfxPropSheetCallback(HWND, UINT message, LPARAM lParam)
		{
			switch (message)
			{
				case PSCB_PRECREATE:
				{
					_AFX_THREAD_STATE* pState = AfxGetThreadState();
					LPDLGTEMPLATE lpTemplate = (LPDLGTEMPLATE)lParam;
					if (lpTemplate->style != pState->m_dwPropStyle ||
						lpTemplate->dwExtendedStyle != pState->m_dwPropExStyle)
					{
						// Mark the dialog template as read-write.
						DWORD dwOldProtect;
						VirtualProtect(lpTemplate, sizeof(DLGTEMPLATE), PAGE_READWRITE, &dwOldProtect);

						// Ensure DS_SETFONT is set correctly.
						lpTemplate->style = lpTemplate->style & DS_SETFONT ?
											pState->m_dwPropStyle | DS_SETFONT :
											pState->m_dwPropStyle & ~DS_SETFONT;

						lpTemplate->dwExtendedStyle = pState->m_dwPropExStyle;
						return TRUE;
					}
					return FALSE;
				}
			}

			return 0;
		}

		virtual BOOL OnInitDialog()
		{
			static int _afxPropSheetButtons[] = { IDOK, IDCANCEL, ID_APPLY_NOW, IDHELP };

			// change tab style if scrolling tabs desired (stacked tabs are default)
			if (!m_bStacked)
			{
				HWND hWndTab = (HWND)::GetDlgItem(m_hWnd, AFX_IDC_TAB_CONTROL);
				if (hWndTab != NULL)
					CWnd::ModifyStyle(hWndTab, TCS_MULTILINE, TCS_SINGLELINE, 0);
			}

			if (!IsWizard())
			{
				// resize the tab control so the layout is less restrictive
				HWND hWnd = (HWND)::GetDlgItem(m_hWnd, AFX_IDC_TAB_CONTROL);
				ASSERT(hWnd != NULL);
				CRect rectOld;
				::GetWindowRect(hWnd, &rectOld);
				ScreenToClient(rectOld);
				CRect rectNew(0, 0, 0, 32);
				::MapDialogRect(m_hWnd, &rectNew);
				if (rectNew.bottom < rectOld.bottom)
				{
					// move tab control
					int cyDiff = rectOld.Height() - rectNew.bottom;
					::SetWindowPos(hWnd, NULL, 0, 0, rectOld.Width(), rectNew.bottom, 
									SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

					// move buttons by similar amount
					for (int i = 0; i < _countof(_afxPropSheetButtons); i++)
					{
						hWnd = ::GetDlgItem(m_hWnd, _afxPropSheetButtons[i]);
						if (hWnd != NULL)
						{
							::GetWindowRect(hWnd, &rectOld);
							ScreenToClient(&rectOld);
							::SetWindowPos(hWnd, NULL, rectOld.left, rectOld.top - cyDiff,
											0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
						}
					}

					// resize property sheet itself similarly
					GetWindowRect(&rectOld);
					SetWindowPos(NULL, 0, 0, rectOld.Width(), rectOld.Height() - cyDiff, 
									SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
				}
			}

			BOOL bResult = (BOOL)Default();

			if (m_bModeless && !IsWizard())
			{
				// layout property sheet so button area is not accounted for
				CRect rectWnd;
				GetWindowRect(rectWnd);
				CRect rectButton;
				HWND hWnd = ::GetDlgItem(m_hWnd, IDOK);
				if (hWnd != NULL)
				{
					::GetWindowRect(hWnd, rectButton);
					SetWindowPos(NULL, 0, 0, rectWnd.Width(), rectButton.top - rectWnd.top,
									SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
				}

				// remove standard buttons for modeless dialogs
				for (int i = 0; i < _countof(_afxPropSheetButtons); i++)
				{
					HWND hWndButton = ::GetDlgItem(m_hWnd, _afxPropSheetButtons[i]);
					if (hWndButton != NULL)
					{
						::ShowWindow(hWndButton, SW_HIDE);
						::EnableWindow(hWndButton, FALSE);
					}
				}
			}

			// center the property sheet relative to the parent window
			if (!(GetStyle() & WS_CHILD))
				CenterWindow();

			return bResult;
		}

		virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) ;

		virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam)
		{
			// allow message map override
			if (CWnd::OnCommand(wParam, lParam))
				return TRUE;

			// crack message parameters
			UINT nID = LOWORD(wParam);
			HWND hWndCtrl = (HWND)lParam;
			int nCode = HIWORD(wParam);

			// set m_nModalResult to ID of button, whenever button is clicked
			if (hWndCtrl != NULL && nCode == BN_CLICKED)
			{
				if (::SendMessage(hWndCtrl, WM_GETDLGCODE, 0, 0) & (DLGC_BUTTON | DLGC_DEFPUSHBUTTON))
				{
					LONG lStyle = ::GetWindowLong(hWndCtrl, GWL_STYLE) & 0x0F;
					if (lStyle == BS_PUSHBUTTON || lStyle == BS_DEFPUSHBUTTON ||
						lStyle == BS_USERBUTTON || lStyle == BS_OWNERDRAW)
					{
						m_nModalResult = nID;
					}
				}
			}
			return FALSE;
		}


		void OnClose()
		{
			m_nModalResult = IDCANCEL;
			if (m_bModeless)
			{
				::PropSheet_PressButton(m_hWnd, PSBTN_CANCEL); //Now PSN_RESET and other notifications are sent to property pages.
			}
			else
			{
				Default();
			}
		}

		//LRESULT OnCommandHelp(WPARAM, LPARAM);

		HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
		{
			ENSURE_ARG(pWnd != NULL);
			LRESULT lResult;
			if (pWnd->SendChildNotifyLastMsg(&lResult))
				return (HBRUSH)lResult;

			return CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
		}

		LRESULT OnKickIdle(WPARAM wp, LPARAM lp)
		{
			ASSERT_VALID(this);
			/* Forward the message on to the active page of the property sheet */
			CPropertyPageMTL * pPage = GetActivePage();
			if (pPage != NULL)
			{
				ASSERT_VALID(pPage);
				return pPage->SendMessage(WM_KICKIDLE, wp, lp);
			}
			else
				return 0;
		}

		BOOL OnNcCreate(LPCREATESTRUCT)
		{
			// By default, MFC does not directly support the new style
			// help button in the caption, so it is turned off here.
			// It can be added back in and implemented by derived classes
			// from CPropertySheet.
			ModifyStyleEx(WS_EX_CONTEXTHELP, 0);

			return (BOOL)Default();
		}

		LRESULT OnSetDefID(WPARAM wParam, LPARAM lParam)
		{
			// A wrong or invalid ID may be passed in here.  If this is the case, then look for a valid one
			HWND hWndParam;
			if (IsWizard() &&
				(
				((hWndParam = ::GetDlgItem(m_hWnd, (int)wParam)) == NULL) ||
				!(::GetWindowLong(hWndParam, GWL_STYLE) & WS_VISIBLE) ||
				!::IsWindowEnabled(hWndParam)
				))
			{
				static const int _afxPropSheetIDs[4] = { ID_WIZNEXT, ID_WIZFINISH, ID_WIZBACK, IDCANCEL };

				for (int i = 0; i < _countof(_afxPropSheetIDs); i++)
				{
					// find first button that is visible and  enabled
					HWND hWnd = ::GetDlgItem(m_hWnd, _afxPropSheetIDs[i]);
					if ((::GetWindowLong(hWnd, GWL_STYLE) & WS_VISIBLE) &&
						::IsWindowEnabled(hWnd))
					{
						// focus could be incorrect as well in this case
						// so ... let's set it to the default button
						HWND hWndFocus = ::GetFocus();
						if (!::IsWindowEnabled(hWndFocus))
							::SetFocus(hWnd);
						return DefWindowProc(DM_SETDEFID, _afxPropSheetIDs[i], lParam);
					}
				}
			}
			return Default();
		}

		void OnSysCommand(UINT nID, LPARAM)
		{
			m_nModalResult = IDCANCEL;
			switch (nID & 0xFFF0)
			{
				case SC_CLOSE:
					if (m_bModeless)
					{
						SendMessage(WM_CLOSE);
						return;
					}
			}
			Default();
		}

	
		void AddPage(CPropertyPageMTL* pPage)
		{
			ASSERT_VALID(this);
			ENSURE_VALID(pPage);
			//ASSERT_KINDOF(CPropertyPageMTL, pPage);

			// add page to internal list
			m_pages.Add(pPage);

			// add page externally
			if (m_hWnd != NULL)
			{
				// determine size of PROPSHEETPAGE array
				PROPSHEETPAGE* ppsp = (PROPSHEETPAGE*)(m_psh.ppsp);
				int nBytes = 0;
				int nNextBytes;
				for (UINT i = 0; i < m_psh.nPages; i++)
				{
					nNextBytes = nBytes + ppsp->dwSize;
					if ((nNextBytes < nBytes) || (nNextBytes < (int)ppsp->dwSize))
						AfxThrowMemoryException();
					nBytes = nNextBytes;
					(BYTE*&)ppsp += ppsp->dwSize;
				}

				nNextBytes = nBytes + pPage->m_psp.dwSize;
				if ((nNextBytes < nBytes) || (nNextBytes < (int)pPage->m_psp.dwSize))
					AfxThrowMemoryException();

				// build new prop page array
				ppsp = (PROPSHEETPAGE*)realloc((void*)m_psh.ppsp, nNextBytes);
				if (ppsp == NULL)
					AfxThrowMemoryException();
				m_psh.ppsp = ppsp;

				// copy processed PROPSHEETPAGE struct to end
				(BYTE*&)ppsp += nBytes;
				memcpy(ppsp, &pPage->m_psp, pPage->m_psp.dwSize);
				pPage->PreProcessPageTemplate(*ppsp, IsWizard());
				if (!pPage->m_strHeaderTitle.IsEmpty())
				{
					ppsp->pszHeaderTitle = pPage->m_strHeaderTitle;
					ppsp->dwFlags |= PSP_USEHEADERTITLE;
				}
				if (!pPage->m_strHeaderSubTitle.IsEmpty())
				{
					ppsp->pszHeaderSubTitle = pPage->m_strHeaderSubTitle;
					ppsp->dwFlags |= PSP_USEHEADERSUBTITLE;
				}
				HPROPSHEETPAGE hPSP = CreatePropertySheetPage(ppsp);
				if (hPSP == NULL)
					AfxThrowMemoryException();

				if (!SendMessage(PSM_ADDPAGE, 0, (LPARAM)hPSP))
				{
					DestroyPropertySheetPage(hPSP);
					AfxThrowMemoryException();
				}
				++m_psh.nPages;
			}
		}

		virtual void BuildPropPageArray()
		{
			// delete existing prop page array
			free((void*)m_psh.ppsp);
			m_psh.ppsp = NULL;

			// determine size of PROPSHEETPAGE array
			int i;
			int nBytes = 0;
			for (i = 0; i < m_pages.GetSize(); i++)
			{
				CPropertyPageMTL* pPage = GetPage(i);
				nBytes += pPage->m_psp.dwSize;
			}

			// build new PROPSHEETPAGE array
			PROPSHEETPAGE* ppsp = (PROPSHEETPAGE*)malloc(nBytes);
			BYTE* ppspOrigByte = (BYTE*)(ppsp);
			if (ppsp == NULL)
				AfxThrowMemoryException();
			BYTE* pPropSheetPagesArrEnd = ppspOrigByte + nBytes;
			ENSURE(pPropSheetPagesArrEnd >= ppspOrigByte);
			m_psh.ppsp = ppsp;
			BOOL bWizard = (m_psh.dwFlags & (PSH_WIZARD | PSH_WIZARD97));
			for (i = 0; i < m_pages.GetSize(); i++)
			{
				CPropertyPageMTL* pPage = GetPage(i);
				BYTE* ppspByte = (BYTE*)(ppsp);
				ENSURE_THROW(ppspByte >= ppspOrigByte && ppspByte <= pPropSheetPagesArrEnd, AfxThrowMemoryException());
				memcpy(ppsp, &pPage->m_psp, pPage->m_psp.dwSize);

				if (!pPage->m_strHeaderTitle.IsEmpty())
				{
					ppsp->pszHeaderTitle = pPage->m_strHeaderTitle;
					ppsp->dwFlags |= PSP_USEHEADERTITLE;
				}
				if (!pPage->m_strHeaderSubTitle.IsEmpty())
				{
					ppsp->pszHeaderSubTitle = pPage->m_strHeaderSubTitle;
					ppsp->dwFlags |= PSP_USEHEADERSUBTITLE;
				}
				pPage->PreProcessPageTemplate(*ppsp, bWizard);
				(BYTE*&)ppsp += ppsp->dwSize;
			}
			m_psh.nPages = (int)m_pages.GetSize();
		}

		void CommonConstruct(CWnd* pParentWnd, UINT iSelectPage)
		{
			memset(&m_psh, 0, sizeof(m_psh));
			m_psh.dwSize = sizeof(m_psh);
			m_psh.dwFlags = PSH_PROPSHEETPAGE;
			m_psh.pszCaption = m_strCaption;
			m_psh.nStartPage = iSelectPage;
			m_bStacked = TRUE;
			m_bModeless = FALSE;

			if (AfxHelpEnabled())
				m_psh.dwFlags |= PSH_HASHELP;

			m_pParentWnd = pParentWnd;  // m_psh.hwndParent set in DoModal/create
		}

		void CommonConstruct(CWnd* pParentWnd, UINT iSelectPage, HBITMAP hbmWatermark, 
								HPALETTE hpalWatermark, HBITMAP hbmHeader)
		{
			CommonConstruct(pParentWnd, iSelectPage);

			if (hbmWatermark != NULL)
			{
				m_psh.hbmWatermark = hbmWatermark;
				m_psh.dwFlags |= (PSH_USEHBMWATERMARK | PSH_WATERMARK);
				m_psh.dwSize = sizeof(m_psh);
			}
			if (hpalWatermark != NULL)
			{
				m_psh.hplWatermark = hpalWatermark;
				m_psh.dwFlags |= PSH_USEHPLWATERMARK;
				m_psh.dwSize = sizeof(m_psh);
			}
			if (hbmHeader != NULL)
			{
				m_psh.hbmHeader = hbmHeader;
				m_psh.dwFlags |= (PSH_USEHBMHEADER | PSH_HEADER);
				m_psh.dwSize = sizeof(m_psh);
			}
		}

		virtual BOOL ContinueModal()
		{
			// allow CWnd::EndModalLoop to be used
			if (!CWnd::ContinueModal())
				return FALSE;

			// when active page is NULL, the modal loop should end
			ASSERT(::IsWindow(m_hWnd));
			BOOL bResult = SendMessage(PSM_GETCURRENTPAGEHWND) != 0;
			return bResult;
		}


		virtual INT_PTR DoModal() ;

		void EnableStackedTabs(BOOL bStacked) { m_bStacked = bStacked; }

		void EndDialog(int nEndID) // used to terminate a modal dialog
		{
			ASSERT_VALID(this);
			CWnd::EndModalLoop(nEndID);
			::PropSheet_PressButton(m_hWnd, PSBTN_CANCEL); //Now PSN_RESET and other notifications are sent to property pages.				
		}

		int GetActiveIndex() const
		{
			if (m_hWnd == NULL)
				return m_psh.nStartPage;

			CTabCtrl* pTab = GetTabControl();
			ENSURE(pTab);
			return pTab->GetCurSel();
		}


		CPropertyPageMTL* GetActivePage() const
		{
			ASSERT_VALID(this);

			CPropertyPageMTL* pPage;
			if (m_hWnd != NULL)
				pPage = (CPropertyPageMTL*)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, PSM_GETCURRENTPAGEHWND, 0, 0));
			else
				pPage = GetPage(GetActiveIndex());
			return pPage;
		}

		CPropertyPageMTL* GetPage(int nPage) const
		{
			CPropertyPageMTL *pPage = (CPropertyPageMTL*)m_pages[nPage];
			ENSURE(pPage);
			return pPage;
		}

		int GetPageCount() const
		{
			ASSERT_VALID(this);

			if (m_hWnd == NULL)
				return (int)m_pages.GetSize();

			CTabCtrl* pTab = GetTabControl();
			ENSURE(pTab);
			return pTab->GetItemCount();
		}

		int GetPageIndex(CPropertyPageMTL* pPage)
		{
			for (int i = 0; i < GetPageCount(); i++)
			{
				if (GetPage(i) == pPage)
					return i;
			}
			return -1;  // pPage not found
		}


		CTabCtrl* GetTabControl() const
		{
			ASSERT(::IsWindow(m_hWnd)); 
			return (CTabCtrl*)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, PSM_GETTABCONTROL, 0, 0));
		}

		LRESULT HandleInitDialog(WPARAM, LPARAM)
		{
			LRESULT lResult = OnInitDialog();
			return lResult;
		}

		BOOL IsModeless() const { return m_bModeless; }

		BOOL IsWizard() const { return (m_psh.dwFlags & (PSH_WIZARD | PSH_WIZARD97)) != 0; }

		void MapDialogRect(LPRECT lpRect) const { ASSERT(::IsWindow(m_hWnd)); ::MapDialogRect(m_hWnd, lpRect); }

		void RemovePage(int nPage)
		{
			ASSERT_VALID(this);

			// remove the page externally
			if (m_hWnd != NULL)
				SendMessage(PSM_REMOVEPAGE, nPage);

			// remove the page from internal list
			m_pages.RemoveAt(nPage);
		}

		void RemovePage(CPropertyPageMTL* pPage)
		{
			ASSERT_VALID(this);
			ENSURE_VALID(pPage);
			//ASSERT_KINDOF(CPropertyPageMTL, pPage);

			int nPage = GetPageIndex(pPage);
			ASSERT(nPage >= 0);

			RemovePage(nPage);
		}

		void PressButton(int nButton)
		{
			ASSERT(::IsWindow(m_hWnd));
			if (nButton == PSBTN_FINISH)
				m_nModalResult = ID_WIZFINISH;
			::SendMessage(m_hWnd, PSM_PRESSBUTTON, nButton, 0);
		}

		virtual BOOL PreTranslateMessage(MSG* pMsg)
		{
			ASSERT_VALID(this);

			// allow tooltip messages to be filtered
			if (CWnd::PreTranslateMessage(pMsg))
				return TRUE;

			HGLOBAL hMemProp = (HGLOBAL)GetProp(this->m_hWnd, PROP_CLOSEPENDING_NAME);
			BOOL* pBool = (BOOL*)(GlobalLock(hMemProp));

			if (pBool != NULL)
			{
				if (*pBool == TRUE && NULL == PropSheet_GetCurrentPageHwnd(m_hWnd))
				{
					GlobalUnlock(hMemProp);
					hMemProp = RemoveProp(this->m_hWnd, PROP_CLOSEPENDING_NAME);
					if (hMemProp)
						GlobalFree(hMemProp);
					DestroyWindow();
					return TRUE;
				}
				else
				{
					GlobalUnlock(hMemProp);
				}
			}

			// allow sheet to translate Ctrl+Tab, Shift+Ctrl+Tab,
			//  Ctrl+PageUp, and Ctrl+PageDown
			if (pMsg->message == WM_KEYDOWN && GetAsyncKeyState(VK_CONTROL) < 0 &&
				(pMsg->wParam == VK_TAB || pMsg->wParam == VK_PRIOR || pMsg->wParam == VK_NEXT))
			{
				if (SendMessage(PSM_ISDIALOGMESSAGE, 0, (LPARAM)pMsg))
					return TRUE;
			}

			// handle rest with IsDialogMessage
			return PreTranslateInput(pMsg);
		}

		BOOL SetActivePage(int nPage)
		{
			if (m_hWnd == NULL)
			{
				m_psh.nStartPage = nPage;
				return TRUE;
			}
			return (BOOL)SendMessage(PSM_SETCURSEL, nPage);
		}

		BOOL SetActivePage(CPropertyPageMTL* pPage)
		{
			ASSERT_VALID(this);
			ENSURE_VALID(pPage);
			//ASSERT_KINDOF(CPropertyPageMTL, pPage);

			int nPage = GetPageIndex(pPage);
			ASSERT(pPage >= 0);

			return SetActivePage(nPage);
		}

		void SetFinishText(LPCTSTR lpszText) 
		{ 
			ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, PSM_SETFINISHTEXT, 0, (LPARAM)lpszText); 
		}

		void SetTitle(LPCTSTR lpszText, UINT nStyle = 0)
		{
			ASSERT((nStyle & ~PSH_PROPTITLE) == 0); // only PSH_PROPTITLE is valid
			ASSERT(lpszText == NULL || AfxIsValidString(lpszText));

			if (m_hWnd == NULL)
			{
				// set internal state
				m_strCaption = lpszText;
				m_psh.pszCaption = m_strCaption;
				m_psh.dwFlags &= ~PSH_PROPTITLE;
				m_psh.dwFlags |= nStyle;
			}
			else
			{
				// set external state
				SendMessage(PSM_SETTITLE, nStyle, (LPARAM)lpszText);
			}
		}


		void SetWizardButtons(DWORD dwFlags)
		{
			ASSERT(::IsWindow(m_hWnd)); ::PostMessage(m_hWnd, PSM_SETWIZBUTTONS, 0, dwFlags);
		}

		void SetWizardMode() { m_psh.dwFlags |= PSH_WIZARD; }

};


#define CPropertySheetEx CPropertySheetMTL

BEGIN_MESSAGE_MAP(CPropertySheetMTL, CWnd)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_WM_NCCREATE()
	ON_WM_SYSCOMMAND()

	//ON_MESSAGE(WM_COMMANDHELP,&CPropertySheet::OnCommandHelp)
	ON_MESSAGE(WM_INITDIALOG, &CPropertySheetMTL::HandleInitDialog)
	ON_MESSAGE(WM_KICKIDLE, &CPropertySheetMTL::OnKickIdle)
	ON_MESSAGE(DM_SETDEFID, &CPropertySheetMTL::OnSetDefID)
END_MESSAGE_MAP()



_INLINE BOOL CPropertyPageMTL::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	ASSERT(pResult != NULL);
	NMHDR* pNMHDR = (NMHDR*)lParam;

	// allow message map to override
	if (CDialog::OnNotify(wParam, lParam, pResult))
		return TRUE;

	// don't handle messages not from the page/sheet itself
	if (pNMHDR->hwndFrom != m_hWnd && pNMHDR->hwndFrom != ::GetParent(m_hWnd))
		return FALSE;

	// handle default
	switch (pNMHDR->code)
	{
		case PSN_SETACTIVE:
		{
			CPropertySheetMTL* pSheet = GetParentSheet();
			if (pSheet != NULL && !(pSheet->m_nFlags & WF_CONTINUEMODAL) && !(pSheet->m_bModeless) && !(pSheet->m_psh.dwFlags & PSH_AEROWIZARD))
				*pResult = -1;
			else
				*pResult = OnSetActive() ? 0 : -1;
		}
		break;
		case PSN_KILLACTIVE:
			*pResult = !OnKillActive();
			break;
		case PSN_APPLY:
			*pResult = OnApply() ? PSNRET_NOERROR : PSNRET_INVALID_NOCHANGEPAGE;
			break;
		case PSN_RESET:
			OnReset();
			break;
		case PSN_QUERYCANCEL:
			*pResult = !OnQueryCancel();
			break;
		case PSN_WIZNEXT:
			*pResult = MapWizardResult(OnWizardNext());
			break;
		case PSN_WIZBACK:
			*pResult = MapWizardResult(OnWizardBack());
			break;
		case PSN_WIZFINISH:
			*pResult = (LRESULT)(OnWizardFinishEx());
			break;
		case PSN_HELP:
			SendMessage(WM_COMMAND, ID_HELP);
			break;

		default:
			return FALSE;   // not handled
	}

	return TRUE;    // handled
}

_INLINE BOOL CPropertyPageMTL::OnWizardFinish()
{
	BOOL bClose = FALSE;
	if (UpdateData())
	{
		CPropertySheetMTL *pSheet = GetParentSheet();
		if (pSheet != NULL)
		{
			if (pSheet->IsModeless() && pSheet->IsWizard())
			{
				//Msg is posted so PreTranslateMessage of CPropertySheet is called
				//and it will immediatly DestoryWindow().
				pSheet->PostMessage(WM_NULL, 0, 0);
			}
		}
		bClose = TRUE;
	}
	return bClose;
}

_INLINE void CPropertyPageMTL::CancelToClose()
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(GetParentSheet() != NULL);

	GetParentSheet()->SendMessage(PSM_CANCELTOCLOSE);
}

_INLINE void CPropertyPageMTL::EndDialog(int nID) // called for error scenarios
{
	// Normally you shouldn't call EndDialog from a page. But in case it does
	// happen during error situations, call CPropertySheet::EndDialog instead.

	CPropertySheetMTL* pParent = GetParentSheet();
	if (pParent != NULL)
		pParent->EndDialog(nID);
}

_INLINE LRESULT CPropertyPageMTL::MapWizardResult(LRESULT lToMap)
{
	// -1 and 0 are special
	if (lToMap == -1 || lToMap == 0)
		return lToMap;

	// only do special stuff if MFC owns the property sheet
	CPropertySheetMTL* pSheet = GetParentSheet();
	if (pSheet != NULL)
	{
		// search the pages for a matching ID
		const PROPSHEETPAGE* ppsp = pSheet->m_psh.ppsp;
		for (int i = 0; i < pSheet->m_pages.GetSize(); i++)
		{
			// check page[i] for a match
			CPropertyPageMTL* pPage = pSheet->GetPage(i);
			if ((LRESULT)pPage->m_psp.pszTemplate == lToMap)
				return (LRESULT)ppsp->pResource;

			// jump to next page
			(BYTE*&)ppsp += ppsp->dwSize;
		}
	}
	// otherwise, just use the original value
	return lToMap;
}

_INLINE LRESULT CPropertyPageMTL::QuerySiblings(WPARAM wParam, LPARAM lParam)
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(GetParentSheet() != NULL);

	return GetParentSheet()->SendMessage(PSM_QUERYSIBLINGS, wParam, lParam);
}






#if 1
#define CPropertyPage CPropertyPageMTL
#define CPropertySheet CPropertySheetMTL
#endif