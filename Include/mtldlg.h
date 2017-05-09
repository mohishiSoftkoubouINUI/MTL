



#if 0
template <class T>
class ATL_NO_VTABLE CDialogWMHnd : public CDialogImpl<T, CWMHnd>
{
	typedef CDialogImpl<T, CWMHnd> CDialogWMHndBase;
public:

public:

	// メッセージマップ
	DEFAULT_MAP()

public:
	virtual LRESULT Default()
	{
		LRESULT lResult;
		const _ATL_MSG* pMsg = GetCurrentMessage();
		CWMHnd::_ProcessWindowMessage(pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam, lResult);

		return 0;
	}

	virtual const _ATL_MSG* GetCurrentMsg() { return GetCurrentMessage(); }

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
#ifndef _MTL_ATL3
		WNDPROC proc = (WNDPROC)CDialogWMHndBase::m_thunk.GetWNDPROC();
#else
		WNDPROC proc = (WNDPROC)&(CDialogWMHndBase::m_thunk.thunk);
#endif
		return proc(m_hWnd, message, wParam, lParam);
	}

};

class CDialog : public CDialogWMHnd<CDialog>
{
public:

	CDialog(LPCTSTR lpszTemplateName, CWMHnd* pParentWnd = NULL)
	{
		m_lpszTemplateName = lpszTemplateName;
		m_pParentWnd = pParentWnd;
	}

	CDialog(UINT nIDTemplate, CWMHnd* pParent = NULL)
	{
		m_lpszTemplateName = MAKEINTRESOURCE(nIDTemplate);
		m_pParentWnd = pParent;
	}

	CDialog(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;

		m_lpszTemplateName = NULL;
		m_pParentWnd = NULL;
	}

public:

	LPCTSTR m_lpszTemplateName;     // name or MAKEINTRESOURCE

	CWMHnd* m_pParentWnd;             // parent/owner window

public:
	DECLARE_MESSAGE_MAP()

	IMPLEMENT_HANDLE_CREATE(CDialog, HWND)

		DECLARE_WMH(CDialog)

public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	// WM_NCDESTROY is the absolute LAST message sent.
	void OnNcDestroy()
	{
		// cleanup main and active windows
		CWMHnd::OnNcDestroy();
	}


	virtual void OnOK()
	{
		if (!UpdateData(TRUE))
		{
			TRACE(traceAppMsg, 0, "UpdateData failed during dialog termination.\n");
			// the UpdateData routine will set focus to correct item
			return;
		}
		EndDialog(IDOK);
	}

	virtual void OnCancel()
	{
		EndDialog(IDCANCEL);
	}

	// modal processing
	virtual INT_PTR DoModal()
	{
		ATLASSUME(m_hWnd == NULL);

		// Allocate the thunk structure here, where we can fail gracefully.

		BOOL result = m_thunk.Init(NULL, NULL);
		if (result == FALSE)
		{
			SetLastError(ERROR_OUTOFMEMORY);
			return -1;
		}

#ifdef _DEBUG
		m_bModal = true;
#endif //_DEBUG

		_AtlWinModule.AddCreateWndData(&m_thunk.cd, (CDialogImplBaseT< CWMHnd >*)this);

		HWND hWndParent = ::GetActiveWindow();
		LPARAM dwInitParam = NULL;

		return ::DialogBoxParam(_AtlBaseModule.GetResourceInstance(), m_lpszTemplateName,
			hWndParent, CDialog::StartDialogProc, dwInitParam);
	}



	BOOL _ProcessedWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID)
	{
		CDialogWMHnd<CDialog>::_ProcessedWindowMessage(hWnd, uMsg, wParam, lParam, lResult, dwMsgMapID);

		switch (uMsg)
		{
		case WM_INITDIALOG:
			CenterWindow(hWnd);
			FromHandle(hWnd, this, NULL)->OnInitDialog();
			if (!UpdateData(FALSE))
			{
				TRACE(traceAppMsg, 0, "Warning: UpdateData failed during dialog init.\n");
				EndDialog(-1);
				return FALSE;
			}
			break;
		}
		return FALSE;
	}


};
#endif



_INLINE BOOL CWnd::CreateDlg(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// load resource
	LPCDLGTEMPLATE lpDialogTemplate = NULL;
	HGLOBAL hDialogTemplate = NULL;
	HINSTANCE hInst = AfxFindResourceHandle(lpszTemplateName, RT_DIALOG);
	HRSRC hResource = ::FindResource(hInst, lpszTemplateName, RT_DIALOG);
	hDialogTemplate = LoadResource(hInst, hResource);
	if (hDialogTemplate != NULL)
		lpDialogTemplate = (LPCDLGTEMPLATE)LockResource(hDialogTemplate);
	ASSERT(lpDialogTemplate != NULL);

	// create a modeless dialog
	BOOL bSuccess = CreateDlgIndirect(lpDialogTemplate, pParentWnd, hInst);

	// free resource
	UnlockResource(hDialogTemplate);
	FreeResource(hDialogTemplate);

	return bSuccess;
}

_INLINE BOOL CWnd::CreateDlgIndirect(LPCDLGTEMPLATE lpDialogTemplate, CWnd* pParentWnd, HINSTANCE hInst)
{
	VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTLS_REG));
	AfxDeferRegisterClass(AFX_WNDCOMMCTLSNEW_REG);

	HookWindowCreate(this);

	m_hWnd = ::CreateDialogIndirect(hInst, lpDialogTemplate, pParentWnd->GetSafeHwnd(), (DLGPROC)AfxDlgProc);

	UnhookWindowCreate();

	return (m_hWnd != NULL);
}




class CDialog : public CWnd
{
	DECLARE_DYNAMIC(CDialog)

	public:

		CDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL) : CWnd(NULL)
		{
			m_nIDHelp = 0;

			m_lpszTemplateName = lpszTemplateName;
			m_hDialogTemplate = 0 ;

			m_pParentWnd = pParentWnd;

			m_bClosedByEndDialog = FALSE;
		}

		CDialog(UINT nIDTemplate, CWnd* pParent = NULL) : CWnd(NULL)
		{
			m_nIDHelp = 0;

			m_lpszTemplateName = MAKEINTRESOURCE(nIDTemplate);
			m_hDialogTemplate = 0;

			m_pParentWnd = pParent;

			m_bClosedByEndDialog = FALSE;
		}

		CDialog(HWND hWnd = NULL) : CWnd(hWnd)
		{
			m_nIDHelp = 0;

			m_lpszTemplateName = NULL;
			m_hDialogTemplate = 0;

			m_pParentWnd = NULL;

			m_bClosedByEndDialog = FALSE;
		}

	public:
		DECLARE_MESSAGE_MAP()

		IMPLEMENT_HANDLE_CREATE(CDialog, HWND)


		UINT m_nIDHelp;                 // Help ID (0 for none, see HID_BASE_RESOURCE)

		LPCTSTR m_lpszTemplateName;     // name or MAKEINTRESOURCE
		HGLOBAL m_hDialogTemplate;      // indirect (m_lpDialogTemplate == NULL)

		CWnd* m_pParentWnd;             // parent/owner window

		BOOL m_bClosedByEndDialog ;


	public:

		virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);


		virtual void OnOK()
		{
			if (!UpdateData(TRUE))
			{
				TRACE(traceAppMsg, 0, "UpdateData failed during dialog termination.\n");
				// the UpdateData routine will set focus to correct item
				return;
			}
			EndDialog(IDOK);
		}

		virtual void OnCancel()
		{
			EndDialog(IDCANCEL);
		}

		virtual BOOL OnInitDialog()
		{
			// execute dialog RT_DLGINIT resource
			BOOL bDlgInit = ExecuteDlgInit(m_lpszTemplateName);
			if (!bDlgInit)
			{
				TRACE(traceAppMsg, 0, "Warning: ExecuteDlgInit failed during dialog init.\n");
				EndDialog(-1);
				return FALSE;
			}

			// transfer data into the dialog from member variables
			if (!UpdateData(FALSE))
			{
				TRACE(traceAppMsg, 0, "Warning: UpdateData failed during dialog init.\n");
				EndDialog(-1);
				return FALSE;
			}

			return TRUE;    // set focus to first one
		}


		virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL) { return Create(ATL_MAKEINTRESOURCE(nIDTemplate), pParentWnd); }

		virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL)
		{
			if (pParentWnd == NULL)
				pParentWnd = AfxGetMainWnd();

			m_lpszTemplateName = lpszTemplateName;  // used for help

			return CWnd::CreateDlg(lpszTemplateName, pParentWnd);
		}

		virtual BOOL CreateIndirect(HGLOBAL hDialogTemplate, CWnd* pParentWnd = NULL, HINSTANCE hInst = NULL)
		{
			ASSERT(hDialogTemplate != NULL);

			if (pParentWnd == NULL)
				pParentWnd = AfxGetMainWnd();

			LPCDLGTEMPLATE lpDialogTemplate = (LPCDLGTEMPLATE)LockResource(hDialogTemplate);

			CWnd::CreateDlgIndirect(lpDialogTemplate, pParentWnd, hInst	) ;

			UnlockResource(hDialogTemplate);

			return (m_hWnd != NULL);
		}


		virtual INT_PTR DoModal()
		{
			BOOL bEnableParent = FALSE;
			BOOL bEnableMainWnd = FALSE;

			HWND hWndParent = CWnd::GetSafeOwner_(m_pParentWnd->GetSafeHwnd(), NULL);

			CWMHnd* pMainWnd = AfxGetMainWnd();

			if (hWndParent && hWndParent != ::GetDesktopWindow() && ::IsWindowEnabled(hWndParent))
			{
				::EnableWindow(hWndParent, FALSE);
				bEnableParent = TRUE;
				if (pMainWnd && pMainWnd->IsFrameWnd() && pMainWnd->IsWindowEnabled())
				{
					//
					// We are hosted by non-MFC container
					// 
					pMainWnd->EnableWindow(FALSE);
					bEnableMainWnd = TRUE;
				}
			}

			// setup for modal loop and creation
			m_nModalResult = -1;
			m_nFlags |= WF_CONTINUEMODAL;

			CWnd* pCWndParent = FromHandle(hWndParent) ;

			if (Create(m_lpszTemplateName, pCWndParent))
			{
				if (m_nFlags & WF_CONTINUEMODAL)
				{
					// enter modal loop
					DWORD dwFlags = MLF_SHOWONIDLE;
					if (GetStyle() & DS_NOIDLEMSG)
						dwFlags |= MLF_NOIDLEMSG;
					VERIFY(RunModalLoop(dwFlags) == m_nModalResult);
				}

				// hide the window before enabling the parent, etc.
				if (m_hWnd != NULL)
					SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW |
								SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
			}


			if (bEnableMainWnd)
				pMainWnd->EnableWindow(TRUE);
			if (bEnableParent)
				::EnableWindow(hWndParent, TRUE);
			if (hWndParent != NULL && ::GetActiveWindow() == m_hWnd)
				::SetActiveWindow(hWndParent);


			// destroy modal window
			DestroyWindow();

			return m_nModalResult;
		}

		void EndDialog(int nResult)
		{
			ASSERT(::IsWindow(m_hWnd));

			m_bClosedByEndDialog = TRUE;

			if (m_nFlags & (WF_MODALLOOP | WF_CONTINUEMODAL))
				EndModalLoop(nResult);

			::EndDialog(m_hWnd, nResult);
		}

		DWORD GetDefID() const { ASSERT(::IsWindow(m_hWnd)); return DWORD(::SendMessage(m_hWnd, DM_GETDEFID, 0, 0)); }
		void GotoDlgCtrl(CWnd* pWndCtrl) { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, WM_NEXTDLGCTL, (WPARAM)pWndCtrl->m_hWnd, 1L); }
		void MapDialogRect(LPRECT lpRect) const { ASSERT(::IsWindow(m_hWnd)); ::MapDialogRect(m_hWnd, lpRect); }
		void NextDlgCtrl() const { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, WM_NEXTDLGCTL, 0, 0); }
		void PrevDlgCtrl() const { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, WM_NEXTDLGCTL, 1, 0); }


		static BOOL _AfxCompareClassName(HWND hWnd, LPCTSTR lpszClassName)
		{
			ASSERT(::IsWindow(hWnd));
			TCHAR szTemp[32];
			::GetClassName(hWnd, szTemp, _countof(szTemp));
			return ::lstrcmpi(szTemp, lpszClassName) == 0;
		}

		BOOL PreTranslateMessage(MSG* pMsg)
		{
			// for modeless processing (or modal)
			ASSERT(m_hWnd != NULL);

			// allow tooltip messages to be filtered
			if (CWnd::PreTranslateMessage(pMsg))
				return TRUE;

			// fix around for VK_ESCAPE in a multiline Edit that is on a Dialog
			// that doesn't have a cancel or the cancel is disabled.
			if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_CANCEL) &&
				(::GetWindowLong(pMsg->hwnd, GWL_STYLE) & ES_MULTILINE) &&
				_AfxCompareClassName(pMsg->hwnd, _T("Edit")))
			{
				HWND hItem = ::GetDlgItem(m_hWnd, IDCANCEL);
				if (hItem == NULL || ::IsWindowEnabled(hItem))
				{
					SendMessage(WM_COMMAND, IDCANCEL, 0);
					return TRUE;
				}
			}
			// filter both messages to dialog and from children
			return PreTranslateInput(pMsg);
		}

		void SetDefID(UINT nID) { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, DM_SETDEFID, nID, 0); }

};


BEGIN_MESSAGE_MAP(CDialog, CWnd)
	//TRACE("CDialog::hWnd=%d, uMsg=%d, wParam=%08x, lParam=%08x\n", hWnd, uMsg, wParam, lParam);
	//ON_WM_NCDESTROY()
	ON_COMMAND(IDOK, &CDialog::OnOK)
	ON_COMMAND(IDCANCEL, &CDialog::OnCancel)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// AfxDlgProc - does nothing since all messages are handled via AfxWndProc

_INLINE INT_PTR CALLBACK CWnd::AfxDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_INITDIALOG)
	{
		// special case for WM_INITDIALOG
		CDialog* pDlg = DYNAMIC_DOWNCAST(CDialog, CWnd::FromHandlePermanent(hWnd));
		if (pDlg == NULL)
			return TRUE ;

		if (pDlg->m_nFlags & WF_CONTINUEMODAL)
			pDlg->CenterWindow();

		return pDlg->OnInitDialog();
	}

	return FALSE ;
}




// Printing Dialog

/////////////////////////////////////////////////////////////////////////////
// _AFX_WIN_STATE

class _AFX_WIN_STATE
{
	public:
		// printing abort
		BOOL m_bUserAbort;
};


LPCTSTR AfxGetAppName();
BOOL AfxPumpMessage();

class CPrintingDialog : public CDialog
{
	public:

	public:
		enum { IDD = AFX_IDD_PRINTDLG };

	public:
		static _AFX_WIN_STATE* GetAfxWinState()
		{
			static _AFX_WIN_STATE _afxWinState ;

			return &_afxWinState ;
		}

		static BOOL CALLBACK _AfxAbortProc(HDC, int)
		{
			_AFX_WIN_STATE* pWinState = GetAfxWinState();
			MSG msg;
			while (!pWinState->m_bUserAbort && ::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
			{
				if (!AfxPumpMessage())
					return FALSE;   // terminate if WM_QUIT received
			}
			return !pWinState->m_bUserAbort;
		}


		CPrintingDialog(CWnd* pParent)
		{
			Create(CPrintingDialog::IDD, pParent);      // modeless !
			GetAfxWinState()->m_bUserAbort = FALSE;
		}

		virtual BOOL OnInitDialog()
		{
			SetWindowText(AfxGetAppName());
			CenterWindow();
			return CDialog::OnInitDialog();
		}

		virtual void OnCancel()
		{
			GetAfxWinState()->m_bUserAbort = TRUE;  // flag that user aborted print
			CDialog::OnCancel();
		}


};

/*============================================================================*/
// CDialogTemplate

class CDialogTemplate
{
	public:
		CDialogTemplate(const DLGTEMPLATE* pTemplate = NULL)
		{
			if (pTemplate == NULL)
			{
				m_hTemplate = NULL;
				m_dwTemplateSize = 0;
				m_bSystemFont = FALSE;
			}
			else
			{
				BOOL bSet = SetTemplate(pTemplate, GetTemplateSize(pTemplate));
				if (!bSet)
				{
					AfxThrowMemoryException();
				}
			}
		}

		CDialogTemplate(HGLOBAL hTemplate)
		{
			if (hTemplate == NULL)
			{
				m_hTemplate = NULL;
				m_dwTemplateSize = 0;
				m_bSystemFont = FALSE;
			}
			else
			{
				DLGTEMPLATE* pTemplate = (DLGTEMPLATE*)GlobalLock(hTemplate);
				BOOL bSet = SetTemplate(pTemplate, GetTemplateSize(pTemplate));
				GlobalUnlock(hTemplate);
				if (!bSet)
				{
					AfxThrowMemoryException();
				}
			}
		}

		~CDialogTemplate()
		{
			if (m_hTemplate != NULL)
				GlobalFree(m_hTemplate);
		}


	public:
		HGLOBAL m_hTemplate;
		DWORD m_dwTemplateSize;
		BOOL m_bSystemFont;

	public:
		HGLOBAL Detach()
		{
			HGLOBAL hTmp = m_hTemplate;
			m_hTemplate = NULL;
			return hTmp;
		}

		static BYTE* GetFontSizeField(const DLGTEMPLATE* pTemplate)
		{
			BOOL bDialogEx = IsDialogEx(pTemplate);
			WORD* pw;

			if (bDialogEx)
				pw = (WORD*)((DLGTEMPLATEEX*)pTemplate + 1);
			else
				pw = (WORD*)(pTemplate + 1);

			if (*pw == (WORD)-1)        // Skip menu name string or ordinal
				pw += 2; // WORDs
			else
				while (*pw++);

			if (*pw == (WORD)-1)        // Skip class name string or ordinal
				pw += 2; // WORDs
			else
				while (*pw++);

			while (*pw++);          // Skip caption string

			return (BYTE*)pw;
		}

		static int FontAttrSize(BOOL bDialogEx)
		{
			return (int)sizeof(WORD) * (bDialogEx ? 3 : 1);
		}

		static BOOL GetFont(const DLGTEMPLATE* pTemplate, CString& strFace, WORD& nFontSize)
		{
			ASSERT(pTemplate != NULL);

			if (!HasFont(pTemplate))
				return FALSE;

			BYTE* pb = GetFontSizeField(pTemplate);
			nFontSize = *(WORD*)pb;
			pb += FontAttrSize(IsDialogEx(pTemplate));

#if defined(_UNICODE)
			// Copy font name
			strFace = (LPCTSTR)pb;
#else
			// Convert Unicode font name to MBCS
			WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)pb, -1,
				strFace.GetBufferSetLength(LF_FACESIZE), LF_FACESIZE, NULL, NULL);
			strFace.ReleaseBuffer();
#endif

			return TRUE;
		}

		static BOOL HasFont(const DLGTEMPLATE* pTemplate)
		{
			return (DS_SETFONT & (IsDialogEx(pTemplate) ? ((DLGTEMPLATEEX*)pTemplate)->style : pTemplate->style));
		}

		static BOOL IsDialogEx(const DLGTEMPLATE* pTemplate)
		{
			return ((DLGTEMPLATEEX*)pTemplate)->signature == 0xFFFF;
		}

		UINT GetTemplateSize(const DLGTEMPLATE* pTemplate)
		{
			BOOL bDialogEx = IsDialogEx(pTemplate);
			BYTE* pb = GetFontSizeField(pTemplate);

			if (HasFont(pTemplate))
			{
				// Skip font size and name
				pb += FontAttrSize(bDialogEx);  // Skip font size, weight, (italic, charset)
				pb += 2 * (lstrlenW((WCHAR*)pb) + 1);
			}

			WORD nCtrl = bDialogEx ? (WORD)((DLGTEMPLATEEX*)pTemplate)->cDlgItems : (WORD)pTemplate->cdit;

			while (nCtrl > 0)
			{
				pb = (BYTE*)(((DWORD_PTR)pb + 3) & ~DWORD_PTR(3)); // DWORD align

				pb += (bDialogEx ? sizeof(DLGITEMTEMPLATEEX) : sizeof(DLGITEMTEMPLATE));

				if (*(WORD*)pb == (WORD)-1)     // Skip class name string or ordinal
					pb += 2 * sizeof(WORD);
				else
					pb = (BYTE*)_SkipString((WCHAR*)pb);

				if (*(WORD*)pb == (WORD)-1)     // Skip text string or ordinal
					pb += 2 * sizeof(WORD);
				else
					pb = (BYTE*)_SkipString((WCHAR*)pb);

				WORD cbExtra = *(WORD*)pb;      // Skip extra data
				if (cbExtra != 0 && !bDialogEx)
					cbExtra -= 2;
				pb += sizeof(WORD) + cbExtra;
				--nCtrl;
			}

			return UINT(pb - (BYTE*)pTemplate);
		}

		BOOL SetFont(LPCTSTR lpFaceName, WORD nFontSize)
		{
			ASSERT(m_hTemplate != NULL);

			if (m_dwTemplateSize == 0)
				return FALSE;

			DLGTEMPLATE* pTemplate = (DLGTEMPLATE*)GlobalLock(m_hTemplate);

			BOOL bDialogEx = IsDialogEx(pTemplate);
			BOOL bHasFont = HasFont(pTemplate);
			int cbFontAttr = FontAttrSize(bDialogEx);

			if (bDialogEx)
				((DLGTEMPLATEEX*)pTemplate)->style |= DS_SETFONT;
			else
				pTemplate->style |= DS_SETFONT;

			int nFaceNameLen = lstrlen(lpFaceName);
			if (nFaceNameLen >= LF_FACESIZE)
			{
				// Name too long
				return FALSE;
			}

#ifdef _UNICODE
			int cbNew = cbFontAttr + ((nFaceNameLen + 1) * sizeof(TCHAR));
			BYTE* pbNew = (BYTE*)lpFaceName;
#else
			WCHAR wszFaceName[LF_FACESIZE];
			int cbNew = cbFontAttr + 2 * MultiByteToWideChar(CP_ACP, 0, lpFaceName, -1, wszFaceName, LF_FACESIZE);
			BYTE* pbNew = (BYTE*)wszFaceName;
#endif
			if (cbNew < cbFontAttr)
			{
				return FALSE;
			}
			BYTE* pb = GetFontSizeField(pTemplate);
			int cbOld = (int)(bHasFont ? cbFontAttr + 2 * (lstrlenW((WCHAR*)(pb + cbFontAttr)) + 1) : 0);

			BYTE* pOldControls = (BYTE*)(((DWORD_PTR)pb + cbOld + 3) & ~DWORD_PTR(3));
			BYTE* pNewControls = (BYTE*)(((DWORD_PTR)pb + cbNew + 3) & ~DWORD_PTR(3));

			WORD nCtrl = bDialogEx ? (WORD)((DLGTEMPLATEEX*)pTemplate)->cDlgItems :
				(WORD)pTemplate->cdit;

			if (cbNew != cbOld && nCtrl > 0)
			{
				size_t nBuffLeftSize = (size_t)(m_dwTemplateSize - (pOldControls - (BYTE*)pTemplate));
				if (nBuffLeftSize > m_dwTemplateSize)
				{
					return FALSE;
				}
				memmove(pNewControls, pOldControls, nBuffLeftSize);
			}

			*(WORD*)pb = nFontSize;
			memmove(pb + cbFontAttr, pbNew, cbNew - cbFontAttr);

			m_dwTemplateSize += ULONG(pNewControls - pOldControls);

			GlobalUnlock(m_hTemplate);
			m_bSystemFont = FALSE;
			return TRUE;
		}

		BOOL SetTemplate(const DLGTEMPLATE* pTemplate, UINT cb)
		{
			m_dwTemplateSize = cb;
			SIZE_T nAllocSize = m_dwTemplateSize + LF_FACESIZE * 2;
			if (nAllocSize < m_dwTemplateSize)
			{
				return FALSE;
			}
			if ((m_hTemplate = GlobalAlloc(GPTR, nAllocSize)) == NULL)
			{
				return FALSE;
			}
			DLGTEMPLATE* pNew = (DLGTEMPLATE*)GlobalLock(m_hTemplate);
			memcpy((BYTE*)pNew, pTemplate, (size_t)m_dwTemplateSize);

			m_bSystemFont = (HasFont(pNew) == 0);

			GlobalUnlock(m_hTemplate);
			return TRUE;
		}

		static WCHAR* _SkipString(WCHAR* p)
		{
			while (*p++);
			return p;
		}
};
