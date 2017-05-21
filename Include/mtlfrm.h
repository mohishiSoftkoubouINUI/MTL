


class CMiniDockFrameWnd ;

class CFrameWnd : public CWnd
{
	DECLARE_DYNCREATE(CFrameWnd)

	public:
		CFrameWnd()
		{
			m_bAutoMenuEnable = TRUE;       // auto enable on by default

			m_nWindow = 0 ;

			m_hMenuDefault = NULL ;
			m_hAccelTable = NULL ;

			m_pFloatingFrameClass = NULL ;

			m_dwMenuBarVisibility = AFX_MBV_KEEPVISIBLE;
			m_dwMenuBarState = AFX_MBS_VISIBLE;
			m_hMenu = NULL;
			m_bTempShowMenu = FALSE;
			m_bMouseHitMenu = FALSE;

			m_nIDHelp = 0;
			m_nIDTracking = 0;
			m_nIDLastMessage = 0;

#ifndef MTL_REMOVE_CFRAMEWND_VIEW
			m_pViewActive = NULL;
#endif	// MTL_REMOVE_CFRAMEWND_VIEW

			m_lpfnCloseProc = NULL;


			m_hMenuAlt = NULL ;

			m_bInRecalcLayout = FALSE;

			m_nShowDelay = -1;              // no delay pending

			m_nIdleFlags = 0;               // no idle work at start

			AddFrameWnd() ;
		}

		virtual ~CFrameWnd()
		{
			RemoveFrameWnd();
		}


	public:
		DECLARE_MESSAGE_MAP()

		// idle update of frame user interface
		enum IdleFlags
		{
			idleMenu = 1, idleTitle = 2, idleNotify = 4, idleLayout = 8
		};


		BOOL m_bAutoMenuEnable;
		// TRUE => menu items without handlers will be disabled

		int m_nWindow;  // general purpose window number - display as ":n"
		// -1 => unknown, 0 => only window viewing document
		// 1 => first of many windows viewing document, 2=> second

		HMENU m_hMenuDefault;       // default menu resource for this frame
		HACCEL m_hAccelTable;       // accelerator table

		CRect m_rectBorder;         // for OLE border space negotiation

		CPtrList m_listControlBars; // array of all control bars that have this

		int m_nShowDelay;           // SW_ command for delay show/hide

		UINT m_nIDHelp;                 // Help ID (0 for none, see HID_BASE_RESOURCE)
		UINT m_nIDTracking;         // tracking command ID or string IDS
		UINT m_nIDLastMessage;      // last displayed message string IDS

#ifndef MTL_REMOVE_CFRAMEWND_VIEW
		CView* m_pViewActive;       // current active view
#endif	// MTL_REMOVE_CFRAMEWND_VIEW

		BOOL(CALLBACK* m_lpfnCloseProc)(CFrameWnd* pFrameWnd);

		HMENU m_hMenuAlt;           // menu to update to (NULL means default)

		CString m_strTitle;         // default title (original)

		BOOL m_bInRecalcLayout;
		CRuntimeClass* m_pFloatingFrameClass;

		DWORD m_dwMenuBarVisibility;      // menu bar visibility style
		DWORD m_dwMenuBarState;           // menu bar visibility state
		HMENU m_hMenu;                    // backed menu for restoring from the hidden state
		BOOL  m_bTempShowMenu;            // temporarily show the menu bar to enable menu access keys
		BOOL  m_bMouseHitMenu;            // if TRUE, the mouse is hitting the menu bar

		UINT m_nIdleFlags;          // set of bit flags for idle processing

		static const CRect rectDefault ; //(CW_USEDEFAULT, CW_USEDEFAULT, 0 /* 2*CW_USEDEFAULT */, 0 /* 2*CW_USEDEFAULT */);

	public:
		typedef struct _DocBarMap
		{
			DWORD dwDockBarMap[4][2] ;
		} DOCBARMAP ;

		static DOCBARMAP& GetDockBarMap()
		{
			static DOCBARMAP dmDockBarMap = {
												 AFX_IDW_DOCKBAR_TOP, CBRS_TOP ,
												 AFX_IDW_DOCKBAR_BOTTOM, CBRS_BOTTOM ,
												 AFX_IDW_DOCKBAR_LEFT, CBRS_LEFT ,
												 AFX_IDW_DOCKBAR_RIGHT, CBRS_RIGHT 
											} ; // CFrameWnd Docking
			return dmDockBarMap;
		}


#ifndef MTL_REMOVE_CFRAMEWND_CONTROLBAR
		BOOL OnBarCheck(UINT nID)
		{
			ASSERT(ID_VIEW_STATUS_BAR == AFX_IDW_STATUS_BAR);
			ASSERT(ID_VIEW_TOOLBAR == AFX_IDW_TOOLBAR);
			ASSERT(ID_VIEW_REBAR == AFX_IDW_REBAR);

			CControlBar* pBar = GetControlBar(nID);
			if (pBar != NULL)
			{
				ShowControlBar(pBar, (pBar->GetStyle() & WS_VISIBLE) == 0, FALSE);
				return TRUE;
			}
			return FALSE;
		}
#endif	// MTL_REMOVE_CFRAMEWND_CONTROLBAR

		void OnClose() ;

		virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) ;

		int OnCreate(LPCREATESTRUCT lpcs)
		{
			ENSURE_ARG(lpcs != NULL);
			CCreateContext* pContext = (CCreateContext*)lpcs->lpCreateParams;
			return OnCreateHelper(lpcs, pContext);
		}

		virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
		{
			// default create client will create a view if asked for it
			if (pContext != NULL && pContext->m_pNewViewClass != NULL)
			{
				if (CreateView(pContext, AFX_IDW_PANE_FIRST) == NULL)
					return FALSE;
			}
			return TRUE;
		}

		int OnCreateHelper(LPCREATESTRUCT lpcs, CCreateContext* pContext)
		{
			if (CWnd::OnCreate(lpcs) == -1)
				return -1;

			// create special children first
			if (!OnCreateClient(lpcs, pContext))
			{
				TRACE(traceAppMsg, 0, "Failed to create client pane/view for frame.\n");
				return -1;
			}

			// post message for initial message string
			PostMessage(WM_SETMESSAGESTRING, AFX_IDS_IDLEMESSAGE);

			// make sure the child windows have been properly sized
			RecalcLayout();

			return 0;   // create ok
		}

		void OnDestroy() ;

#ifndef MTL_REMOVE_CFRAMEWND_CONTROLBAR
		void OnUpdateControlBarMenu(CCmdUI* pCmdUI)
		{
			ASSERT(ID_VIEW_STATUS_BAR == AFX_IDW_STATUS_BAR);
			ASSERT(ID_VIEW_TOOLBAR == AFX_IDW_TOOLBAR);
			ASSERT(ID_VIEW_REBAR == AFX_IDW_REBAR);
			ENSURE_ARG(pCmdUI != NULL);

			CControlBar* pBar = GetControlBar(pCmdUI->m_nID);
			if (pBar != NULL)
			{
				pCmdUI->SetCheck((pBar->GetStyle() & WS_VISIBLE) != 0);
				return;
			}
			pCmdUI->ContinueRouting();
		}
#endif	// MTL_REMOVE_CFRAMEWND_CONTROLBAR

		// default drop processing will attempt to open the file
		void OnDropFiles(HDROP hDropInfo) ;

		void OnEnterIdle(UINT nWhy, CWnd* pWho)
		{
			CWnd::OnEnterIdle(nWhy, pWho);

			if (nWhy != MSGF_MENU || m_nIDTracking == m_nIDLastMessage)
				return;

			SetMessageText(m_nIDTracking);
			ASSERT(m_nIDTracking == m_nIDLastMessage);
		}

		void OnHelp() {}

#ifndef MTL_REMOVE_CFRAMEWND_VIEW
		BOOL OnEraseBkgnd(CDC* pDC)
		{
			if (m_pViewActive != NULL)
				return TRUE;        // active view will erase/paint itself

			// for view-less frame just use the default background fill
			return CWnd::OnEraseBkgnd(pDC);
		}
#endif	// MTL_REMOVE_CFRAMEWND_VIEW

		virtual void OnHideMenuBar() {}

		void OnIdleUpdateCmdUI()
		{
			// update menu if necessary
			if (m_nIdleFlags & idleMenu)
			{
				m_nIdleFlags &= ~idleMenu;
				OnUpdateFrameMenu(m_hMenuAlt);
			}

			// update title if necessary
			if (m_nIdleFlags & idleTitle)
				OnUpdateFrameTitle(TRUE);

			// recalc layout if necessary
			if (m_nIdleFlags & idleLayout)
			{
				RecalcLayout(m_nIdleFlags & idleNotify);
				UpdateWindow();
			}

			// set the current message string if necessary
			if (m_nIDTracking != m_nIDLastMessage)
			{
				SetMessageText(m_nIDTracking);
				ASSERT(m_nIDTracking == m_nIDLastMessage);
			}
			m_nIdleFlags = 0;
		}

		/////////////////////////////////////////////////////////////////////////////
		// Command prompts

		void OnInitMenuPopup(CMenu* pMenu, UINT nIndex, BOOL bSysMenu)
		{
			if (bSysMenu)
				return;     // don't support system menu

			ENSURE_VALID(pMenu);

			// check the enabled state of various menu items

			CCmdUI state;
			state.m_pMenu = pMenu;
			ASSERT(state.m_pOther == NULL);
			ASSERT(state.m_pParentMenu == NULL);

			// determine if menu is popup in top-level menu and set m_pOther to
			//  it if so (m_pParentMenu == NULL indicates that it is secondary popup)
			HMENU hParentMenu;
			if (AfxGetThreadState()->m_hTrackingMenu == pMenu->m_hMenu)
				state.m_pParentMenu = pMenu;    // parent == child for tracking popup
			else if ((hParentMenu = (m_dwMenuBarState == AFX_MBS_VISIBLE) ? ::GetMenu(m_hWnd) : m_hMenu) != NULL)
			{
				CWnd* pParent = GetTopLevelParent();
				// child windows don't have menus -- need to go to the top!
				if (pParent != NULL &&
					(hParentMenu = pParent->GetMenu()->GetSafeHmenu()) != NULL)
				{
					int nIndexMax = ::GetMenuItemCount(hParentMenu);
					for (int nItemIndex = 0; nItemIndex < nIndexMax; nItemIndex++)
					{
						if (::GetSubMenu(hParentMenu, nItemIndex) == pMenu->m_hMenu)
						{
							// when popup is found, m_pParentMenu is containing menu
							state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
							break;
						}
					}
				}
			}

			state.m_nIndexMax = pMenu->GetMenuItemCount();
			for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax; state.m_nIndex++)
			{
				state.m_nID = pMenu->GetMenuItemID(state.m_nIndex);
				if (state.m_nID == 0)
					continue; // menu separator or invalid cmd - ignore it

				ASSERT(state.m_pOther == NULL);
				ASSERT(state.m_pMenu != NULL);
				if (state.m_nID == (UINT)-1)
				{
					// possibly a popup menu, route to first item of that popup
					state.m_pSubMenu = pMenu->GetSubMenu(state.m_nIndex);
					if (state.m_pSubMenu == NULL ||
						(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 || state.m_nID == (UINT)-1)
					{
						continue;       // first item of popup can't be routed to
					}
					state.DoUpdate(this, FALSE);    // popups are never auto disabled
				}
				else
				{
					// normal menu item
					// Auto enable/disable if frame window has 'm_bAutoMenuEnable'
					//    set and command is _not_ a system command.
					state.m_pSubMenu = NULL;
					state.DoUpdate(this, m_bAutoMenuEnable && state.m_nID < 0xF000);
				}

				// adjust for menu deletions and additions
				UINT nCount = pMenu->GetMenuItemCount();
				if (nCount < state.m_nIndexMax)
				{
					state.m_nIndex -= (state.m_nIndexMax - nCount);
					while (state.m_nIndex < nCount &&
						pMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
					{
						state.m_nIndex++;
					}
				}
				state.m_nIndexMax = nCount;
			}
		}

		void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
		{
			CFrameWnd* pFrameWnd = GetTopLevelFrame();
			ENSURE_VALID(pFrameWnd);

			// set the tracking state (update on idle)
			if (nFlags == 0xFFFF)
			{
				// cancel menu operation (go back to idle now)
				m_nFlags &= ~WF_NOPOPMSG;
				m_nIDTracking = AFX_IDS_IDLEMESSAGE;

				SendMessage(WM_SETMESSAGESTRING, (WPARAM)m_nIDTracking);
				ASSERT(m_nIDTracking == m_nIDLastMessage);

				// update right away
				CWnd* pWnd = GetMessageBar();
				if (pWnd != NULL)
					pWnd->UpdateWindow();

				if (hSysMenu == 0 && // the menu has been closed
					(m_dwMenuBarVisibility & AFX_MBV_KEEPVISIBLE) == 0 &&
						::GetKeyState(VK_F10) >= 0 && // not pressed
						::GetKeyState(VK_MENU) >= 0 &&
						!m_bMouseHitMenu)
				{
					SetMenuBarState(AFX_MBS_HIDDEN);
				}
			}
			else
			{
				if (m_bTempShowMenu)
				{
					m_bTempShowMenu = FALSE;
					if ((nFlags & MF_SYSMENU) &&
						(m_dwMenuBarVisibility & AFX_MBV_KEEPVISIBLE) == 0)
					{
						SetMenuBarState(AFX_MBS_HIDDEN);
					}
				}

				if (nItemID == 0 || nFlags & (MF_SEPARATOR | MF_POPUP))
				{
					// nothing should be displayed
					m_nIDTracking = 0;
				}
				else if (nItemID >= 0xF000 && nItemID < 0xF1F0) // max of 31 SC_s
				{
					// special strings table entries for system commands
					m_nIDTracking = ID_COMMAND_FROM_SC(nItemID);
					ASSERT(m_nIDTracking >= AFX_IDS_SCFIRST && m_nIDTracking < AFX_IDS_SCFIRST + 31);
				}
				else if (nItemID >= AFX_IDM_FIRST_MDICHILD)
				{
					// all MDI Child windows map to the same help id
					m_nIDTracking = AFX_IDS_MDICHILD;
				}
				else
				{
					// track on idle
					m_nIDTracking = nItemID;
				}
				pFrameWnd->m_nFlags |= WF_NOPOPMSG;
			}

			// when running in-place, it is necessary to cause a message to
			//  be pumped through the queue.
			if (m_nIDTracking != m_nIDLastMessage && GetParent() != NULL)
				PostMessage(WM_KICKIDLE);
		}

		LRESULT OnPopMessageString(WPARAM wParam, LPARAM lParam)
		{
			if (m_nFlags & WF_NOPOPMSG)
				return 0;

			return SendMessage(WM_SETMESSAGESTRING, wParam, lParam);
		}

		LRESULT OnSetMessageString(WPARAM wParam, LPARAM lParam)
		{
			UINT nIDLast = m_nIDLastMessage;
			m_nFlags &= ~WF_NOPOPMSG;

			CWnd* pMessageBar = GetMessageBar();
			if (pMessageBar != NULL)
			{
				LPCTSTR lpsz = NULL;
				CString strMessage;

				// set the message bar text
				if (lParam != 0)
				{
					ASSERT(wParam == 0);    // can't have both an ID and a string
					lpsz = (LPCTSTR)lParam; // set an explicit string
				}
				else if (wParam != 0)
				{
					// map SC_CLOSE to PREVIEW_CLOSE when in print preview mode
					if (wParam == AFX_IDS_SCCLOSE && m_lpfnCloseProc != NULL)
						wParam = AFX_IDS_PREVIEW_CLOSE;

					// get message associated with the ID indicated by wParam
					//NT64: Assume IDs are still 32-bit
					GetMessageString((UINT)wParam, strMessage);
					lpsz = strMessage;
				}
				pMessageBar->SetWindowText(lpsz);

				// update owner of the bar in terms of last message selected
				CFrameWnd* pFrameWnd = pMessageBar->GetParentFrame();
				if (pFrameWnd != NULL)
				{
					pFrameWnd->m_nIDLastMessage = (UINT)wParam;
					pFrameWnd->m_nIDTracking = (UINT)wParam;
				}
			}

			m_nIDLastMessage = (UINT)wParam;    // new ID (or 0)
			m_nIDTracking = (UINT)wParam;       // so F1 on toolbar buttons work
			return nIDLast;
		}

		virtual void OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState) ;

		void OnSize(UINT nType, int cx, int cy)
		{
			CWnd::OnSize(nType, cx, cy);    // important for MDI Children
			if (nType != SIZE_MINIMIZED)
				RecalcLayout();
		}

		virtual void OnShowMenuBar() {}

#ifndef MTL_REMOVE_CFRAMEWND_TOOLTIPTEXT
		BOOL OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
		{
			ENSURE_ARG(pNMHDR != NULL);
			ENSURE_ARG(pResult != NULL);
			ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

			// need to handle both ANSI and UNICODE versions of the message
			TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
			TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
			TCHAR szFullText[256];
			CString strTipText;
			UINT_PTR nID = pNMHDR->idFrom;
			if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
				pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
			{
				// idFrom is actually the HWND of the tool
				nID = _AfxGetDlgCtrlID((HWND)nID);
			}

			if (nID != 0) // will be zero on a separator
			{
				// don't handle the message if no string resource found
				if (AfxLoadString((UINT)nID, szFullText) == 0)
					return FALSE;

				// this is the command id, not the button index
				AfxExtractSubString(strTipText, szFullText, 1, '\n');
			}
#ifndef _UNICODE
			if (pNMHDR->code == TTN_NEEDTEXTA)
				lstrcpyn(pTTTA->szText, strTipText, _countof(pTTTA->szText));
			else
				_mbstowcsz(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#else
			if (pNMHDR->code == TTN_NEEDTEXTA)
				_wcstombsz(pTTTA->szText, (LPCTSTR)strTipText, _countof(pTTTA->szText));
			else
				lstrcpyn(pTTTW->szText, (LPCTSTR)strTipText, _countof(pTTTW->szText));
#endif
			*pResult = 0;

			// bring the tooltip window above other popup windows
			::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
							SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE | SWP_NOOWNERZORDER);

			return TRUE;    // message was handled
		}
#endif	// MTL_REMOVE_CFRAMEWND_TOOLTIPTEXT

		virtual void OnUpdateFrameMenu(HMENU hMenuAlt);
		virtual void OnUpdateFrameTitle(BOOL bAddToTitle);

#ifndef  MTL_REMOVE_CFRAMEWND_INDICATOR
		/////////////////////////////////////////////////////////////////////////////
		// Support for standard status bar

		void OnUpdateKeyIndicator(CCmdUI* pCmdUI)
		{
			ENSURE_ARG(pCmdUI != NULL);
			UINT nVK;
			UINT flag = 0x0001;

			switch (pCmdUI->m_nID)
			{
				case ID_INDICATOR_CAPS:
					nVK = VK_CAPITAL;
					break;

				case ID_INDICATOR_NUM:
					nVK = VK_NUMLOCK;
					break;

				case ID_INDICATOR_SCRL:
					nVK = VK_SCROLL;
					break;

				case ID_INDICATOR_KANA:
					nVK = VK_KANA;
					break;

				default:
					TRACE(traceAppMsg, 0, "Warning: OnUpdateKeyIndicator - unknown indicator 0x%04X.\n",
						pCmdUI->m_nID);
					pCmdUI->ContinueRouting();
					return; // not for us
			}

			pCmdUI->Enable(::GetKeyState(nVK) & flag);
			// enable static text based on toggled key state
			ASSERT(pCmdUI->m_bEnableChanged);
		}
#endif	// MTL_REMOVE_CFRAMEWND_INDICATOR


		virtual void ActivateFrame(int nCmdShow = -1)
		{
			// translate default nCmdShow (-1)
			if (nCmdShow == -1)
			{
				if (!IsWindowVisible())
					nCmdShow = SW_SHOWNORMAL;
				else if (IsIconic())
					nCmdShow = SW_RESTORE;
			}

			// bring to top before showing
			BringToTop(nCmdShow);

			if (nCmdShow != -1)
			{
				// show the window as specified
				ShowWindow(nCmdShow);

				// and finally, bring to top after showing
				BringToTop(nCmdShow);
			}
		}

		void AddControlBar(CControlBar *pBar) { m_listControlBars.AddTail(pBar); }

		void AddFrameWnd() ;

		void BringToTop(int nCmdShow)
		{
			// place the window on top except for certain nCmdShow
			if (nCmdShow != SW_HIDE &&
				nCmdShow != SW_MINIMIZE && nCmdShow != SW_SHOWMINNOACTIVE &&
				nCmdShow != SW_SHOWNA && nCmdShow != SW_SHOWNOACTIVATE)
			{
				// if no last active popup, it will return m_hWnd
				HWND hWndLastPop = ::GetLastActivePopup(m_hWnd);
				::BringWindowToTop(hWndLastPop);
			}
		}

		DWORD CanDock(CRect rect, DWORD dwDockStyle, CDockBar** ppDockBar = NULL)
		{
			// dwDockStyle -- allowable styles of bar
			// don't allow to dock to floating unless multi is specified
			dwDockStyle &= CBRS_ALIGN_ANY | CBRS_FLOAT_MULTI;

			if (ppDockBar != NULL)
				*ppDockBar = NULL;
			POSITION pos = m_listControlBars.GetHeadPosition();
			while (pos != NULL)
			{
				CDockBar* pDockBar = (CDockBar*)m_listControlBars.GetNext(pos);
				if (pDockBar->IsDockBar() && pDockBar->IsWindowVisible() &&
					(pDockBar->m_dwStyle & dwDockStyle & CBRS_ALIGN_ANY) &&
					(!pDockBar->m_bFloating || (dwDockStyle & pDockBar->m_dwStyle & CBRS_FLOAT_MULTI)))
				{
					CRect rectBar;
					pDockBar->GetWindowRect(&rectBar);
					if (rectBar.Width() == 0)
						rectBar.right++;
					if (rectBar.Height() == 0)
						rectBar.bottom++;
					if (rectBar.IntersectRect(rectBar, rect))
					{
						if (ppDockBar != NULL)
							*ppDockBar = pDockBar;
						return pDockBar->m_dwStyle & dwDockStyle;
					}
				}
			}
			return 0;
		}

		virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault,
							CWnd* pParentWnd = NULL, LPCTSTR lpszMenuName = NULL, DWORD dwExStyle = 0, CCreateContext* pContext = NULL)
		{
			HMENU hMenu = NULL;
			if (lpszMenuName != NULL)
			{
				// load in a menu that will get destroyed when window gets destroyed
				HINSTANCE hInst = AfxFindResourceHandle(lpszMenuName, ATL_RT_MENU);
				if ((hMenu = ::LoadMenu(hInst, lpszMenuName)) == NULL)
				{
					TRACE(traceAppMsg, 0, "Warning: failed to load menu for CFrameWnd.\n");
					PostNcDestroy();            // perhaps delete the C++ object
					return FALSE;
				}
			}

			m_strTitle = lpszWindowName;    // save title for later

			if (!CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle,
							rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
							pParentWnd->GetSafeHwnd(), hMenu, (LPVOID)pContext))
			{
				TRACE(traceAppMsg, 0, "Warning: failed to create CFrameWnd.\n");
				if (hMenu != NULL)
					DestroyMenu(hMenu);
				return FALSE;
			}

			return TRUE;
		}

		CMiniDockFrameWnd* CreateFloatingFrame(DWORD dwStyle) ;

			
		CWnd* CreateView(CCreateContext* pContext, UINT nID = AFX_IDW_PANE_FIRST)
		{
			ASSERT(m_hWnd != NULL);
			ASSERT(::IsWindow(m_hWnd));
			ENSURE_ARG(pContext != NULL);
			ENSURE_ARG(pContext->m_pNewViewClass != NULL);

			// Note: can be a CWnd with PostNcDestroy self cleanup
			CWnd* pView = (CWnd*)pContext->m_pNewViewClass->CreateObject();
			if (pView == NULL)
			{
				TRACE(traceAppMsg, 0, "Warning: Dynamic create of view type %hs failed.\n", pContext->m_pNewViewClass->m_lpszClassName);
				return NULL;
			}
			ASSERT_KINDOF(CWnd, pView);

			// views are always created with a border!
			if (!pView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, nID, pContext))
			{
				TRACE(traceAppMsg, 0, "Warning: could not create view for frame.\n");
				return NULL;        // can't continue without a view
			}

			if (pView->GetExStyle() & WS_EX_CLIENTEDGE)
			{
				// remove the 3d style from the frame, since the view is
				//  providing it.
				// make sure to recalc the non-client area
				ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
			}
			return pView;
		}

		void DelayRecalcLayout(BOOL bNotify = TRUE) { m_nIdleFlags |= (idleLayout | (bNotify ? idleNotify : 0)); };

		void DelayUpdateFrameTitle()
		{
			m_nIdleFlags |= idleTitle;
		}

		void DestroyDockBars()
		{
			// create a list of all the dock bars
			// this is necessary because m_listControlBars will change
			// as the dock bars and floating frames are destroyed
			CList<HWND, HWND> listDockBars, listCtrlBars;
			POSITION pos = m_listControlBars.GetHeadPosition();
			while (pos != NULL)
			{
				CControlBar* pBar = (CControlBar*)m_listControlBars.GetNext(pos);
				ASSERT(pBar != NULL);
				if (pBar->IsDockBar())
				{
					listDockBars.AddTail(pBar->GetSafeHwnd());
				}
				else
				{
					listCtrlBars.AddTail(pBar->GetSafeHwnd());
				}
			}
			pos = listDockBars.GetHeadPosition();
			while (pos != NULL)
			{
				HWND hwndDock = listDockBars.GetNext(pos);
				CDockBar* pDockBar = DYNAMIC_DOWNCAST(CDockBar, FromHandlePermanent(hwndDock));
				if (pDockBar)
				{
					ASSERT(::IsWindow(hwndDock));
					if (pDockBar->m_bFloating)
					{
						CFrameWnd* pFrameWnd = pDockBar->EnsureParentFrame();
						pFrameWnd->DestroyWindow();
					}
					else
					{
						pDockBar->DestroyWindow();
					}
				}
			}

			pos = listCtrlBars.GetHeadPosition();
			while (pos != NULL)
			{
				HWND hwndCtrlBar = listCtrlBars.GetNext(pos);
				//pDockBar->DestroyWindow() (see listDockBars) will destroy all child windows of this dockbar, 
				//including control bars, so check if HWND is still valid. We do not want to destroy a second time. 		
				CControlBar *pCtrlBar = DYNAMIC_DOWNCAST(CControlBar, FromHandlePermanent(hwndCtrlBar));
				if (pCtrlBar)
				{
					ASSERT(::IsWindow(hwndCtrlBar));
					pCtrlBar->DestroyWindow();
				}
			}
		}

		void DockControlBar(CControlBar* pBar, UINT nDockBarID = 0, LPCRECT lpRect = NULL)
		{
			CDockBar* pDockBar = (nDockBarID == 0) ? NULL : (CDockBar*)GetControlBar(nDockBarID);
			DockControlBar(pBar, pDockBar, lpRect);
		}

		void DockControlBar(CControlBar* pBar, CDockBar* pDockBar, LPCRECT lpRect = NULL)
		{
			DWORD(*dwDockBarMap)[2] = GetDockBarMap().dwDockBarMap;

			ENSURE_ARG(pBar != NULL);
			// make sure CControlBar::EnableDocking has been called
			ASSERT(pBar->m_pDockContext != NULL);

			if (pDockBar == NULL)
			{
				for (int i = 0; i < 4; i++)
				{
					if ((dwDockBarMap[i][1] & CBRS_ALIGN_ANY) == (pBar->m_dwStyle & CBRS_ALIGN_ANY))
					{
						pDockBar = (CDockBar*)GetControlBar(dwDockBarMap[i][0]);
						ASSERT(pDockBar != NULL);
						// assert fails when initial CBRS_ of bar does not
						// match available docking sites, as set by EnableDocking()
						break;
					}
				}
			}
			ENSURE_ARG(pDockBar != NULL);
			ASSERT(m_listControlBars.Find(pBar) != NULL);
			ASSERT(pBar->m_pDockSite == this);
			// if this assertion occurred it is because the parent of pBar was not initially
			// this CFrameWnd when pBar's OnCreate was called
			// i.e. this control bar should have been created with a different parent initially

			pDockBar->DockControlBar(pBar, lpRect);
		}


		void EnableDocking(DWORD dwDockStyle) ;

		void FloatControlBar(CControlBar* pBar, CPoint point, DWORD dwStyle = CBRS_ALIGN_TOP);

#ifndef MTL_REMOVE_CFRAMEWND_VIEW
		CView* GetActiveView() const { return m_pViewActive; }
#else	// MTL_REMOVE_CFRAMEWND_VIEW
		CView* GetActiveView() const { return NULL ; }
#endif	// MTL_REMOVE_CFRAMEWND_VIEW

		CControlBar* GetControlBar(UINT nID)
		{
			if (nID == 0)
				return NULL;
			POSITION pos = m_listControlBars.GetHeadPosition();
			while (pos != NULL)
			{
				CControlBar* pBar = (CControlBar*)m_listControlBars.GetNext(pos);
				ASSERT(pBar != NULL);
				if (_AfxGetDlgCtrlID(pBar->m_hWnd) == nID)
				{
					return pBar;
				}
			}
			return NULL;
		}

		CDocument* GetActiveDocument();

		virtual CFrameWnd* GetActiveFrame()
		{
			// by default, the active frame is the frame itself (MDI is different)
			return this;
		}

		virtual HACCEL GetDefaultAccelerator();

		LPCTSTR GetIconWndClass(DWORD dwDefaultStyle, UINT nIDResource)
		{
			HINSTANCE hInst = AfxFindResourceHandle(ATL_MAKEINTRESOURCE(nIDResource), ATL_RT_GROUP_ICON);
			HICON hIcon = ::LoadIconW(hInst, ATL_MAKEINTRESOURCEW(nIDResource));
			if (hIcon != NULL)
			{
				CREATESTRUCT cs;
				memset(&cs, 0, sizeof(CREATESTRUCT));
				cs.style = dwDefaultStyle;
				PreCreateWindow(cs);
				// will fill lpszClassName with default WNDCLASS name
				// ignore instance handle from PreCreateWindow.

				WNDCLASS wndcls;
				if (cs.lpszClass != NULL &&
					GetClassInfo(AfxGetInstanceHandle(), cs.lpszClass, &wndcls) && wndcls.hIcon != hIcon)
				{
					// register a very similar WNDCLASS
					return AfxRegisterWndClass(wndcls.style, wndcls.hCursor, wndcls.hbrBackground, hIcon);
				}
			}
			return NULL;        // just use the default
		}

		CWnd* GetMessageBar() { return GetDescendantWindow(AFX_IDW_STATUS_BAR, TRUE); }

		void GetMessageString(UINT nID, CString& rMessage) const
		{
			// load appropriate string
			LPTSTR lpsz = rMessage.GetBuffer(255);
			if (AfxLoadString(nID, lpsz) != 0)
			{
				// first newline terminates actual string
				lpsz = _tcschr(lpsz, '\n');
				if (lpsz != NULL)
					*lpsz = '\0';
			}
			else
			{
				// not found
				TRACE(traceAppMsg, 0, "Warning: no message line prompt for ID 0x%04X.\n", nID);
			}
			rMessage.ReleaseBuffer();
		}

		CString GetTitle() const { return m_strTitle; }

		void InitialUpdateFrame(CDocument* pDoc, BOOL bMakeVisible) ;

		virtual BOOL IsFrameWnd() { return TRUE; }

		BOOL LoadAccelTable(LPCTSTR lpszResourceName)
		{
			ASSERT(m_hAccelTable == NULL);  // only do once
			ASSERT(lpszResourceName != NULL);

			HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, ATL_RT_ACCELERATOR);
			m_hAccelTable = ::LoadAccelerators(hInst, lpszResourceName);
			return (m_hAccelTable != NULL);
		}

		virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL)
		{
			// only do this once
			ASSERT(m_nIDHelp == 0 || m_nIDHelp == nIDResource);

			m_nIDHelp = nIDResource;    // ID for help context (+HID_BASE_RESOURCE)

			CString strFullString;
			if (strFullString.LoadString(nIDResource))
				AfxExtractSubString(m_strTitle, strFullString, 0);    // first sub-string

			// attempt to create the window
			LPCTSTR lpszClass = GetIconWndClass(dwDefaultStyle, nIDResource);
			CString strTitle = m_strTitle;
			if (!Create(lpszClass, strTitle, dwDefaultStyle, rectDefault, pParentWnd, ATL_MAKEINTRESOURCE(nIDResource), 0L, pContext))
			{
				return FALSE;   // will self destruct on failure normally
			}

			// save the default menu handle
			ASSERT(m_hWnd != NULL);
			m_hMenuDefault = m_dwMenuBarState == AFX_MBS_VISIBLE ? ::GetMenu(m_hWnd) : m_hMenu;

			// load accelerator resource
			LoadAccelTable(ATL_MAKEINTRESOURCE(nIDResource));

			if (pContext == NULL)   // send initial update
				SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, TRUE, TRUE);

			return TRUE;
		}

		void PostNcDestroy()
		{
			// default for frame windows is to allocate them on the heap
			//  the default post-cleanup is to 'delete this'.
			// never explicitly call 'delete' on a CFrameWnd, use DestroyWindow instead
			delete this;
		}

		virtual BOOL PreCreateWindow(CREATESTRUCT& cs)
		{
			if (cs.lpszClass == NULL)
			{
				VERIFY(AfxDeferRegisterClass(AFX_WNDFRAMEORVIEW_REG));
				cs.lpszClass = _afxWndFrameOrView;  // COLOR_WINDOW background
			}

			if (cs.style & FWS_ADDTOTITLE)
				cs.style |= FWS_PREFIXTITLE;

			cs.dwExStyle |= WS_EX_CLIENTEDGE;

			return TRUE;
		}

		virtual BOOL PreTranslateMessage(MSG* pMsg)
		{
			// allow tooltip messages to be filtered
			if (CWnd::PreTranslateMessage(pMsg))
				return TRUE;

			if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
			{
				// finally, translate the message
				HACCEL hAccel = GetDefaultAccelerator();
				return hAccel != NULL &&  ::TranslateAccelerator(m_hWnd, hAccel, pMsg);
			}
			return FALSE;
		}

		virtual void RecalcLayout(BOOL bNotify = TRUE)
		{
			if (m_bInRecalcLayout)
				return;

			m_bInRecalcLayout = TRUE;
			// clear idle flags for recalc layout if called elsewhere
			if (m_nIdleFlags & idleNotify)
				bNotify = TRUE;
			m_nIdleFlags &= ~(idleLayout | idleNotify);

			// reposition all the child windows (regardless of ID)
			if (GetStyle() & FWS_SNAPTOBARS)
			{
				CRect rect(0, 0, 32767, 32767);
				RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, &rect, &rect, FALSE);
				RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposExtra, &m_rectBorder, &rect, TRUE);
				CalcWindowRect(&rect);
				SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(),
					SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
			}
			else
				RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposExtra, &m_rectBorder);
			m_bInRecalcLayout = FALSE;
		}

		void ReDockControlBar(CControlBar* pBar, CDockBar* pDockBar, LPCRECT lpRect = NULL)
		{
			ENSURE_ARG(pBar != NULL);
			// make sure CControlBar::EnableDocking has been called
			ASSERT(pBar->m_pDockContext != NULL);

			if (pDockBar == NULL)
			{
				DWORD(*dwDockBarMap)[2] = GetDockBarMap().dwDockBarMap;

				// Search for the place holder.

				// In case we don't find a place holder, find a bar with the correct alignment
				// and keep it in pPossibleBar.
				CDockBar* pPossibleBar = NULL;
				for (int i = 0; i < 4; i++)
				{
					CDockBar* pTempBar = (CDockBar*)GetControlBar(dwDockBarMap[i][0]);
					if (pTempBar != NULL)
					{
						// Is this the same bar we docked with before?
						if (pTempBar->FindBar((CControlBar*)(DWORD_PTR)_AfxGetDlgCtrlID(pBar->m_hWnd)) > 0)
						{
							pDockBar = pTempBar;
							break;
						}
					}

					if ((dwDockBarMap[i][1] & CBRS_ALIGN_ANY) == (pBar->m_dwStyle & CBRS_ALIGN_ANY))
					{
						pPossibleBar = (CDockBar*)GetControlBar(dwDockBarMap[i][0]);
						ASSERT(pPossibleBar != NULL);
						// assert fails when initial CBRS_ of bar does not
						// match available docking sites, as set by EnableDocking()
					}
				}

				// Did we find the place holder?
				if (pDockBar == NULL)
					pDockBar = pPossibleBar;
			}
			ENSURE_ARG(pDockBar != NULL);
			ASSERT(m_listControlBars.Find(pBar) != NULL);
			ASSERT(pBar->m_pDockSite == this);
			// if this assertion occurred it is because the parent of pBar was not initially
			// this CFrameWnd when pBar's OnCreate was called
			// i.e. this control bar should have been created with a different parent initially

			pDockBar->ReDockControlBar(pBar, lpRect);
		}

		void RemoveControlBar(CControlBar *pBar)
		{
			POSITION pos = m_listControlBars.Find(pBar);
			if (pos != NULL)
				m_listControlBars.RemoveAt(pos);
		}

		void RemoveFrameWnd();

		void SetActiveView(CView* pViewNew, BOOL bNotify = TRUE);

		BOOL SetMenuBarState(DWORD dwState)
		{
			ENSURE_ARG(dwState == AFX_MBS_VISIBLE || dwState == AFX_MBS_HIDDEN);

			if (m_dwMenuBarState == dwState)
			{
				return FALSE;
			}

			if (dwState == AFX_MBS_VISIBLE)
			{
				OnShowMenuBar();
				::SetMenu(m_hWnd, m_hMenu);
			}
			else
			{
				m_hMenu = ::GetMenu(m_hWnd);
				OnHideMenuBar();
				::SetMenu(m_hWnd, NULL);
			}

			m_dwMenuBarState = dwState;
			return TRUE;
		}

		void SetMessageText(LPCTSTR lpszText) { SendMessage(WM_SETMESSAGESTRING, 0, (LPARAM)lpszText); }
		void SetMessageText(UINT nID) { SendMessage(WM_SETMESSAGESTRING, (WPARAM)nID); }

		void SetTitle(LPCTSTR lpszTitle) { m_strTitle = lpszTitle; }

		void ShowControlBar(CControlBar* pBar, BOOL bShow, BOOL bDelay)
		{
			ENSURE_VALID(pBar);
			CFrameWnd* pParentFrame = pBar->GetDockingFrame();
			ASSERT(pParentFrame->GetTopLevelParent() == GetTopLevelParent());
			// parent frame of bar must be related

			if (bDelay)
			{
				pBar->DelayShow(bShow);
				pParentFrame->DelayRecalcLayout();
			}
			else
			{
				pBar->SetWindowPos(NULL, 0, 0, 0, 0,
					SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE |
					(bShow ? SWP_SHOWWINDOW : SWP_HIDEWINDOW));
				// call DelayShow to clear any contradictory DelayShow
				pBar->DelayShow(bShow);
				if (bShow || !pBar->IsFloating())
					pParentFrame->RecalcLayout(FALSE);
			}

			// show or hide the floating frame as appropriate
			if (pBar->IsFloating())
			{
				int nVisCount = pBar->m_pDockBar != NULL ?
					pBar->m_pDockBar->GetDockedVisibleCount() : bShow ? 1 : 0;
				if (nVisCount == 1 && bShow)
				{
					pParentFrame->m_nShowDelay = -1;
					if (bDelay)
					{
						pParentFrame->m_nShowDelay = SW_SHOWNA;
						pParentFrame->RecalcLayout(FALSE);
					}
					else
						pParentFrame->ShowWindow(SW_SHOWNA);
				}
				else if (nVisCount == 0)
				{
					ASSERT(!bShow);
					pParentFrame->m_nShowDelay = -1;
					if (bDelay)
						pParentFrame->m_nShowDelay = SW_HIDE;
					else
						pParentFrame->ShowWindow(SW_HIDE);
				}
				else if (!bDelay)
				{
					pParentFrame->RecalcLayout(FALSE);
				}
			}
		}

		static BOOL AfxIsDescendant(HWND hWndParent, HWND hWndChild)
			// helper for detecting whether child descendent of parent
			//  (works with owned popups as well)
		{
			ASSERT(::IsWindow(hWndParent));
			ASSERT(::IsWindow(hWndChild));

			do
			{
				if (hWndParent == hWndChild)
					return TRUE;

				hWndChild = AfxGetParentOwner(hWndChild);
			} while (hWndChild != NULL);

			return FALSE;
		}

		void ShowOwnedWindows(BOOL bShow)
		{
			// walk through all top-level windows
			HWND hWnd = ::GetWindow(::GetDesktopWindow(), GW_CHILD);
			while (hWnd != NULL)
			{
				CWnd* pWnd = CWnd::FromHandle(hWnd);
				if (pWnd != NULL && m_hWnd != hWnd && AfxIsDescendant(m_hWnd, hWnd))
				{
					DWORD dwStyle = ::GetWindowLong(hWnd, GWL_STYLE);
					if (!bShow && (dwStyle & (WS_VISIBLE | WS_DISABLED)) == WS_VISIBLE)
					{
						::ShowWindow(hWnd, SW_HIDE);
						pWnd->m_nFlags |= WF_TEMPHIDE;
					}
					// don't show temporarily hidden windows if we're in print preview mode
					else if (bShow && (dwStyle & (WS_VISIBLE | WS_DISABLED)) == 0 &&
						(pWnd->m_nFlags & WF_TEMPHIDE) && !m_lpfnCloseProc)
					{
						::ShowWindow(hWnd, SW_SHOWNOACTIVATE);
						pWnd->m_nFlags &= ~WF_TEMPHIDE;
					}
				}
				hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
			}
		}

		void UpdateFrameTitleForDocument(LPCTSTR lpszDocName)
		{
			CString WindowText;

			if (GetStyle() & FWS_PREFIXTITLE)
			{
				// get name of currently active view
				if (lpszDocName != NULL)
				{
					WindowText += lpszDocName;

					// add current window # if needed
					if (m_nWindow > 0)
					{
						TCHAR szText[32];

						// :%d will produce a maximum of 11 TCHARs
						wsprintf(szText, _T(":%d"), m_nWindow);
						WindowText += szText;
					}
					WindowText += _T(" - ");
				}
				WindowText += m_strTitle;
			}
			else
			{
				// get name of currently active view
				WindowText += m_strTitle;
				if (lpszDocName != NULL)
				{
					WindowText += _T(" - ");
					WindowText += lpszDocName;

					// add current window # if needed
					if (m_nWindow > 0)
					{
						TCHAR szText[32];

						// :%d will produce a maximum of 11 TCHARs
						wsprintf(szText, _T(":%d"), m_nWindow);
						WindowText += szText;
					}
				}
			}

			// set title if changed, but don't remove completely
			// Note: will be excessive for MDI Frame with maximized child
			AfxSetWindowText(m_hWnd, (LPCTSTR)WindowText);
		}
};


BEGIN_MESSAGE_MAP(CFrameWnd, CWnd)
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
#ifndef MTL_REMOVE_CFRAMEWND_DROPFILES
	ON_WM_DROPFILES()
#endif // MTL_REMOVE_CFRAMEWND_DROPFILES

	ON_WM_INITMENU()
	ON_WM_INITMENUPOPUP()
	ON_WM_MENUSELECT()
	ON_WM_MENUCHAR()

	ON_WM_ENTERIDLE()

#ifndef MTL_REMOVE_CFRAMEWND_VIEW
	ON_WM_ERASEBKGND()
#endif	// MTL_REMOVE_CFRAMEWND_VIEW
	ON_WM_SIZE()

	ON_MESSAGE(WM_POPMESSAGESTRING, &CFrameWnd::OnPopMessageString)
	ON_MESSAGE(WM_SETMESSAGESTRING, &CFrameWnd::OnSetMessageString)
	ON_MESSAGE_VOID(WM_IDLEUPDATECMDUI, &CFrameWnd::OnIdleUpdateCmdUI)


#ifndef MTL_REMOVE_CFRAMEWND_CONTROLBAR
	// turning on and off standard frame gadgetry
	ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS_BAR, &CFrameWnd::OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_STATUS_BAR, &CFrameWnd::OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, &CFrameWnd::OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_TOOLBAR, &CFrameWnd::OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_REBAR, &CFrameWnd::OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_REBAR, &CFrameWnd::OnBarCheck)
#endif // MTL_REMOVE_CFRAMEWND_CONTROLBAR

#ifndef MTL_REMOVE_CFRAMEWND_TOOLTIPTEXT
	// toolbar "tooltip" notification
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, &CFrameWnd::OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, &CFrameWnd::OnToolTipText)
#endif	// MTL_REMOVE_CFRAMEWND_TOOLTIPTEXT

#ifndef MTL_REMOVE_CFRAMEWND_INDICATOR
	// turning on and off standard mode indicators
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_CAPS, &CFrameWnd::OnUpdateKeyIndicator)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_NUM, &CFrameWnd::OnUpdateKeyIndicator)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_SCRL, &CFrameWnd::OnUpdateKeyIndicator)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_KANA, &CFrameWnd::OnUpdateKeyIndicator)
#endif	// MTL_REMOVE_CFRAMEWND_INDICATOR
END_MESSAGE_MAP()





class CMiniFrameWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CMiniFrameWnd)

	public:
		CMiniFrameWnd()
		{
			m_bActive = FALSE;
		}

		virtual ~CMiniFrameWnd()
		{
			DestroyWindow();
		}

	public:
		DECLARE_MESSAGE_MAP()

		BOOL m_bActive;
		CString m_strCaption;

	public:
		LRESULT OnFloatStatus(WPARAM wParam, LPARAM)
		{
			// these asserts make sure no conflicting actions are requested
			ASSERT(!((wParam & FS_SHOW) && (wParam & FS_HIDE)));
			ASSERT(!((wParam & FS_ENABLE) && (wParam & FS_DISABLE)));
			ASSERT(!((wParam & FS_ACTIVATE) && (wParam & FS_DEACTIVATE)));

			// FS_SYNCACTIVE is used to detect MFS_SYNCACTIVE windows
			LRESULT lResult = 0;
			if ((GetStyle() & MFS_SYNCACTIVE) && (wParam & FS_SYNCACTIVE))
				lResult = 1;

			if (wParam & (FS_SHOW | FS_HIDE))
			{
				SetWindowPos(NULL, 0, 0, 0, 0,
							((wParam & FS_SHOW) ? SWP_SHOWWINDOW : SWP_HIDEWINDOW) | 
								SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
			}
			if (wParam & (FS_ENABLE | FS_DISABLE))
				EnableWindow((wParam & FS_ENABLE) != 0);

			if ((wParam & (FS_ACTIVATE | FS_DEACTIVATE)) &&
				GetStyle() & MFS_SYNCACTIVE)
			{
				ModifyStyle(MFS_SYNCACTIVE, 0);
				SendMessage(WM_NCACTIVATE, (wParam & FS_ACTIVATE) != 0);
				ModifyStyle(0, MFS_SYNCACTIVE);
			}

			return lResult;
		}

		void OnGetMinMaxInfo(MINMAXINFO* pMMI)
		{
			// allow Windows to fill in the defaults
			CFrameWnd::OnGetMinMaxInfo(pMMI);

			// don't allow sizing smaller than the non-client area
			CRect rectWindow, rectClient;
			GetWindowRect(rectWindow);
			GetClientRect(rectClient);
			pMMI->ptMinTrackSize.x = rectWindow.Width() - rectClient.right;
			pMMI->ptMinTrackSize.y = rectWindow.Height() - rectClient.bottom;
		}

		BOOL OnNcActivate(BOOL /* bActive */)
		{
			if ((GetStyle() & MFS_SYNCACTIVE) == 0)
				return Default() != 0;

			if (m_nFlags & WF_KEEPMINIACTIVE)
				return FALSE;

			return TRUE;
		}

		BOOL OnNcCreate(LPCREATESTRUCT lpcs)
		{
			if (!CFrameWnd::OnNcCreate(lpcs))
				return FALSE;

			if (GetStyle() & MFS_SYNCACTIVE)
			{
				// syncronize activation state with top level parent
				CWnd* pParentWnd = EnsureTopLevelParent();
				CWnd* pActiveWnd = GetForegroundWindow();
				BOOL bActive = (pParentWnd == pActiveWnd) ||
					(pParentWnd->GetLastActivePopup() == pActiveWnd && pActiveWnd->SendMessage(WM_FLOATSTATUS, FS_SYNCACTIVE) != 0);

				// the WM_FLOATSTATUS does the actual work
				SendMessage(WM_FLOATSTATUS, bActive ? FS_ACTIVATE : FS_DEACTIVATE);
			}

			return TRUE;
		}

		LRESULT OnNcHitTest(CPoint point)
		{
			DWORD dwStyle = GetStyle();
			CRect rectWindow;
			GetWindowRect(&rectWindow);

			CSize sizeFrame(GetSystemMetrics(SM_CXFRAME), GetSystemMetrics(SM_CYFRAME));

			LRESULT nHit = CFrameWnd::OnNcHitTest(point);

			// MFS_BLOCKSYSMENU translates system menu hit to caption hit
			if (dwStyle & MFS_BLOCKSYSMENU)
			{
				if (nHit == HTSYSMENU)
					nHit = HTCAPTION;
				if (GetKeyState(VK_RBUTTON) < 0)
					return HTNOWHERE;
			}

			if ((nHit < HTSIZEFIRST || nHit > HTSIZELAST) && nHit != HTGROWBOX)
				return nHit;

			// MFS_MOVEFRAME translates all size requests to move requests
			if (dwStyle & MFS_MOVEFRAME)
				return HTCAPTION;

			// MFS_4THICKFRAME does not allow diagonal sizing
			rectWindow.InflateRect(-sizeFrame.cx, -sizeFrame.cy);
			if (dwStyle & MFS_4THICKFRAME)
			{
				switch (nHit)
				{
					case HTTOPLEFT:
						return point.y < rectWindow.top ? HTTOP : HTLEFT;
					case HTTOPRIGHT:
						return point.y < rectWindow.top ? HTTOP : HTRIGHT;
					case HTBOTTOMLEFT:
						return point.y > rectWindow.bottom ? HTBOTTOM : HTLEFT;
					case HTGROWBOX:
					case HTBOTTOMRIGHT:
						return point.y > rectWindow.bottom ? HTBOTTOM : HTRIGHT;
				}
			}
			return nHit;    // no special translation
		}

		LRESULT OnQueryCenterWnd(WPARAM, LPARAM)
		{
			// forward WM_QUERYCENTERWND to parent window
			HWND hWndParent = ::GetParent(m_hWnd);
			LRESULT lResult = ::SendMessage(hWndParent, WM_QUERYCENTERWND, 0, 0);
			if (lResult == 0)
				lResult = (LRESULT)hWndParent;
			return lResult;
		}

		void OnSysCommand(UINT nID, LPARAM lParam)
		{
			DWORD dwStyle = GetStyle();
			if ((dwStyle & WS_POPUP) && 
				((nID & 0xFFF0) != SC_CLOSE ||
				(GetKeyState(VK_F4) < 0 && GetKeyState(VK_MENU) < 0 && (dwStyle & MFS_SYNCACTIVE))))
			{
				if (HandleFloatingSysCommand(nID, lParam))
					return;
			}
			CFrameWnd::OnSysCommand(nID, lParam);
		}



		static void CalcBorders(LPRECT lpClientRect,DWORD dwStyle = WS_THICKFRAME | WS_CAPTION, DWORD dwExStyle = 0)
		{
			AdjustWindowRectEx(lpClientRect, dwStyle, FALSE, WS_EX_PALETTEWINDOW);
		}

		virtual BOOL Create(LPCTSTR lpClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0)
		{
			return CreateEx(0, lpClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID);
		}

		virtual BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd = NULL, UINT nID = 0)
		{
			m_strCaption = lpszWindowName;
			return CWnd::CreateEx(dwExStyle, lpClassName ? lpClassName :
									AfxRegisterWndClass(CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW)),
									lpszWindowName, dwStyle, rect.left, rect.top, rect.right - rect.left,
									rect.bottom - rect.top, pParentWnd->GetSafeHwnd(), (HMENU)(UINT_PTR)nID);
		}


		virtual BOOL PreCreateWindow(CREATESTRUCT& cs)
		{
			// WS_4THICKFRAME and MFS_THICKFRAME imply WS_THICKFRAME
			if (cs.style & (MFS_4THICKFRAME | MFS_THICKFRAME))
				cs.style |= WS_THICKFRAME;

			// WS_CAPTION implies WS_EX_TOOLWINDOW
			if (cs.style & WS_CAPTION)
				cs.dwExStyle |= WS_EX_TOOLWINDOW;

			VERIFY(CFrameWnd::PreCreateWindow(cs));
			cs.dwExStyle &= ~(WS_EX_CLIENTEDGE);

			return TRUE;
		}
};

BEGIN_MESSAGE_MAP(CMiniFrameWnd, CFrameWnd)
	ON_WM_GETMINMAXINFO()
	ON_WM_NCACTIVATE()
	ON_WM_NCCREATE()
	ON_WM_NCHITTEST()
	ON_WM_SYSCOMMAND()

	ON_MESSAGE(WM_FLOATSTATUS, &CMiniFrameWnd::OnFloatStatus)
	ON_MESSAGE(WM_QUERYCENTERWND, &CMiniFrameWnd::OnQueryCenterWnd)
END_MESSAGE_MAP()



_INLINE void CDockContext::StartDrag(CPoint pt)
{
	ASSERT_VALID(m_pBar);
	m_bDragging = TRUE;

	InitLoop();

	// GetWindowRect returns screen coordinates(not mirrored),
	// so if the desktop is mirrored then turn off mirroring
	// for the desktop dc so that we get correct focus rect drawn.
	// This layout change should be remembered, just in case ...

	if (m_pDC->GetLayout() & LAYOUT_RTL)
		m_pDC->SetLayout(LAYOUT_LTR);

	if (m_pBar->m_dwStyle & CBRS_SIZE_DYNAMIC)
	{
		// get true bar size (including borders)
		CRect rect;
		m_pBar->GetWindowRect(rect);
		m_ptLast = pt;
		CSize sizeHorz = m_pBar->CalcDynamicLayout(0, LM_HORZ | LM_HORZDOCK);
		CSize sizeVert = m_pBar->CalcDynamicLayout(0, LM_VERTDOCK);
		CSize sizeFloat = m_pBar->CalcDynamicLayout(0, LM_HORZ | LM_MRUWIDTH);

		m_rectDragHorz = CRect(rect.TopLeft(), sizeHorz);
		m_rectDragVert = CRect(rect.TopLeft(), sizeVert);

		// calculate frame dragging rectangle
		m_rectFrameDragHorz = CRect(rect.TopLeft(), sizeFloat);
		m_rectFrameDragVert = CRect(rect.TopLeft(), sizeFloat);

		CMiniFrameWnd::CalcBorders(&m_rectFrameDragHorz);
		CMiniFrameWnd::CalcBorders(&m_rectFrameDragVert);

		m_rectFrameDragHorz.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
		m_rectFrameDragVert.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
	}
	else if (m_pBar->m_dwStyle & CBRS_SIZE_FIXED)
	{
		// get true bar size (including borders)
		CRect rect;
		m_pBar->GetWindowRect(rect);
		m_ptLast = pt;
		CSize sizeHorz = m_pBar->CalcDynamicLayout(-1, LM_HORZ | LM_HORZDOCK);
		CSize sizeVert = m_pBar->CalcDynamicLayout(-1, LM_VERTDOCK);

		// calculate frame dragging rectangle
		m_rectFrameDragHorz = m_rectDragHorz = CRect(rect.TopLeft(), sizeHorz);
		m_rectFrameDragVert = m_rectDragVert = CRect(rect.TopLeft(), sizeVert);

		CMiniFrameWnd::CalcBorders(&m_rectFrameDragHorz);
		CMiniFrameWnd::CalcBorders(&m_rectFrameDragVert);
		m_rectFrameDragHorz.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
		m_rectFrameDragVert.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
	}
	else
	{
		// get true bar size (including borders)
		CRect rect;
		m_pBar->GetWindowRect(rect);
		m_ptLast = pt;
		BOOL bHorz = _HORZF(m_dwStyle);
		DWORD dwMode = !bHorz ? (LM_HORZ | LM_HORZDOCK) : LM_VERTDOCK;
		CSize size = m_pBar->CalcDynamicLayout(-1, dwMode);

		// calculate inverted dragging rect
		if (bHorz)
		{
			m_rectDragHorz = rect;
			m_rectDragVert = CRect(CPoint(pt.x - rect.Height() / 2, rect.top), size);
		}
		else // vertical orientation
		{
			m_rectDragVert = rect;
			m_rectDragHorz = CRect(CPoint(rect.left, pt.y - rect.Width() / 2), size);
		}

		// calculate frame dragging rectangle
		m_rectFrameDragHorz = m_rectDragHorz;
		m_rectFrameDragVert = m_rectDragVert;

		CMiniFrameWnd::CalcBorders(&m_rectFrameDragHorz);
		CMiniFrameWnd::CalcBorders(&m_rectFrameDragVert);
		m_rectFrameDragHorz.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
		m_rectFrameDragVert.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
	}

	// adjust rectangles so that point is inside
	_AfxAdjustRectangle(m_rectDragHorz, pt);
	_AfxAdjustRectangle(m_rectDragVert, pt);
	_AfxAdjustRectangle(m_rectFrameDragHorz, pt);
	_AfxAdjustRectangle(m_rectFrameDragVert, pt);

	// initialize tracking state and enter tracking loop
	m_dwOverDockStyle = CanDock();
	Move(pt);   // call it here to handle special keys
	Track();

#if 0
	m_pDC->FillSolidRect(&m_rectFrameDragHorz, 0xff);
	CBrush* pHalfTone = CDC::GetHalftoneBrush() ;
	//m_pDC->FillRect(&m_rectFrameDragHorz, pHalfTone);
	CBrush* pBrushOld = m_pDC->SelectObject(pHalfTone);
	m_pDC->PatBlt(m_rectFrameDragHorz.left, m_rectFrameDragHorz.top, m_rectFrameDragHorz.Width(), m_rectFrameDragHorz.Height(), PATINVERT);
	m_pDC->SelectObject(pBrushOld);
	CancelLoop(); return;
#endif
}

_INLINE void CDockContext::StartResize(int nHitTest, CPoint pt)
{
	ASSERT_VALID(m_pBar);
	ASSERT(m_pBar->m_dwStyle & CBRS_SIZE_DYNAMIC);
	m_bDragging = FALSE;

	InitLoop();

	// GetWindowRect returns screen coordinates(not mirrored)
	// So if the desktop is mirrored then turn off mirroring
	// for the desktop dc so that we draw correct focus rect 

	if (m_pDC->GetLayout() & LAYOUT_RTL)
		m_pDC->SetLayout(LAYOUT_LTR);

	// get true bar size (including borders)
	CRect rect;
	m_pBar->GetWindowRect(rect);
	m_ptLast = pt;
	m_nHitTest = nHitTest;

	CSize size = m_pBar->CalcDynamicLayout(0, LM_HORZ | LM_MRUWIDTH);
	m_rectRequestedSize = CRect(rect.TopLeft(), size);
	m_rectActualSize = CRect(rect.TopLeft(), size);
	m_rectActualFrameSize = CRect(rect.TopLeft(), size);

	// calculate frame rectangle
	CMiniFrameWnd::CalcBorders(&m_rectActualFrameSize);
	m_rectActualFrameSize.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);

	m_rectFrameBorders = CRect(CPoint(0, 0),
		m_rectActualFrameSize.Size() - m_rectActualSize.Size());

	// initialize tracking state and enter tracking loop
	m_dwOverDockStyle = 0;
	Stretch(pt);   // call it here to handle special keys
	Track();
}



class CMiniDockFrameWnd : public CMiniFrameWnd
{
	DECLARE_DYNCREATE(CMiniDockFrameWnd)

	public:
		CMiniDockFrameWnd() : m_wndDockBar(TRUE)
		{
			m_wndDockBar.m_bAutoDelete = FALSE;
		}

	public:
		// メッセージマップ
		DECLARE_MESSAGE_MAP()

		CDockBar m_wndDockBar;

	public:
		void OnClose()
		{
			m_wndDockBar.ShowAll(FALSE);
		}

		int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
		{
			if (nHitTest >= HTSIZEFIRST && nHitTest <= HTSIZELAST) // resizing
				return MA_NOACTIVATE;
			return CMiniFrameWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
		}

		void OnNcLButtonDown(UINT nHitTest, CPoint point)
		{
			if (nHitTest == HTCAPTION)
			{
				// special activation for floating toolbars
				ActivateTopParent();

				// initiate toolbar drag for non-CBRS_FLOAT_MULTI toolbars
				if ((m_wndDockBar.m_dwStyle & CBRS_FLOAT_MULTI) == 0)
				{
					int nPos = 1;
					CControlBar* pBar = NULL;
					while (pBar == NULL && nPos < m_wndDockBar.m_arrBars.GetSize())
						pBar = m_wndDockBar.GetDockedControlBar(nPos++);

					ENSURE_VALID(pBar);
					ASSERT_KINDOF(CControlBar, pBar);
					ENSURE(pBar->m_pDockContext != NULL);
					pBar->m_pDockContext->StartDrag(point);
					return;
				}
			}
			else if (nHitTest >= HTSIZEFIRST && nHitTest <= HTSIZELAST)
			{
				// special activation for floating toolbars
				ActivateTopParent();

				int nPos = 1;
				CControlBar* pBar = NULL;
				while (pBar == NULL && nPos < m_wndDockBar.m_arrBars.GetSize())
					pBar = m_wndDockBar.GetDockedControlBar(nPos++);

				ENSURE_VALID(pBar);
				ASSERT_KINDOF(CControlBar, pBar);
				ENSURE(pBar->m_pDockContext != NULL);

				// CBRS_SIZE_DYNAMIC toolbars cannot have the CBRS_FLOAT_MULTI style
				ASSERT((m_wndDockBar.m_dwStyle & CBRS_FLOAT_MULTI) == 0);
				pBar->m_pDockContext->StartResize(nHitTest, point);
				return;
			}
			CMiniFrameWnd::OnNcLButtonDown(nHitTest, point);
		}

		void OnNcLButtonDblClk(UINT nHitTest, CPoint point)
		{
			if (nHitTest == HTCAPTION)
			{
				// special activation for floating toolbars
				ActivateTopParent();

				// initiate toolbar toggle for non-CBRS_FLOAT_MULTI toolbars
				if ((m_wndDockBar.m_dwStyle & CBRS_FLOAT_MULTI) == 0)
				{
					int nPos = 1;
					CControlBar* pBar = NULL;
					while (pBar == NULL && nPos < m_wndDockBar.m_arrBars.GetSize())
						pBar = m_wndDockBar.GetDockedControlBar(nPos++);

					ENSURE_VALID(pBar);
					ASSERT_KINDOF(CControlBar, pBar);
					ENSURE(pBar->m_pDockContext != NULL);
					pBar->m_pDockContext->ToggleDocking();
					return;
				}
			}
			CMiniFrameWnd::OnNcLButtonDblClk(nHitTest, point);
		}

		BOOL Create(CWnd* pParent, DWORD dwBarStyle)
		{
			// set m_bInRecalcLayout to avoid flashing during creation
			// RecalcLayout will be called once something is docked
			m_bInRecalcLayout = TRUE;

			DWORD dwStyle = WS_POPUP | WS_CAPTION | WS_SYSMENU | MFS_MOVEFRAME |
							MFS_4THICKFRAME | MFS_SYNCACTIVE | MFS_BLOCKSYSMENU |
							FWS_SNAPTOBARS;

			if (dwBarStyle & CBRS_SIZE_DYNAMIC)
				dwStyle &= ~MFS_MOVEFRAME;

			DWORD dwExStyle = 0;
			if (!CMiniFrameWnd::CreateEx(dwExStyle, NULL, _T(""), dwStyle, rectDefault, pParent))
			{
				m_bInRecalcLayout = FALSE;
				return FALSE;
			}
			dwStyle = dwBarStyle & (CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT) ? CBRS_ALIGN_LEFT : CBRS_ALIGN_TOP;
			dwStyle |= dwBarStyle & CBRS_FLOAT_MULTI;

			CMenu* pSysMenu = GetSystemMenu(FALSE);
			if (pSysMenu)
			{
				pSysMenu->DeleteMenu(SC_SIZE, MF_BYCOMMAND);
				pSysMenu->DeleteMenu(SC_MINIMIZE, MF_BYCOMMAND);
				pSysMenu->DeleteMenu(SC_MAXIMIZE, MF_BYCOMMAND);
				pSysMenu->DeleteMenu(SC_RESTORE, MF_BYCOMMAND);

				CString strHide;
				if (strHide.LoadString(AFX_IDS_HIDE))
				{
					pSysMenu->DeleteMenu(SC_CLOSE, MF_BYCOMMAND);
					pSysMenu->AppendMenu(MF_STRING | MF_ENABLED, SC_CLOSE, strHide);
				}
			}

			// must initially create with parent frame as parent
			if (!m_wndDockBar.Create(pParent, WS_CHILD | WS_VISIBLE | dwStyle, AFX_IDW_DOCKBAR_FLOAT))
			{
				m_bInRecalcLayout = FALSE;
				return FALSE;
			}

			// set parent to CMiniDockFrameWnd
			m_wndDockBar.SetParent(this);
			m_bInRecalcLayout = FALSE;

			return TRUE;
		}

		virtual void RecalcLayout(BOOL bNotify)
		{
			if (!m_bInRecalcLayout)
			{
				CMiniFrameWnd::RecalcLayout(bNotify);

				// syncronize window text of frame window with dockbar itself
				TCHAR szTitle[_MAX_PATH];
				m_wndDockBar.GetWindowText(szTitle, _countof(szTitle));
				AfxSetWindowText(m_hWnd, szTitle);
			}
		}

};

BEGIN_MESSAGE_MAP(CMiniDockFrameWnd, CMiniFrameWnd)
	ON_WM_CLOSE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONDBLCLK()
END_MESSAGE_MAP()



_INLINE void CFrameWnd::EnableDocking(DWORD dwDockStyle)
{
	//static const DWORD m_dwDockBarMap[4][2];

	DWORD(*dwDockBarMap)[2] = GetDockBarMap().dwDockBarMap;


	// must be CBRS_ALIGN_XXX or CBRS_FLOAT_MULTI only
	ASSERT((dwDockStyle & ~(CBRS_ALIGN_ANY | CBRS_FLOAT_MULTI)) == 0);

	m_pFloatingFrameClass = RUNTIME_CLASS(CMiniDockFrameWnd);

	for (int i = 0; i < 4; i++)
	{
		if (dwDockBarMap[i][1] & dwDockStyle & CBRS_ALIGN_ANY)
		{
			CDockBar* pDock = (CDockBar*)GetControlBar(dwDockBarMap[i][0]);
			if (pDock == NULL)
			{
				pDock = new CDockBar();
				if (!pDock->Create(this, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CHILD | WS_VISIBLE |
									dwDockBarMap[i][1], dwDockBarMap[i][0]))
				{
					//AfxThrowResourceException();
				}
			}
		}
	}
}

_INLINE CMiniDockFrameWnd* CFrameWnd::CreateFloatingFrame(DWORD dwStyle)
{
	CMiniDockFrameWnd* pFrame = NULL;
	ASSERT(m_pFloatingFrameClass != NULL);
	pFrame = (CMiniDockFrameWnd*)m_pFloatingFrameClass->CreateObject();
	if (pFrame == NULL)
		return NULL;
	ASSERT_KINDOF(CMiniDockFrameWnd, pFrame);
	if (!pFrame->Create(this, dwStyle))
		return NULL;
	return pFrame;
}

_INLINE void CFrameWnd::FloatControlBar(CControlBar* pBar, CPoint point, DWORD dwStyle)
{
	ENSURE_ARG(pBar != NULL);

	// if the bar is already floating and the dock bar only contains this
	// bar and same orientation then move the window rather than recreating
	// the frame
	if (pBar->m_pDockSite != NULL && pBar->m_pDockBar != NULL)
	{
		CDockBar* pDockBar = pBar->m_pDockBar;
		ASSERT_KINDOF(CDockBar, pDockBar);
		if (pDockBar->m_bFloating && pDockBar->GetDockedCount() == 1 &&
			(dwStyle & pDockBar->m_dwStyle & CBRS_ALIGN_ANY) != 0)
		{
			CMiniDockFrameWnd* pDockFrame = (CMiniDockFrameWnd*)pDockBar->GetParent();
			ENSURE(pDockFrame != NULL);
			ASSERT_KINDOF(CMiniDockFrameWnd, pDockFrame);
			pDockFrame->SetWindowPos(NULL, point.x, point.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
			pDockFrame->RecalcLayout(TRUE);
			pDockFrame->UpdateWindow();
			return;
		}
	}

	if (pBar->m_dwStyle & CBRS_SIZE_DYNAMIC)
	{
		dwStyle |= CBRS_SIZE_DYNAMIC;
		if (dwStyle & CBRS_ORIENT_VERT)
		{
			dwStyle &= ~CBRS_ALIGN_ANY;
			dwStyle |= CBRS_ALIGN_TOP;
		}
	}

	CMiniDockFrameWnd* pDockFrame = CreateFloatingFrame(dwStyle);
	ENSURE_ARG(pDockFrame != NULL);
	pDockFrame->SetWindowPos(NULL, point.x, point.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	if (pDockFrame->m_hWndOwner == NULL)
		pDockFrame->m_hWndOwner = pBar->m_hWnd;

	CDockBar* pDockBar = (CDockBar*)pDockFrame->GetDlgItem(AFX_IDW_DOCKBAR_FLOAT);
	ENSURE_ARG(pDockBar != NULL);
	ASSERT_KINDOF(CDockBar, pDockBar);

	ASSERT(pBar->m_pDockSite == this);
	// if this assertion occurred it is because the parent of pBar was not
	//  initially this CFrameWnd when pBar's OnCreate was called
	// (this control bar should have been created with a different
	//  parent initially)

	pDockBar->DockControlBar(pBar);
	pDockFrame->RecalcLayout(TRUE);
	if (::GetWindowLong(pBar->m_hWnd, GWL_STYLE) & WS_VISIBLE)
	{
		pDockFrame->ShowWindow(SW_SHOWNA);
		pDockFrame->UpdateWindow();
	}
}


