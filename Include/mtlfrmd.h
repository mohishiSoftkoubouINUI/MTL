




_INLINE CFrameWnd* CWnd::GetTopLevelFrame()
{
	if (GetSafeHwnd() == NULL) // no Window attached
		return NULL;

	ASSERT_VALID(this);

	CFrameWnd* pFrameWnd = (CFrameWnd*)this;
	if (!IsFrameWnd())
		pFrameWnd = GetParentFrame();

	if (pFrameWnd != NULL)
	{
		CFrameWnd* pTemp;
		while ((pTemp = pFrameWnd->GetParentFrame()) != NULL)
			pFrameWnd = pTemp;
	}
	return pFrameWnd;
}



_INLINE CDockContext::CDockContext(CControlBar* pBar) : afxData(CAfx::GetAfxData())
{
	ASSERT(pBar != NULL);
	ASSERT(pBar->m_pDockSite != NULL);

	m_pBar = pBar;
	m_pDockSite = pBar->m_pDockSite;

	m_uMRUDockID = 0;
	m_rectMRUDockPos.left = 0;
	m_rectMRUDockPos.top = 0;
	if (pBar->m_dwStyle & CBRS_SIZE_DYNAMIC)
		m_dwMRUFloatStyle = pBar->m_dwStyle & (CBRS_ALIGN_TOP | CBRS_SIZE_DYNAMIC);
	else if (pBar->m_dwStyle & CBRS_ORIENT_HORZ)
		m_dwMRUFloatStyle = CBRS_ALIGN_TOP | (pBar->m_dwStyle & CBRS_FLOAT_MULTI);
	else
		m_dwMRUFloatStyle = CBRS_ALIGN_LEFT | (pBar->m_dwStyle & CBRS_FLOAT_MULTI);
	m_ptMRUFloatPos.x = CW_USEDEFAULT;

	ASSERT(m_pDockSite->IsFrameWnd());
	m_pDC = NULL;
}

_INLINE DWORD CDockContext::CanDock()
{
	DWORD dwDock = 0; // Dock Canidate
	DWORD dwCurr = 0; // Current Orientation

	// let's check for something in our current orientation first
	// then if the shift key is not forcing our orientation then
	// check for horizontal or vertical orientations as long
	// as we are close enough
	ASSERT(m_dwStyle != 0);

	BOOL bStyleHorz = _HORZF(m_dwStyle);
	bStyleHorz = m_bFlip ? !bStyleHorz : bStyleHorz;

	if (bStyleHorz && _HORZF(m_dwDockStyle))
		dwDock = m_pDockSite->CanDock(m_rectDragHorz, m_dwDockStyle & ~CBRS_ORIENT_VERT);
	else if (_VERTF(m_dwDockStyle))
		dwDock = m_pDockSite->CanDock(m_rectDragVert, m_dwDockStyle & ~CBRS_ORIENT_HORZ);

	if (!m_bFlip)
	{
		if (dwDock == 0 && _HORZF(m_dwDockStyle))
		{
			dwCurr = m_pDockSite->CanDock(m_rectDragVert, m_dwDockStyle & ~CBRS_ORIENT_VERT);
			dwDock = m_pDockSite->CanDock(m_rectDragHorz, m_dwDockStyle & ~CBRS_ORIENT_VERT);
			dwDock = (dwDock == dwCurr) ? dwDock : 0;
		}
		if (dwDock == 0 && _VERTF(m_dwDockStyle))
		{
			dwCurr = m_pDockSite->CanDock(m_rectDragHorz, m_dwDockStyle & ~CBRS_ORIENT_HORZ);
			dwDock = m_pDockSite->CanDock(m_rectDragVert, m_dwDockStyle & ~CBRS_ORIENT_HORZ);
			dwDock = (dwDock == dwCurr) ? dwDock : 0;
		}
	}

	return dwDock;
}

_INLINE void CDockContext::EndDrag()
{
	CancelLoop();

	if (m_dwOverDockStyle != 0)
	{
		CDockBar* pDockBar = GetDockBar(m_dwOverDockStyle);
		ASSERT(pDockBar != NULL);

		CRect rect = (m_dwOverDockStyle & CBRS_ORIENT_VERT) ?
		m_rectDragVert : m_rectDragHorz;

		UINT uID = _AfxGetDlgCtrlID(pDockBar->m_hWnd);
		if (uID >= AFX_IDW_DOCKBAR_TOP && uID <= AFX_IDW_DOCKBAR_BOTTOM)
		{
			m_uMRUDockID = uID;
			m_rectMRUDockPos = rect;
			pDockBar->ScreenToClient(&m_rectMRUDockPos);
		}

		// dock it at the specified position, RecalcLayout will snap
		m_pDockSite->DockControlBar(m_pBar, pDockBar, &rect);
		m_pDockSite->RecalcLayout();

		m_pBar->UpdateWindow();
	}
	else if ((m_dwStyle & CBRS_SIZE_DYNAMIC) || (_HORZF(m_dwStyle) && !m_bFlip) || (_VERTF(m_dwStyle) && m_bFlip))
	{
		m_dwMRUFloatStyle = CBRS_ALIGN_TOP | (m_dwDockStyle & CBRS_FLOAT_MULTI);
		m_ptMRUFloatPos = m_rectFrameDragHorz.TopLeft();
		m_pDockSite->FloatControlBar(m_pBar, m_ptMRUFloatPos, m_dwMRUFloatStyle);
	}
	else // vertical float
	{
		m_dwMRUFloatStyle = CBRS_ALIGN_LEFT | (m_dwDockStyle & CBRS_FLOAT_MULTI);
		m_ptMRUFloatPos = m_rectFrameDragVert.TopLeft();
		m_pDockSite->FloatControlBar(m_pBar, m_ptMRUFloatPos, m_dwMRUFloatStyle);
	}
}

_INLINE void CDockContext::EndResize()
{
	CancelLoop();

	m_pBar->CalcDynamicLayout(m_rectActualSize.Width(), LM_HORZ | LM_COMMIT);
	m_pDockSite->FloatControlBar(m_pBar, m_rectActualFrameSize.TopLeft(),
								CBRS_ALIGN_TOP | (m_dwDockStyle & CBRS_FLOAT_MULTI) | CBRS_SIZE_DYNAMIC);
}

_INLINE CDockBar* CDockContext::GetDockBar(DWORD dwOverDockStyle)
{
	DWORD dw = 0;
	CDockBar* pBar;
	if (_HORZF(dwOverDockStyle))
	{
		dw = m_pDockSite->CanDock(m_rectDragHorz, dwOverDockStyle & ~CBRS_ORIENT_VERT, &pBar);
		ASSERT(dw != 0);
		ASSERT(pBar != NULL);
		return pBar;
	}
	if (_VERTF(dwOverDockStyle))
	{
		dw = m_pDockSite->CanDock(m_rectDragVert, dwOverDockStyle & ~CBRS_ORIENT_HORZ, &pBar);
		ASSERT(dw != 0);
		ASSERT(pBar != NULL);
		return pBar;
	}
	return NULL;
}

_INLINE void CDockContext::Stretch(CPoint pt)
{
	CPoint ptOffset = pt - m_ptLast;

	// offset all drag rects to new position
	int nLength = 0;
	DWORD dwMode = LM_HORZ;
	if (m_nHitTest == HTLEFT || m_nHitTest == HTRIGHT)
	{
		if (m_nHitTest == HTLEFT)
			m_rectRequestedSize.left += ptOffset.x;
		else
			m_rectRequestedSize.right += ptOffset.x;
		nLength = m_rectRequestedSize.Width();
	}
	else
	{
		dwMode |= LM_LENGTHY;
		if (m_nHitTest == HTTOP)
			m_rectRequestedSize.top += ptOffset.y;
		else
			m_rectRequestedSize.bottom += ptOffset.y;
		nLength = m_rectRequestedSize.Height();
	}
	nLength = (nLength >= 0) ? nLength : 0;

	CSize size = m_pBar->CalcDynamicLayout(nLength, dwMode);

	CRect rectDesk;
#ifndef _MTL_ATL3
	rectDesk.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
	rectDesk.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
	rectDesk.right = rectDesk.left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
	rectDesk.bottom = rectDesk.top + GetSystemMetrics(SM_CYVIRTUALSCREEN);
#else
	HWND hWndDesk = ::GetDesktopWindow();
	::GetWindowRect(hWndDesk, &rectDesk);
#endif
	CRect rectTemp = m_rectActualFrameSize;

	if (m_nHitTest == HTLEFT || m_nHitTest == HTTOP)
	{
		rectTemp.left = rectTemp.right - (size.cx + m_rectFrameBorders.Width());
		rectTemp.top = rectTemp.bottom - (size.cy + m_rectFrameBorders.Height());
		CRect rect;
		if (rect.IntersectRect(rectDesk, rectTemp))
		{
			m_rectActualSize.left = m_rectActualSize.right - size.cx;
			m_rectActualSize.top = m_rectActualSize.bottom - size.cy;
			m_rectActualFrameSize.left = rectTemp.left;
			m_rectActualFrameSize.top = rectTemp.top;
		}
	}
	else
	{
		rectTemp.right = rectTemp.left + (size.cx + m_rectFrameBorders.Width());
		rectTemp.bottom = rectTemp.top + (size.cy + m_rectFrameBorders.Height());
		CRect rect;
		if (rect.IntersectRect(rectDesk, rectTemp))
		{
			m_rectActualSize.right = m_rectActualSize.left + size.cx;
			m_rectActualSize.bottom = m_rectActualSize.top + size.cy;
			m_rectActualFrameSize.right = rectTemp.right;
			m_rectActualFrameSize.bottom = rectTemp.bottom;
		}
	}
	m_ptLast = pt;

	// update feedback
	DrawFocusRect();
}

// Double Click Operations
 _INLINE void CDockContext::ToggleDocking()
{
	if (m_pBar->IsFloating())
	{
		// Dock it only if is allowed to be docked
		if (m_pBar->m_dwDockStyle & CBRS_ALIGN_ANY)
		{
			ASSERT((m_uMRUDockID >= AFX_IDW_DOCKBAR_TOP && m_uMRUDockID <= AFX_IDW_DOCKBAR_BOTTOM) ||
					m_uMRUDockID == 0);

			CRect rect = m_rectMRUDockPos;
			CDockBar* pDockBar = NULL;
			if (m_uMRUDockID != 0)
			{
				pDockBar = (CDockBar*)m_pDockSite->GetControlBar(m_uMRUDockID);
				ENSURE(pDockBar);
				pDockBar->ClientToScreen(&rect);
			}

			// dock it at the specified position, RecalcLayout will snap
			m_pDockSite->ReDockControlBar(m_pBar, pDockBar, &rect);
			m_pDockSite->RecalcLayout();
		}
	}
	else
	{
		CPoint ptFloat = m_ptMRUFloatPos;
		if (ptFloat.x < 0 || ptFloat.y < 0)
		{
			ptFloat = m_rectMRUDockPos.TopLeft();
			m_pBar->GetParent()->ClientToScreen(&ptFloat);
		}
		m_pDockSite->FloatControlBar(m_pBar, ptFloat, m_dwMRUFloatStyle);
	}
}

 _INLINE BOOL CDockContext::Track()
 {
	 // don't handle if capture already set
	 if (::GetCapture() != NULL)
		 return FALSE;

	 // set capture to the window which received this message
	 m_pBar->SetCapture();
	 ASSERT(m_pBar == CWnd::GetCapture());

	 // get messages until capture lost or cancelled/accepted
	 while (CWnd::GetCapture() == m_pBar)
	 {
		 MSG msg;
		 if (!::GetMessage(&msg, NULL, 0, 0))
		 {
			 AfxPostQuitMessage((int)msg.wParam);
			 break;
		 }

		 switch (msg.message)
		 {
			 case WM_LBUTTONUP:
				 if (m_bDragging)
					 EndDrag();
				 else
					 EndResize();
				 return TRUE;
			 case WM_MOUSEMOVE:
				 if (m_bDragging)
					 Move(msg.pt);
				 else
					 Stretch(msg.pt);
				 break;
			 case WM_KEYUP:
				 if (m_bDragging)
					 OnKey((int)msg.wParam, FALSE);
				 break;
			 case WM_KEYDOWN:
				 if (m_bDragging)
					 OnKey((int)msg.wParam, TRUE);
				 if (msg.wParam == VK_ESCAPE)
				 {
					 CancelLoop();
					 return FALSE;
				 }
				 break;
			 case WM_RBUTTONDOWN:
				 CancelLoop();
				 return FALSE;

				 // just dispatch rest of the messages
			 default:
				 DispatchMessage(&msg);
				 break;
		 }
	 }

	 CancelLoop();

	 return FALSE;
}





_INLINE int CControlBar::OnCreate(LPCREATESTRUCT lpcs)
{
	if (CWnd::OnCreate(lpcs) == -1)
		return -1;

	if (m_dwStyle & CBRS_TOOLTIPS)
		EnableToolTips();

	CFrameWnd *pFrameWnd = (CFrameWnd*)GetParent();
	if (pFrameWnd->IsFrameWnd())
	{
		m_pDockSite = pFrameWnd;
		m_pDockSite->AddControlBar(this);
	}

	return 0;
}

_INLINE LRESULT CControlBar::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM)
{
	// handle delay hide/show
	BOOL bVis = GetStyle() & WS_VISIBLE;
	UINT swpFlags = 0;
	if ((m_nStateFlags & delayHide) && bVis)
		swpFlags = SWP_HIDEWINDOW;
	else if ((m_nStateFlags & delayShow) && !bVis)
		swpFlags = SWP_SHOWWINDOW;
	m_nStateFlags &= ~(delayShow | delayHide);
	if (swpFlags != 0)
	{
		SetWindowPos(NULL, 0, 0, 0, 0, swpFlags |
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}

	// the style must be visible and if it is docked
	// the dockbar style must also be visible
	if ((GetStyle() & WS_VISIBLE) &&
		(m_pDockBar == NULL || (m_pDockBar->GetStyle() & WS_VISIBLE)))
	{
		CFrameWnd* pTarget = (CFrameWnd*)GetOwner();
		if (pTarget == NULL || !pTarget->IsFrameWnd())
			pTarget = GetParentFrame();
		if (pTarget != NULL)
			OnUpdateCmdUI(pTarget, (BOOL)wParam);
	}
	return 0L;
}

_INLINE CSize CDockBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	ENSURE_VALID(this);

	CSize sizeFixed = CControlBar::CalcFixedLayout(bStretch, bHorz);

	// get max size
	CSize sizeMax;
	if (!m_rectLayout.IsRectEmpty())
	{
		CRect rect = m_rectLayout;
		CalcInsideRect(rect, bHorz);
		sizeMax = rect.Size();
	}
	else
	{
		CRect rectFrame;
		CFrameWnd* pFrame = EnsureParentFrame();
		pFrame->GetClientRect(&rectFrame);
		sizeMax = rectFrame.Size();
	}

	// prepare for layout
	AFX_SIZEPARENTPARAMS layout;
	layout.hDWP = m_bLayoutQuery ?
	NULL : ::BeginDeferWindowPos((int)m_arrBars.GetSize());
	CPoint pt(-afxData.cxBorder2, -afxData.cyBorder2);
	int nWidth = 0;

	BOOL bWrapped = FALSE;

	// layout all the control bars
	for (int nPos = 0; nPos < m_arrBars.GetSize(); nPos++)
	{
		CControlBar* pBar = GetDockedControlBar(nPos);
		void* pVoid = m_arrBars[nPos];

		if (pBar != NULL)
		{
			if (pBar->IsVisible())
			{
				// get ideal rect for bar
				DWORD dwMode = 0;
				if ((pBar->m_dwStyle & CBRS_SIZE_DYNAMIC) &&
					(pBar->m_dwStyle & CBRS_FLOATING))
					dwMode |= LM_HORZ | LM_MRUWIDTH;
				else if (pBar->m_dwStyle & CBRS_ORIENT_HORZ)
					dwMode |= LM_HORZ | LM_HORZDOCK;
				else
					dwMode |= LM_VERTDOCK;

				CSize sizeBar = pBar->CalcDynamicLayout(-1, dwMode);

				CRect rect(pt, sizeBar);

				// get current rect for bar
				CRect rectBar;
				pBar->GetWindowRect(&rectBar);
				ScreenToClient(&rectBar);

				if (bHorz)
				{
					// Offset Calculated Rect out to Actual
					if (rectBar.left > rect.left && !m_bFloating)
						rect.OffsetRect(rectBar.left - rect.left, 0);

					// If ControlBar goes off the right, then right justify
					if (rect.right > sizeMax.cx && !m_bFloating)
					{
						int x = rect.Width() - afxData.cxBorder2;
						x = max(sizeMax.cx - x, pt.x);
						rect.OffsetRect(x - rect.left, 0);
					}

					// If ControlBar has been wrapped, then left justify
					if (bWrapped)
					{
						bWrapped = FALSE;
						rect.OffsetRect(-(rect.left + afxData.cxBorder2), 0);
					}
					// If ControlBar is completely invisible, then wrap it
					else if ((rect.left >= (sizeMax.cx - afxData.cxBorder2)) &&
						(nPos > 0) && (m_arrBars[nPos - 1] != NULL))
					{
						m_arrBars.InsertAt(nPos, (CObject*)NULL);
						pBar = NULL; pVoid = NULL;
						bWrapped = TRUE;
					}
					if (!bWrapped)
					{
						if (rect != rectBar)
						{
							if (!m_bLayoutQuery &&
								!(pBar->m_dwStyle & CBRS_FLOATING))
							{
								pBar->m_pDockContext->m_rectMRUDockPos = rect;
							}
							AfxRepositionWindow(&layout, pBar->m_hWnd, &rect);
						}
						pt.x = rect.left + sizeBar.cx - afxData.cxBorder2;
						nWidth = max(nWidth, sizeBar.cy);
					}
				}
				else
				{
					// Offset Calculated Rect out to Actual
					if (rectBar.top > rect.top && !m_bFloating)
						rect.OffsetRect(0, rectBar.top - rect.top);

					// If ControlBar goes off the bottom, then bottom justify
					if (rect.bottom > sizeMax.cy && !m_bFloating)
					{
						int y = rect.Height() - afxData.cyBorder2;
						y = max(sizeMax.cy - y, pt.y);
						rect.OffsetRect(0, y - rect.top);
					}

					// If ControlBar has been wrapped, then top justify
					if (bWrapped)
					{
						bWrapped = FALSE;
						rect.OffsetRect(0, -(rect.top + afxData.cyBorder2));
					}
					// If ControlBar is completely invisible, then wrap it
					else if ((rect.top >= (sizeMax.cy - afxData.cyBorder2)) &&
						(nPos > 0) && (m_arrBars[nPos - 1] != NULL))
					{
						m_arrBars.InsertAt(nPos, (CObject*)NULL);
						pBar = NULL; pVoid = NULL;
						bWrapped = TRUE;
					}
					if (!bWrapped)
					{
						if (rect != rectBar)
						{
							if (!m_bLayoutQuery &&
								!(pBar->m_dwStyle & CBRS_FLOATING))
							{
								pBar->m_pDockContext->m_rectMRUDockPos = rect;
							}
							AfxRepositionWindow(&layout, pBar->m_hWnd, &rect);
						}
						pt.y = rect.top + sizeBar.cy - afxData.cyBorder2;
						nWidth = max(nWidth, sizeBar.cx);
					}
				}
			}
			if (!bWrapped)
			{
				// handle any delay/show hide for the bar
				pBar->RecalcDelayShow(&layout);
			}
		}
		if (pBar == NULL && pVoid == NULL && nWidth != 0)
		{
			// end of row because pBar == NULL
			if (bHorz)
			{
				pt.y += nWidth - afxData.cyBorder2;
				sizeFixed.cx = max(sizeFixed.cx, pt.x);
				sizeFixed.cy = max(sizeFixed.cy, pt.y);
				pt.x = -afxData.cxBorder2;
			}
			else
			{
				pt.x += nWidth - afxData.cxBorder2;
				sizeFixed.cx = max(sizeFixed.cx, pt.x);
				sizeFixed.cy = max(sizeFixed.cy, pt.y);
				pt.y = -afxData.cyBorder2;
			}
			nWidth = 0;
		}
	}
	if (!m_bLayoutQuery)
	{
		// move and resize all the windows at once!
		if (layout.hDWP == NULL || !::EndDeferWindowPos(layout.hDWP))
			TRACE(traceAppMsg, 0, "Warning: DeferWindowPos failed - low system resources.\n");
	}

	// adjust size for borders on the dock bar itself
	CRect rect;
	rect.SetRectEmpty();
	CalcInsideRect(rect, bHorz);

	if ((!bStretch || !bHorz) && sizeFixed.cx != 0)
		sizeFixed.cx += -rect.right + rect.left;
	if ((!bStretch || bHorz) && sizeFixed.cy != 0)
		sizeFixed.cy += -rect.bottom + rect.top;

	return sizeFixed;
}

_INLINE BOOL CControlBar::DestroyWindow()
{
	if (m_hWnd != NULL && IsFloating())
		return GetDockingFrame()->DestroyWindow();
	else
		return CWnd::DestroyWindow();
}

_INLINE void CDockBar::DockControlBar(CControlBar* pBar, LPCRECT lpRect)
{
	ENSURE_VALID(this);
	ENSURE_VALID(pBar);

	CRect rectBar;
	pBar->GetWindowRect(&rectBar);
	if (pBar->m_pDockBar == this && (lpRect == NULL || rectBar == *lpRect))
	{
		// already docked and no change in position
		return;
	}

	// set CBRS_FLOAT_MULTI style if docking bar has it
	if (m_bFloating && (pBar->m_dwDockStyle & CBRS_FLOAT_MULTI))
		m_dwStyle |= CBRS_FLOAT_MULTI;

	m_dwStyle &= ~(CBRS_SIZE_FIXED | CBRS_SIZE_DYNAMIC);
	m_dwStyle |= pBar->m_dwStyle & (CBRS_SIZE_FIXED | CBRS_SIZE_DYNAMIC);

	if (!(m_dwStyle & CBRS_FLOAT_MULTI))
	{
		TCHAR szTitle[_MAX_PATH];
		pBar->GetWindowText(szTitle, _countof(szTitle));
		AfxSetWindowText(m_hWnd, szTitle);
	}

	// align correctly and turn on all borders
	DWORD dwStyle = pBar->GetBarStyle();
	dwStyle &= ~(CBRS_ALIGN_ANY);
	dwStyle |= (m_dwStyle & CBRS_ALIGN_ANY) | CBRS_BORDER_ANY;

	if (m_bFloating)
		dwStyle |= CBRS_FLOATING;
	else
		dwStyle &= ~CBRS_FLOATING;

	pBar->SetBarStyle(dwStyle);

	// hide first if changing to a new docking site to avoid flashing
	BOOL bShow = FALSE;
	if (pBar->m_pDockBar != this && pBar->IsWindowVisible())
	{
		pBar->SetWindowPos(NULL, 0, 0, 0, 0,
							SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_HIDEWINDOW);
		bShow = TRUE;
	}

	int nPos = -1;
	if (lpRect != NULL)
	{
		// insert into appropriate row
		CRect rect(lpRect);
		ScreenToClient(&rect);
		CPoint ptMid(rect.left + rect.Width() / 2, rect.top + rect.Height() / 2);
		nPos = Insert(pBar, rect, ptMid);

		// position at requested position
		pBar->SetWindowPos(NULL, rect.left, rect.top, rect.Width(),
			rect.Height(), SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS);
	}
	else
	{
		// always add on current row, then create new one
		m_arrBars.Add(pBar);
		m_arrBars.Add(NULL);

		// align off the edge initially
		pBar->SetWindowPos(NULL, -afxData.cxBorder2, -afxData.cyBorder2, 0, 0,
							SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS);
	}

	// attach it to the docking site
	if (pBar->GetParent() != this)
		pBar->SetParent(this);
	if (pBar->m_pDockBar == this)
		pBar->m_pDockBar->RemoveControlBar(pBar, nPos);
	else if (pBar->m_pDockBar != NULL)
		pBar->m_pDockBar->RemoveControlBar(pBar, -1, m_bFloating && !pBar->m_pDockBar->m_bFloating);
	pBar->m_pDockBar = this;

	if (bShow)
	{
		ASSERT(!pBar->IsWindowVisible());
		pBar->SetWindowPos(NULL, 0, 0, 0, 0,
							SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
	}

	// remove any place holder for pBar in this dockbar
	RemovePlaceHolder(pBar);

	// get parent frame for recalc layout
	CFrameWnd* pFrameWnd = GetDockingFrame();
	pFrameWnd->DelayRecalcLayout();
}

_INLINE void CDockBar::ReDockControlBar(CControlBar* pBar, LPCRECT lpRect)
{
	ENSURE_VALID(this);
	ENSURE_VALID(pBar);
	ASSERT_KINDOF(CControlBar, pBar);
	ASSERT(pBar->m_pDockBar != this); // can't redock here if already docked here

	CRect rectBar;
	pBar->GetWindowRect(&rectBar);
	if (pBar->m_pDockBar == this && (lpRect == NULL || rectBar == *lpRect))
	{
		// already docked and no change in position
		return;
	}

	// set CBRS_FLOAT_MULTI style if docking bar has it
	if (m_bFloating && (pBar->m_dwDockStyle & CBRS_FLOAT_MULTI))
		m_dwStyle |= CBRS_FLOAT_MULTI;

	m_dwStyle &= ~(CBRS_SIZE_FIXED | CBRS_SIZE_DYNAMIC);
	m_dwStyle |= pBar->m_dwStyle & (CBRS_SIZE_FIXED | CBRS_SIZE_DYNAMIC);

	if (!(m_dwStyle & CBRS_FLOAT_MULTI))
	{
		TCHAR szTitle[_MAX_PATH];
		pBar->GetWindowText(szTitle, _countof(szTitle));
		AfxSetWindowText(m_hWnd, szTitle);
	}

	// align correctly and turn on all borders
	DWORD dwStyle = pBar->GetBarStyle();
	dwStyle &= ~(CBRS_ALIGN_ANY);
	dwStyle |= (m_dwStyle & CBRS_ALIGN_ANY) | CBRS_BORDER_ANY;

	if (m_bFloating)
		dwStyle |= CBRS_FLOATING;
	else
		dwStyle &= ~CBRS_FLOATING;

	pBar->SetBarStyle(dwStyle);

	int nPos = FindBar((CControlBar*)(DWORD_PTR)_AfxGetDlgCtrlID(pBar->m_hWnd));
	if (nPos > 0)
		m_arrBars[nPos] = pBar;

	if (lpRect != NULL)
	{
		CRect rect(lpRect);
		ScreenToClient(&rect);

		if (nPos < 1)
		{
			CPoint ptMid(rect.left + rect.Width() / 2, rect.top + rect.Height() / 2);
			nPos = Insert(pBar, rect, ptMid);
		}

		// position at requested position
		pBar->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), 
							SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS);
	}
	else
	{
		if (nPos < 1)
		{
			// always add on current row, then create new one
			m_arrBars.Add(pBar);
			m_arrBars.Add(NULL);
		}

		// align off the edge initially
		pBar->SetWindowPos(NULL, -afxData.cxBorder2, -afxData.cyBorder2, 0, 0,
							SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS);
	}

	// attach it to the docking site
	if (pBar->GetParent() != this)
		pBar->SetParent(this);
	if (pBar->m_pDockBar != NULL)
		pBar->m_pDockBar->RemoveControlBar(pBar);
	pBar->m_pDockBar = this;

	// get parent frame for recalc layout
	CFrameWnd* pFrameWnd = GetDockingFrame();
	pFrameWnd->DelayRecalcLayout();
}

_INLINE BOOL CDockBar::RemoveControlBar(CControlBar* pBar, int nPosExclude, int nAddPlaceHolder)
{
	ASSERT(nAddPlaceHolder == 1 || nAddPlaceHolder == 0 || nAddPlaceHolder == -1);
	ENSURE_VALID(this);
	ENSURE_VALID(pBar);
	int nPos = FindBar(pBar, nPosExclude);
	ENSURE(nPos > 0);

	if (nAddPlaceHolder == 1)
	{
		m_arrBars[nPos] = (void*)(DWORD_PTR)_AfxGetDlgCtrlID(pBar->m_hWnd);

		// check for already existing place holder
		int nPosOld = FindBar((CControlBar*)m_arrBars[nPos], nPos);
		if (nPosOld > 0)
		{
			m_arrBars.RemoveAt(nPos);

			// remove section indicator (NULL) if nothing else in section
			if (m_arrBars[nPos - 1] == NULL && m_arrBars[nPos] == NULL)
				m_arrBars.RemoveAt(nPos);
		}
	}
	else
	{
		m_arrBars.RemoveAt(nPos);
		if (m_arrBars[nPos - 1] == NULL && m_arrBars[nPos] == NULL)
			m_arrBars.RemoveAt(nPos);

		// Remove any pre-existing place holders.
		if (nAddPlaceHolder != -1)
			RemovePlaceHolder(pBar);
	}

	// don't do anything more in the shutdown case!
	if (pBar->m_pDockContext == NULL)
		return FALSE;

	// get parent frame for recalc layout/frame destroy
	CFrameWnd* pFrameWnd = GetDockingFrame();
	if (m_bFloating && GetDockedVisibleCount() == 0)
	{
		if (GetDockedCount() == 0)
		{
			pFrameWnd->DestroyWindow();
			return TRUE; // Self-Destruct
		}
		else
			pFrameWnd->ShowWindow(SW_HIDE);
	}
	else
		pFrameWnd->DelayRecalcLayout();

	return FALSE;
}

_INLINE void CDockBar::ShowAll(BOOL bShow)
{
	for (int nPos = 0; nPos < m_arrBars.GetSize(); nPos++)
	{
		CControlBar* pBar = GetDockedControlBar(nPos);
		if (pBar != NULL)
		{
			CFrameWnd* pFrameWnd = pBar->GetDockingFrame();
			pFrameWnd->ShowControlBar(pBar, bShow, TRUE);
		}
	}
}



_INLINE BOOL CReBar::_AddBar(CWnd* pBar, REBARBANDINFO* pRBBI)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));
	ENSURE_ARG(pBar != NULL);
	ASSERT(::IsWindow(pBar->m_hWnd));

	pRBBI->cbSize = m_nReBarBandInfoSize;
	pRBBI->fMask |= RBBIM_SIZE | RBBIM_CHILD | RBBIM_CHILDSIZE;
	pRBBI->hwndChild = pBar->m_hWnd;

	CSize size;
	CControlBar* pTemp = DYNAMIC_DOWNCAST(CControlBar, pBar);
	if (pTemp != NULL)
	{
		size = pTemp->CalcFixedLayout(FALSE, m_dwStyle & CBRS_ORIENT_HORZ);
	}
	else
	{
		CRect rect;
		pBar->GetWindowRect(&rect);
		size = rect.Size();
	}

	//ASSERT(_afxComCtlVersion != -1);
	pRBBI->cyMinChild = size.cy;

	//  COMCTL32.DLL is off by 4 pixels in its sizing logic.  Whatever
	//	is specified as the minimum size, the system rebar will allow that band
	//	to be 4 actual pixels smaller!  That's why we add 4 to the size here.
	pRBBI->cxIdeal = size.cx;//+ (_afxComCtlVersion < VERSION_IE401 ? 4 : 0);

	if (pRBBI->fStyle & RBBS_USECHEVRON)
	{
		pRBBI->fMask |= RBBIM_IDEALSIZE;
		// Make min size square (i.e., pRBBI->cx == pRBBI->cy == size.cy)
		pRBBI->cxMinChild = pRBBI->cyMinChild;
		pRBBI->cx = pRBBI->cxIdeal;
	}
	else
		pRBBI->cx = pRBBI->cxMinChild = pRBBI->cxIdeal;

	if (DefWindowProc(RB_INSERTBAND, (WPARAM)-1, (LPARAM)pRBBI))
	{
		CFrameWnd* pFrameWnd = GetParentFrame();
		if (pFrameWnd != NULL)
			pFrameWnd->RecalcLayout();

		REBARBANDINFO rbBand;
		rbBand.cbSize = m_nReBarBandInfoSize;
		rbBand.fMask = RBBIM_STYLE;
		VERIFY(DefWindowProc(RB_GETBANDINFO, 0, (LPARAM)&rbBand));
		if ((rbBand.fStyle & RBBS_HIDDEN) == 0)
		{
			GetReBarCtrl().MaximizeBand(0);
		}

		return TRUE;
	}
	return FALSE;
}

_INLINE void CReBar::OnHeightChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	// does the CReBar have a frame?
	CFrameWnd* pFrameWnd = GetParentFrame();
	if (pFrameWnd != NULL)
	{
		// it does -- tell it to recalc its layout
		if (!pFrameWnd->m_bInRecalcLayout)
			pFrameWnd->RecalcLayout();
		else
			PostMessage(WM_RECALCPARENT);
	}
	*pResult = 0;
}

_INLINE void CReBar::OnRecalcParent()
{
	CFrameWnd* pFrameWnd = EnsureParentFrame();
	pFrameWnd->RecalcLayout();
}



