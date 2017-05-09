





// for getting access to the actual controls
_INLINE void DDX_Control(CDataExchange* pDX, int nIDC, CWnd& rControl)
{
	if (rControl.m_hWnd == NULL)    // not subclassed yet
	{
		ASSERT(!pDX->m_bSaveAndValidate);

		pDX->PrepareCtrl(nIDC);
		HWND hWndCtrl;
		pDX->m_pDlgWnd->GetDlgItem(nIDC, &hWndCtrl);

		if ((hWndCtrl != NULL) && !rControl.SubclassWindow(hWndCtrl))
		{
			ASSERT(FALSE);      // possibly trying to subclass twice?
			AfxThrowNotSupportedException();
		}
	}
}


// Operations to do in ON_UPDATE_COMMAND_UI
_INLINE void CCmdUI::Enable(BOOL bOn)
{
	if (m_pMenu != NULL)
	{
		if (m_pSubMenu != NULL)
			return; // don't change popup menus indirectly

		m_pMenu->EnableMenuItem(m_nIndex, MF_BYPOSITION | (bOn ? MF_ENABLED : (MF_DISABLED | MF_GRAYED)));
	}
	else
	{
		// enable/disable a control (i.e. child window)

		// if control has the focus, move the focus before disabling
		if (!bOn && (::GetFocus() == m_pOther->m_hWnd))
			m_pOther->GetParent()->SendMessage(WM_NEXTDLGCTL, 0, FALSE);
		m_pOther->EnableWindow(bOn);
	}
	m_bEnableChanged = TRUE;
}

_INLINE void CCmdUI::SetCheck(int nCheck)   // 0, 1 or 2 (indeterminate)
{
	if (m_pMenu != NULL)
	{
		if (m_pSubMenu != NULL)
			return; // don't change popup menus indirectly

		// place checkmark next to menu item

		m_pMenu->CheckMenuItem(m_nIndex, MF_BYPOSITION |
			(nCheck ? MF_CHECKED : MF_UNCHECKED));
	}
	else
	{
		// we can only check buttons or controls acting like buttons

		if (m_pOther->SendMessage(WM_GETDLGCODE) & DLGC_BUTTON)
			m_pOther->SendMessage(BM_SETCHECK, nCheck);
		// otherwise ignore it
	}
}

_INLINE void CCmdUI::SetRadio(BOOL bOn)
{
	SetCheck(bOn ? 1 : 0); // this default works for most things as well
	if (m_pMenu != NULL)
	{
		if (m_pSubMenu != NULL)
			return; // don't change popup menus indirectly

		// for menu item - use dot instead of checkmark

		::CheckMenuItem(m_pMenu->m_hMenu, m_nIndex, MF_BYPOSITION | ((bOn) ? MF_CHECKED : MF_UNCHECKED));
	}
}

_INLINE void CCmdUI::SetText(LPCTSTR lpszText)
{
	if (m_pMenu != NULL)
	{
		if (m_pSubMenu != NULL)
			return; // don't change popup menus indirectly

		// set menu text

#if 0
		MENUITEMINFO menuInfo;
		menuInfo.cbSize = sizeof(MENUITEMINFO);
		menuInfo.fMask = MIIM_STRING;
		menuInfo.dwTypeData = LPTSTR(lpszText);
		VERIFY(m_pMenu->SetMenuItemInfo(m_nIndex, &menuInfo, TRUE));
#else
		// get current menu state so it doesn't change
		UINT nState = m_pMenu->GetMenuState(m_nIndex, MF_BYPOSITION);
		nState &= ~(MF_BITMAP | MF_OWNERDRAW | MF_SEPARATOR);

		// set menu text
		ASSERT(m_nIndex < m_nIndexMax);
		VERIFY(m_pMenu->ModifyMenu(m_nIndex, MF_BYPOSITION | MF_STRING | nState, m_nID, lpszText));
#endif
	}
	else
	{
		AfxSetWindowText(m_pOther->m_hWnd, lpszText);
	}
}



_INLINE BOOL CMenu::TrackPopupMenu(UINT nFlags, int x, int y, CWnd* pWnd, LPCRECT lpRect)
{
	ASSERT(m_hMenu != NULL);

	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
	HWND hWndOld = pThreadState->m_hTrackingWindow;
	HMENU hMenuOld = pThreadState->m_hTrackingMenu;
	pThreadState->m_hTrackingWindow = pWnd->GetSafeHwnd();
	pThreadState->m_hTrackingMenu = m_hMenu;
	BOOL bOK = ::TrackPopupMenu(m_hMenu, nFlags, x, y, 0, pThreadState->m_hTrackingWindow, lpRect);
	pThreadState->m_hTrackingWindow = hWndOld;
	pThreadState->m_hTrackingMenu = hMenuOld;

	return bOK;
}



_INLINE CScrollBar* CWMHnd::GetScrollBarCtrl(int) const
{
	return NULL;        // no special scrollers supported
}

_INLINE int CWMHnd::SetScrollPos(int nBar, int nPos, BOOL bRedraw)
{
	CScrollBar* pScrollBar;
	if ((pScrollBar = GetScrollBarCtrl(nBar)) != NULL)
		return pScrollBar->SetScrollPos(nPos, bRedraw);
	else
		return ::SetScrollPos(m_hWnd, nBar, nPos, bRedraw);
}

_INLINE int CWMHnd::GetScrollPos(int nBar) const
{
	CScrollBar* pScrollBar;
	if ((pScrollBar = GetScrollBarCtrl(nBar)) != NULL)
		return pScrollBar->GetScrollPos();
	else
		return ::GetScrollPos(m_hWnd, nBar);
}

_INLINE void CWMHnd::SetScrollRange(int nBar, int nMinPos, int nMaxPos, BOOL bRedraw)
{
	CScrollBar* pScrollBar;
	if ((pScrollBar = GetScrollBarCtrl(nBar)) != NULL)
		pScrollBar->SetScrollRange(nMinPos, nMaxPos, bRedraw);
	else
		::SetScrollRange(m_hWnd, nBar, nMinPos, nMaxPos, bRedraw);
}

_INLINE void CWMHnd::GetScrollRange(int nBar, LPINT lpMinPos, LPINT lpMaxPos) const
{
	CScrollBar* pScrollBar;
	if ((pScrollBar = GetScrollBarCtrl(nBar)) != NULL)
		pScrollBar->GetScrollRange(lpMinPos, lpMaxPos);
	else
		::GetScrollRange(m_hWnd, nBar, lpMinPos, lpMaxPos);
}

// Turn on/off non-control scrollbars
//   for WS_?SCROLL scrollbars - show/hide them
//   for control scrollbar - enable/disable them
_INLINE void CWMHnd::EnableScrollBarCtrl(int nBar, BOOL bEnable)
{
	CScrollBar* pScrollBar;
	if (nBar == SB_BOTH)
	{
		EnableScrollBarCtrl(SB_HORZ, bEnable);
		EnableScrollBarCtrl(SB_VERT, bEnable);
	}
	else if ((pScrollBar = GetScrollBarCtrl(nBar)) != NULL)
	{
		// control scrollbar - enable or disable
		pScrollBar->EnableWindow(bEnable);
	}
	else
	{
		// WS_?SCROLL scrollbar - show or hide
		ShowScrollBar(nBar, bEnable);
	}
}

_INLINE BOOL CWMHnd::SetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, BOOL bRedraw)
{
	ASSERT(lpScrollInfo != NULL);

	HWND hWnd = m_hWnd;
	CScrollBar* pScrollBar;
	if (nBar != SB_CTL && (pScrollBar = GetScrollBarCtrl(nBar)) != NULL)
	{
		hWnd = pScrollBar->m_hWnd;
		nBar = SB_CTL;
	}
	lpScrollInfo->cbSize = sizeof(*lpScrollInfo);
	::SetScrollInfo(hWnd, nBar, lpScrollInfo, bRedraw);
	return TRUE;
}

_INLINE BOOL CWMHnd::GetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, UINT nMask)
{
	ASSERT(lpScrollInfo != NULL);

	HWND hWnd = m_hWnd;
	CScrollBar* pScrollBar;
	if (nBar != SB_CTL && (pScrollBar = GetScrollBarCtrl(nBar)) != NULL)
	{
		hWnd = pScrollBar->m_hWnd;
		nBar = SB_CTL;
	}
	lpScrollInfo->cbSize = sizeof(*lpScrollInfo);
	lpScrollInfo->fMask = nMask;
	return ::GetScrollInfo(hWnd, nBar, lpScrollInfo);
}



_INLINE void CWnd::OnHScroll(UINT, UINT, CScrollBar* pScrollBar)
{
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;     // eat it

	Default();
}

_INLINE void CWnd::OnVScroll(UINT, UINT, CScrollBar* pScrollBar)
{
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;     // eat it

	Default();
}
