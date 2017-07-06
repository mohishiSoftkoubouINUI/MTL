


class CMDIChildWnd ;

class CMDIFrameWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CMDIFrameWnd)

	public:
		CMDIFrameWnd()
		{
			m_hWndMDIClient = NULL;
		}

	public:
		DECLARE_MESSAGE_MAP()


		HWND m_hWndMDIClient;       // MDI Client window handle

	public:
		//virtual LRESULT DefWindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam) { return CWnd::DefWindowProc(nMsg, wParam, lParam); }
		virtual LRESULT DefWindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam) { return ::DefFrameProc(m_hWnd, m_hWndMDIClient, nMsg, wParam, lParam); }


		virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam) ;

		virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext*)
		{
			CMenu* pMenu = NULL;
			if (m_hMenuDefault == NULL)
			{
				// default implementation for MFC V1 backward compatibility
				pMenu = GetMenu();
				ASSERT(pMenu != NULL);
				// This is attempting to guess which sub-menu is the Window menu.
				// The Windows user interface guidelines say that the right-most
				// menu on the menu bar should be Help and Window should be one
				// to the left of that.
				int iMenu = pMenu->GetMenuItemCount() - 2;

				// If this assertion fails, your menu bar does not follow the guidelines
				// so you will have to override this function and call CreateClient
				// appropriately or use the MFC V2 MDI functionality.
				ASSERT(iMenu >= 0);
				pMenu = pMenu->GetSubMenu(iMenu);
				ASSERT(pMenu != NULL);
			}

			return CreateClient(lpcs, pMenu);
		}

		virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) ;
			
		void OnDestroy()
		{
			CFrameWnd::OnDestroy();     // exit and misc cleanup

			// owned menu stored in shared slot for MDIFRAME
			if (m_hMenuDefault != NULL && ::GetMenu(m_hWnd) != m_hMenuDefault)
			{
				// must go through MDI client to get rid of MDI menu additions
				::SendMessage(m_hWndMDIClient, WM_MDISETMENU, (WPARAM)m_hMenuDefault, NULL);
				ASSERT(::GetMenu(m_hWnd) == m_hMenuDefault);
			}
		}

		void OnIdleUpdateCmdUI()
		{
			if (m_nIdleFlags & idleMenu)
			{
				DrawMenuBar();
				m_nIdleFlags &= ~idleMenu;
			}
			CFrameWnd::OnIdleUpdateCmdUI();
		}

		BOOL OnMDIWindowCmd(UINT nID)
		{
			ASSERT(m_hWndMDIClient != NULL);

			UINT msg;
			UINT wParam = 0;
			switch (nID)
			{
				default:
					return FALSE;       // not for us
				case ID_WINDOW_ARRANGE:
					msg = WM_MDIICONARRANGE;
					break;
				case ID_WINDOW_CASCADE:
					msg = WM_MDICASCADE;
					break;
				case ID_WINDOW_TILE_HORZ:
					wParam = MDITILE_HORIZONTAL;
					// fall through
				case ID_WINDOW_TILE_VERT:
					ASSERT(MDITILE_VERTICAL == 0);
					msg = WM_MDITILE;
					break;
			}

			::SendMessage(m_hWndMDIClient, msg, wParam, 0);
			return TRUE;
		}

		void CMDIFrameWnd::OnSize(UINT nType, int, int)
		{
			// do not call default - it will reposition the MDICLIENT
			if (nType != SIZE_MINIMIZED)
				RecalcLayout();
		}

		void OnUpdateMDIWindowCmd(CCmdUI* pCmdUI)
		{
			ASSERT(m_hWndMDIClient != NULL);
			pCmdUI->Enable(MDIGetActive() != NULL);
		}

		void OnWindowNew() ;
			

		virtual BOOL CreateClient(LPCREATESTRUCT lpCreateStruct, CMenu* pWindowMenu)
		{
			ASSERT(m_hWnd != NULL);
			ASSERT(m_hWndMDIClient == NULL);
			DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_BORDER |
							WS_CLIPCHILDREN | WS_CLIPSIBLINGS |
							MDIS_ALLCHILDSTYLES;    // allow children to be created invisible
			DWORD dwExStyle = 0;
			// will be inset by the frame

			// special styles for 3d effect on Win4
			dwStyle &= ~WS_BORDER;
			dwExStyle = WS_EX_CLIENTEDGE;

			CLIENTCREATESTRUCT ccs;
			ccs.hWindowMenu = pWindowMenu->GetSafeHmenu();
			// set hWindowMenu for MFC V1 backward compatibility
			// for MFC V2, window menu will be set in OnMDIActivate
			ccs.idFirstChild = AFX_IDM_FIRST_MDICHILD;

			if (lpCreateStruct->style & (WS_HSCROLL | WS_VSCROLL))
			{
				// parent MDIFrame's scroll styles move to the MDICLIENT
				dwStyle |= (lpCreateStruct->style & (WS_HSCROLL | WS_VSCROLL));

				// fast way to turn off the scrollbar bits (without a resize)
				ModifyStyle(WS_HSCROLL | WS_VSCROLL, 0, SWP_NOREDRAW | SWP_FRAMECHANGED);
			}

			// Create MDICLIENT control with special IDC
			if ((m_hWndMDIClient = CreateWindowEx(dwExStyle, _T("mdiclient"), NULL,
												dwStyle, 0, 0, 0, 0, m_hWnd, (HMENU)AFX_IDW_PANE_FIRST,
												AfxGetInstanceHandle(), (LPVOID)&ccs)) == NULL)
			{
				TRACE(traceAppMsg, 0, "Warning: CMDIFrameWnd::OnCreateClient: failed to create MDICLIENT."
										" GetLastError returns 0x%8.8X\n", ::GetLastError());
				return FALSE;
			}

			CWnd* pCWndMDIClient = FromHandle(m_hWndMDIClient, NULL, m_hWnd);

			// Move it to the top of z-order
			::BringWindowToTop(m_hWndMDIClient);

			return TRUE;
		}

		CMDIChildWnd* CreateNewChild(CRuntimeClass* pClass, UINT nResources, HMENU hMenu = NULL, HACCEL hAccel = NULL) ;

		virtual void DelayUpdateFrameMenu(HMENU hMenuAlt)
		{
			OnUpdateFrameMenu(hMenuAlt);

			m_nIdleFlags |= idleMenu;
		}


		virtual CFrameWnd* GetActiveFrame()
		{
			CMDIChildWnd* pActiveChild = MDIGetActive();
			if (pActiveChild == NULL)
				return this;
			return (CFrameWnd*)pActiveChild;
		}

		/////////////////////////////////////////////////////////////////////////////
		// Smarts for the "Window" menu

		virtual HMENU GetWindowMenuPopup(HMENU hMenuBar) // find which popup is the "Window" menu
		{
			if (hMenuBar == NULL)
				return NULL;

			ASSERT(::IsMenu(hMenuBar));

			int iItem = ::GetMenuItemCount(hMenuBar);
			while (iItem--)
			{
				HMENU hMenuPop = ::GetSubMenu(hMenuBar, iItem);
				if (hMenuPop != NULL)
				{
					int iItemMax = ::GetMenuItemCount(hMenuPop);
					for (int iItemPop = 0; iItemPop < iItemMax; iItemPop++)
					{
						UINT nID = GetMenuItemID(hMenuPop, iItemPop);
						if (nID >= AFX_IDM_WINDOW_FIRST && nID <= AFX_IDM_WINDOW_LAST)
							return hMenuPop;
					}
				}
			}

			// no default menu found
			TRACE(traceAppMsg, 0, "Warning: GetWindowMenuPopup failed!\n");
			return NULL;
		}

		virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL)
		{
			if (!CFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
				return FALSE;

			// save menu to use when no active MDI child window is present
			ASSERT(m_hWnd != NULL);
			m_hMenuDefault = ::GetMenu(m_hWnd);
			return TRUE;
		}

		void MDIActivate(CWnd* pWndActivate) { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWndMDIClient, WM_MDIACTIVATE, (WPARAM)pWndActivate->m_hWnd, 0); }

		void MDIActivate() { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(GetParent()->m_hWnd, WM_MDIACTIVATE, (WPARAM)m_hWnd, 0L); }

		void MDINext() { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWndMDIClient, WM_MDINEXT, 0, 0); }

		CMDIChildWnd* MDIGetActive(BOOL* pbMaximized = NULL) const ;

		virtual BOOL PreCreateWindow(CREATESTRUCT& cs)
		{
			if (cs.lpszClass == NULL)
			{
				VERIFY(AfxDeferRegisterClass(AFX_WNDMDIFRAME_REG));
				cs.lpszClass = _afxWndMDIFrame;
			}
			return TRUE;
		}



		virtual BOOL PreTranslateMessage(MSG* pMsg);
			




} ;

BEGIN_MESSAGE_MAP(CMDIFrameWnd, CFrameWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()

	ON_MESSAGE_VOID(WM_IDLEUPDATECMDUI, &CMDIFrameWnd::OnIdleUpdateCmdUI)

	ON_UPDATE_COMMAND_UI(ID_WINDOW_ARRANGE, &CMDIFrameWnd::OnUpdateMDIWindowCmd)
	ON_COMMAND_EX(ID_WINDOW_ARRANGE, &CMDIFrameWnd::OnMDIWindowCmd)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_CASCADE, &CMDIFrameWnd::OnUpdateMDIWindowCmd)
	ON_COMMAND_EX(ID_WINDOW_CASCADE, &CMDIFrameWnd::OnMDIWindowCmd)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_NEW, &CMDIFrameWnd::OnUpdateMDIWindowCmd)
	ON_COMMAND(ID_WINDOW_NEW, &CMDIFrameWnd::OnWindowNew)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_TILE_HORZ, &CMDIFrameWnd::OnUpdateMDIWindowCmd)
	ON_COMMAND_EX(ID_WINDOW_TILE_HORZ, &CMDIFrameWnd::OnMDIWindowCmd)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_TILE_VERT, &CMDIFrameWnd::OnUpdateMDIWindowCmd)
	ON_COMMAND_EX(ID_WINDOW_TILE_VERT, &CMDIFrameWnd::OnMDIWindowCmd)
END_MESSAGE_MAP()


class CMDIChildWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CMDIChildWnd)

	public:
		CMDIChildWnd()
		{
			m_hMenuShared = NULL;
			m_bPseudoInactive = FALSE;
		}

	public:
		DECLARE_MESSAGE_MAP()


		HMENU m_hMenuShared;        // menu when we are active

		BOOL m_bPseudoInactive;     // TRUE if window is MDI active according to
		//  windows, but not according to MFC...

	public:
		virtual LRESULT DefWindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam) { return ::DefMDIChildProc(m_hWnd, nMsg, wParam, lParam); }

		int OnCreate(LPCREATESTRUCT lpCreateStruct)
		{
			// call base class with lParam context (not MDI one)
			MDICREATESTRUCT* lpmcs;
			lpmcs = (MDICREATESTRUCT*)lpCreateStruct->lpCreateParams;
			CCreateContext* pContext = (CCreateContext*)lpmcs->lParam;

			return OnCreateHelper(lpCreateStruct, pContext);
		}

		void OnDestroy()
		{
			UpdateClientEdge();

			CFrameWnd::OnDestroy();
		}

		void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd*)
		{
			m_bPseudoInactive = FALSE;  // must be happening for real

			// make sure MDI client window has correct client edge
			UpdateClientEdge();

			// send deactivate notification to active view
			CView* pActiveView = GetActiveView();
			if (!bActivate && pActiveView != NULL)
				pActiveView->OnActivateView(FALSE, pActiveView, pActiveView);

			// update titles (don't AddToTitle if deactivate last)
			OnUpdateFrameTitle(bActivate || (pActivateWnd != NULL));

			// re-activate the appropriate view
			if (bActivate)
			{
				if (pActiveView != NULL && GetMDIFrame() == GetActiveWindow())
					pActiveView->OnActivateView(TRUE, pActiveView, pActiveView);
			}

			// update menus
			OnUpdateFrameMenu(bActivate, pActivateWnd, NULL);
			GetMDIFrame()->DrawMenuBar();
		}

		int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
		{
			int nResult = CFrameWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
			if (nResult == MA_NOACTIVATE || nResult == MA_NOACTIVATEANDEAT)
				return nResult;   // frame does not want to activate

			// activate this window if necessary
			CMDIFrameWnd* pFrameWnd = GetMDIFrame();
			ENSURE_VALID(pFrameWnd);
			CMDIChildWnd* pActive = pFrameWnd->MDIGetActive();
			if (pActive != this)
				MDIActivate();

			return nResult;
		}

		BOOL OnNcActivate(BOOL bActive)
		{
			// bypass CFrameWnd::OnNcActivate()
			return CWnd::OnNcActivate(bActive);
		}

		BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct)
		{
			if (!CFrameWnd::OnNcCreate(lpCreateStruct))
				return FALSE;

			// handle extended styles under Win4
			// call PreCreateWindow again just to get dwExStyle
			VERIFY(PreCreateWindow(*lpCreateStruct));
			::SetWindowLong(m_hWnd, GWL_EXSTYLE, lpCreateStruct->dwExStyle);

			return TRUE;
		}

#ifndef MTL_REMOVE_CFRAMEWND_TOOLTIPTEXT
		BOOL OnToolTipText(UINT msg, NMHDR* pNMHDR, LRESULT* pResult)
		{
			ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);
			UNUSED(pNMHDR);

			// check to see if the message is going directly to this window or not
			const MSG* pMsg = GetCurrentMessage();
			if (pMsg->hwnd != m_hWnd)
			{
				// let top level frame handle this for us
				return FALSE;
			}

			// otherwise, handle it ourselves
			return CFrameWnd::OnToolTipText(msg, pNMHDR, pResult);
		}
#endif // MTL_REMOVE_CFRAMEWND_TOOLTIPTEXT

		void OnSize(UINT nType, int cx, int cy)
		{
			CFrameWnd::OnSize(nType, cx, cy);

			// update our parent frame - in case we are now maximized or not
			GetMDIFrame()->OnUpdateFrameTitle(TRUE);
		}

		void OnUpdateFrameMenu(BOOL bActivate, CWnd* pActivateWnd, HMENU hMenuAlt)
		{
			CMDIFrameWnd* pFrame = GetMDIFrame();

			if (hMenuAlt == NULL && bActivate)
			{
				// attempt to get default menu from document
				CDocument* pDoc = GetActiveDocument();
				if (pDoc != NULL)
					hMenuAlt = pDoc->GetDefaultMenu();
			}

			// use default menu stored in frame if none from document
			if (hMenuAlt == NULL)
				hMenuAlt = m_hMenuShared;

			if (hMenuAlt != NULL && bActivate)
			{
				ASSERT(pActivateWnd == this);

				// activating child, set parent menu
				::SendMessage(pFrame->m_hWndMDIClient, WM_MDISETMENU,
					(WPARAM)hMenuAlt, (LPARAM)pFrame->GetWindowMenuPopup(hMenuAlt));
			}
			else if (hMenuAlt != NULL && !bActivate && pActivateWnd == NULL)
			{
				// destroying last child
				HMENU hMenuLast = NULL;
				::SendMessage(pFrame->m_hWndMDIClient, WM_MDISETMENU,
					(WPARAM)pFrame->m_hMenuDefault, (LPARAM)hMenuLast);
			}
			else
			{
				// refresh MDI Window menu (even if non-shared menu)
				::SendMessage(pFrame->m_hWndMDIClient, WM_MDIREFRESHMENU, 0, 0);
			}
		}

		virtual void OnUpdateFrameTitle(BOOL bAddToTitle)
		{
			// update our parent window first
			GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);

			if ((GetStyle() & FWS_ADDTOTITLE) == 0)
				return;     // leave child window alone!

			CDocument* pDocument = GetActiveDocument();
			if (bAddToTitle)
			{
				TCHAR szText[256 + _MAX_PATH];
				lstrcpyn(szText, (pDocument != NULL) ? pDocument->GetTitle() : m_strTitle, _countof(szText));

				if (m_nWindow > 0)
				{
					TCHAR szWinNumber[16 + 1];
					INT iLenNumber = wsprintf(szWinNumber, _T(":%d"), m_nWindow);
					INT iLenText = lstrlen(szText) ;

					if (iLenText + iLenNumber < _countof(szText))
					{
						lstrcpy(szText + iLenText, szWinNumber);
					}
				}

				// set title if changed, but don't remove completely
				AfxSetWindowText(m_hWnd, szText);
			}
		}

		void OnWindowPosChanging(LPWINDOWPOS lpWndPos)
		{
			if (!(lpWndPos->flags & SWP_NOSIZE))
			{
				CRect rectClient;
				if (UpdateClientEdge(rectClient) && (GetStyle() & WS_MAXIMIZE))
				{
					// adjust maximized window size and position based on new
					//  size/position of the MDI client area.
					::AdjustWindowRectEx(rectClient, GetStyle(), FALSE, GetExStyle());
					lpWndPos->x = rectClient.left;
					lpWndPos->y = rectClient.top;
					lpWndPos->cx = rectClient.Width();
					lpWndPos->cy = rectClient.Height();
				}
			}

			CFrameWnd::OnWindowPosChanging(lpWndPos);
		}


		void ActivateFrame(int nCmdShow)
		{
			BOOL bVisibleThen = (GetStyle() & WS_VISIBLE) != 0;
			CMDIFrameWnd* pFrameWnd = GetMDIFrame();
			ASSERT_VALID(pFrameWnd);

			// determine default show command
			if (nCmdShow == -1)
			{
				// get maximized state of frame window (previously active child)
				BOOL bMaximized;
				pFrameWnd->MDIGetActive(&bMaximized);

				// convert show command based on current style
				DWORD dwStyle = GetStyle();
				if (bMaximized || (dwStyle & WS_MAXIMIZE))
					nCmdShow = SW_SHOWMAXIMIZED;
				else if (dwStyle & WS_MINIMIZE)
					nCmdShow = SW_SHOWMINIMIZED;
			}

			// finally, show the window
			CFrameWnd::ActivateFrame(nCmdShow);

			// update the Window menu to reflect new child window
			CMDIFrameWnd* pFrame = GetMDIFrame();
			::SendMessage(pFrame->m_hWndMDIClient, WM_MDIREFRESHMENU, 0, 0);

			// Note: Update the m_bPseudoInactive flag.  This is used to handle the
			//  last MDI child getting hidden.  Windows provides no way to deactivate
			//  an MDI child window.

			BOOL bVisibleNow = (GetStyle() & WS_VISIBLE) != 0;
			if (bVisibleNow == bVisibleThen)
				return;

			if (!bVisibleNow)
			{
				// get current active window according to Windows MDI
				HWND hWnd = (HWND)::SendMessage(pFrameWnd->m_hWndMDIClient, WM_MDIGETACTIVE, 0, 0);
				if (hWnd != m_hWnd)
				{
					// not active any more -- window must have been deactivated
					ASSERT(!m_bPseudoInactive);
					return;
				}

				// check next window
				ASSERT(hWnd != NULL);
				pFrameWnd->MDINext();

				// see if it has been deactivated now...
				hWnd = (HWND)::SendMessage(pFrameWnd->m_hWndMDIClient, WM_MDIGETACTIVE, 0, 0);
				if (hWnd == m_hWnd)
				{
					// still active -- fake deactivate it
					ASSERT(hWnd != NULL);
					::SendMessage(pFrameWnd->m_hWndMDIClient, WM_MDIACTIVATE, (WPARAM)m_hWnd, NULL);
					m_bPseudoInactive = TRUE;   // so MDIGetActive returns NULL
				}
			}
			else if (m_bPseudoInactive)
			{
				// if state transitioned from not visible to visible, but
				//  was pseudo deactivated -- send activate notify now
				::SendMessage(pFrameWnd->m_hWndMDIClient, WM_MDIACTIVATE, NULL, (LPARAM)m_hWnd);
				ASSERT(!m_bPseudoInactive); // should get set in OnMDIActivate!
			}
		}


		virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
							DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
							const RECT& rect = rectDefault,
							CMDIFrameWnd* pParentWnd = NULL,
							CCreateContext* pContext = NULL) ;
			
		virtual BOOL DestroyWindow()
		{
			if (m_hWnd == NULL)
				return FALSE;

			// avoid changing the caption during the destroy message(s)
			CMDIFrameWnd* pFrameWnd = GetMDIFrame();
			HWND hWndFrame = pFrameWnd->m_hWnd;
			ASSERT(::IsWindow(hWndFrame));
			DWORD dwStyle = ::SetWindowLong(hWndFrame, GWL_STYLE, ::GetWindowLong(hWndFrame, GWL_STYLE) & ~FWS_ADDTOTITLE);

			MDIDestroy();

			if (::IsWindow(hWndFrame))
			{
				ASSERT(hWndFrame == pFrameWnd->m_hWnd);
				::SetWindowLong(hWndFrame, GWL_STYLE, dwStyle);
				pFrameWnd->OnUpdateFrameTitle(TRUE);
			}

			return TRUE;
		}

		// walk up two parents for MDIFrame that owns MDIChild (skip MDIClient)
		CMDIFrameWnd* GetMDIFrame()
		{
			ASSERT_KINDOF(CMDIChildWnd, this);
			ASSERT(m_hWnd != NULL);
			HWND hWndMDIClient = ::GetParent(m_hWnd);
			ASSERT(hWndMDIClient != NULL);

			CMDIFrameWnd* pMDIFrame;
			pMDIFrame = (CMDIFrameWnd*)CWnd::FromHandle(::GetParent(hWndMDIClient));
			ASSERT(pMDIFrame != NULL);
			ASSERT_KINDOF(CMDIFrameWnd, pMDIFrame);
			ASSERT(pMDIFrame->m_hWndMDIClient == hWndMDIClient);
			ASSERT_VALID(pMDIFrame);
			return pMDIFrame;
		}

		virtual BOOL IsTabbedMDIChild() { return FALSE; }

		virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext = NULL);


		void MDIActivate() { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(GetParent()->m_hWnd, WM_MDIACTIVATE, (WPARAM)m_hWnd, 0L); }
		void MDIDestroy() { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(GetParent()->m_hWnd, WM_MDIDESTROY, (WPARAM)m_hWnd, 0L); }

		virtual BOOL PreCreateWindow(CREATESTRUCT& cs)
		{
			ASSERT(cs.style & WS_CHILD);
			// MFC V2 requires that MDI Children are created with proper styles,
			//  usually: WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW.
			// See Technical note TN019 for more details on MFC V1->V2 migration.

			return CFrameWnd::PreCreateWindow(cs);
		}



		virtual BOOL PreTranslateMessage(MSG* pMsg)
		{
			// allow tooltip messages to be filtered
			if (CWnd::PreTranslateMessage(pMsg))
				return TRUE;

			// we can't call 'CFrameWnd::PreTranslate' since it will translate
			//  accelerators in the context of the MDI Child - but since MDI Child
			//  windows don't have menus this doesn't work properly.  MDI Child
			//  accelerators must be translated in context of their MDI Frame.

			if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
			{
				// use document specific accelerator table over m_hAccelTable
				HACCEL hAccel = GetDefaultAccelerator();
				return hAccel != NULL && ::TranslateAccelerator(GetMDIFrame()->m_hWnd, hAccel, pMsg);
			}
			return FALSE;
		}

		BOOL UpdateClientEdge(LPRECT lpRect = NULL)
		{
			// only adjust for active MDI child window
			CMDIFrameWnd* pFrameWnd = GetMDIFrame();
			CMDIChildWnd* pChild = pFrameWnd->MDIGetActive();

			// Only adjust for regular MDI child windows, not tabbed windows.  Attempting to set WS_EX_CLIENTEDGE on the tabbed
			// MDI client area window is subverted by CMDIClientAreaWnd::OnStyleChanging, so we always try to reset the style and
			// always repaint, none of which is necessary since the tabbed MDI children never change from maximized to restored.
			BOOL bIsTabbedMDIChild = pChild != NULL && pChild->IsTabbedMDIChild();

			if ((pChild == NULL || pChild == this) && !bIsTabbedMDIChild)
			{
				// need to adjust the client edge style as max/restore happens
				DWORD dwStyle = ::GetWindowLong(pFrameWnd->m_hWndMDIClient, GWL_EXSTYLE);
				DWORD dwNewStyle = dwStyle;
				if (pChild != NULL && !(GetExStyle() & WS_EX_CLIENTEDGE) && (GetStyle() & WS_MAXIMIZE))
				{
					dwNewStyle &= ~(WS_EX_CLIENTEDGE);
				}
				else
				{
					dwNewStyle |= WS_EX_CLIENTEDGE;
				}

				if (dwStyle != dwNewStyle)
				{
					// SetWindowPos will not move invalid bits
					::RedrawWindow(pFrameWnd->m_hWndMDIClient, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);

					// remove/add WS_EX_CLIENTEDGE to MDI client area
					::SetWindowLong(pFrameWnd->m_hWndMDIClient, GWL_EXSTYLE, dwNewStyle);
					::SetWindowPos(pFrameWnd->m_hWndMDIClient, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOCOPYBITS);

					// return new client area
					if (lpRect != NULL)
					{
						::GetClientRect(pFrameWnd->m_hWndMDIClient, lpRect);
					}

					return TRUE;
				}
			}

			return FALSE;
		}

		void SetHandles(HMENU hMenu, HACCEL hAccel)
		{
			m_hMenuShared = hMenu;
			m_hAccelTable = hAccel;
		}


};

BEGIN_MESSAGE_MAP(CMDIChildWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_MDIACTIVATE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_NCACTIVATE()
	ON_WM_NCCREATE()
	ON_WM_SIZE()
	ON_WM_WINDOWPOSCHANGING()
#ifndef MTL_REMOVE_CFRAMEWND_TOOLTIPTEXT
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, &CMDIChildWnd::OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, &CMDIChildWnd::OnToolTipText)
#endif // MTL_REMOVE_CFRAMEWND_TOOLTIPTEXT
END_MESSAGE_MAP()


_INLINE BOOL CMDIFrameWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// send to MDI child first - will be re-sent through OnCmdMsg later
	CMDIChildWnd* pActiveChild = MDIGetActive();
	if (pActiveChild != NULL && pActiveChild->WindowProc(WM_COMMAND, wParam, lParam) != 0)
		return TRUE; // handled by child

	if (CFrameWnd::OnCommand(wParam, lParam))
		return TRUE; // handled through normal mechanism (MDI child or frame)

	HWND hWndCtrl = (HWND)lParam;

	ASSERT(AFX_IDM_FIRST_MDICHILD == 0xFF00);
	if (hWndCtrl == NULL && (LOWORD(wParam) & 0xf000) == 0xf000)
	{
		// menu or accelerator within range of MDI children
		// default frame proc will handle it
		DefWindowProc(WM_COMMAND, wParam, lParam);
		return TRUE;
	}

	return FALSE;   // not handled
}

_INLINE BOOL CMDIFrameWnd::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	CMDIChildWnd* pActiveChild = MDIGetActive();
	// pump through active child FIRST
	if (pActiveChild != NULL)
	{
		if (pActiveChild->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;
	}

	// then pump through normal frame
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


_INLINE void CMDIFrameWnd::OnWindowNew()
{
	CMDIChildWnd* pActiveChild = MDIGetActive();
	CDocument* pDocument;
	if (pActiveChild == NULL || (pDocument = pActiveChild->GetActiveDocument()) == NULL)
	{
		TRACE(traceAppMsg, 0, "Warning: No active document for WindowNew command.\n");
		AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
		return;     // command failed
	}

	// otherwise we have a new frame !
	CDocTemplate* pTemplate = pDocument->GetDocTemplate();
	ASSERT_VALID(pTemplate);
	CFrameWnd* pFrame = pTemplate->CreateNewFrame(pDocument, pActiveChild);
	if (pFrame == NULL)
	{
		TRACE(traceAppMsg, 0, "Warning: failed to create new frame.\n");
		return;     // command failed
	}

	pTemplate->InitialUpdateFrame(pFrame, pDocument);
}


_INLINE CMDIChildWnd* CMDIFrameWnd::CreateNewChild(CRuntimeClass* pClass,
													UINT nResources, HMENU hMenu /* = NULL */, HACCEL hAccel /* = NULL */)
{
	ASSERT(pClass != NULL);
	CMDIChildWnd* pFrame = (CMDIChildWnd*)pClass->CreateObject();
	ASSERT_KINDOF(CMDIChildWnd, pFrame);

	// load the frame
	CCreateContext context;
	context.m_pCurrentFrame = this;

	pFrame->SetHandles(hMenu, hAccel);
	if (!pFrame->LoadFrame(nResources, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, &context))
	{
		TRACE(traceAppMsg, 0, "Couldn't load frame window.\n");
		return NULL;
	}

	CString strFullString, strTitle;
	if (strFullString.LoadString(nResources))
		AfxExtractSubString(strTitle, strFullString, CDocTemplate::docName);

	// redraw the frame and parent
	pFrame->SetTitle(strTitle);
	pFrame->InitialUpdateFrame(NULL, TRUE);

	return pFrame;
}

_INLINE CMDIChildWnd* CMDIFrameWnd::MDIGetActive(BOOL* pbMaximized) const
{
	// check first for MDI client window not created
	if (m_hWndMDIClient == NULL)
	{
		if (pbMaximized != NULL)
			*pbMaximized = FALSE;
		return NULL;
	}

	// MDI client has been created, get active MDI child
	HWND hWnd = (HWND)::SendMessage(m_hWndMDIClient, WM_MDIGETACTIVE, 0, (LPARAM)pbMaximized);
	CMDIChildWnd* pWnd = (CMDIChildWnd*)CWnd::FromHandlePermanent(hWnd);
	ASSERT(pWnd == NULL || pWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)));

	// check for special pseudo-inactive state
	if (pWnd != NULL && pWnd->m_bPseudoInactive && (pWnd->GetStyle() & WS_VISIBLE) == 0)
	{
		// Window is hidden, active, but m_bPseudoInactive -- return NULL
		pWnd = NULL;
		// Ignore maximized flag if pseudo-inactive and maximized
		if (pbMaximized != NULL)
			*pbMaximized = FALSE;
	}
	return pWnd;
}

_INLINE BOOL CMDIFrameWnd::PreTranslateMessage(MSG* pMsg)
{
	// allow tooltip messages to be filtered
	if (CWnd::PreTranslateMessage(pMsg))
		return TRUE;

	CMDIChildWnd* pActiveChild = MDIGetActive();

	// current active child gets first crack at it
	if (pActiveChild != NULL && pActiveChild->PreTranslateMessage(pMsg))
		return TRUE;

	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		// translate accelerators for frame and any children
		if (m_hAccelTable != NULL &&
			::TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg))
		{
			return TRUE;
		}

		// special processing for MDI accelerators last
		// and only if it is not in SDI mode (print preview)
		if (GetActiveView() == NULL)
		{
			if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN)
			{
				// the MDICLIENT window may translate it
				if (::TranslateMDISysAccel(m_hWndMDIClient, pMsg))
					return TRUE;
			}
		}
	}

	return FALSE;
}



// MDI support (zero or more documents)
class CMultiDocTemplate : public CDocTemplate
{
	DECLARE_DYNAMIC(CMultiDocTemplate)

	public:
		CMultiDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass)
			: CDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
		{
			ASSERT(m_docList.IsEmpty());

			m_hMenuShared = NULL;
			m_hAccelTable = NULL;
			m_nUntitledCount = 0;   // start at 1

			LoadTemplate();
		}


		virtual ~CMultiDocTemplate()
		{
			// delete shared components
			if (m_hMenuShared != NULL)
				::DestroyMenu(m_hMenuShared);
			if (m_hAccelTable != NULL)
				::FreeResource((HGLOBAL)m_hAccelTable);
		}

	public:
		// Menu and accel table for MDI Child windows of this type
		HMENU m_hMenuShared;
		HACCEL m_hAccelTable;

		CPtrList m_docList;          // open documents of this type
		UINT m_nUntitledCount;   // start at 0, for "Document1" title

	public:
		virtual void AddDocument(CDocument* pDoc)
		{
			ASSERT_VALID(pDoc);

			CDocTemplate::AddDocument(pDoc);
			ASSERT(m_docList.Find(pDoc, NULL) == NULL); // must not be in list
			m_docList.AddTail(pDoc);
		}

		virtual POSITION GetFirstDocPosition() const { return m_docList.GetHeadPosition(); }
		virtual CDocument* GetNextDoc(POSITION& rPos) const { return (CDocument*)m_docList.GetNext(rPos); }

		virtual void LoadTemplate()
		{
			CDocTemplate::LoadTemplate();

			if (m_nIDResource != 0 && m_hMenuShared == NULL)
			{
				HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(m_nIDResource), RT_MENU);
				m_hMenuShared = ::LoadMenuW(hInst, MAKEINTRESOURCEW(m_nIDResource));
				m_hAccelTable = ::LoadAcceleratorsW(hInst, MAKEINTRESOURCEW(m_nIDResource));
			}
		}

		virtual CDocument* OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible = TRUE) { return OpenDocumentFile(lpszPathName, TRUE, bMakeVisible); }

		virtual CDocument* OpenDocumentFile(LPCTSTR lpszPathName, BOOL bAddToMRU, BOOL bMakeVisible) ;

		virtual void RemoveDocument(CDocument* pDoc)
		{
			ASSERT_VALID(pDoc);

			CDocTemplate::RemoveDocument(pDoc);
			m_docList.RemoveAt(m_docList.Find(pDoc));
		}

		virtual void SetDefaultTitle(CDocument* pDocument)
		{
			CString strDocName;
			if (GetDocString(strDocName, CDocTemplate::docName) && !strDocName.IsEmpty())
			{
				TCHAR szNum[16];
				wsprintf(szNum, _T("%d"), m_nUntitledCount + 1);
				strDocName += szNum;
			}
			else
			{
				// use generic 'untitled' - ignore untitled count
				ENSURE(strDocName.LoadString(AFX_IDS_UNTITLED));
			}
			pDocument->SetTitle(strDocName);
		}

} ;

_INLINE BOOL CMDIChildWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	// only do this once
	ASSERT_VALID_IDR(nIDResource);
	ASSERT(m_nIDHelp == 0 || m_nIDHelp == nIDResource);

	m_nIDHelp = nIDResource;    // ID for help context (+HID_BASE_RESOURCE)

	// parent must be MDI Frame (or NULL for default)
	ASSERT(pParentWnd == NULL || pParentWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)));
	// will be a child of MDIClient
	ASSERT(!(dwDefaultStyle & WS_POPUP));
	dwDefaultStyle |= WS_CHILD;

#ifndef MTL_REMOVE_CWINAPP_DOCMANAGER
	// if available - get MDI child menus from doc template
	CMultiDocTemplate* pTemplate;
	if (pContext != NULL && (pTemplate = (CMultiDocTemplate*)pContext->m_pNewDocTemplate) != NULL)
	{
		ASSERT_KINDOF(CMultiDocTemplate, pTemplate);
		// get shared menu from doc template
		m_hMenuShared = pTemplate->m_hMenuShared;
		m_hAccelTable = pTemplate->m_hAccelTable;
	}
	else
	{
		TRACE(traceAppMsg, 0, "Warning: no shared menu/acceltable for MDI Child window.\n");
		// if this happens, programmer must load these manually
	}
#endif // MTL_REMOVE_CWINAPP_DOCMANAGER

	CString strFullString, strTitle;
	if (strFullString.LoadString(nIDResource))
		AfxExtractSubString(strTitle, strFullString, 0);    // first sub-string

	ASSERT(m_hWnd == NULL);
	if (!Create(GetIconWndClass(dwDefaultStyle, nIDResource), strTitle, dwDefaultStyle, rectDefault, (CMDIFrameWnd*)pParentWnd, pContext))
	{
		return FALSE;   // will self destruct on failure normally
	}

	// it worked !
	return TRUE;
}

