


/*
	CObject
	CCmdTarget
	CWMHnd : CWindow
	CWnd
*/


/*
	WindowProc
		OnWndMsg
			_ProcessWindowMessage
				MESSAGE_MAP
				base::_ProcessWindowMessage
					...
						CCmdTarget::_ProcessWindowMessage

	virtual OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) -> _ProcessWindowMessage
*/


#define DELETE_HWND(t) { \
						if ((t)->m_hWnd != NULL && \
							(t) != &wndTop && (t) != &wndBottom && (t) != &wndTopMost && (t) != &wndNoTopMost) \
																		{ \
							TRACE(traceAppMsg, 0, "Warning: calling DestroyWindow in ~(); " \
													"OnDestroy or PostNcDestroy in derived class will not be called.\n"); \
							if ((t)->DestroyWindow()) \
								(t)->m_hWnd = NULL; \
																		} }




/////////////////////////////////////////////////////////////////////////////
// CTestCmdUI - used to test for disabled commands before dispatching

class CTestCmdUI : public CCmdUI
{
	public:
		CTestCmdUI()
		{
			m_bEnabled = TRUE;  // assume it is enabled
		}

	public:
		BOOL m_bEnabled;

	public: // re-implementations only
		virtual void Enable(BOOL bOn)
		{
			m_bEnabled = bOn;
			m_bEnableChanged = TRUE;
		}
		virtual void SetCheck(int nCheck) {}
		virtual void SetRadio(BOOL bOn) {}
		virtual void SetText(LPCTSTR) {}

};


class CWnd : public CWMHnd
{
	DECLARE_DYNCREATE(CWnd)


	public:
		CWnd(HWND hWnd = NULL) : CWMHnd(hWnd)
		{
			m_nFlags = 0;
			m_nModalResult = 0 ;

			m_hWndOwner = NULL;

			m_pfnSuperWindowProc = ::DefWindowProc;
		}

		void CWndDestruct()
		{
			if (m_hWnd != NULL)
				RemoveHandle(m_hWnd);

			if (m_bFromHandle)
			{
				m_hWnd = NULL;
				return;
			}

			DELETE_HWND(this)
		}

		virtual ~CWnd()
		{
			_VOLATILE_CLASS_FUNC_V_V(CWnd, CWndDestruct)
		}

	public:
		DECLARE_MESSAGE_MAP()

		IMPLEMENT_HANDLE_CREATE(CWnd, HWND)


		enum RepositionFlags
		{
			reposDefault = 0, reposQuery = 1, reposExtra = 2, reposNoPosLeftOver = 0x8000
		};

		UINT m_nFlags;      // see WF_ flags above
		int m_nModalResult; // for return values from CWnd::RunModalLoop

		HWND	m_hWndOwner;

		CWndProcThunk m_thunk;
		WNDPROC m_pfnSuperWindowProc;


	public:

		typedef struct _CreateWindowParam
		{
			HHOOK	hHookOldCbtFilter;
			CWnd*	pCWnd;
			ATOM	hAtom;
		} CREATEWINDOWPARAM;

		static CREATEWINDOWPARAM& GetHookOldCbtFilter()
		{
			static CREATEWINDOWPARAM cwp = { NULL, NULL, NULL };
			return cwp;
		}

		static LRESULT CALLBACK CbtFilterHook(int code, WPARAM wParam, LPARAM lParam)
		{
			CREATEWINDOWPARAM& cwp = GetHookOldCbtFilter();
			HHOOK	hHookOldCbtFilter = cwp.hHookOldCbtFilter;

			if (code == HCBT_CREATEWND)
			{
				ATOM hAtom = (ATOM)::GetClassLong((HWND)wParam, GCW_ATOM);
				if ((hAtom == cwp.hAtom) || (cwp.hAtom == NULL))
				{
					UnhookWindowCreate();

					//CWnd* pThis = (CWnd*)_AtlWinModule.ExtractCreateWndData();
					CWnd* pThis = cwp.pCWnd ;

					pThis->AfxSubclassWindow((HWND)wParam, ((LPCBT_CREATEWND)lParam)->lpcs);
				}
			}
			return ::CallNextHookEx(hHookOldCbtFilter, code, wParam, lParam);
		}

		static BOOL HookWindowCreate(CWnd* pCWnd, ATOM hAtom = NULL)
		{
			//if (pCWnd != NULL)
			//	_AtlWinModule.AddCreateWndData(&pCWnd->m_thunk.cd, pCWnd);

			CREATEWINDOWPARAM& cwp = GetHookOldCbtFilter();
			cwp.hHookOldCbtFilter = ::SetWindowsHookEx(WH_CBT, CbtFilterHook, NULL, ::GetCurrentThreadId());
			cwp.pCWnd = pCWnd ;
			cwp.hAtom = hAtom;
			return (cwp.hHookOldCbtFilter != NULL);
		}

		static BOOL UnhookWindowCreate()
		{
			CREATEWINDOWPARAM& cwp = GetHookOldCbtFilter();

			if (cwp.hHookOldCbtFilter == NULL)
				return TRUE;

			::UnhookWindowsHookEx(cwp.hHookOldCbtFilter);
			cwp.hHookOldCbtFilter = NULL;
			return TRUE;
		}

		static LRESULT CALLBACK AfxWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) throw()
		{
			CWnd* pThis = (CWnd*)hWnd;

			// set a ptr to this message and save the old value
			_ATL_MSG*& pLastMsg = GetLastSentMsg();
			_ATL_MSG* pOldLastMsg = pLastMsg;

			_ATL_MSG msg;
			msg.hwnd = pThis->m_hWnd;
			msg.message = uMsg;
			msg.wParam = wParam;
			msg.lParam = lParam;

			pLastMsg = &msg;

			LRESULT lResult = pThis->WindowProc(uMsg, wParam, lParam);

			pLastMsg = pOldLastMsg;

			return lResult;
		}

		BOOL AfxSubclassWindow(HWND hWnd, LPCREATESTRUCT lpcs = NULL)
		{
			ASSERT(m_hWnd == NULL);     // only attach once, detach on destroy

			if (hWnd == NULL)
				return FALSE;

			m_thunk.Init(&AfxWindowProc, this);

#ifndef _MTL_ATL3
			WNDPROC pProc = m_thunk.GetWNDPROC();
#else
			WNDPROC pProc = (WNDPROC)&(m_thunk.thunk);
#endif
			WNDPROC wndproc = (WNDPROC)::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)pProc);
			if (wndproc == NULL)
				return FALSE;

			m_pfnSuperWindowProc = wndproc;
			m_hWnd = hWnd;

			FromHandle(hWnd, this, (lpcs != NULL) ? lpcs->hwndParent : NULL);

			return TRUE;
		}

		HWND AfxUnsubclassWindow()
		{
			ASSERT(m_hWnd != NULL);

			HWND hWnd = m_hWnd;

			if (m_pfnSuperWindowProc != ::DefWindowProc)
			{
				if (!::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)m_pfnSuperWindowProc))
					return NULL;

				m_pfnSuperWindowProc = ::DefWindowProc;
			}

			m_hWnd = NULL;

			if (hWnd != NULL)
				RemoveHandle(hWnd);

			return hWnd;
		}

		LRESULT AfxCallWndProc(CWnd* pWnd, UINT nMsg, WPARAM wParam = 0, LPARAM lParam = 0)
		{
			_ATL_MSG*& pMsg = GetLastSentMsg();
			_ATL_MSG* pOldMsg = pMsg;

			_ATL_MSG wndMsg;
			wndMsg.hwnd = pWnd->m_hWnd;
			wndMsg.message = nMsg;
			wndMsg.wParam = wParam;
			wndMsg.lParam = lParam;

			pMsg = &wndMsg;

			LRESULT lResult = pWnd->WindowProc(nMsg, wParam, lParam);

			pMsg = pOldMsg;

			return lResult;
		}


		virtual LRESULT DefWindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam) { return ::CallWindowProc(m_pfnSuperWindowProc, m_hWnd, nMsg, wParam, lParam); }

		virtual LRESULT WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
		{
			// OnWndMsg does most of the work, except for DefWindowProc call

			LRESULT lResult = 0;

			if (!OnWndMsg(nMsg, wParam, lParam, &lResult))
				lResult = DefWindowProc(nMsg, wParam, lParam);

			return lResult;
		}

		virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
		{
			if (message == WM_COMMAND)	// special case for commands
			{
				ASSERT(!IS_MFCNOTIFYCODE(wParam));

				if (OnCommand(wParam, lParam))
				{
					*pResult = 1;
					return TRUE;
				}
				return FALSE;
			}
			else if (message == WM_NOTIFY)	// special case for notifies
			{

				NMHDR* pNMHDR = (NMHDR*)lParam;

				ASSERT(!IS_MFCNOTIFYCODE(wParam) || ::IsWindow(pNMHDR->hwndFrom));

				if (pNMHDR->hwndFrom != NULL && OnNotify(wParam, lParam, pResult))
				{
					return TRUE;
				}
				return FALSE;
			}

			return _ProcessWindowMessage(m_hWnd, message, wParam, lParam, *pResult);
		}


		// Dialog
		static INT_PTR CALLBACK AfxDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		BOOL CreateDlg(LPCTSTR lpszTemplateName, CWnd* pParentWnd);
		BOOL CreateDlgIndirect(LPCDLGTEMPLATE lpDialogTemplate, CWnd* pParentWnd, HINSTANCE hInst);



		virtual BOOL OnChildNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
		{
			return ReflectChildNotify(uMsg, wParam, lParam, pResult);
		}

		/////////////////////////////////////////////////////////////////////////////
		// CWnd command handling

		virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam)
			// return TRUE if command invocation was attempted
		{
			UINT nID = LOWORD(wParam);
			HWND hWndCtrl = (HWND)lParam;
			int nCode = HIWORD(wParam);

			// default routing for command messages (through closure table)

			if (hWndCtrl == NULL)
			{
				// zero IDs for normal commands are not allowed
				if (nID == 0)
					return FALSE;

				// make sure command has not become disabled before routing
				CTestCmdUI state;
				state.m_nID = nID;
				BOOL bRet = OnCmdMsg(nID, CN_UPDATE_COMMAND_UI, &state, NULL);
				if (!state.m_bEnabled)
				{
					TRACE(traceAppMsg, 0, "Warning: not executing disabled command %d\n", nID);
					return TRUE;
				}

				// menu or accelerator
				nCode = CN_COMMAND;
			}
			else
			{
				// control notification
				ASSERT(nID == 0 || ::IsWindow(hWndCtrl));

				if (AfxGetThreadState()->m_hLockoutNotifyWindow == m_hWnd)
					return TRUE;        // locked out - ignore control notification

				// reflect notification to child window control
				if (ReflectLastMsg(hWndCtrl))
					return TRUE;    // eaten by child

				// zero IDs for normal commands are not allowed
				if (nID == 0)
					return FALSE;
			}

			return OnCmdMsg(nID, nCode, NULL, NULL);
		}

		HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
		{
			ASSERT(pWnd != NULL && pWnd->m_hWnd != NULL);
			LRESULT lResult;
			if (pWnd->SendChildNotifyLastMsg(&lResult))
				return (HBRUSH)lResult;     // eat it
			return (HBRUSH)Default();
		}

		void OnDestroy() { Default(); }

		// WM_NCDESTROY is the absolute LAST message sent.
		void OnNcDestroy();

		virtual BOOL OnNotify(WPARAM, LPARAM lParam, LRESULT* pResult)
		{
			ASSERT(pResult != NULL);
			NMHDR* pNMHDR = (NMHDR*)lParam;
			HWND hWndCtrl = pNMHDR->hwndFrom;

			// get the child ID from the window itself
			UINT_PTR nID = _AfxGetDlgCtrlID(hWndCtrl);
			int nCode = pNMHDR->code;

			ASSERT(hWndCtrl != NULL);
			ASSERT(::IsWindow(hWndCtrl));

			if (AfxGetThreadState()->m_hLockoutNotifyWindow == m_hWnd)
				return TRUE;        // locked out - ignore control notification

			// reflect notification to child window control
			if (ReflectLastMsg(hWndCtrl, pResult))
				return TRUE;        // eaten by child

			AFX_NOTIFY notify;
			notify.pResult = pResult;
			notify.pNMHDR = pNMHDR;
			return OnCmdMsg((UINT)nID, MAKELONG(nCode, WM_NOTIFY), &notify, NULL);
		}

		static HWND _AfxTopChildWindowFromPoint(HWND hWnd, POINT pt)
		{
			ASSERT(hWnd != NULL);

			// ask Windows for the child window at the point
			//HWND hWndRet = ::RealChildWindowFromPoint(hWnd, pt);
			HWND hWndRet = ::ChildWindowFromPointEx(hWnd, pt, CWP_SKIPTRANSPARENT | CWP_SKIPDISABLED | CWP_SKIPINVISIBLE);
			if (hWndRet != NULL)
			{
				return ((hWndRet == hWnd) ? NULL : hWndRet);
			}

			// fallback: check child windows, return the topmost child that contains the point
			::ClientToScreen(hWnd, &pt);
			HWND hWndChild = ::GetWindow(hWnd, GW_CHILD);
			for (; hWndChild != NULL; hWndChild = ::GetWindow(hWndChild, GW_HWNDNEXT))
			{
				if (_AfxGetDlgCtrlID(hWndChild) != (WORD)-1 && (::GetWindowLong(hWndChild, GWL_STYLE) & WS_VISIBLE))
				{
					// see if point hits the child window
					CRect rect;
					::GetWindowRect(hWndChild, rect);
					if (rect.PtInRect(pt))
					{
						hWndRet = hWndChild;
					}
				}
			}

			return hWndRet;    // not found
		}

		virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const
		{
			// find child window which hits the point
			// (don't use WindowFromPoint, because it ignores disabled windows)
			HWND hWndChild = _AfxTopChildWindowFromPoint(m_hWnd, point);
			if (hWndChild != NULL)
			{
				// return positive hit if control ID isn't -1
				INT_PTR nHit = _AfxGetDlgCtrlID(hWndChild);

				// hits against child windows always center the tip
				if (pTI != NULL && pTI->cbSize >= sizeof(AFX_OLDTOOLINFO))
				{
					// setup the TOOLINFO structure
					pTI->hwnd = m_hWnd;
					pTI->uId = (UINT_PTR)hWndChild;
					pTI->uFlags |= TTF_IDISHWND;
					pTI->lpszText = LPSTR_TEXTCALLBACK;

					// set TTF_NOTBUTTON and TTF_CENTERTIP if it isn't a button
					if (!(::SendMessage(hWndChild, WM_GETDLGCODE, 0, 0) & DLGC_BUTTON))
						pTI->uFlags |= TTF_NOTBUTTON | TTF_CENTERTIP;
				}
				return nHit;
			}
			return -1;  // not found
		}


 		void OnHScroll(UINT, UINT, CScrollBar* pScrollBar) ;
		void OnVScroll(UINT, UINT, CScrollBar* pScrollBar) ;



		void ActivateTopParent()
		{
			// special activate logic for floating toolbars and palettes
			CWnd* pActiveWnd = GetForegroundWindow();
			if (pActiveWnd == NULL || !(pActiveWnd->m_hWnd == m_hWnd || ::IsChild(pActiveWnd->m_hWnd, m_hWnd)))
			{
				// clicking on floating frame when it does not have
				// focus itself -- activate the toplevel frame instead.
				EnsureTopLevelParent()->SetForegroundWindow();
			}
		}

		void CancelToolTips(BOOL bKeys = FALSE) ;

		void CenterWindow(CWnd* pAlternateOwner = NULL) { CWMHnd::CenterWindow(pAlternateOwner->GetSafeHwnd()); }

		virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, 
							const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL)
		{
			// can't use for desktop or pop-up windows (use CreateEx instead)
			ASSERT(pParentWnd != NULL);
			ASSERT((dwStyle & WS_POPUP) == 0);

			if (((dwStyle & WS_TABSTOP) == WS_TABSTOP) && (nID == 0))
			{
				// Warn about nID == 0.  A zero ID will be overridden in CWnd::PreCreateWindow when the
				// check is done for (cs.hMenu == NULL).  This will cause the dialog control ID to be
				// different than passed in, so ::GetDlgItem(nID) will not return the control HWND.
				TRACE(traceAppMsg, 0, "Warning: creating a dialog control with nID == 0; "
										"nID will overridden in CWnd::PreCreateWindow and GetDlgItem with nID == 0 will fail.\n");
			}

			return CreateEx(0, lpszClassName, lpszWindowName, dwStyle | WS_CHILD,
							rect.left, rect.top,rect.right - rect.left, rect.bottom - rect.top,
							pParentWnd->GetSafeHwnd(), (HMENU)(UINT_PTR)nID, (LPVOID)pContext);
		}

		// advanced creation (allows access to extended styles)
		virtual BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, 
								int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU nIDorHMenu, LPVOID lpParam = NULL)
		{
			// allow modification of several common create parameters
			CREATESTRUCT cs;
			cs.dwExStyle = dwExStyle;
			cs.lpszClass = lpszClassName;
			cs.lpszName = lpszWindowName;
			cs.style = dwStyle;
			cs.x = x;
			cs.y = y;
			cs.cx = nWidth;
			cs.cy = nHeight;
			cs.hwndParent = hWndParent;
			cs.hMenu = nIDorHMenu;
			cs.hInstance = AfxGetInstanceHandle();
			cs.lpCreateParams = lpParam;

			if (!PreCreateWindow(cs))
			{
				PostNcDestroy();
				return FALSE;
			}

			ATOM hAtom = NULL ;

			if (cs.lpszClass != NULL)
			{
				WNDCLASSEX wndclsex;
				wndclsex.cbSize = sizeof(WNDCLASSEX) ;
				hAtom = GetClassInfoEx(cs.hInstance, cs.lpszClass, &wndclsex) ;
			}

			HookWindowCreate(this, hAtom) ;

			HWND hWnd = CreateWindowEx(cs.dwExStyle, MAKEINTATOM(hAtom), cs.lpszName, cs.style,
										cs.x, cs.y, cs.cx, cs.cy,
										cs.hwndParent, cs.hMenu, cs.hInstance, cs.lpCreateParams);

			UnhookWindowCreate();

			// CbtFilterHook
			//m_hWnd = hWnd;

			return (hWnd != NULL);
		}

		virtual BOOL DestroyWindow()
		{
			if (m_hWnd == NULL)
				return FALSE;

			BOOL bResult = CWMHnd::DestroyWindow();

			// LRESULT CALLBACK CWindowImplBaseT< TBase, TWinTraits >::WindowProc(
			// 	if((pThis->m_dwState & WINSTATE_DESTROYED) && pOldMsg== NULL)
			// pThis->m_hWnd = NULL;

			return bResult;
		}

		// ToolTip Functions
		BOOL _EnableToolTips(BOOL bEnable, UINT nFlag) ;

		BOOL EnableToolTips(BOOL bEnable = TRUE) { return _EnableToolTips(bEnable, WF_TOOLTIPS); }

		CFrameWnd* EnsureParentFrame()
		{
			CFrameWnd *pFrameWnd = GetParentFrame();
			ENSURE_VALID(pFrameWnd);
			return pFrameWnd;
		}

		CWnd* EnsureTopLevelParent()
		{
			CWnd *pWnd = GetTopLevelParent();
			ENSURE_VALID(pWnd);
			return pWnd;
		}

		/////////////////////////////////////////////////////////////////////////////
		// Dialog initialization support

		BOOL ExecuteDlgInit(LPCTSTR lpszResourceName)
		{
			// find resource handle
			LPVOID lpResource = NULL;
			HGLOBAL hResource = NULL;
			if (lpszResourceName != NULL)
			{
				HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_DLGINIT);
				HRSRC hDlgInit = ::FindResource(hInst, lpszResourceName, RT_DLGINIT);
				if (hDlgInit != NULL)
				{
					// load it
					hResource = LoadResource(hInst, hDlgInit);
					if (hResource == NULL)
						return FALSE;
					// lock it
					lpResource = LockResource(hResource);
					ASSERT(lpResource != NULL);
				}
			}

			// execute it
			BOOL bResult = ExecuteDlgInit(lpResource);

			// cleanup
			if (lpResource != NULL && hResource != NULL)
			{
				UnlockResource(hResource);
				FreeResource(hResource);
			}
			return bResult;
		}

		BOOL ExecuteDlgInit(LPVOID lpResource)
		{
			BOOL bSuccess = TRUE;
			if (lpResource != NULL)
			{
				UNALIGNED WORD* lpnRes = (WORD*)lpResource;
				while (bSuccess && *lpnRes != 0)
				{
					WORD nIDC = *lpnRes++;
					WORD nMsg = *lpnRes++;
					DWORD dwLen = *((UNALIGNED DWORD*&)lpnRes)++;

					// In Win32 the WM_ messages have changed.  They have
					// to be translated from the 16-bit values to 32-bit
					// values here.

#define WIN16_LB_ADDSTRING  0x0401
#define WIN16_CB_ADDSTRING  0x0403
					if (nMsg == WIN16_LB_ADDSTRING)
						nMsg = LB_ADDSTRING;
					else if (nMsg == WIN16_CB_ADDSTRING)
						nMsg = CB_ADDSTRING;

					// check for invalid/unknown message types
					ASSERT(nMsg == LB_ADDSTRING || nMsg == CB_ADDSTRING);

					if (nMsg == LB_ADDSTRING || nMsg == CB_ADDSTRING)
					{
						// List/Combobox returns -1 for error
						if (::SendDlgItemMessageA(m_hWnd, nIDC, nMsg, 0, (LPARAM)lpnRes) == -1)
							bSuccess = FALSE;
					}

					// skip past data
					lpnRes = (WORD*)((LPBYTE)lpnRes + (UINT)dwLen);
				}
			}

			// send update message to all controls after all other siblings loaded
			if (bSuccess)
				SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, FALSE, FALSE);

			return bSuccess;
		}

		static void _FilterToolTipMessage(MSG* pMsg, CWnd* pWnd)
		{
			pWnd->FilterToolTipMessage(pMsg);
		}

		static void _AfxRelayToolTipMessage(CToolTipCtrl* pToolTip, MSG* pMsg) ;

		void FilterToolTipMessage(MSG* pMsg) ;

		CWnd* GetDescendantWindow(HWND hWnd, int nID, BOOL bOnlyPerm = FALSE)
		{
			// GetDlgItem recursive (return first found)
			// breadth-first for 1 level, then depth-first for next level

			// use GetDlgItem since it is a fast USER function
			HWND hWndChild;
			CWnd* pWndChild;
			if ((hWndChild = ::GetDlgItem(hWnd, nID)) != NULL)
			{
				if (::GetTopWindow(hWndChild) != NULL)
				{
					// children with the same ID as their parent have priority
					pWndChild = GetDescendantWindow(hWndChild, nID, bOnlyPerm);
					if (pWndChild != NULL)
						return pWndChild;
				}
				// return temporary handle if allowed
				if (!bOnlyPerm)
					return FromHandle(hWndChild);

				// return only permanent handle
				pWndChild = FromHandlePermanent(hWndChild);
				if (pWndChild != NULL)
					return pWndChild;
			}

			// walk each child
			for (hWndChild = ::GetTopWindow(hWnd); hWndChild != NULL; hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
			{
				pWndChild = GetDescendantWindow(hWndChild, nID, bOnlyPerm);
				if (pWndChild != NULL)
					return pWndChild;
			}
			return NULL;    // not found
		}

		int GetDlgItemText(int nID, CString& rString) const
		{
			ASSERT(::IsWindow(m_hWnd));
			rString = _T("");    // empty without deallocating

			HWND hWnd = ::GetDlgItem(m_hWnd, nID);
			if (hWnd != NULL)
			{
				int nLen = ::GetWindowTextLength(hWnd);
				::GetWindowText(hWnd, rString.GetBufferSetLength(nLen), nLen + 1);
				rString.ReleaseBuffer();
			}

			return (int)rString.GetLength();
		}

		CFrameWnd* GetParentFrame()
		{
			if (GetSafeHwnd() == NULL) // no Window attached
			{
				return NULL;
			}

			ASSERT_VALID(this);

			CWnd* pParentWnd = GetParent();  // start with one parent up
			while (pParentWnd != NULL)
			{
				if (pParentWnd->IsFrameWnd())
				{
					return (CFrameWnd*)pParentWnd;
				}
				pParentWnd = pParentWnd->GetParent();
			}
			return NULL;
		}

		CWnd* GetParentOwner()
		{
			if (GetSafeHwnd() == NULL) // no Window attached
				return NULL;

			ASSERT_VALID(this);

			HWND hWndParent = m_hWnd;
			HWND hWndT;
			while ((::GetWindowLong(hWndParent, GWL_STYLE) & WS_CHILD) && (hWndT = ::GetParent(hWndParent)) != NULL)
			{
				hWndParent = hWndT;
			}

			return CWnd::FromHandle(hWndParent);
		}


		static CWnd* GetSafeOwner(CWnd* pParent = NULL, HWND* pWndTop = NULL)
		{
			HWND hWnd = GetSafeOwner_(pParent->GetSafeHwnd(), pWndTop);
			return FromHandle(hWnd);
		}

		static HWND GetSafeOwner_(HWND hParent, HWND* pWndTop)
		{
			// get window to start with
			HWND hWnd = hParent;
			if (hWnd == NULL)
			{
				hWnd = AfxGetMainWnd()->GetSafeHwnd();
			}

			// a popup window cannot be owned by a child window
			while (hWnd != NULL && (::GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD))
				hWnd = ::GetParent(hWnd);

			// determine toplevel window to disable as well
			HWND hWndTop = hWnd, hWndTemp = hWnd;
			for (;;)
			{
				if (hWndTemp == NULL)
					break;
				else
					hWndTop = hWndTemp;
				hWndTemp = ::GetParent(hWndTop);
			}

			// get last active popup of first non-child that was found
			if (hParent == NULL && hWnd != NULL)
				hWnd = ::GetLastActivePopup(hWnd);

			// disable and store top level parent window if specified
			if (pWndTop != NULL)
			{
				if (hWndTop != NULL && ::IsWindowEnabled(hWndTop) && hWndTop != hWnd)
				{
					*pWndTop = hWndTop;
					::EnableWindow(hWndTop, FALSE);
				}
				else
					*pWndTop = NULL;
			}

			return hWnd;    // return the owner as HWND
		}

		CFrameWnd* GetTopLevelFrame();

		static HWND AfxGetParentOwner(HWND hWnd)
		{
			// check for permanent-owned window first
			CWnd* pWnd = FromHandlePermanent(hWnd);
			if (pWnd != NULL)
				return pWnd->GetOwner()->GetSafeHwnd();

			// otherwise, return parent in the Windows sense
			return (::GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD) ? ::GetParent(hWnd) : ::GetWindow(hWnd, GW_OWNER);
		}

		CWnd* GetTopLevelParent()
		{
			if (GetSafeHwnd() == NULL) // no Window attached
				return NULL;

			ASSERT_VALID(this);

			HWND hWndParent = m_hWnd;
			HWND hWndT;
			while ((hWndT = AfxGetParentOwner(hWndParent)) != NULL)
				hWndParent = hWndT;

			return (CWnd*)FromHandle(hWndParent);
		}

		/////////////////////////////////////////////////////////////////////////////
		// Special keyboard/system command processing

		BOOL HandleFloatingSysCommand(UINT nID, LPARAM lParam)
		{
			CWnd* pParent = GetTopLevelParent();
			switch (nID & 0xfff0)
			{
				case SC_PREVWINDOW:
				case SC_NEXTWINDOW:
					if (LOWORD(lParam) == VK_F6 && pParent != NULL)
					{
						pParent->SetFocus();
						return TRUE;
					}
					break;

				case SC_CLOSE:
				case SC_KEYMENU:
					// Check lParam.  If it is 0L, then the user may have done
					// an Alt+Tab, so just ignore it.  This breaks the ability to
					// just press the Alt-key and have the first menu selected,
					// but this is minor compared to what happens in the Alt+Tab
					// case.
					if ((nID & 0xfff0) == SC_CLOSE || lParam != 0L)
					{
						if (pParent != NULL)
						{
							// Sending the above WM_SYSCOMMAND may destroy the app,
							// so we have to be careful about restoring activation
							// and focus after sending it.
							HWND hWndSave = m_hWnd;
							HWND hWndFocus = ::GetFocus();
							pParent->SetActiveWindow();
							pParent->SendMessage(WM_SYSCOMMAND, nID, lParam);

							// be very careful here...
							if (::IsWindow(hWndSave))
								::SetActiveWindow(hWndSave);
							if (::IsWindow(hWndFocus))
								::SetFocus(hWndFocus);
						}
					}
					return TRUE;
			}

			return FALSE;
		}

		BOOL IsTopParentActive()
		{
			ASSERT(m_hWnd != NULL);
			ASSERT_VALID(this);

			CWnd *pWndTopLevel = EnsureTopLevelParent();

			return CWnd::GetForegroundWindow() == pWndTopLevel->GetLastActivePopup();
		}

		virtual BOOL PreCreateWindow(CREATESTRUCT& cs)
		{
			if (cs.lpszClass == NULL)
			{
				// make sure the default window class is registered
				VERIFY(AfxDeferRegisterClass(AFX_WND_REG));

				// no WNDCLASS provided - use child window default
				ASSERT(cs.style & WS_CHILD);
				cs.lpszClass = _afxWnd;
			}

			if ((cs.hMenu == NULL) && (cs.style & WS_CHILD))
			{
				cs.hMenu = (HMENU)(UINT_PTR)this;
			}

			return TRUE;
		}

		BOOL PreTranslateInput(LPMSG lpMsg)
		{
			ASSERT(::IsWindow(m_hWnd));

			// don't translate non-input events
			if ((lpMsg->message < WM_KEYFIRST || lpMsg->message > WM_KEYLAST) &&
				(lpMsg->message < WM_MOUSEFIRST || lpMsg->message > AFX_WM_MOUSELAST))
				return FALSE;

			return IsDialogMessage(lpMsg);
		}

		virtual BOOL PreTranslateMessage(MSG* pMsg) ;

		BOOL ReflectChildNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
		{
			// Note: reflected messages are send directly to CWnd::OnWndMsg
			//  and CWnd::OnCmdMsg for speed and because these messages are not
			//  routed by normal OnCmdMsg routing (they are only dispatched)

			switch (uMsg)
			{
					// normal messages (just wParam, lParam through OnWndMsg)
				case WM_HSCROLL:
				case WM_VSCROLL:
				case WM_PARENTNOTIFY:
				case WM_DRAWITEM:
				case WM_MEASUREITEM:
				case WM_DELETEITEM:
				case WM_VKEYTOITEM:
				case WM_CHARTOITEM:
				case WM_COMPAREITEM:
					// reflect the message through the message map as WM_REFLECT_BASE+uMsg
					return OnWndMsg(WM_REFLECT_BASE + uMsg, wParam, lParam, pResult);

					// special case for WM_COMMAND
				case WM_COMMAND:
				{
					// reflect the message through the message map as OCM_COMMAND
					int nCode = HIWORD(wParam);
					if (OnCmdMsg(0, MAKELONG(nCode, WM_REFLECT_BASE + WM_COMMAND), NULL, NULL))
					{
						if (pResult != NULL)
							*pResult = 1;
						return TRUE;
					}
				}
				break;

				// special case for WM_NOTIFY
				case WM_NOTIFY:
				{
					// reflect the message through the message map as OCM_NOTIFY
					NMHDR* pNMHDR = (NMHDR*)lParam;
					int nCode = pNMHDR->code;
					AFX_NOTIFY notify;
					notify.pResult = pResult;
					notify.pNMHDR = pNMHDR;
					return CWnd::OnCmdMsg(0, MAKELONG(nCode, WM_REFLECT_BASE + WM_NOTIFY), &notify, NULL);
				}

				// other special cases (WM_CTLCOLOR family)
				default:
					if (uMsg >= WM_CTLCOLORMSGBOX && uMsg <= WM_CTLCOLORSTATIC)
					{
						// fill in special struct for compatiblity with 16-bit WM_CTLCOLOR
						AFX_CTLCOLOR ctl;
						ctl.hDC = (HDC)wParam;
						ctl.nCtlType = uMsg - WM_CTLCOLORMSGBOX;
						//ASSERT(ctl.nCtlType >= CTLCOLOR_MSGBOX);
						ASSERT(ctl.nCtlType <= CTLCOLOR_STATIC);

						// reflect the message through the message map as OCM_CTLCOLOR
						BOOL bResult = OnWndMsg(WM_REFLECT_BASE + WM_CTLCOLOR, 0, (LPARAM)&ctl, pResult);
						if ((HBRUSH)*pResult == NULL)
							bResult = FALSE;
						return bResult;
					}
					break;
			}

			return FALSE;   // let the parent handle it
		}

		static BOOL ReflectLastMsg(HWND hWndChild, LRESULT* pResult = NULL)
		{
			// check if in permanent map, if it is reflect it (could be OLE control)
			CWnd* pWnd = FromHandlePermanent(hWndChild);
			ASSERT(pWnd == NULL || pWnd->m_hWnd == hWndChild);
			if (pWnd == NULL)
				return FALSE ;

			// only OLE controls and permanent windows will get reflected msgs
			ASSERT(pWnd != NULL);
			return pWnd->SendChildNotifyLastMsg(pResult);
		}

		void RepositionBars(UINT nIDFirst, UINT nIDLast, UINT nIDLeftOver, UINT nFlags = reposDefault, LPRECT lpRectParam = NULL, LPCRECT lpRectClient = NULL, BOOL bStretch = TRUE)
		{
			ASSERT(nFlags == 0 || (nFlags & ~reposNoPosLeftOver) == reposQuery ||
				(nFlags & ~reposNoPosLeftOver) == reposExtra);

			// walk kids in order, control bars get the resize notification
			//   which allow them to shrink the client area
			// remaining size goes to the 'nIDLeftOver' pane
			// NOTE: nIDFirst->nIDLast are usually 0->0xffff

			AFX_SIZEPARENTPARAMS layout;
			HWND hWndLeftOver = NULL;

			layout.bStretch = bStretch;
			layout.sizeTotal.cx = layout.sizeTotal.cy = 0;
			if (lpRectClient != NULL)
				layout.rect = *lpRectClient;    // starting rect comes from parameter
			else
				GetClientRect(&layout.rect);    // starting rect comes from client rect

			if ((nFlags & ~reposNoPosLeftOver) != reposQuery)
				layout.hDWP = ::BeginDeferWindowPos(8); // reasonable guess
			else
				layout.hDWP = NULL; // not actually doing layout

			for (HWND hWndChild = ::GetTopWindow(m_hWnd); hWndChild != NULL ; hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
			{
				UINT_PTR nIDC = _AfxGetDlgCtrlID(hWndChild);
				CWnd* pWnd = FromHandlePermanent(hWndChild);
				if (nIDC == nIDLeftOver)
					hWndLeftOver = hWndChild;
				else if (nIDC >= nIDFirst && nIDC <= nIDLast && pWnd != NULL)
					::SendMessage(hWndChild, WM_SIZEPARENT, 0, (LPARAM)&layout);
			}

			// if just getting the available rectangle, return it now...
			if ((nFlags & ~reposNoPosLeftOver) == reposQuery)
			{
				ASSERT(lpRectParam != NULL);
				if (bStretch)
					::CopyRect(lpRectParam, &layout.rect);
				else
				{
					lpRectParam->left = lpRectParam->top = 0;
					lpRectParam->right = layout.sizeTotal.cx;
					lpRectParam->bottom = layout.sizeTotal.cy;
				}
				return;
			}

			// the rest is the client size of the left-over pane
			if (nIDLeftOver != 0 && hWndLeftOver != NULL)
			{
				// allow extra space as specified by lpRectBorder
				if ((nFlags & ~reposNoPosLeftOver) == reposExtra)
				{
					ASSERT(lpRectParam != NULL);
					layout.rect.left += lpRectParam->left;
					layout.rect.top += lpRectParam->top;
					layout.rect.right -= lpRectParam->right;
					layout.rect.bottom -= lpRectParam->bottom;
				}
				// reposition the window
				if ((nFlags & reposNoPosLeftOver) != reposNoPosLeftOver)
				{
					CWnd* pLeftOver = (CWnd*)FromHandle(hWndLeftOver);
					pLeftOver->CalcWindowRect(&layout.rect);
					AfxRepositionWindow(&layout, hWndLeftOver, &layout.rect);
				}
			}

			// move and resize all the windows at once!
			if (layout.hDWP == NULL || !::EndDeferWindowPos(layout.hDWP))
				TRACE(traceAppMsg, 0, "Warning: DeferWindowPos failed - low system resources.\n");
		}

		int RunModalLoop(DWORD dwFlags = 0) ;

		virtual BOOL ContinueModal()
		{
			return m_nFlags & WF_CONTINUEMODAL;
		}

		virtual void EndModalLoop(int nResult)
		{
			ASSERT(::IsWindow(m_hWnd));

			// this result will be returned from CWnd::RunModalLoop
			m_nModalResult = nResult;

			// make sure a message goes through to exit the modal loop
			if (m_nFlags & WF_CONTINUEMODAL)
			{
				m_nFlags &= ~WF_CONTINUEMODAL;
				PostMessage(WM_NULL);
			}
		}

		BOOL SendChildNotifyLastMsg(LRESULT* pResult = NULL)
		{
			const _ATL_MSG* pMsg = GetCurrentMessage();
			return OnChildNotify(pMsg->message, pMsg->wParam, pMsg->lParam, pResult);
		}

		void SendMessageToDescendants(HWND hWnd, UINT message, WPARAM wParam = 0, LPARAM lParam = 0, BOOL bDeep = TRUE, BOOL bOnlyPerm = FALSE)
		{
			// walk through HWNDs to avoid creating temporary CWnd objects
			// unless we need to call this function recursively
			for (HWND hWndChild = ::GetTopWindow(hWnd); hWndChild != NULL; hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
			{
				// if bOnlyPerm is TRUE, don't send to non-permanent windows
				if (bOnlyPerm)
				{
					CWnd* pWnd = FromHandlePermanent(hWndChild);
					if (pWnd != NULL)
					{
						// call window proc directly since it is a C++ window
						AfxCallWndProc(pWnd, message, wParam, lParam);
					}
				}
				else
				{
					// send message with Windows SendMessage API
					::SendMessage(hWndChild, message, wParam, lParam);
				}
				if (bDeep && ::GetTopWindow(hWndChild) != NULL)
				{
					// send to child windows after parent
					SendMessageToDescendants(hWndChild, message, wParam, lParam, bDeep, bOnlyPerm);
				}
			}
		}

		void SendMessageToDescendants(UINT message, WPARAM wParam = 0, LPARAM lParam = 0, BOOL bDeep = TRUE, BOOL bOnlyPerm = FALSE)  
		{ ASSERT(::IsWindow(m_hWnd)); SendMessageToDescendants(m_hWnd, message, wParam, lParam, bDeep, bOnlyPerm); }

		void SetOwner(CWMHnd* pOwnerWnd) { m_hWndOwner = (pOwnerWnd != NULL) ? (pOwnerWnd)->m_hWnd : NULL; }


		/////////////////////////////////////////////////////////////////////////////
		// Extra CWnd support for dynamic subclassing of controls

		BOOL SubclassWindow(HWND hWnd)
		{
			return AfxSubclassWindow(hWnd) ;
		}

		BOOL SubclassDlgItem(UINT nID, CWnd* pParent)
		{
			ASSERT(pParent != NULL);
			ASSERT(::IsWindow(pParent->m_hWnd));

			// check for normal dialog control first
			HWND hWndControl = ::GetDlgItem(pParent->m_hWnd, nID);
			if (hWndControl != NULL)
				return SubclassWindow(hWndControl);

			return FALSE;   // control not found
		}

		HWND UnsubclassWindow()
		{
			return AfxUnsubclassWindow() ;
		}


		void UpdateDialogControls(CCmdTarget* pTarget, BOOL bDisableIfNoHndler)
		{
			CCmdUI state;
			CWnd wndTemp;       // very temporary window just for CmdUI update

			// walk all the kids - assume the IDs are for buttons
			for (HWND hWndChild = ::GetTopWindow(m_hWnd); hWndChild != NULL; hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
			{
				// send to buttons
				wndTemp.m_hWnd = hWndChild; // quick and dirty attach
				state.m_nID = _AfxGetDlgCtrlID(hWndChild);
				state.m_pOther = &wndTemp;

				// check for reflect handlers in the child window
				CWnd* pWnd = FromHandlePermanent(hWndChild);
				if (pWnd != NULL)
				{
					// call it directly to disable any routing
					if (pWnd->CWnd::OnCmdMsg(0, MAKELONG(0xffff, WM_COMMAND + WM_REFLECT_BASE), &state, NULL))
						continue;
				}

				// check for handlers in the parent window
				if (CWnd::OnCmdMsg((UINT)state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
					continue;

				// determine whether to disable when no handler exists
				BOOL bDisableTemp = bDisableIfNoHndler;
				if (bDisableTemp)
				{
					if ((wndTemp.SendMessage(WM_GETDLGCODE) & DLGC_BUTTON) == 0)
					{
						// non-button controls don't get automagically disabled
						bDisableTemp = FALSE;
					}
					else
					{
						// only certain button controls get automagically disabled
						UINT nStyle = (UINT)(wndTemp.GetStyle() & 0x0F);
						if (nStyle == (UINT)BS_AUTOCHECKBOX ||
							nStyle == (UINT)BS_AUTO3STATE ||
							nStyle == (UINT)BS_GROUPBOX ||
							nStyle == (UINT)BS_AUTORADIOBUTTON)
						{
							bDisableTemp = FALSE;
						}
					}
				}
				// check for handlers in the target (owner)
				state.DoUpdate(pTarget, bDisableTemp);
			}
			wndTemp.m_hWnd = NULL;      // quick and dirty detach
		}

		static BOOL WalkPreTranslateTree(HWND hWndStop, MSG* pMsg)
		{
			// walk from the target window up to the hWndStop window checking
			//  if any window wants to translate this message

			for (HWND hWnd = pMsg->hwnd; hWnd != NULL; hWnd = ::GetParent(hWnd))
			{
				CWnd* pCWnd = FromHandlePermanent(hWnd);
				if (pCWnd != NULL)
				{
					// target window is a C++ window
					if (pCWnd->PreTranslateMessage(pMsg))
						return TRUE; // trapped by target window (eg: accelerators)
				}

				// got to hWndStop window without interest
				if (hWnd == hWndStop)
					break;
			}

			return FALSE;       // no special processing
		}





		CWnd* ChildWindowFromPoint(POINT point) { ASSERT(::IsWindow(m_hWnd)); return FromHandle(::ChildWindowFromPoint(m_hWnd, point)); }
		CWnd* ChildWindowFromPoint(POINT point, UINT nFlags) { ASSERT(::IsWindow(m_hWnd)); return FromHandle(::ChildWindowFromPointEx(m_hWnd, point, nFlags)); }
		CWnd* FindWindow(LPCTSTR lpszClassName, LPCTSTR lpszWindowName) { return FromHandle(::FindWindow(lpszClassName, lpszWindowName)); }
		CWnd* FindWindowEx(HWND hwndParent, HWND hwndChildAfter, LPCTSTR lpszClass, LPCTSTR lpszWindow) { return FromHandle(::FindWindowEx(hwndParent, hwndChildAfter, lpszClass, lpszWindow)); }
		static CWnd* GetActiveWindow() { return FromHandle(::GetActiveWindow()); }
		static CWnd* GetCapture() { return FromHandle(::GetCapture()); }
		static CWnd* GetClipboardOwner() { return FromHandle(::GetClipboardOwner()); }
		static CWnd* GetClipboardViewer() { return FromHandle(::GetClipboardViewer()); }
		CWnd* GetDescendantWindow(int nID, BOOL bOnlyPerm) { ASSERT(::IsWindow(m_hWnd)); return GetDescendantWindow(m_hWnd, nID, bOnlyPerm); }
		static CWnd* GetDesktopWindow() { return FromHandle(::GetDesktopWindow()); }
		CWnd* GetDlgItem(int nID) { ASSERT(::IsWindow(m_hWnd)); return FromHandle(::GetDlgItem(m_hWnd, nID)); }
		static CWnd* GetFocus() { return FromHandle(::GetFocus()); }
		static CWnd* GetForegroundWindow() { return FromHandle(::GetForegroundWindow()); }
		CWnd* GetLastActivePopup() { ASSERT(::IsWindow(m_hWnd)); return FromHandle(::GetLastActivePopup(m_hWnd)); }
		CWnd* GetNextDlgGroupItem(CWnd* pWndCtl, BOOL bPrevious)  { ASSERT(::IsWindow(m_hWnd)); return FromHandle(::GetNextDlgGroupItem(m_hWnd, pWndCtl->GetSafeHwnd(), bPrevious)); }
		CWnd* GetNextDlgTabItem(CWnd* pWndCtl, BOOL bPrevious) { ASSERT(::IsWindow(m_hWnd)); return FromHandle(::GetNextDlgTabItem(m_hWnd, pWndCtl->GetSafeHwnd(), bPrevious)); }
		static CWnd* GetOpenClipboardWindow() { return FromHandle(::GetOpenClipboardWindow()); }
		CWnd* GetOwner() { return (m_hWndOwner != NULL) ? FromHandle(m_hWndOwner) : GetParent(); }
		CWnd* GetParent() { ASSERT(::IsWindow(m_hWnd)); return FromHandle(::GetParent(m_hWnd)); }
		CWnd* GetTopWindow() { ASSERT(::IsWindow(m_hWnd)); return FromHandle(::GetTopWindow(m_hWnd)); }
		CWnd* GetWindow(UINT nCmd) { ASSERT(::IsWindow(m_hWnd)); return FromHandle(::GetWindow(m_hWnd, nCmd)); }
		CWnd* SetActiveWindow() { ASSERT(::IsWindow(m_hWnd)); return FromHandle(::SetActiveWindow(m_hWnd)); }
		CWnd* SetCapture() { ASSERT(::IsWindow(m_hWnd)); return FromHandle(::SetCapture(m_hWnd)); }
		CWnd* SetParent(CWnd* pWndNewParent)  { ASSERT(::IsWindow(m_hWnd)); return FromHandle(::SetParent(m_hWnd, pWndNewParent->GetSafeHwnd())); }
		static CWnd* WindowFromPoint(POINT point) { return FromHandle(::WindowFromPoint(point)); }







};

BEGIN_MESSAGE_MAP(CWnd, CWMHnd)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_ENTERIDLE()
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()

