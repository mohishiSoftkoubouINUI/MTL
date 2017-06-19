






_INLINE BOOL CObject::IsKindOf(const CRuntimeClass* pClass) const
{
	ENSURE(this != NULL);

	// simple SI case
	CRuntimeClass* pClassThis = GetRuntimeClass();

	ENSURE(pClassThis);
	return pClassThis->IsDerivedFrom(pClass);
}







/*============================================================================*/
// Implementation of command routing

struct AFX_CMDHANDLERINFO
{
	CCmdTarget* pTarget;
	//void (CCmdTarget::*pmf)(void);
};


#define CMDHAS_HANDLER_V	(0x8000)
#define CMDHAS_HANDLER(c) ((c) | CMDHAS_HANDLER_V)
#define CMDHAS_HANDLER_WPARAM_MASK(w) ((w) & ~MAKEWPARAM(0, CMDHAS_HANDLER_V))
#define IS_CMDHAS_HANDLER_WPARAM(w) ((w) & MAKEWPARAM(0, CMDHAS_HANDLER_V))


class CCmdTarget : public CObject
{
	DECLARE_DYNAMIC(CCmdTarget)

	public:

	public:

	public:
		virtual BOOL _ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
		{
			return FALSE;
		}

		static _ATL_MSG*& GetLastSentMsg()
		{
			static _ATL_MSG* m_lastSentMsg;

			return m_lastSentMsg;
		}

		virtual const _ATL_MSG* GetCurrentMessage() const { return GetLastSentMsg(); }


		virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
		{
			UINT nMsg = 0;
			if (nCode != CN_UPDATE_COMMAND_UI)
			{
				nMsg = HIWORD(nCode);
				nCode = LOWORD(nCode);
			}

			// for backward compatibility HIWORD(nCode)==0 is WM_COMMAND
			if (nMsg == 0)
				nMsg = WM_COMMAND;

			if ((nCode == CN_COMMAND) && (pHandlerInfo != NULL))
			{
				pHandlerInfo->pTarget = this ;
				nCode = CMDHAS_HANDLER(nCode);
			}

			LRESULT lResult;

			_ATL_MSG*& pMsg = GetLastSentMsg();

			// NMHDR->code, IS_MFCNOTIFYCODE(nCode), IS_CMDHAS_HANDLER_WPARAM(ncode)
			if (nCode != CN_COMMAND)
			{
				struct MTL_ONCMDMSG onParam = { pExtra, pHandlerInfo };
				return _ProcessWindowMessage((pMsg != NULL) ? pMsg->hwnd : NULL, nMsg, MAKEWPARAM(nID, nCode), (LPARAM)&onParam, lResult);
			}

			ASSERT(pHandlerInfo == NULL) ;

			_ATL_MSG* pOldMsg = pMsg;

			_ATL_MSG cmdMsg ;
			cmdMsg.hwnd = (pOldMsg != NULL) ? pOldMsg->hwnd : NULL ;
			cmdMsg.message = nMsg ;
			cmdMsg.wParam = MAKEWPARAM(nID, nCode) ;
			cmdMsg.lParam = (LPARAM)pExtra ;
			pMsg = &cmdMsg;

			BOOL bRet = _ProcessWindowMessage(cmdMsg.hwnd, nMsg, cmdMsg.wParam, cmdMsg.lParam, lResult);

			pMsg = pOldMsg;

			return bRet;

		}

		void BeginWaitCursor();
		void EndWaitCursor();
		void RestoreWaitCursor();

		/////////////////////////////////////////////////////////////////////////////
		// Locate event sink map entry

#if 0
		PTM_WARNING_DISABLE
		static const AFX_EVENTSINKMAP* _GetEventSinkMap()
		{
			static const AFX_EVENTSINKMAP_ENTRY _eventsinkEntries[] =
			{
				{ NULL, -1, NULL, 0,
				(AFX_PMSG)NULL, (AFX_PMSG)NULL, (size_t)-1, afxDispCustom, (UINT)-1, (UINT)-1 }
				// nothing here
			};

			static UINT _eventsinkEntryCount = (UINT)0;

			static const AFX_EVENTSINKMAP eventsinkMap =
			{
				NULL,
				&_eventsinkEntries[0],
				&_eventsinkEntryCount
			};

			return &eventsinkMap;
		}
		PTM_WARNING_RESTORE
#endif
		static const AFX_EVENTSINKMAP* _GetEventSinkMap() { return NULL ; }

		virtual const AFX_EVENTSINKMAP* GetEventSinkMap() const { return _GetEventSinkMap() ; }

		const AFX_EVENTSINKMAP_ENTRY* GetEventSinkEntry(UINT idCtrl, AFX_EVENT* pEvent)
		{
			ENSURE_ARG(pEvent != NULL);

			size_t flag = (pEvent->m_eventKind != AFX_EVENT::event);

			const AFX_EVENTSINKMAP* pSinkMap = GetEventSinkMap();

			while (pSinkMap != NULL)
			{
				// find matching AFX_EVENTSINKMAP_ENTRY
				const AFX_EVENTSINKMAP_ENTRY* pEntry = pSinkMap->lpEntries;

				while (pEntry->dispEntry.nPropOffset != -1)
				{
					if ((pEntry->dispEntry.lDispID == pEvent->m_dispid) &&
						(pEntry->dispEntry.nPropOffset == flag))
					{
						if (pEntry->nCtrlIDLast == (UINT)-1)
						{
							// check for wildcard match or exact match
							if ((pEntry->nCtrlIDFirst == (UINT)-1) ||
								(pEntry->nCtrlIDFirst == idCtrl))
								return pEntry;
						}
						else
						{
							// check for range match
							if ((pEntry->nCtrlIDFirst <= idCtrl) &&
								(idCtrl <= pEntry->nCtrlIDLast))
								return pEntry;
						}
					}

					++pEntry;
				}
				// check base class
				pSinkMap = pSinkMap->pBaseMap;
			}

			return NULL;    // no matching entry
		}

};





class CWinMsg
{
	public:
		MSG m_msgCur;                   // current message
};


class CDataExchange
{
	public:
		CDataExchange(CWMHnd* pDlgWnd, BOOL bSaveAndValidate)
		{
			m_bSaveAndValidate = bSaveAndValidate;
			m_pDlgWnd = pDlgWnd;
			m_idLastControl = 0;
		}

		// Attributes
	public:
		BOOL m_bSaveAndValidate;   // TRUE => save and validate data
		CWMHnd* m_pDlgWnd;           // container usually a dialog

	public:
		// Implementation
		UINT m_idLastControl;      // last control used (for validation)
		BOOL m_bEditLastControl;   // last control was an edit item


		// Operations (for implementors of DDX and DDV procs)
		HWND PrepareCtrl(int nIDC);

		HWND PrepareEditCtrl(int nIDC)
		{
			HWND hWndCtrl = PrepareCtrl(nIDC);
			m_bEditLastControl = TRUE;

			return hWndCtrl;
		}

		void Fail()                    // will throw exception
		{
		}

};


/////////////////////////////////////////////////////////////////////////////
// CCmdUI - User Interface for a command

// CCmdUI is a protocol class for all command handler variants
//      CCmdUI is an implementation class for menus and general dialog
//        controls (usually buttons)

class CCmdUI        // simple helper class
{
	public:
		CCmdUI()
		{
			// zero out everything
			m_nID = m_nIndex = m_nIndexMax = 0;
			m_pMenu = m_pSubMenu = m_pParentMenu = NULL;
			m_pOther = NULL;
			m_bEnableChanged = m_bContinueRouting = FALSE;
		}

	public:
		// Attributes
		UINT m_nID;
		UINT m_nIndex;          // menu item or other index

		// if a menu item
		CMenu* m_pMenu;         // NULL if not a menu
		CMenu* m_pSubMenu;      // sub containing menu item
		// if a popup sub menu - ID is for first in popup

		// if from some other window
		CWnd* m_pOther;         // NULL if a menu or not a CWnd

		// Implementation
		BOOL m_bEnableChanged;
		BOOL m_bContinueRouting;
		UINT m_nIndexMax;       // last + 1 for iterating m_nIndex

		CMenu* m_pParentMenu;   // NULL if parent menu not easily determined
		//  (probably a secondary popup menu)

	public:
		// Advanced operation
		void ContinueRouting() { m_bContinueRouting = TRUE; }

		BOOL DoUpdate(CCmdTarget* pTarget, BOOL bDisableIfNoHndler)
		{
			if (m_nID == 0 || LOWORD(m_nID) == 0xFFFF)
				return TRUE;     // ignore invalid IDs

			m_bEnableChanged = FALSE;

			BOOL bResult = pTarget->OnCmdMsg(m_nID, CN_UPDATE_COMMAND_UI, this, NULL);
			if (!bResult)
				ASSERT(!m_bEnableChanged); // not routed

			if (bDisableIfNoHndler && !m_bEnableChanged)
			{
				AFX_CMDHANDLERINFO info;
				info.pTarget = NULL;
				BOOL bHandler = pTarget->OnCmdMsg(m_nID, CN_COMMAND, this, &info);

				// Enable or Disable based on whether there is a handler there
				Enable(bHandler);
			}
			return bResult;
		}

		// Operations to do in ON_UPDATE_COMMAND_UI
		virtual void Enable(BOOL bOn = TRUE);
		virtual void SetCheck(int nCheck = 1);   // 0, 1 or 2 (indeterminate)
		virtual void SetRadio(BOOL bOn = TRUE);
		virtual void SetText(LPCTSTR lpszText);

};




class _NO_VTABLE CWMHnd : public CCmdTarget, public CWindow
{
	public:
		CWMHnd(HWND hWnd = NULL) : CWindow(hWnd), m_bFromHandle(hWnd != NULL)
		{
		}

		virtual ~CWMHnd()
		{
			if (m_bFromHandle)
			{
				CWindow::Detach();
				return ;
			}
		}

	public:

		// Advanced: virtual AdjustWindowRect
		enum AdjustType { adjustBorder = 0, adjustOutside = 1 };


		static const CWMHnd wndTop; // SetWindowPos's pWndInsertAfter
		static const CWMHnd wndBottom; // SetWindowPos's pWndInsertAfter
		static const CWMHnd wndTopMost; // SetWindowPos pWndInsertAfter
		static const CWMHnd wndNoTopMost; // SetWindowPos pWndInsertAfter

		const BOOL m_bFromHandle;

	public:
		operator HWND() const { return (this == NULL) ? NULL : m_hWnd; }
		BOOL operator==(const CWMHnd& wnd) const { return ((HWND)wnd) == m_hWnd; }
		BOOL operator!=(const CWMHnd& wnd) const { return ((HWND)wnd) != m_hWnd; }

		LRESULT Default()
		{
			const _ATL_MSG* pMsg = GetCurrentMessage();
			return DefWindowProc(pMsg->message, pMsg->wParam, pMsg->lParam);
		}

		virtual LRESULT DefWindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(m_hWnd, nMsg, wParam, lParam); }




		virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder)
		{
			DWORD dwExStyle = GetExStyle();
			if (nAdjustType == 0)
				dwExStyle &= ~WS_EX_CLIENTEDGE;
			::AdjustWindowRectEx(lpClientRect, GetStyle(), FALSE, dwExStyle);
		}

		void ClientToScreen(LPPOINT lpPoint) const { ASSERT(::IsWindow(m_hWnd)); ::ClientToScreen(m_hWnd, lpPoint); }

		void ClientToScreen(LPRECT lpRect) const
		{
			ASSERT(::IsWindow(m_hWnd));
			::ClientToScreen(m_hWnd, (LPPOINT)lpRect);
			::ClientToScreen(m_hWnd, ((LPPOINT)lpRect) + 1);
#if(WINVER >= 0x0500)
			if (GetExStyle() & WS_EX_LAYOUTRTL)
				SwapLeftRight(lpRect);
#endif
		}

		virtual BOOL DestroyWindow()
		{
			if (m_hWnd == NULL)
				return FALSE;

			BOOL bResult = ::DestroyWindow(m_hWnd);

			return bResult;
		}

		// for dialog data exchange and validation
		virtual void DoDataExchange(CDataExchange* pDX) {}

		void DrawMenuBar() { ASSERT(::IsWindow(m_hWnd)); ::DrawMenuBar(m_hWnd); }

		virtual void EnableWindow(BOOL bEnable = TRUE)
		{
			HWND hWnd = GetSafeHwnd() ;
			if (hWnd != NULL)
				::EnableWindow(hWnd, bEnable);
		}

		void GetDlgItem(int nID, HWND* phWnd) const { *phWnd = ::GetDlgItem(m_hWnd, nID); }

		//CWMHnd* GetDlgItem(int nID) { ASSERT(::IsWindow(m_hWnd)); return FromHandle(::GetDlgItem(m_hWnd, nID)); }

		// CMenu Functions - non-Child windows only
		virtual CMenu* GetMenu() const
		{
			ASSERT(::IsWindow(m_hWnd));
			return FROM_HANDLE_PWND(CMenu, ::GetMenu(m_hWnd));
		}

		DWORD GetStyle() const { ASSERT(::IsWindow(m_hWnd)); return (DWORD)::GetWindowLong(m_hWnd, GWL_STYLE); }

		DWORD GetExStyle() const { ASSERT(::IsWindow(m_hWnd)); return (DWORD)::GetWindowLong(m_hWnd, GWL_EXSTYLE); }

		static HWND GetTopLevelParent(HWND hWnd)
		{
			HWND hWndParent = hWnd;
			HWND hWndTmp;
			while ((hWndTmp = ::GetParent(hWndParent)) != NULL)
				hWndParent = hWndTmp;

			return hWndParent;
		}

		int GetWindowText(LPTSTR lpszString, int nMaxCount) const 
		{
			ASSERT(::IsWindow(m_hWnd));

			return ::GetWindowText(m_hWnd, lpszString, nMaxCount);
		}

		void GetWindowText(CString& rString) const
		{
			ASSERT(::IsWindow(m_hWnd));

			int nLen = ::GetWindowTextLength(m_hWnd);
			::GetWindowText(m_hWnd, rString.GetBufferSetLength(nLen), nLen + 1);
			rString.ReleaseBuffer();
		}

		virtual BOOL IsFrameWnd() { return FALSE ; }

		static BOOL _AfxModifyStyle(HWND hWnd, int nStyleOffset, DWORD dwRemove, DWORD dwAdd, UINT nFlags)
		{
			ASSERT(hWnd != NULL);
			DWORD dwStyle = ::GetWindowLong(hWnd, nStyleOffset);
			DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
			if (dwStyle == dwNewStyle)
				return FALSE;

			::SetWindowLong(hWnd, nStyleOffset, dwNewStyle);
			if (nFlags != 0)
			{
				::SetWindowPos(hWnd, NULL, 0, 0, 0, 0,
								SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
			}
			return TRUE;
		}

		BOOL ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0)
		{
			ASSERT(::IsWindow(m_hWnd));

			return ModifyStyle(m_hWnd, dwRemove, dwAdd, nFlags);
		}

		BOOL ModifyStyle(HWND hWnd, DWORD dwRemove, DWORD dwAdd, UINT nFlags)
		{
			return _AfxModifyStyle(hWnd, GWL_STYLE, dwRemove, dwAdd, nFlags);
		}

		BOOL ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0)
		{
			ASSERT(::IsWindow(m_hWnd));

			return ModifyStyleEx(m_hWnd, dwRemove, dwAdd, nFlags);
		}

		BOOL ModifyStyleEx(HWND hWnd, DWORD dwRemove, DWORD dwAdd, UINT nFlags)
		{
			return _AfxModifyStyle(hWnd, GWL_EXSTYLE, dwRemove, dwAdd, nFlags);
		}

		virtual void PostNcDestroy() { }

		virtual BOOL PreTranslateMessage(MSG* pMsg) { return FALSE; }

		void ScreenToClient(LPPOINT lpPoint) const { ASSERT(::IsWindow(m_hWnd)); ::ScreenToClient(m_hWnd, lpPoint); }

		void ScreenToClient(LPRECT lpRect) const
		{
			ASSERT(::IsWindow(m_hWnd));
			::ScreenToClient(m_hWnd, (LPPOINT)lpRect);
			::ScreenToClient(m_hWnd, ((LPPOINT)lpRect) + 1);
#if(WINVER >= 0x0500)
			if (GetExStyle() & WS_EX_LAYOUTRTL)
				SwapLeftRight(lpRect);
#endif
		}

		LRESULT SendMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0) const
		{
			return ::SendMessage(m_hWnd, message, wParam, lParam);
		}

		virtual BOOL SetMenu(CMenu* pMenu)
		{
			ASSERT(::IsWindow(m_hWnd));
			return ::SetMenu(m_hWnd, pMenu->GetSafeHmenu());
		}

		BOOL SetWindowPos(const CWMHnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags)
		{
			return ::SetWindowPos(m_hWnd, pWndInsertAfter->GetSafeHwnd(), x, y, cx, cy, nFlags);
		}

		BOOL ShowWindow(int nCmdShow) { ASSERT(::IsWindow(m_hWnd)); return ::ShowWindow(m_hWnd, nCmdShow); }

		static void SwapLeftRight(LPRECT lpRect)
		{
			LONG temp = lpRect->left;
			lpRect->left = lpRect->right;
			lpRect->right = temp;
		}

		BOOL UpdateData(BOOL bSaveAndValidate = TRUE)
		{
			ASSERT(::IsWindow(m_hWnd)); // calling UpdateData before DoModal?

			CDataExchange dx(this, bSaveAndValidate);

			// prevent control notifications from being dispatched during UpdateData
			_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
			HWND hWndOldLockout = pThreadState->m_hLockoutNotifyWindow;
			ASSERT(hWndOldLockout != m_hWnd);   // must not recurse
			pThreadState->m_hLockoutNotifyWindow = m_hWnd;

			DoDataExchange(&dx);

			pThreadState->m_hLockoutNotifyWindow = hWndOldLockout;

			return TRUE;
		}

		void UpdateWindow() { ASSERT(::IsWindow(m_hWnd)); ::UpdateWindow(m_hWnd); }




		/////////////////////////////////////////////////////////////////////////////
		// Scroll bar helpers
		//  hook for CWnd functions
		//    only works for derived class (eg: CView) that override 'GetScrollBarCtrl'
		// if the window doesn't have a _visible_ windows scrollbar - then
		//   look for a sibling with the appropriate ID

		void EnableScrollBarCtrl(int nBar, BOOL bEnable = TRUE);
		virtual CScrollBar* GetScrollBarCtrl(int) const;
		BOOL GetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, UINT nMask = SIF_ALL);
		int GetScrollPos(int nBar) const;
		void GetScrollRange(int nBar, LPINT lpMinPos, LPINT lpMaxPos) const;
		BOOL SetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, BOOL bRedraw = TRUE);
		int SetScrollPos(int nBar, int nPos, BOOL bRedraw = TRUE);
		void SetScrollRange(int nBar, int nMinPos, int nMaxPos, BOOL bRedraw);


		int GetScrollLimit(int nBar)
		{
			int nMin, nMax;
			GetScrollRange(nBar, &nMin, &nMax);
			SCROLLINFO info;
			if (GetScrollInfo(nBar, &info, SIF_PAGE) && info.nPage > 1)
			{
				nMax -= (info.nPage - 1);
			}
			return nMax;
		}

		void ScrollWindow(int xAmount, int yAmount, LPCRECT lpRect = NULL, LPCRECT lpClipRect = NULL)
		{
			ASSERT(::IsWindow(m_hWnd));

			if (IsWindowVisible() || lpRect != NULL || lpClipRect != NULL)
			{
				// When visible, let Windows do the scrolling
				::ScrollWindow(m_hWnd, xAmount, yAmount, lpRect, lpClipRect);
			}
			else
			{
				// Windows does not perform any scrolling if the window is
				// not visible.  This leaves child windows unscrolled.
				// To account for this oversight, the child windows are moved
				// directly instead.
				HWND hWndChild = ::GetWindow(m_hWnd, GW_CHILD);
				if (hWndChild != NULL)
				{
					for (; hWndChild != NULL;
						hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
					{
						CRect rect;
						::GetWindowRect(hWndChild, &rect);
						ScreenToClient(&rect);
						::SetWindowPos(hWndChild, NULL,
							rect.left + xAmount, rect.top + yAmount, 0, 0,
							SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
					}
				}
			}

		}








	public:
		// Default message map implementations

		void OnActivate(UINT, CWMHnd*, BOOL) { Default(); }
		void OnActivateApp(BOOL, DWORD) { Default(); }
		BOOL OnAppCommand(CWMHnd*, UINT, UINT, UINT) { return (BOOL)Default(); }
		void OnCancelMode() { Default(); }
		void OnChangeCbChain(HWND, HWND) { Default(); }
		void OnChar(UINT, UINT, UINT) { Default(); }
		void OnChildActivate() { Default(); }
		void OnClipboardUpdate() { Default(); }
		void OnClose() { Default(); }
		void OnCompacting(UINT) { Default(); }
		void OnContextMenu(CWMHnd*, CPoint) { Default(); }
		int OnCopyData(CWMHnd*, COPYDATASTRUCT*) { return (int)Default(); }
		int OnCreate(LPCREATESTRUCT) { return (int)Default(); }
		void OnDeadChar(UINT, UINT, UINT) { Default(); }
		void OnDestroyClipboard() { Default(); }
		void OnDrawClipboard() { Default(); }
		void OnDropFiles(HDROP) { Default(); }
		void OnEnable(BOOL) { Default(); }
		void OnEndSession(BOOL) { Default(); }
		void OnEnterIdle(UINT /*nWhy*/, CWnd* /*pWho*/) { Default(); }
		void OnEnterMenuLoop(BOOL) { Default(); }
		BOOL OnEraseBkgnd(CDC*) { return (BOOL)Default(); }
		void OnExitMenuLoop(BOOL) { Default(); }
		void OnFontChange() { Default(); }
		UINT OnGetDlgCode() { return (UINT)Default(); }
		void OnGetMinMaxInfo(MINMAXINFO*) { Default(); }
		void OnHotKey(UINT, UINT, UINT) { Default(); }
		void OnHScrollClipboard(CWMHnd*, UINT, UINT) { Default(); }
		void OnIconEraseBkgnd(CDC*) { Default(); }
		virtual BOOL OnInitDialog() { return (BOOL)Default(); }
		void OnInitMenu(CMenu*) { Default(); }
		void OnInitMenuPopup(CMenu*, UINT, BOOL) { Default(); }
		void OnInputDeviceChange(unsigned short, HANDLE) { Default(); }
		void OnInputLangChange(UINT, UINT) { Default(); }
		void OnInputLangChangeRequest(UINT, UINT) { Default(); }
		void OnKeyDown(UINT, UINT, UINT) { Default(); }
		void OnKeyUp(UINT, UINT, UINT) { Default(); }
		void OnKillFocus(CWMHnd*) { Default(); }
		void OnLButtonDblClk(UINT, CPoint) { Default(); }
		void OnLButtonDown(UINT, CPoint) { Default(); }
		void OnLButtonUp(UINT, CPoint) { Default(); }
		LRESULT OnMenuChar(UINT, UINT, CMenu*) { return Default(); }
		void OnMButtonDblClk(UINT, CPoint) { Default(); }
		void OnMButtonDown(UINT, CPoint) { Default(); }
		void OnMButtonUp(UINT, CPoint) { Default(); }
		void OnMDIActivate(BOOL, CWMHnd*, CWMHnd*) { Default(); }
		int OnMouseActivate(CWMHnd*, UINT, UINT) { return (int)Default(); }
		void OnMenuCommand(UINT, CMenu*) { Default(); }
		UINT OnMenuDrag(UINT, CMenu*) { return (UINT)Default(); }
		void OnMenuRButtonUp(UINT, CMenu*) { Default(); }
		void OnMenuSelect(UINT, UINT, HMENU) { Default(); }
		void OnMouseHover(UINT, CPoint) { Default(); }
		void OnMouseHWheel(UINT, short, CPoint) { Default(); }
		BOOL OnMouseWheel(UINT, short, CPoint) { return (BOOL)Default(); }
		void OnMouseLeave() { Default(); }
		void OnMouseMove(UINT, CPoint) { Default(); }
		void OnMove(int, int) { Default(); }
		void OnNcCalcSize(BOOL, NCCALCSIZE_PARAMS*) { Default(); }
		BOOL OnNcActivate(BOOL) { return (BOOL)Default(); }
		BOOL OnNcCreate(LPCREATESTRUCT) { return (BOOL)Default(); }
		void OnNcDestroy(); // m_pMainWnd
		LRESULT OnNcHitTest(CPoint) { return Default(); }
		void OnNcLButtonDblClk(UINT, CPoint) { Default(); }
		void OnNcLButtonDown(UINT, CPoint) { Default(); }
		void OnNcLButtonUp(UINT, CPoint) { Default(); }
		void OnNcMButtonDblClk(UINT, CPoint) { Default(); }
		void OnNcMButtonDown(UINT, CPoint) { Default(); }
		void OnNcMButtonUp(UINT, CPoint) { Default(); }
		void OnNcMouseHover(UINT, CPoint) { Default(); }
		void OnNcMouseLeave() { Default(); }
		void OnNcMouseMove(UINT, CPoint) { Default(); }
		void OnNcPaint() { Default(); }
		void OnNcRButtonDblClk(UINT, CPoint) { Default(); }
		void OnNcRButtonDown(UINT, CPoint) { Default(); }
		void OnNcRButtonUp(UINT, CPoint) { Default(); }
		void OnNcXButtonDblClk(short, UINT, CPoint) { Default(); }
		void OnNcXButtonDown(short, UINT, CPoint) { Default(); }
		void OnNcXButtonUp(short, UINT, CPoint) { Default(); }
		void OnNextMenu(UINT, LPMDINEXTMENU) { Default(); }
		UINT OnNotifyFormat(CWMHnd*, UINT) { return (UINT)Default(); }
		void OnPaint() { Default(); }
		void OnPaintClipboard(CWMHnd*, HGLOBAL) { Default(); }
		void OnPaletteChanged(CWMHnd*) { Default(); }
		void OnPaletteIsChanging(CWMHnd*) { Default(); }
		UINT OnPowerBroadcast(UINT, LPARAM) { return (UINT)Default(); }
		HCURSOR OnQueryDragIcon() { return (HCURSOR)Default(); }
		BOOL OnQueryEndSession() { return (BOOL)Default(); }
		BOOL OnQueryNewPalette() { return (BOOL)Default(); }
		BOOL OnQueryOpen() { return (BOOL)Default(); }
		void OnRButtonDblClk(UINT, CPoint) { Default(); }
		void OnRButtonDown(UINT, CPoint) { Default(); }
		void OnRButtonUp(UINT, CPoint) { Default(); }
		void OnRenderAllFormats() { Default(); }
		void OnRenderFormat(UINT) { Default(); }
		void OnSessionChange(UINT, UINT) { Default(); }
		BOOL OnSetCursor(CWMHnd*, UINT, UINT) { return (BOOL)Default(); }
		void OnShowWindow(BOOL, UINT) { Default(); }
		void OnSize(UINT, int, int) { Default(); }
		void OnSizeClipboard(CWMHnd*, HGLOBAL) { Default(); }
		void OnSpoolerStatus(UINT, UINT) { Default(); }
		void OnSyncPaint() { Default(); }
		void OnSysChar(UINT, UINT, UINT) { Default(); }
		void OnSysCommand(UINT, LPARAM) { Default(); }
		void OnSysDeadChar(UINT, UINT, UINT) { Default(); }
		void OnSysKeyDown(UINT, UINT, UINT) { Default(); }
		void OnSysKeyUp(UINT, UINT, UINT) { Default(); }
		void OnTCard(UINT, DWORD) { Default(); }
		void OnTimeChange() { Default(); }
		void OnTimer(UINT_PTR) { Default(); }
		void OnUniChar(UINT, UINT, UINT) { Default(); }
		void OnUnInitMenuPopup(CMenu*, UINT) { Default(); }
		void OnUserChanged() { Default(); }
		void OnVScrollClipboard(CWMHnd*, UINT, UINT) { Default(); }
		void OnWindowPosChanged(WINDOWPOS*) { Default(); }
		void OnWindowPosChanging(WINDOWPOS*) { Default(); }
		void OnXButtonDblClk(UINT, UINT, CPoint) { Default(); }
		void OnXButtonDown(UINT, UINT, CPoint) { Default(); }
		void OnXButtonUp(UINT, UINT, CPoint) { Default(); }

		// Win4 support
		void OnCaptureChanged(CWMHnd*) { Default(); }
		void OnChangeUIState(UINT, UINT) { Default(); }
		BOOL OnDeviceChange(UINT, DWORD_PTR) { return (BOOL)Default(); }
		void OnEnterSizeMove() { Default(); }
		void OnExitSizeMove() { Default(); }
		void OnMoving(UINT, LPRECT) { Default(); }
		UINT OnQueryUIState() { return (UINT)Default(); }
		void OnSizing(UINT, LPRECT) { Default(); }
		void OnStyleChanged(int, LPSTYLESTRUCT) { Default(); }
		void OnStyleChanging(int, LPSTYLESTRUCT) { Default(); }
		void OnUpdateUIState(UINT, UINT) { Default(); }
		void OnWinIniChange(LPCTSTR) { Default(); }

		// Desktop Windows Manager support
		void OnColorizationColorChanged(DWORD, BOOL) { Default(); }
		void OnCompositionChanged() { Default(); }
		void OnNcRenderingChanged(BOOL) { Default(); }
		void OnWindowMaximizedChange(BOOL) { Default(); }

		void OnAskCbFormatName(UINT nMaxCount, LPTSTR pszName)
		{
			(nMaxCount);
			if (nMaxCount>0)
			{
				/* defwindow proc should do this for us, but to be safe, we'll do it here too */
				pszName[0] = _T('\0');
			}
			Default();
		}

#if(WINVER >= 0x0500)
		UINT OnMenuGetObject(MENUGETOBJECTINFO*) { return (UINT)Default(); }
		void OnRawInput(UINT, HRAWINPUT) { Default(); }
#endif


		// CWnd

		//CWMHnd* GetOwner() { return (m_hWndOwner != NULL) ? FromHandle(m_hWndOwner) : GetParent(); }
		HWND GetSafeHwnd() const { return (this == NULL) ? NULL : m_hWnd; }
		//void SetOwner(CWMHnd* pOwnerWnd) { m_hWndOwner = (pOwnerWnd != NULL) ? (pOwnerWnd)->m_hWnd : NULL; }

		//void DragAcceptFiles(BOOL bAccept) { ASSERT(::IsWindow(m_hWnd)); ::DragAcceptFiles(m_hWnd, bAccept); }
		BOOL DragDetect(POINT pt) const { ASSERT(::IsWindow(m_hWnd)); return ::DragDetect(m_hWnd, pt); }
		//void DrawMenuBar() { ASSERT(::IsWindow(m_hWnd)); ::DrawMenuBar(m_hWnd); }
		CFont* GetFont() { ASSERT(::IsWindow(m_hWnd)); return FROM_HANDLE_PWND(CFont, (HFONT)::SendMessage(m_hWnd, WM_GETFONT, 0, 0)); }
		CMenu* GetSystemMenu(BOOL bRevert) { ASSERT(::IsWindow(m_hWnd)); return FROM_HANDLE_PWND(CMenu, ::GetSystemMenu(m_hWnd, bRevert)); }
		BOOL HiliteMenuItem(CMenu* pMenu, UINT nIDHiliteItem, UINT nHilite) { ASSERT(::IsWindow(m_hWnd)); return ::HiliteMenuItem(m_hWnd, pMenu->m_hMenu, nIDHiliteItem, nHilite); }
		//BOOL PostMessage(UINT message, WPARAM wParam, LPARAM lParam) { ASSERT(::IsWindow(m_hWnd)); return ::PostMessage(m_hWnd, message, wParam, lParam); }
		void SetFont(CFont* pFont, BOOL bRedraw = TRUE) { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, WM_SETFONT, (WPARAM)pFont->GetSafeHandle(), bRedraw); }

		//BOOL IsIconic() const { ASSERT(::IsWindow(m_hWnd)); return ::IsIconic(m_hWnd); }
		//BOOL IsZoomed() const { ASSERT(::IsWindow(m_hWnd)); return ::IsZoomed(m_hWnd); }
		//void MoveWindow(LPCRECT lpRect, BOOL bRepaint) { ::MoveWindow(m_hWnd, lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, bRepaint); }
		UINT ArrangeIconicWindows() { ASSERT(::IsWindow(m_hWnd)); return ::ArrangeIconicWindows(m_hWnd); }
		//int SetWindowRgn(HRGN hRgn, BOOL bRedraw) { ASSERT(::IsWindow(m_hWnd)); return ::SetWindowRgn(m_hWnd, hRgn, bRedraw); }
		//int GetWindowRgn(HRGN hRgn) const { ASSERT(::IsWindow(m_hWnd) && hRgn != NULL); return ::GetWindowRgn(m_hWnd, hRgn); }
		//void BringWindowToTop() { ASSERT(::IsWindow(m_hWnd)); ::BringWindowToTop(m_hWnd); }
		//void GetWindowRect(LPRECT lpRect) const { ASSERT(::IsWindow(m_hWnd)); ::GetWindowRect(m_hWnd, lpRect); }
		//void GetClientRect(LPRECT lpRect) const { ASSERT(::IsWindow(m_hWnd)); ::GetClientRect(m_hWnd, lpRect); }
		void MapWindowPoints(CWMHnd* pwndTo, LPPOINT lpPoint, UINT nCount) const { ASSERT(::IsWindow(m_hWnd)); ::MapWindowPoints(m_hWnd, pwndTo->GetSafeHwnd(), lpPoint, nCount); }
		//void MapWindowPoints(CWMHnd* pwndTo, LPRECT lpRect) const { ASSERT(::IsWindow(m_hWnd)); ::MapWindowPoints(m_hWnd, pwndTo->GetSafeHwnd(), (LPPOINT)lpRect, 2); }
		//void ClientToScreen(LPPOINT lpPoint) const { ASSERT(::IsWindow(m_hWnd)); ::ClientToScreen(m_hWnd, lpPoint); }
		//void ScreenToClient(LPPOINT lpPoint) const { ASSERT(::IsWindow(m_hWnd)); ::ScreenToClient(m_hWnd, lpPoint); }
		CDC* BeginPaint(LPPAINTSTRUCT lpPaint) { ASSERT(::IsWindow(m_hWnd)); return FROM_HANDLE_PWND(CDC, ::BeginPaint(m_hWnd, lpPaint)) ; }
		//void EndPaint(LPPAINTSTRUCT lpPaint) { ASSERT(::IsWindow(m_hWnd)); ::EndPaint(m_hWnd, lpPaint); }
		CDC* GetDC() { ASSERT(::IsWindow(m_hWnd)); return FROM_HANDLE_PWND(CDC, ::GetDC(m_hWnd)); }
		CDC* GetDCEx(CRgn* prgnClip, DWORD flags) { ASSERT(::IsWindow(m_hWnd)); return FROM_HANDLE_PWND(CDC, ::GetDCEx(m_hWnd, (HRGN)prgnClip->GetSafeHandle(), flags)); }
		CDC* GetWindowDC() { ASSERT(::IsWindow(m_hWnd)); return FROM_HANDLE_PWND(CDC, ::GetWindowDC(m_hWnd)); }
		int ReleaseDC(CDC* pDC) { ASSERT(::IsWindow(m_hWnd)); return ::ReleaseDC(m_hWnd, CDC::RemoveHandle(pDC->m_hDC)); }
		//void UpdateWindow() { ASSERT(::IsWindow(m_hWnd)); ::UpdateWindow(m_hWnd); }
		//void SetRedraw(BOOL bRedraw) { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, WM_SETREDRAW, bRedraw, 0); }
		//BOOL GetUpdateRect(LPRECT lpRect, BOOL bErase) { ASSERT(::IsWindow(m_hWnd)); return ::GetUpdateRect(m_hWnd, lpRect, bErase); }
		int GetUpdateRgn(CRgn* pRgn, BOOL bErase) { ASSERT(::IsWindow(m_hWnd)); return ::GetUpdateRgn(m_hWnd, (HRGN)pRgn->GetSafeHandle(), bErase); }
		//void Invalidate(BOOL bErase) { ASSERT(::IsWindow(m_hWnd)); ::InvalidateRect(m_hWnd, NULL, bErase); }
		//void InvalidateRect(LPCRECT lpRect, BOOL bErase) { ASSERT(::IsWindow(m_hWnd)); ::InvalidateRect(m_hWnd, lpRect, bErase); }
		void InvalidateRgn(CRgn* pRgn, BOOL bErase) { ASSERT(::IsWindow(m_hWnd)); ::InvalidateRgn(m_hWnd, (HRGN)pRgn->GetSafeHandle(), bErase); }
		//void ValidateRect(LPCRECT lpRect) { ASSERT(::IsWindow(m_hWnd)); ::ValidateRect(m_hWnd, lpRect); }
		void ValidateRgn(CRgn* pRgn) { ASSERT(::IsWindow(m_hWnd)); ::ValidateRgn(m_hWnd, (HRGN)pRgn->GetSafeHandle()); }
		//BOOL IsWindowVisible() const { ASSERT(::IsWindow(m_hWnd)); return ::IsWindowVisible(m_hWnd); }
		//void ShowOwnedPopups(BOOL bShow) { ASSERT(::IsWindow(m_hWnd)); ::ShowOwnedPopups(m_hWnd, bShow); }
		//void SendMessageToDescendants(UINT message, WPARAM wParam = 0, LPARAM lParam = 0, BOOL bDeep = TRUE, BOOL bOnlyPerm = FALSE)  { ASSERT(::IsWindow(m_hWnd)); SendMessageToDescendants(m_hWnd, message, wParam, lParam, bDeep, bOnlyPerm); }
		//CWMHnd* GetDescendantWindow(int nID, BOOL bOnlyPerm) { ASSERT(::IsWindow(m_hWnd)); return GetDescendantWindow(m_hWnd, nID, bOnlyPerm); }

		BOOL DrawAnimatedRects(int idAni, CONST RECT *lprcFrom, CONST RECT *lprcTo) { ASSERT(::IsWindow(m_hWnd)); return (BOOL)::DrawAnimatedRects(m_hWnd, idAni, lprcFrom, lprcTo); }
		BOOL DrawCaption(CDC* pDC, LPCRECT lprc, UINT uFlags) { ASSERT(::IsWindow(m_hWnd)); return (BOOL)::DrawCaption(m_hWnd, pDC->GetSafeHdc(), lprc, uFlags); }
		BOOL EnableScrollBar(int nSBFlags, UINT nArrowFlags) { ASSERT(::IsWindow(m_hWnd)); return (BOOL)::EnableScrollBar(m_hWnd, nSBFlags, nArrowFlags); }
		BOOL LockWindowUpdate() { ASSERT(::IsWindow(m_hWnd)); return ::LockWindowUpdate(m_hWnd); }
		BOOL RedrawWindow(LPCRECT lpRectUpdate, CRgn* prgnUpdate, UINT flags) { ASSERT(::IsWindow(m_hWnd)); return ::RedrawWindow(m_hWnd, lpRectUpdate, (HRGN)prgnUpdate->GetSafeHandle(), flags); }
		void UnlockWindowUpdate() { ASSERT(::IsWindow(m_hWnd)); ::LockWindowUpdate(NULL); }

		UINT_PTR SetTimer(UINT_PTR nIDEvent, UINT nElapse, TIMERPROC lpfnTimer) { ASSERT(::IsWindow(m_hWnd)); return ::SetTimer(m_hWnd, nIDEvent, nElapse, lpfnTimer); }
		BOOL KillTimer(UINT_PTR nIDEvent) { ASSERT(::IsWindow(m_hWnd)); return ::KillTimer(m_hWnd, nIDEvent); }
		//static CWMHnd* GetActiveWindow() { return FromHandle(::GetActiveWindow()); }
		//CWMHnd* SetActiveWindow() { ASSERT(::IsWindow(m_hWnd)); return FromHandle(::SetActiveWindow(m_hWnd)); }
		//static CWMHnd* GetCapture() { return FromHandle(::GetCapture()); }
		//CWMHnd* SetCapture() { ASSERT(::IsWindow(m_hWnd)); return FromHandle(::SetCapture(m_hWnd)); }
		//static CWMHnd* GetFocus() { return FromHandle(::GetFocus()); }
		//static CWMHnd* GetDesktopWindow() { return FromHandle(::GetDesktopWindow()); }
		//int DlgDirList(_Inout_z_ LPTSTR lpPathSpec, int nIDListBox, int nIDStaticPath, UINT nFileType) { ASSERT(::IsWindow(m_hWnd)); return ::DlgDirList(m_hWnd, lpPathSpec, nIDListBox, nIDStaticPath, nFileType); }
		//int DlgDirListComboBox(_Inout_z_ LPTSTR lpPathSpec, int nIDComboBox, int nIDStaticPath, UINT nFileType) { ASSERT(::IsWindow(m_hWnd)); return ::DlgDirListComboBox(m_hWnd, lpPathSpec, nIDComboBox, nIDStaticPath, nFileType); }
		BOOL DlgDirSelect(LPTSTR lpString, int nIDListBox) { ASSERT(::IsWindow(m_hWnd)); return ::DlgDirSelectEx(m_hWnd, lpString, _MAX_PATH, nIDListBox); }
		//BOOL DlgDirSelect(_Out_writes_z_(nSize) LPTSTR lpString, int nSize, int nIDListBox) { ASSERT(::IsWindow(m_hWnd)); return ::DlgDirSelectEx(m_hWnd, lpString, nSize, nIDListBox); }
		BOOL DlgDirSelectComboBox(LPTSTR lpString, int nIDComboBox) { ASSERT(::IsWindow(m_hWnd)); return ::DlgDirSelectComboBoxEx(m_hWnd, lpString, _MAX_PATH, nIDComboBox); }
		//BOOL DlgDirSelectComboBox(_Out_writes_z_(nSize) LPTSTR lpString, int nSize, int nIDComboBox) { ASSERT(::IsWindow(m_hWnd)); return ::DlgDirSelectComboBoxEx(m_hWnd, lpString, nSize, nIDComboBox); }
		//CWMHnd* GetNextDlgGroupItem(CWMHnd* pWndCtl, BOOL bPrevious)  { ASSERT(::IsWindow(m_hWnd)); return FromHandle(::GetNextDlgGroupItem(m_hWnd, pWndCtl->GetSafeHwnd(), bPrevious)); }
		//CWMHnd* GetNextDlgTabItem(CWMHnd* pWndCtl, BOOL bPrevious) { ASSERT(::IsWindow(m_hWnd)); return FromHandle(::GetNextDlgTabItem(m_hWnd, pWndCtl->GetSafeHwnd(), bPrevious)); }

		//CWMHnd* ChildWindowFromPoint(POINT point) { ASSERT(::IsWindow(m_hWnd)); return FromHandle(::ChildWindowFromPoint(m_hWnd, point)); }
		//CWMHnd* ChildWindowFromPoint(POINT point, UINT nFlags) { ASSERT(::IsWindow(m_hWnd)); return FromHandle(::ChildWindowFromPointEx(m_hWnd, point, nFlags)); }
		//CWMHnd* FindWindow(LPCTSTR lpszClassName, LPCTSTR lpszWindowName) { return FromHandle(::FindWindow(lpszClassName, lpszWindowName)); }
		//CWMHnd* FindWindowEx(HWND hwndParent, HWND hwndChildAfter, LPCTSTR lpszClass, LPCTSTR lpszWindow) { return FromHandle(::FindWindowEx(hwndParent, hwndChildAfter, lpszClass, lpszWindow)); }
		void ShowScrollBar(UINT nBar, BOOL bShow) { ASSERT(::IsWindow(m_hWnd)); ::ShowScrollBar(m_hWnd, nBar, bShow); }

		#ifdef GetNextWindow
		#undef GetNextWindow
		#define GetNextWindow GetWindow
		#endif
		//CWMHnd* GetNextWindow(UINT nFlag) { ASSERT(::IsWindow(m_hWnd)); return FromHandle(::GetNextWindow(m_hWnd, nFlag)); }
		//CWMHnd* GetLastActivePopup() { ASSERT(::IsWindow(m_hWnd)); return FromHandle(::GetLastActivePopup(m_hWnd)); }
		//CWMHnd* GetTopWindow() { ASSERT(::IsWindow(m_hWnd)); return FromHandle(::GetTopWindow(m_hWnd)); }
		//CWMHnd* GetWindow(UINT nCmd) { ASSERT(::IsWindow(m_hWnd)); return FromHandle(::GetWindow(m_hWnd, nCmd)); }
		//CWMHnd* GetParent() { ASSERT(::IsWindow(m_hWnd)); return FromHandle(::GetParent(m_hWnd)); }
		BOOL IsChild(const CWMHnd* pWnd) const { ASSERT(::IsWindow(m_hWnd)); return ::IsChild(m_hWnd, pWnd->GetSafeHwnd()); }
		//CWMHnd* SetParent(CWMHnd* pWndNewParent)  { ASSERT(::IsWindow(m_hWnd)); return FromHandle(::SetParent(m_hWnd, pWndNewParent->GetSafeHwnd())); }
		//static CWMHnd* WindowFromPoint(POINT point) { return FromHandle(::WindowFromPoint(point)); }

		//BOOL FlashWindow(BOOL bInvert) { ASSERT(::IsWindow(m_hWnd)); return ::FlashWindow(m_hWnd, bInvert); }
		//BOOL ChangeClipboardChain(HWND hWndNext) { ASSERT(::IsWindow(m_hWnd)); return ::ChangeClipboardChain(m_hWnd, hWndNext); }
		//HWND SetClipboardViewer() { ASSERT(::IsWindow(m_hWnd)); return ::SetClipboardViewer(m_hWnd); }
		//BOOL OpenClipboard() { ASSERT(::IsWindow(m_hWnd)); return ::OpenClipboard(m_hWnd); }
		//static CWMHnd* GetOpenClipboardWindow() { return FromHandle(::GetOpenClipboardWindow()); }
		//static CWMHnd* GetClipboardOwner() { return FromHandle(::GetClipboardOwner()); }
		//static CWMHnd* GetClipboardViewer() { return FromHandle(::GetClipboardViewer()); }
		void CreateCaret(CBitmap* pBitmap) { ASSERT(::IsWindow(m_hWnd)); ::CreateCaret(m_hWnd, (HBITMAP)pBitmap->GetSafeHandle(), 0, 0); }
		//void CreateSolidCaret(int nWidth, int nHeight) { ASSERT(::IsWindow(m_hWnd)); ::CreateCaret(m_hWnd, (HBITMAP)0, nWidth, nHeight); }
		//void CreateGrayCaret(int nWidth, int nHeight) { ASSERT(::IsWindow(m_hWnd)); ::CreateCaret(m_hWnd, (HBITMAP)1, nWidth, nHeight); }
		CPoint GetCaretPos() { CPoint point; ::GetCaretPos((LPPOINT)&point); return point; }
		void SetCaretPos(POINT point) { ::SetCaretPos(point.x, point.y); }
		//void HideCaret() { ASSERT(::IsWindow(m_hWnd)); ::HideCaret(m_hWnd); }
		//void ShowCaret() { ASSERT(::IsWindow(m_hWnd)); ::ShowCaret(m_hWnd); }
		BOOL SetForegroundWindow() { ASSERT(::IsWindow(m_hWnd)); return ::SetForegroundWindow(m_hWnd); }
		//static CWMHnd* GetForegroundWindow() { return FromHandle(::GetForegroundWindow()); }

		//BOOL SendNotifyMessage(UINT message, WPARAM wParam, LPARAM lParam) { ASSERT(::IsWindow(m_hWnd)); return ::SendNotifyMessage(m_hWnd, message, wParam, lParam); }

		// Win4
		//HICON GetIcon(BOOL bBigIcon) const { ASSERT(::IsWindow(m_hWnd)); return (HICON)::SendMessage(m_hWnd, WM_GETICON, bBigIcon, 0); }
		//HICON SetIcon(HICON hIcon, BOOL bBigIcon) { ASSERT(::IsWindow(m_hWnd)); return (HICON)::SendMessage(m_hWnd, WM_SETICON, bBigIcon, (LPARAM)hIcon); }
		void Print(CDC* pDC, DWORD dwFlags) const { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, WM_PRINT, (WPARAM)pDC->GetSafeHdc(), dwFlags); }
		void PrintClient(CDC* pDC, DWORD dwFlags) const { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, WM_PRINTCLIENT, (WPARAM)pDC->GetSafeHdc(), dwFlags); }
		DWORD GetWindowContextHelpId() const { ASSERT(::IsWindow(m_hWnd)); return ::GetWindowContextHelpId(m_hWnd); }
		BOOL SetWindowContextHelpId(DWORD dwContextHelpId) { ASSERT(::IsWindow(m_hWnd)); return ::SetWindowContextHelpId(m_hWnd, dwContextHelpId); }

		// ActiveAccessibility
#if(WINVER >= 0x0500)
		void NotifyWinEvent(DWORD event, LONG idObjectType, LONG idObject) { ASSERT(::IsWindow(m_hWnd)); ::NotifyWinEvent(event, m_hWnd, idObjectType, idObject); }
#endif

		// CWnd modality support
		void BeginModalState() { ::EnableWindow(m_hWnd, FALSE); }
		void EndModalState() { ::EnableWindow(m_hWnd, TRUE); }


};




_INLINE HWND CDataExchange::PrepareCtrl(int nIDC)
{
	ASSERT(nIDC != 0);
	ASSERT(nIDC != -1); // not allowed
	HWND hWndCtrl;

	m_pDlgWnd->GetDlgItem(nIDC, &hWndCtrl);

	m_idLastControl = nIDC;
	m_bEditLastControl = FALSE; // not an edit item by default

	return hWndCtrl;
}





class CClientDCMTL : public CDC
{
	public:
		CClientDCMTL(CWMHnd* pCWnd) : CDC(::GetDC(pCWnd->GetSafeHwnd()))
		{
			HWND hWnd = pCWnd->GetSafeHwnd();

			ATLASSERT(hWnd == NULL || ::IsWindow(hWnd));
			m_hWnd = hWnd;
		}

		virtual ~CClientDCMTL()
		{
			ATLASSERT(m_hDC != NULL);
			::ReleaseDC(m_hWnd, m_hDC);
		}

	public:
		// Data members
		HWND m_hWnd;

	public:

};


class CPaintDCMTL : public CDC
{
	public:
		CPaintDCMTL(CWMHnd* pCWnd) : CDC(::BeginPaint(pCWnd->GetSafeHwnd(), &m_ps))
		{
			HWND hWnd = pCWnd->GetSafeHwnd();

			ATLASSERT(::IsWindow(hWnd));
			m_hWnd = hWnd;

		}

		virtual ~CPaintDCMTL()
		{
			ATLASSERT(m_hDC != NULL);
			ATLASSERT(::IsWindow(m_hWnd));
			::EndPaint(m_hWnd, &m_ps);
		}

	public:
		// Data members
		HWND m_hWnd;
		PAINTSTRUCT m_ps;

	public:

};


class CWindowDCMTL : public CDC
{
	public:
		// Constructor/destructor
		CWindowDCMTL(CWMHnd* pCWnd) : CDC(::GetWindowDC(pCWnd->GetSafeHwnd()))
		{
			HWND hWnd = pCWnd->GetSafeHwnd();

			ATLASSERT(hWnd == NULL || ::IsWindow(hWnd));
			m_hWnd = hWnd;
		}

		virtual ~CWindowDCMTL()
		{
			ATLASSERT(m_hDC != NULL);
			::ReleaseDC(m_hWnd, m_hDC);
		}

	public:
		// Data members
		HWND m_hWnd;

	public:

};

#if 1
#define CClientDC CClientDCMTL
#define CPaintDC CPaintDCMTL
#define CWindowDC CWindowDCMTL
#endif

