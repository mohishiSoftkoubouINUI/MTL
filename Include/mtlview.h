

class CView : public CWnd
{
	DECLARE_DYNCREATE(CView)

	public:
		CView()
		{
			m_pDocument = NULL;
		}

		void CViewDestruct() ;

		virtual ~CView()
		{
			_VOLATILE_CLASS_FUNC_V_V(CView, CViewDestruct)
		}

	public:
		DECLARE_MESSAGE_MAP()


		CDocument* m_pDocument;

	public:
		virtual void OnActivateFrame(UINT /*nState*/, CFrameWnd* /*pFrameWnd*/) { }

		virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView*)
		{
			if (bActivate)
			{
				ASSERT(pActivateView == this);

				// take the focus if this frame/view/pane is now active
				if (IsTopParentActive())
					SetFocus();
			}
		}

		virtual void OnBeginPrinting(CDC* pDC, CPrintInfo*)
		{
			ASSERT_VALID(pDC);
			UNUSED(pDC);     // unused in release builds

			// Do printing initialization here
		}

		// OnEndPrintPreview is here for swap tuning reasons
		//  (see viewprev.cpp for complete preview mode implementation)
		virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT, CPreviewView* pView) ;

		virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

		int OnCreate(LPCREATESTRUCT lpcs) ;

		void OnDestroy()
		{
#ifndef MTL_REMOVE_CFRAMEWND_VIEW
			CFrameWnd* pFrame = GetParentFrame();
			if (pFrame != NULL && pFrame->GetActiveView() == this)
				pFrame->SetActiveView(NULL);    // deactivate during death
#endif	// MTL_REMOVE_CFRAMEWND_VIEW

			CWnd::OnDestroy();
		}

		virtual void OnDraw(CDC*) {}

		virtual void OnEndPrinting(CDC* pDC, CPrintInfo*)
		{
			ASSERT_VALID(pDC);
			UNUSED(pDC);     // unused in release builds

			// Do printing cleanup here
		}

		virtual void OnInitialUpdate()
		{
			OnUpdate(NULL, 0, NULL);        // initial update
		}

		void OnFilePrint() ;

		/////////////////////////////////////////////////////////////////////////////
		// CView's OnPrintPreview.  Here to force linkage

		void OnFilePrintPreview() ;

		int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
		{
			int nResult = CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
			if (nResult == MA_NOACTIVATE || nResult == MA_NOACTIVATEANDEAT)
				return nResult;   // frame does not want to activate

			CFrameWnd* pParentFrame = GetParentFrame();
			if (pParentFrame != NULL)
			{
				// eat it if this will cause activation
				ASSERT(pParentFrame == pDesktopWnd || pDesktopWnd->IsChild(pParentFrame));

				// either re-activate the current view, or set this view to be active
				CView* pView = pParentFrame->GetActiveView();
				HWND hWndFocus = ::GetFocus();
				if (pView == this && m_hWnd != hWndFocus && !::IsChild(m_hWnd, hWndFocus))
				{
					// re-activate this view
					OnActivateView(TRUE, this, this);
				}
				else
				{
					// activate this view
					pParentFrame->SetActiveView(this);
				}
			}
			return nResult;
		}

		void OnPaint()
		{
			// standard paint routine
			CPaintDC dc(this);
			OnPrepareDC(&dc);
			OnDraw(&dc);
		}

		virtual void OnPrint(CDC* pDC, CPrintInfo*)
		{
			ASSERT_VALID(pDC);

			// Override and set printing variables based on page number
			OnDraw(pDC);                    // Call Draw
		}

		LRESULT OnPrintClient(CDC* pDC, UINT nFlags)
		{
			ASSERT_VALID(pDC);
			if (nFlags & PRF_ERASEBKGND)
			{
				SendMessage(WM_ERASEBKGND, (WPARAM)pDC->GetSafeHdc());
			}

			if (nFlags & PRF_CLIENT)
			{
				OnDraw(pDC);
			}

			return 0L;
		}

		virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL)
		{
			ASSERT_VALID(pDC);
			UNUSED(pDC); // unused in release builds

			// Default to one page printing if doc length not known
			if (pInfo != NULL)
				pInfo->m_bContinuePrinting =
				(pInfo->GetMaxPage() != 0xffff || (pInfo->m_nCurPage == 1));
		}

		virtual BOOL OnPreparePrinting(CPrintInfo*)
		{
			// Do print DC initialization here
			// override and call DoPreparePrinting (in viewprnt.cpp)

			return TRUE;
		}

		/////////////////////////////////////////////////////////////////////////////
		// CView scrolling support

		virtual BOOL OnScroll(UINT /*nScrollCode*/, UINT /*nPos*/, BOOL /*bDoScroll*/)
		{
			return FALSE;
		}

		virtual BOOL OnScrollBy(CSize /*sizeScroll*/, BOOL /*bDoScroll*/)
		{
			return FALSE;
		}


		virtual void OnUpdate(CView* pSender, LPARAM /*lHint*/, CObject* /*pHint*/)
		{
			ASSERT(pSender != this);
			UNUSED(pSender);     // unused in release builds

			// invalidate the entire pane, erase background too
			Invalidate(TRUE);
		}


		BOOL DoPreparePrinting(CPrintInfo* pInfo) ;
		BOOL DoPrintPreview(UINT nIDResource, CView* pPrintView, CRuntimeClass* pPreviewViewClass, CPrintPreviewState* pState) ;

		CDocument* GetDocument()  const { ASSERT(this != NULL); return m_pDocument; }

		static CSplitterWnd* GetParentSplitter(CWnd* pWnd, BOOL bAnyState) ;

		// self destruction
		void PostNcDestroy()
		{
			// default for views is to allocate them on the heap
			//  the default post-cleanup is to 'delete this'.
			//  never explicitly call 'delete' on a view
			delete this;
		}


		virtual BOOL PreCreateWindow(CREATESTRUCT & cs)
		{
			ASSERT(cs.style & WS_CHILD);

			if (cs.lpszClass == NULL)
			{
				VERIFY(AfxDeferRegisterClass(AFX_WNDFRAMEORVIEW_REG));
				cs.lpszClass = _afxWndFrameOrView;  // COLOR_WINDOW background
			}

			if (cs.style & WS_BORDER)
			{
				cs.dwExStyle |= WS_EX_CLIENTEDGE;
				cs.style &= ~WS_BORDER;
			}

			return TRUE;
		}

};

BEGIN_MESSAGE_MAP(CView, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_MOUSEACTIVATE()
	ON_WM_PAINT()
	ON_WM_PRINTCLIENT()

	// special command for Initial Update
	ON_MESSAGE_VOID(WM_INITIALUPDATE, &CView::OnInitialUpdate)
END_MESSAGE_MAP()

#ifndef MTL_REMOVE_CFRAMEWND_VIEW

_INLINE void CFrameWnd::SetActiveView(CView* pViewNew, BOOL bNotify)
{
	CView* pViewOld = m_pViewActive;
	if (pViewNew == pViewOld)
		return;     // do not re-activate if SetActiveView called more than once

	m_pViewActive = NULL;   // no active for the following processing

	// deactivate the old one
	if (pViewOld != NULL)
		pViewOld->OnActivateView(FALSE, pViewNew, pViewOld);

	// if the OnActivateView moves the active window,
	//    that will veto this change
	if (m_pViewActive != NULL)
		return;     // already set
	m_pViewActive = pViewNew;

	// activate
	if (pViewNew != NULL && bNotify)
		pViewNew->OnActivateView(TRUE, pViewNew, pViewOld);
}

#endif // MTL_REMOVE_CFRAMEWND_VIEW





#define AFX_CX_ANCHOR_BITMAP	32
#define AFX_CY_ANCHOR_BITMAP	32


class _AFX_MOUSEANCHORWND : public CWnd
{
	public:
		_AFX_MOUSEANCHORWND(CPoint& ptAnchor) : m_ptAnchor(ptAnchor), m_bQuitTracking(FALSE) { }

		virtual ~_AFX_MOUSEANCHORWND() { }

	public:
		DECLARE_MESSAGE_MAP()

		CPoint m_ptAnchor;
		BOOL m_bQuitTracking;

		CRect m_rectDrag;

		UINT m_nAnchorID;
		HCURSOR m_hAnchorCursor;

	public:
		void OnPaint()
		{
			CPaintDC dc(this);
			CRect rect;
			GetClientRect(&rect);

			// shrink a pixel in every dimension for border
			rect.DeflateRect(1, 1, 1, 1);
			dc.Ellipse(rect);

			// draw anchor shape
			dc.DrawIcon(0, 0, m_hAnchorCursor);
		}

		void OnTimer(UINT_PTR nIDEvent) ;

		BOOL Create(CScrollView* pParent) ;

		// self destruction
		void PostNcDestroy()
		{
			//  the default post-cleanup is to 'delete this'.
			//  never explicitly call 'delete'
			delete this;
		}

		void SetBitmap(UINT nID)
		{
			HINSTANCE hInst = AfxFindResourceHandle(ATL_MAKEINTRESOURCE(nID), ATL_RT_GROUP_CURSOR);
			ASSERT(hInst != NULL);
			m_hAnchorCursor = ::LoadCursorW(hInst, ATL_MAKEINTRESOURCEW(nID));
			m_nAnchorID = nID;
		}

		LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
		{
			switch (message)
			{
				// any of these messages cause us to quit scrolling
				case WM_MOUSEWHEEL:
				case WM_KEYDOWN:
				case WM_CHAR:
				case WM_KEYUP:
				case WM_SYSKEYDOWN:
				case WM_SYSKEYUP:
				case WM_LBUTTONDOWN:
				case WM_LBUTTONUP:
				case WM_RBUTTONDOWN:
				case WM_RBUTTONUP:
				case WM_MBUTTONDOWN:
					m_bQuitTracking = TRUE;
					break;

				// Button up message depend on the position of cursor
				// This enables the user to click and drag for a quick pan.
				case WM_MBUTTONUP:
				{
					CPoint pt((DWORD)lParam);
					ClientToScreen(&pt);
					if (!PtInRect(&m_rectDrag, pt))
						m_bQuitTracking = TRUE;
					break;
				}
			}

			return CWnd::WindowProc(message, wParam, lParam);
	}


};

BEGIN_MESSAGE_MAP(_AFX_MOUSEANCHORWND, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// Special mapping modes just for CScrollView implementation
#define MM_NONE             0
#define MM_SCALETOFIT       (-1)
// standard GDI mapping modes are > 0

class CScrollView : public CView
{
	DECLARE_DYNAMIC(CScrollView)

	public:
		CScrollView() : afxData(CAfx::GetAfxData())
		{
			m_pAnchorWindow = NULL;
			m_nMapMode = MM_NONE;

			m_totalLog.cx = m_totalLog.cy = 0;
			m_totalDev.cx = m_totalDev.cy = 0;
			m_pageDev.cx = m_pageDev.cy = 0;
			m_lineDev.cx = m_lineDev.cy = 0;

			m_bCenter = FALSE;
			m_bInsideUpdate = FALSE;
		}

		~CScrollView()
		{
			if (m_pAnchorWindow != NULL)
			{
				m_pAnchorWindow->DestroyWindow();
				delete m_pAnchorWindow;
			}
		}

	public:
		DECLARE_MESSAGE_MAP()

		const struct _AUX_DATA& afxData;

		static const SIZE sizeDefault ;

		_AFX_MOUSEANCHORWND* m_pAnchorWindow; // window for wheel mouse anchor

		int m_nMapMode;				 // mapping mode for window creation
		CSize m_totalLog;           // total size in logical units (no rounding)
		CSize m_totalDev;           // total size in device units
		CSize m_pageDev;            // per page scroll size in device units
		CSize m_lineDev;            // per line scroll size in device units

		BOOL m_bCenter;             // Center output if larger than total size
		BOOL m_bInsideUpdate;       // internal state for OnSize callback

	public:

		void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
		{
			if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
				return;     // eat it

			// ignore scroll bar msgs from other controls
			if (pScrollBar != GetScrollBarCtrl(SB_HORZ))
				return;

			OnScroll(MAKEWORD(nSBCode, 0xff), nPos);
		}

		BOOL OnMouseWheel(UINT fFlags, short zDelta, CPoint point)
		{
			// we don't handle anything but scrolling
			if (fFlags & (MK_SHIFT | MK_CONTROL))
				return FALSE;

			// if the parent is a splitter, it will handle the message
			if (GetParentSplitter(this, TRUE))
				return FALSE;

			// we can't get out of it--perform the scroll ourselves
			return DoMouseWheel(fFlags, zDelta, point);
		}

		virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL)
		{
			ASSERT_VALID(pDC);

			ASSERT(m_totalDev.cx >= 0 && m_totalDev.cy >= 0);
			switch (m_nMapMode)
			{
				case MM_SCALETOFIT:
					pDC->SetMapMode(MM_ANISOTROPIC);
					pDC->SetWindowExt(m_totalLog);  // window is in logical coordinates
					pDC->SetViewportExt(m_totalDev);
					if (m_totalDev.cx == 0 || m_totalDev.cy == 0)
						TRACE(traceAppMsg, 0, "Warning: CScrollView scaled to nothing.\n");
					break;

				default:
					ASSERT(m_nMapMode > 0);
					pDC->SetMapMode(m_nMapMode);
					break;
				}

				CPoint ptVpOrg(0, 0);       // assume no shift for printing
				if (!pDC->IsPrinting())
				{
					ASSERT(pDC->GetWindowOrg() == CPoint(0, 0));

					// by default shift viewport origin in negative direction of scroll
					ptVpOrg = -GetDeviceScrollPosition();

					if (m_bCenter)
					{
						CRect rect;
						GetClientRect(&rect);

						// if client area is larger than total device size,
						// override scroll positions to place origin such that
						// output is centered in the window
						if (m_totalDev.cx < rect.Width())
							ptVpOrg.x = (rect.Width() - m_totalDev.cx) / 2;
						if (m_totalDev.cy < rect.Height())
							ptVpOrg.y = (rect.Height() - m_totalDev.cy) / 2;
					}
			}

			pDC->SetViewportOrg(ptVpOrg);

			CView::OnPrepareDC(pDC, pInfo);     // For default Printing behavior
		}

		LRESULT OnPrintClient(CDC* pDC, UINT nFlags)
		{
			UNREFERENCED_PARAMETER(nFlags);

			if (pDC->GetSafeHdc() == NULL)
			{
				return 0L;
			}

			CDC dcView;
			dcView.CreateCompatibleDC(pDC);

			CRect rect;
			GetClientRect(rect);

			CImage bmpSrc;
			bmpSrc.CreateEx(rect.Width(), rect.Height(), 32, BI_RGB, NULL, CImage::createAlphaChannel);

			HBITMAP hBmpOld = (HBITMAP)dcView.SelectObject((HBITMAP)bmpSrc);

			dcView.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);

			dcView.SetViewportOrg(0, 0);
			dcView.SetWindowOrg(0, 0);
			dcView.SetMapMode(MM_TEXT);

			OnPrepareDC(&dcView, NULL);
			OnDraw(&dcView);

			pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &dcView, 0, 0, SRCCOPY);

			dcView.SelectObject(hBmpOld);

			return 0L;
		}

		virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE)
		{
			// calc new x position
			int x = GetScrollPos(SB_HORZ);
			int xOrig = x;

			switch (LOBYTE(nScrollCode))
			{
				case SB_TOP:
					x = 0;
					break;
				case SB_BOTTOM:
					x = INT_MAX;
					break;
				case SB_LINEUP:
					x -= m_lineDev.cx;
					break;
				case SB_LINEDOWN:
					x += m_lineDev.cx;
					break;
				case SB_PAGEUP:
					x -= m_pageDev.cx;
					break;
				case SB_PAGEDOWN:
					x += m_pageDev.cx;
					break;
				case SB_THUMBTRACK:
					x = nPos;
					break;
			}

			// calc new y position
			int y = GetScrollPos(SB_VERT);
			int yOrig = y;

			switch (HIBYTE(nScrollCode))
			{
				case SB_TOP:
					y = 0;
					break;
				case SB_BOTTOM:
					y = INT_MAX;
					break;
				case SB_LINEUP:
					y -= m_lineDev.cy;
					break;
				case SB_LINEDOWN:
					y += m_lineDev.cy;
					break;
				case SB_PAGEUP:
					y -= m_pageDev.cy;
					break;
				case SB_PAGEDOWN:
					y += m_pageDev.cy;
					break;
				case SB_THUMBTRACK:
					y = nPos;
					break;
			}

			BOOL bResult = OnScrollBy(CSize(x - xOrig, y - yOrig), bDoScroll);
			if (bResult && bDoScroll)
				UpdateWindow();

			return bResult;
		}

		virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE)
		{
			int xOrig, x;
			int yOrig, y;

			// don't scroll if there is no valid scroll range (ie. no scroll bar)
			CScrollBar* pBar;
			DWORD dwStyle = GetStyle();
			pBar = GetScrollBarCtrl(SB_VERT);
			if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
				(pBar == NULL && !(dwStyle & WS_VSCROLL)))
			{
				// vertical scroll bar not enabled
				sizeScroll.cy = 0;
			}
			pBar = GetScrollBarCtrl(SB_HORZ);
			if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
				(pBar == NULL && !(dwStyle & WS_HSCROLL)))
			{
				// horizontal scroll bar not enabled
				sizeScroll.cx = 0;
			}

			// adjust current x position
			xOrig = x = GetScrollPos(SB_HORZ);
			int xMax = GetScrollLimit(SB_HORZ);
			x += sizeScroll.cx;
			if (x < 0)
				x = 0;
			else if (x > xMax)
				x = xMax;

			// adjust current y position
			yOrig = y = GetScrollPos(SB_VERT);
			int yMax = GetScrollLimit(SB_VERT);
			y += sizeScroll.cy;
			if (y < 0)
				y = 0;
			else if (y > yMax)
				y = yMax;

			// did anything change?
			if (x == xOrig && y == yOrig)
				return FALSE;

			if (bDoScroll)
			{
				// do scroll and update scroll positions
				ScrollWindow(-(x - xOrig), -(y - yOrig));
				if (x != xOrig)
					SetScrollPos(SB_HORZ, x);
				if (y != yOrig)
					SetScrollPos(SB_VERT, y);
			}
			return TRUE;
		}

		void OnSize(UINT nType, int cx, int cy)
		{
			CView::OnSize(nType, cx, cy);
			if (m_nMapMode == MM_SCALETOFIT)
			{
				// force recalculation of scale to fit parameters
				SetScaleToFitSize(m_totalLog);
			}
			else
			{
				// UpdateBars() handles locking out recursion
				UpdateBars();
			}
		}

		void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
		{
			if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
				return;     // eat it

			// ignore scroll bar msgs from other controls
			if (pScrollBar != GetScrollBarCtrl(SB_VERT))
				return;

			OnScroll(MAKEWORD(0xff, nSBCode), nPos);
		}



		virtual void CCalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
		{
			if (nAdjustType == adjustOutside)
			{
				// allow for special client-edge style
				::AdjustWindowRectEx(lpClientRect, 0, FALSE, GetExStyle());

				if (m_nMapMode != MM_SCALETOFIT)
				{
					// if the view is being used in-place, add scrollbar sizes
					//  (scollbars should appear on the outside when in-place editing)
					CSize sizeClient(lpClientRect->right - lpClientRect->left,
									lpClientRect->bottom - lpClientRect->top);

					CSize sizeRange = m_totalDev - sizeClient;
					// > 0 => need to scroll

					// get scroll bar sizes (used to adjust the window)
					CSize sizeSb;
					GetScrollBarSizes(sizeSb);

					// adjust the window size based on the state
					if (sizeRange.cy > 0)
					{   // vertical scroll bars take up horizontal space
						lpClientRect->right += sizeSb.cx;
					}
					if (sizeRange.cx > 0)
					{   // horizontal scroll bars take up vertical space
						lpClientRect->bottom += sizeSb.cy;
					}
				}
			}
			else
			{
				// call default to handle other non-client areas
				::AdjustWindowRectEx(lpClientRect, GetStyle(), FALSE, GetExStyle() & ~(WS_EX_CLIENTEDGE));
			}
		}



		void CenterOnPoint(CPoint ptCenter) // center in device coords
		{
			CRect rect;
			GetClientRect(&rect);           // find size of client window

			int xDesired = ptCenter.x - rect.Width() / 2;
			int yDesired = ptCenter.y - rect.Height() / 2;

			DWORD dwStyle = GetStyle();

			if ((dwStyle & WS_HSCROLL) == 0 || xDesired < 0)
			{
				xDesired = 0;
			}
			else
			{
				int xMax = GetScrollLimit(SB_HORZ);
				if (xDesired > xMax)
					xDesired = xMax;
			}

			if ((dwStyle & WS_VSCROLL) == 0 || yDesired < 0)
			{
				yDesired = 0;
			}
			else
			{
				int yMax = GetScrollLimit(SB_VERT);
				if (yDesired > yMax)
					yDesired = yMax;
			}

			ASSERT(xDesired >= 0);
			ASSERT(yDesired >= 0);

			SetScrollPos(SB_HORZ, xDesired);
			SetScrollPos(SB_VERT, yDesired);
		}

		void CheckScrollBars(BOOL& bHasHorzBar, BOOL& bHasVertBar) const
		{
			DWORD dwStyle = GetStyle();
			CScrollBar* pBar = GetScrollBarCtrl(SB_VERT);
			bHasVertBar = ((pBar != NULL) && pBar->IsWindowEnabled()) || (dwStyle & WS_VSCROLL);

			pBar = GetScrollBarCtrl(SB_HORZ);
			bHasHorzBar = ((pBar != NULL) && pBar->IsWindowEnabled()) || (dwStyle & WS_HSCROLL);
		}

		// This function isn't virtual. If you need to override it,
		// you really need to override OnMouseWheel() here or in
		// CSplitterWnd.

		UINT _AfxGetMouseScrollLines()
		{
			static BOOL _afxGotScrollLines = FALSE ;
			static UINT uCachedScrollLines;

			// if we've already got it and we're not refreshing,
			// return what we've already got

			if (_afxGotScrollLines)
				return uCachedScrollLines;

			// see if we can find the mouse window

			_afxGotScrollLines = TRUE;

			static UINT msgGetScrollLines;
			static WORD nRegisteredMessage;

			// couldn't use the window -- try system settings
			uCachedScrollLines = 3; // reasonable default
			::SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &uCachedScrollLines, 0);

			return uCachedScrollLines;
		}

		BOOL DoMouseWheel(UINT fFlags, short zDelta, CPoint point)
		{
			UNUSED_ALWAYS(point);
			UNUSED_ALWAYS(fFlags);

			// if we have a vertical scroll bar, the wheel scrolls that
			// if we have _only_ a horizontal scroll bar, the wheel scrolls that
			// otherwise, don't do any work at all

			BOOL bHasHorzBar, bHasVertBar;
			CheckScrollBars(bHasHorzBar, bHasVertBar);
			if (!bHasVertBar && !bHasHorzBar)
				return FALSE;

			BOOL bResult = FALSE;
			UINT uWheelScrollLines = _AfxGetMouseScrollLines();
			int nToScroll = MulDiv(-zDelta, uWheelScrollLines, WHEEL_DELTA);
			int nDisplacement;

			if (bHasVertBar)
			{
				if (uWheelScrollLines == WHEEL_PAGESCROLL)
				{
					nDisplacement = m_pageDev.cy;
					if (zDelta > 0)
						nDisplacement = -nDisplacement;
				}
				else
				{
					nDisplacement = nToScroll * m_lineDev.cy;
					nDisplacement = min(nDisplacement, m_pageDev.cy);
				}
				bResult = OnScrollBy(CSize(0, nDisplacement), TRUE);
			}
			else if (bHasHorzBar)
			{
				if (uWheelScrollLines == WHEEL_PAGESCROLL)
				{
					nDisplacement = m_pageDev.cx;
					if (zDelta > 0)
						nDisplacement = -nDisplacement;
				}
				else
				{
					nDisplacement = nToScroll * m_lineDev.cx;
					nDisplacement = min(nDisplacement, m_pageDev.cx);
				}
				bResult = OnScrollBy(CSize(nDisplacement, 0), TRUE);
			}

			if (bResult)
				UpdateWindow();

			return bResult;
		}

		CPoint GetDeviceScrollPosition() const
		{
			CPoint pt(GetScrollPos(SB_HORZ), GetScrollPos(SB_VERT));
			ASSERT(pt.x >= 0 && pt.y >= 0);
			return pt;
		}

		void GetScrollBarSizes(CSize& sizeSb)
		{
			sizeSb.cx = sizeSb.cy = 0;
			DWORD dwStyle = GetStyle();

			if (GetScrollBarCtrl(SB_VERT) == NULL)
			{
				// vert scrollbars will impact client area of this window
				sizeSb.cx = afxData.cxVScroll;
				if (dwStyle & WS_BORDER)
					sizeSb.cx -= AFX_CX_BORDER;
			}
			if (GetScrollBarCtrl(SB_HORZ) == NULL)
			{
				// horz scrollbars will impact client area of this window
				sizeSb.cy = afxData.cyHScroll;
				if (dwStyle & WS_BORDER)
					sizeSb.cy -= AFX_CY_BORDER;
			}
		}

		void GetScrollBarState(CSize sizeClient, CSize& needSb, CSize& sizeRange, CPoint& ptMove, BOOL bInsideClient)
		{
			// get scroll bar sizes (the part that is in the client area)
			CSize sizeSb;
			GetScrollBarSizes(sizeSb);

			// enough room to add scrollbars
			sizeRange = m_totalDev - sizeClient;
			// > 0 => need to scroll
			ptMove = GetDeviceScrollPosition();
			// point to move to (start at current scroll pos)

			BOOL bNeedH = sizeRange.cx > 0;
			if (!bNeedH)
				ptMove.x = 0;                       // jump back to origin
			else if (bInsideClient)
				sizeRange.cy += sizeSb.cy;          // need room for a scroll bar

			BOOL bNeedV = sizeRange.cy > 0;
			if (!bNeedV)
				ptMove.y = 0;                       // jump back to origin
			else if (bInsideClient)
				sizeRange.cx += sizeSb.cx;          // need room for a scroll bar

			if (bNeedV && !bNeedH && sizeRange.cx > 0)
			{
				ASSERT(bInsideClient);
				// need a horizontal scrollbar after all
				bNeedH = TRUE;
				sizeRange.cy += sizeSb.cy;
			}

			// if current scroll position will be past the limit, scroll to limit
			if (sizeRange.cx > 0 && ptMove.x >= sizeRange.cx)
				ptMove.x = sizeRange.cx;
			if (sizeRange.cy > 0 && ptMove.y >= sizeRange.cy)
				ptMove.y = sizeRange.cy;

			// now update the bars as appropriate
			needSb.cx = bNeedH;
			needSb.cy = bNeedV;

			// needSb, sizeRange, and ptMove area now all updated
		}

		BOOL GetTrueClientSize(CSize& size, CSize& sizeSb)
			// return TRUE if enough room to add scrollbars if needed
		{
			CRect rect;
			GetClientRect(&rect);
			ASSERT(rect.top == 0 && rect.left == 0);
			size.cx = rect.right;
			size.cy = rect.bottom;
			DWORD dwStyle = GetStyle();

			// first get the size of the scrollbars for this window
			GetScrollBarSizes(sizeSb);

			// first calculate the size of a potential scrollbar
			// (scroll bar controls do not get turned on/off)
			if (sizeSb.cx != 0 && (dwStyle & WS_VSCROLL))
			{
				// vert scrollbars will impact client area of this window
				size.cx += sizeSb.cx;   // currently on - adjust now
			}
			if (sizeSb.cy != 0 && (dwStyle & WS_HSCROLL))
			{
				// horz scrollbars will impact client area of this window
				size.cy += sizeSb.cy;   // currently on - adjust now
			}

			// return TRUE if enough room
			return (size.cx > sizeSb.cx && size.cy > sizeSb.cy);
		}

		virtual CSize GetWheelScrollDistance(CSize sizeDistance, BOOL bHorz, BOOL bVert)
		{
			CSize sizeRet;

			if (bHorz)
				sizeRet.cx = sizeDistance.cx / 10;
			else
				sizeRet.cx = 0;

			if (bVert)
				sizeRet.cy = sizeDistance.cy / 10;
			else
				sizeRet.cy = 0;

			return sizeRet;
		}

		LRESULT HandleMButtonDown(WPARAM wParam, LPARAM lParam)
		{
			UINT nFlags = (UINT)(wParam);
			CPoint point((DWORD)lParam);

			// if the user has CTRL or SHIFT down, we certainly
			// do not handle the message

			if (nFlags & (MK_SHIFT | MK_CONTROL))
			{
				CView::OnMButtonDown(nFlags, point);
				return FALSE;
			}

			// Otherwise, we actually have scrolling work to do.
			// See if we have a wheel mouse...

			BOOL bSupport = ::GetSystemMetrics(SM_MOUSEWHEELPRESENT);

			// If not a wheel mouse, the middle button is really the
			// middle button and not the wheel; the application should've
			//	handled it.

			if (!bSupport)
				CView::OnMButtonDown(nFlags, point);
			else
			{
				if (m_pAnchorWindow == NULL)
				{
					BOOL bVertBar;
					BOOL bHorzBar;
					CheckScrollBars(bHorzBar, bVertBar);

					UINT nBitmapID = 0;

					// based on which scrollbars we have, figure out which
					// anchor cursor to use for the anchor window
					if (bVertBar)
					{
						if (bHorzBar)
							nBitmapID = AFX_IDC_MOUSE_ORG_HV;
						else
							nBitmapID = AFX_IDC_MOUSE_ORG_VERT;
					}
					else if (bHorzBar)
						nBitmapID = AFX_IDC_MOUSE_ORG_HORZ;

					// if there's no scrollbars, we don't do anything!
					if (nBitmapID == 0)
					{
						CView::OnMButtonDown(nFlags, point);
						return FALSE;
					}
					else
					{
						m_pAnchorWindow = new _AFX_MOUSEANCHORWND(point);
						m_pAnchorWindow->SetBitmap(nBitmapID);
						m_pAnchorWindow->Create(this);
						m_pAnchorWindow->ShowWindow(SW_SHOWNA);
					}
				}
				else
				{
					m_pAnchorWindow->DestroyWindow();
					delete m_pAnchorWindow;
					m_pAnchorWindow = NULL;
				}
			}

			return TRUE;
		}

		void ScrollToDevicePosition(POINT ptDev)
		{
			ASSERT(ptDev.x >= 0);
			ASSERT(ptDev.y >= 0);

			// Note: ScrollToDevicePosition can and is used to scroll out-of-range
			//  areas as far as CScrollView is concerned -- specifically in
			//  the print-preview code.  Since OnScrollBy makes sure the range is
			//  valid, ScrollToDevicePosition does not vector through OnScrollBy.

			int xOrig = GetScrollPos(SB_HORZ);
			SetScrollPos(SB_HORZ, ptDev.x);
			int yOrig = GetScrollPos(SB_VERT);
			SetScrollPos(SB_VERT, ptDev.y);
			ScrollWindow(xOrig - ptDev.x, yOrig - ptDev.y);
		}

		void SetScaleToFitSize(SIZE sizeTotal)
		{
			// Note: It is possible to set sizeTotal members to negative values to
			//  effectively invert either the X or Y axis.

			ASSERT(m_hWnd != NULL);
			m_nMapMode = MM_SCALETOFIT;     // special internal value
			m_totalLog = sizeTotal;

			// reset and turn any scroll bars off
			if (m_hWnd != NULL && (GetStyle() & (WS_HSCROLL|WS_VSCROLL)))
			{
				SetScrollPos(SB_HORZ, 0);
				SetScrollPos(SB_VERT, 0);
				EnableScrollBarCtrl(SB_BOTH, FALSE);
				ASSERT((GetStyle() & (WS_HSCROLL|WS_VSCROLL)) == 0);
			}

			CRect rectT;
			GetClientRect(rectT);
			m_totalDev = rectT.Size();

			if (m_hWnd != NULL)
			{
				// window has been created, invalidate
				UpdateBars();
				Invalidate(TRUE);
			}
		}

		void SetScrollSizes(int nMapMode, SIZE sizeTotal, const SIZE& sizePage = sizeDefault, const SIZE& sizeLine = sizeDefault)
		{
			ASSERT(sizeTotal.cx >= 0 && sizeTotal.cy >= 0);
			ASSERT(nMapMode > 0);
			ASSERT(nMapMode != MM_ISOTROPIC && nMapMode != MM_ANISOTROPIC);

			int nOldMapMode = m_nMapMode;
			m_nMapMode = nMapMode;
			m_totalLog = sizeTotal;

			//BLOCK: convert logical coordinate space to device coordinates
			{
				CWindowDC dc(NULL);
				ASSERT(m_nMapMode > 0);
				dc.SetMapMode(m_nMapMode);

				// total size
				m_totalDev = m_totalLog;
				dc.LPtoDP((LPPOINT)&m_totalDev);
				m_pageDev = sizePage;
				dc.LPtoDP((LPPOINT)&m_pageDev);
				m_lineDev = sizeLine;
				dc.LPtoDP((LPPOINT)&m_lineDev);
				if (m_totalDev.cy < 0)
					m_totalDev.cy = -m_totalDev.cy;
				if (m_pageDev.cy < 0)
					m_pageDev.cy = -m_pageDev.cy;
				if (m_lineDev.cy < 0)
					m_lineDev.cy = -m_lineDev.cy;
			} // release DC here

			// now adjust device specific sizes
			ASSERT(m_totalDev.cx >= 0 && m_totalDev.cy >= 0);
			if (m_pageDev.cx == 0)
				m_pageDev.cx = m_totalDev.cx / 10;
			if (m_pageDev.cy == 0)
				m_pageDev.cy = m_totalDev.cy / 10;
			if (m_lineDev.cx == 0)
				m_lineDev.cx = m_pageDev.cx / 10;
			if (m_lineDev.cy == 0)
				m_lineDev.cy = m_pageDev.cy / 10;

			if (m_hWnd != NULL)
			{
				// window has been created, invalidate now
				UpdateBars();
				if (nOldMapMode != m_nMapMode)
					Invalidate(TRUE);
			}
		}

		void UpdateBars()
		{
			// UpdateBars may cause window to be resized - ignore those resizings
			if (m_bInsideUpdate)
				return;         // Do not allow recursive calls

			// Lock out recursion
			m_bInsideUpdate = TRUE;

			// update the horizontal to reflect reality
			// NOTE: turning on/off the scrollbars will cause 'OnSize' callbacks
			ASSERT(m_totalDev.cx >= 0 && m_totalDev.cy >= 0);

			CRect rectClient;
			BOOL bCalcClient = TRUE;

			// allow parent to do inside-out layout first
			CWnd* pParentWnd = GetParent();
			if (pParentWnd != NULL)
			{
				// if parent window responds to this message, use just
				//  client area for scroll bar calc -- not "true" client area
				if ((BOOL)pParentWnd->SendMessage(WM_RECALCPARENT, 0, (LPARAM)(LPCRECT)&rectClient) != 0)
				{
					// use rectClient instead of GetTrueClientSize for
					//  client size calculation.
					bCalcClient = FALSE;
				}
			}

			CSize sizeClient;
			CSize sizeSb;

			if (bCalcClient)
			{
				// get client rect
				if (!GetTrueClientSize(sizeClient, sizeSb))
				{
					// no room for scroll bars (common for zero sized elements)
					CRect rect;
					GetClientRect(&rect);
					if (rect.right > 0 && rect.bottom > 0)
					{
						// if entire client area is not invisible, assume we have
						//  control over our scrollbars
						EnableScrollBarCtrl(SB_BOTH, FALSE);
					}
					m_bInsideUpdate = FALSE;
					return;
				}
			}
			else
			{
				// let parent window determine the "client" rect
				GetScrollBarSizes(sizeSb);
				sizeClient.cx = rectClient.right - rectClient.left;
				sizeClient.cy = rectClient.bottom - rectClient.top;
			}

			// enough room to add scrollbars
			CSize sizeRange;
			CPoint ptMove;
			CSize needSb;

			// get the current scroll bar state given the true client area
			GetScrollBarState(sizeClient, needSb, sizeRange, ptMove, bCalcClient);
			if (needSb.cx)
				sizeClient.cy -= sizeSb.cy;
			if (needSb.cy)
				sizeClient.cx -= sizeSb.cx;

			// first scroll the window as needed
			ScrollToDevicePosition(ptMove); // will set the scroll bar positions too

			// this structure needed to update the scrollbar page range
			SCROLLINFO info;
			info.fMask = SIF_PAGE | SIF_RANGE;
			info.nMin = 0;

			// now update the bars as appropriate
			EnableScrollBarCtrl(SB_HORZ, needSb.cx);
			if (needSb.cx)
			{
				info.nPage = sizeClient.cx;
				info.nMax = m_totalDev.cx - 1;
				if (!SetScrollInfo(SB_HORZ, &info, TRUE))
					SetScrollRange(SB_HORZ, 0, sizeRange.cx, TRUE);
			}
			EnableScrollBarCtrl(SB_VERT, needSb.cy);
			if (needSb.cy)
			{
				info.nPage = sizeClient.cy;
				info.nMax = m_totalDev.cy - 1;
				if (!SetScrollInfo(SB_VERT, &info, TRUE))
					SetScrollRange(SB_VERT, 0, sizeRange.cy, TRUE);
			}

			// remove recursion lockout
			m_bInsideUpdate = FALSE;
		}

};

BEGIN_MESSAGE_MAP(CScrollView, CView)
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_MOUSEWHEEL()
#ifndef MTL_REMOVE_CVIEW_PRINT
	ON_WM_PRINTCLIENT()
#endif // MTL_REMOVE_CVIEW_PRINT
	ON_WM_VSCROLL()
#ifndef _MTL_ATL3
	ON_MESSAGE(WM_MBUTTONDOWN, &CScrollView::HandleMButtonDown)
#endif
END_MESSAGE_MAP()



_INLINE BOOL _AFX_MOUSEANCHORWND::Create(CScrollView* pParent)
{
	ENSURE_VALID(pParent);
	ASSERT_KINDOF(CScrollView, pParent);

	pParent->ClientToScreen(&m_ptAnchor);

	m_rectDrag.top = m_ptAnchor.y - GetSystemMetrics(SM_CYDOUBLECLK);
	m_rectDrag.bottom = m_ptAnchor.y + GetSystemMetrics(SM_CYDOUBLECLK);
	m_rectDrag.left = m_ptAnchor.x - GetSystemMetrics(SM_CXDOUBLECLK);
	m_rectDrag.right = m_ptAnchor.x + GetSystemMetrics(SM_CXDOUBLECLK);

	BOOL bRetVal = CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, AfxRegisterWndClass(CS_SAVEBITS), NULL, WS_POPUP,
							m_ptAnchor.x - AFX_CX_ANCHOR_BITMAP / 2, m_ptAnchor.y - AFX_CY_ANCHOR_BITMAP / 2,
							AFX_CX_ANCHOR_BITMAP, AFX_CY_ANCHOR_BITMAP, NULL, NULL);
	SetOwner(pParent);

	if (bRetVal)
	{
		// set window's region for round effect
		CRgn rgn;
		rgn.CreateEllipticRgn(0, 0, AFX_CX_ANCHOR_BITMAP, AFX_CY_ANCHOR_BITMAP);
		SetWindowRgn(rgn, TRUE);

		// fire timer ever 50ms
		SetCapture();
		SetTimer(AFX_TIMER_ID_TIMER_TRACKING, 50, NULL);
	}

	return bRetVal;
}



/*============================================================================*/
// CFormView - generic view constructed from a dialog template

class CFormView : public CScrollView
{
	DECLARE_DYNAMIC(CFormView)

	// Construction
	public:      // must derive your own class
		CFormView(UINT nIDTemplate)
		{
			ASSERT_VALID_IDR(nIDTemplate);
			m_lpszTemplateName = MAKEINTRESOURCE(nIDTemplate);
			m_pCreateContext = NULL;
			m_hWndFocus = NULL;     // focus window is unknown
		}

		CFormView(LPCTSTR lpszTemplateName)
		{
			m_lpszTemplateName = lpszTemplateName;
			m_pCreateContext = NULL;
			m_hWndFocus = NULL;     // focus window is unknown
		}


	public:
		DECLARE_MESSAGE_MAP()

		LPCTSTR m_lpszTemplateName;
		CCreateContext* m_pCreateContext;
		HWND m_hWndFocus;   // last window to have focus

	public:

		virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
		{
			if (SaveFocusControl())
				return;     // don't call base class when focus is already set

			CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
		}

		virtual void OnActivateFrame(UINT nState, CFrameWnd* /*pFrameWnd*/)
		{
			if (nState == WA_INACTIVE)
				SaveFocusControl();     // save focus when frame loses activation
		}

		int OnCreate(LPCREATESTRUCT lpcs)
		{
			// since we can't get the create context parameter passed in
			//  through CreateDialog, we use a temporary member variable
			lpcs->lpCreateParams = (LPVOID)m_pCreateContext;
			return CScrollView::OnCreate(lpcs);
		}

		virtual void OnInitialUpdate()
		{
			ASSERT_VALID(this);

			if (!UpdateData(FALSE))
				TRACE(traceAppMsg, 0, "UpdateData failed during formview initial update.\n");

			CScrollView::OnInitialUpdate();
		}

		LRESULT OnPrintClient(CDC* pDC, UINT nFlags)
		{
			return CView::OnPrintClient(pDC, nFlags);
		}

		void OnSetFocus(CWnd* pOldWnd)
		{
			if (!::IsWindow(m_hWndFocus) || !::IsChild(m_hWnd, m_hWndFocus))
			{
				// invalid or unknown focus window... let windows handle it
				m_hWndFocus = NULL;
				Default();
				return;
			}
			// otherwise, set focus to the last known focus window
			::SetFocus(m_hWndFocus);
		}

		virtual	BOOL Create(LPCTSTR /*lpszClassName*/, LPCTSTR /*lpszWindowName*/,
							DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
		{
			ASSERT(pParentWnd != NULL);
			ASSERT(m_lpszTemplateName != NULL);

			m_pCreateContext = pContext;    // save state for later OnCreate

			// initialize common controls
			VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTLS_REG));
			AfxDeferRegisterClass(AFX_WNDCOMMCTLSNEW_REG);

			// call PreCreateWindow to get prefered extended style
			CREATESTRUCT cs; memset(&cs, 0, sizeof(CREATESTRUCT));
			if (dwRequestedStyle == 0)
				dwRequestedStyle = AFX_WS_DEFAULT_VIEW;
			cs.style = dwRequestedStyle;
			if (!PreCreateWindow(cs))
				return FALSE;

			// create a modeless dialog
			if (!CreateDlg(m_lpszTemplateName, pParentWnd))
				return FALSE;

			m_pCreateContext = NULL;

			// we use the style from the template - but make sure that
			//  the WS_BORDER bit is correct
			// the WS_BORDER bit will be whatever is in dwRequestedStyle
			ModifyStyle(WS_BORDER | WS_CAPTION, cs.style & (WS_BORDER | WS_CAPTION));
			ModifyStyleEx(WS_EX_CLIENTEDGE, cs.dwExStyle & WS_EX_CLIENTEDGE);

			SetDlgCtrlID(nID);

			CRect rectTemplate;
			GetWindowRect(rectTemplate);
			SetScrollSizes(MM_TEXT, rectTemplate.Size());

			// initialize controls etc
			if (!ExecuteDlgInit(m_lpszTemplateName))
				return FALSE;

			// force the size requested
			SetWindowPos(NULL, rect.left, rect.top,
				rect.right - rect.left, rect.bottom - rect.top,
				SWP_NOZORDER | SWP_NOACTIVATE);

			// make visible if requested
			if (dwRequestedStyle & WS_VISIBLE)
				ShowWindow(SW_NORMAL);

			return TRUE;
		}

		virtual BOOL PreTranslateMessage(MSG* pMsg)
		{
			ASSERT(pMsg != NULL);
			ASSERT_VALID(this);
			ASSERT(m_hWnd != NULL);

			// allow tooltip messages to be filtered
			if (CView::PreTranslateMessage(pMsg))
				return TRUE;

			// don't translate dialog messages when in Shift+F1 help mode
			CFrameWnd* pFrameWnd = GetTopLevelFrame();
			//if (pFrameWnd != NULL && pFrameWnd->m_bHelpMode)
			//	return FALSE;

			// since 'IsDialogMessage' will eat frame window accelerators,
			//   we call all frame windows' PreTranslateMessage first
			pFrameWnd = GetParentFrame();   // start with first parent frame
			while (pFrameWnd != NULL)
			{
				// allow owner & frames to translate before IsDialogMessage does
				if (pFrameWnd->PreTranslateMessage(pMsg))
					return TRUE;

				// try parent frames until there are no parent frames
				pFrameWnd = pFrameWnd->GetParentFrame();
			}

			// don't call IsDialogMessage if form is empty
			if (::GetWindow(m_hWnd, GW_CHILD) == NULL)
				return FALSE;

			// filter both messages to dialog and from children
			return PreTranslateInput(pMsg);
		}

		BOOL SaveFocusControl()    // updates m_hWndFocus
		{
			// save focus window if focus is on this window's controls
			HWND hWndFocus = ::GetFocus();
			if (hWndFocus != NULL && ::IsChild(m_hWnd, hWndFocus))
			{
				m_hWndFocus = hWndFocus;
				return TRUE;
			}
			return FALSE;
		}

};

BEGIN_MESSAGE_MAP(CFormView, CScrollView)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_PRINTCLIENT()
END_MESSAGE_MAP()






// Zoom States
#define _AFX_ZOOM_OUT    0
#define _AFX_ZOOM_MIDDLE 1
#define _AFX_ZOOM_IN     2

#define PREVIEW_MARGIN  8
#define PREVIEW_PAGEGAP 8

#ifndef MTL_REMOVE_CFRAMEWND_VIEW

class CPreviewView : public CScrollView
{
	DECLARE_DYNCREATE(CPreviewView)

	public:
		CPreviewView()
		{
			m_pToolBar = NULL;
			m_pPrintView = NULL;
			m_pOrigView = NULL;
			m_pPreviewInfo = NULL;
			m_pPreviewDC = NULL;
			m_pPreviewState = NULL;
			m_hMagnifyCursor = NULL;
			m_bPageNumDisplayed = FALSE;
			m_nZoomState = _AFX_ZOOM_OUT;

			// default to pointing to embedded array.  Allows for 2 pages
			m_pPageInfo = m_pageInfoArray;
			m_nMaxPages = 2;

			// initialize CScrollView members
			m_bCenter = TRUE;                   // Center Zoomed output in Scrollview
			m_nMapMode = MM_TEXT;
		}

		virtual ~CPreviewView()
		{
			m_dcPrint.Detach();         // print DC is deleted by CPrintInfo destructor

			delete m_pPreviewInfo;      // get rid of preview info
			delete m_pPreviewState;     // Get rid of preview state
			delete m_pPreviewDC;        // Get rid of preview DC object

			if (m_hMagnifyCursor != NULL)
			{
				// make sure that m_hMagnifyCursor isn't the current cursor when we destroy it
				::SetCursor(::LoadCursor(NULL, IDC_ARROW));
				DestroyCursor(m_hMagnifyCursor);
			}
		}

	public:
		DECLARE_MESSAGE_MAP()

		struct PAGE_INFO
		{
			PAGE_INFO() { }

			CRect rectScreen; // screen rect (screen device units)
			CSize sizeUnscaled; // unscaled screen rect (screen device units)
			CSize sizeScaleRatio; // scale ratio (cx/cy)
			CSize sizeZoomOutRatio; // scale ratio when zoomed out (cx/cy)
		};


		CDialogBar* m_pToolBar; // Toolbar for preview

		CView* m_pPrintView;
		CView* m_pOrigView;

		CPrintInfo* m_pPreviewInfo;
		CPreviewDC* m_pPreviewDC;  // Output and attrib DCs Set, not created
		CPrintPreviewState* m_pPreviewState; // State to 

		HCURSOR m_hMagnifyCursor;

		BOOL m_bPageNumDisplayed;// Flags whether or not page number has yet
		// been displayed on status line

		UINT m_nZoomState;
		UINT m_nZoomOutPages; // number of pages when zoomed out

		PAGE_INFO* m_pPageInfo; // Array of page info structures
		PAGE_INFO m_pageInfoArray[2]; // Embedded array for the default implementation

		UINT m_nMaxPages; // for sanity checks
		UINT m_nCurrentPage;
		UINT m_nPages;
		int m_nSecondPageOffset; // used to shift second page position

		CSize m_sizePrinterPPI; // printer pixels per inch
		CPoint m_ptCenterPoint;

		CDC m_dcPrint;             // Actual printer DC

	public:
		virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
		{
			if (bActivate)
			{
				CWnd* pFocusWnd = GetFocus();
				if (pFocusWnd == NULL ||
					(m_pToolBar != NULL && ::IsWindow(m_pToolBar->m_hWnd)))
				{
					if (!m_pToolBar->IsChild(pFocusWnd))
					{
						// focus is not already on a toolbar button - set it to one
						m_pToolBar->GetDlgItem(AFX_ID_PREVIEW_PRINT)->SetFocus();
					}
				}
			}
		}

		int OnCreate(LPCREATESTRUCT lpCreateStruct)
		{
			int retVal = CView::OnCreate(lpCreateStruct);
			if (retVal == -1)
				return -1;      // if -1 bag out

			CCreateContext* pContext = (CCreateContext*)lpCreateStruct->lpCreateParams;

			m_pOrigView = pContext->m_pLastView;
			ASSERT(m_pOrigView != NULL);
			ASSERT_KINDOF(CView, m_pOrigView);

			return retVal;
		}

		virtual void OnDraw(CDC* pDC)
		{
			ASSERT_VALID(pDC);

			// don't do anything if not fully initialized
			if (m_pPrintView == NULL || m_dcPrint.m_hDC == NULL)
				return;

			CPoint ViewportOrg = pDC->GetViewportOrg();

			CPen rectPen;
			rectPen.CreatePen(PS_SOLID, 2, GetSysColor(COLOR_WINDOWFRAME));
			CPen shadowPen;
			shadowPen.CreatePen(PS_SOLID, 3, GetSysColor(COLOR_BTNSHADOW));

			m_pPreviewInfo->m_bContinuePrinting = TRUE;     // do this once each paint

			for (UINT nPage = 0; nPage < m_nPages; nPage++)
			{
				int nSavedState = m_dcPrint.SaveDC();       // Save pristine state of DC

				// Use paint DC for print preview output
				m_pPreviewDC->SetOutputDC(pDC->GetSafeHdc());

				m_pPreviewInfo->m_nCurPage = m_nCurrentPage + nPage;

				// Only call PrepareDC if within page range, otherwise use default
				// rect to draw page rectangle
				if (m_nCurrentPage + nPage >= m_nCurrentPage &&
					m_nCurrentPage + nPage >= nPage &&
					m_nCurrentPage + nPage <= m_pPreviewInfo->GetMaxPage())
				{
					m_pPrintView->OnPrepareDC(m_pPreviewDC, m_pPreviewInfo);
				}

				// Set up drawing rect to entire page (in logical coordinates)
				m_pPreviewInfo->m_rectDraw.SetRect(0, 0,
					m_pPreviewDC->GetDeviceCaps(HORZRES),
					m_pPreviewDC->GetDeviceCaps(VERTRES));
				m_pPreviewDC->DPtoLP(&m_pPreviewInfo->m_rectDraw);

				// Draw empty page on screen

				pDC->SaveDC();          // save the output dc state

				CSize* pRatio = &m_pPageInfo[nPage].sizeScaleRatio;
				CRect* pRect = &m_pPageInfo[nPage].rectScreen;

				if (pRatio->cx == 0)
				{   // page position has not been determined
					PositionPage(nPage);    // compute page position
					if (m_nZoomState != _AFX_ZOOM_OUT)
					{
						ViewportOrg = -GetDeviceScrollPosition();
						if (m_bCenter)
						{
							CRect rect;
							GetClientRect(&rect);
							// if client area is larger than total device size,
							// override scroll positions to place origin such that
							// output is centered in the window
							if (m_totalDev.cx < rect.Width())
								ViewportOrg.x = (rect.Width() - m_totalDev.cx) / 2;
							if (m_totalDev.cy < rect.Height())
								ViewportOrg.y = (rect.Height() - m_totalDev.cy) / 2;
						}
					}
				}

				pDC->SetMapMode(MM_TEXT);   // Page Rectangle is in screen device coords
				pDC->SetViewportOrg(ViewportOrg);
				pDC->SetWindowOrg(0, 0);

				pDC->SelectStockObject(HOLLOW_BRUSH);
				pDC->SelectObject(&rectPen);
				pDC->Rectangle(pRect);

				pDC->SelectObject(&shadowPen);

				pDC->MoveTo(pRect->right + 1, pRect->top + 3);
				pDC->LineTo(pRect->right + 1, pRect->bottom + 1);
				pDC->MoveTo(pRect->left + 3, pRect->bottom + 1);
				pDC->LineTo(pRect->right + 1, pRect->bottom + 1);

				// erase background to white (most paper is white)
				CRect rectFill = *pRect;
				rectFill.left += 1;
				rectFill.top += 1;
				rectFill.right -= 2;
				rectFill.bottom -= 2;
				::FillRect(pDC->m_hDC, rectFill, (HBRUSH)GetStockObject(WHITE_BRUSH));

				pDC->RestoreDC(-1);     // restore to synchronized state

				if (!m_pPreviewInfo->m_bContinuePrinting ||
					m_nCurrentPage + nPage > m_pPreviewInfo->GetMaxPage())
				{
					m_pPreviewDC->ReleaseOutputDC();
					m_dcPrint.RestoreDC(nSavedState);   // restore to untouched state

					// if the first page is not displayable, back up one page
					// but never go below 1
					if (nPage == 0 && m_nCurrentPage > 1)
						SetCurrentPage(m_nCurrentPage - 1, TRUE);
					break;
				}

				// Display page number
				OnDisplayPageNumber(m_nCurrentPage, nPage + 1);

				// Set scale ratio for this page
				m_pPreviewDC->SetScaleRatio(pRatio->cx, pRatio->cy);

				CSize PrintOffset;
				VERIFY(m_pPreviewDC->Escape(GETPRINTINGOFFSET, 0, NULL, (LPVOID)&PrintOffset));
				m_pPreviewDC->PrinterDPtoScreenDP((LPPOINT)&PrintOffset);
				PrintOffset += (CSize)pRect->TopLeft();
				PrintOffset += CSize(1, 1);
				PrintOffset += (CSize)ViewportOrg;  // For Scrolling

				m_pPreviewDC->SetTopLeftOffset(PrintOffset);

				m_pPreviewDC->ClipToPage();
				m_pPrintView->OnPrint(m_pPreviewDC, m_pPreviewInfo);

				m_pPreviewDC->ReleaseOutputDC();

				m_dcPrint.RestoreDC(nSavedState);   // restore to untouched state

			}

			rectPen.DeleteObject();
			shadowPen.DeleteObject();
		}

		virtual void OnDisplayPageNumber(UINT nPage, UINT nPagesDisplayed) ;

		BOOL OnEraseBkgnd(CDC* pDC)
		{
			ASSERT_VALID(pDC);

			// Fill background with APPWORKSPACE
			CBrush backBrush(GetSysColor(COLOR_APPWORKSPACE));
			CBrush* pOldBrush = pDC->SelectObject(&backBrush);
			CRect rect;
			pDC->GetClipBox(&rect);     // Erase the area needed

			pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
			pDC->SelectObject(pOldBrush);
			return TRUE;
		}

		void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
		{
			if (m_nZoomState != _AFX_ZOOM_OUT)
				CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
		}

		void OnLButtonDown(UINT, CPoint point)
		{
			UINT nPage;
			if (!FindPageRect(point, nPage))
				return;                         // Didn't click on a page

			// Set new zoom state
			SetZoomState((m_nZoomState == _AFX_ZOOM_IN) ? _AFX_ZOOM_OUT : m_nZoomState + 1, nPage, point);
		}

		void OnNumPageChange() ;

		void OnNextPage()
		{
			SetCurrentPage(m_nCurrentPage + 1, TRUE);
		}

		// Only use the PrepareDC from CScrollView if we are zoomed in
		virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
		{
			ASSERT_VALID(pDC);
			if (m_nZoomState == _AFX_ZOOM_OUT)
				CView::OnPrepareDC(pDC, pInfo);
			else if (m_pPageInfo[0].sizeScaleRatio.cx != 0)
				CScrollView::OnPrepareDC(pDC, pInfo);
		}

		void OnPreviewClose()
		{
			CWnd* pMainWnd = GetParentFrame();
			if (DYNAMIC_DOWNCAST(CFrameWnd, pMainWnd) == NULL)
			{
				// if it's not a frame, we'll try the main window
				pMainWnd = AfxGetMainWnd();
			}

			CFrameWnd* pParent = (CFrameWnd*)pMainWnd;
			ASSERT_VALID(pParent);

			if (m_pToolBar)
				m_pToolBar->DestroyWindow();

			m_pToolBar = NULL;

			m_pPreviewInfo->m_nCurPage = m_nCurrentPage;
			m_pOrigView->OnEndPrintPreview(m_pPreviewDC, m_pPreviewInfo, CPoint(0, 0), this);
		}

		void OnPreviewPrint() ;
		
		void OnPrevPage()
		{
			SetCurrentPage(m_nCurrentPage - 1, TRUE);
		}

		BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
		{
			if (nHitTest != HTCLIENT)
				return CScrollView::OnSetCursor(pWnd, nHitTest, message);

			CPoint point;
			::GetCursorPos(&point);
			ScreenToClient(&point);     // client coordinates of mouse position

			UINT nPage;
			if (m_nZoomState != _AFX_ZOOM_IN && FindPageRect(point, nPage))
			{                       // On a page and not zoomed all the way in
				if (m_hMagnifyCursor == NULL)
				{
					HINSTANCE hInst = AfxFindResourceHandle(ATL_MAKEINTRESOURCE(AFX_IDC_MAGNIFY), ATL_RT_GROUP_CURSOR);
					m_hMagnifyCursor = ::LoadCursorW(hInst, ATL_MAKEINTRESOURCEW(AFX_IDC_MAGNIFY));
				}
				::SetCursor(m_hMagnifyCursor);
			}
			else
			{
				::SetCursor(::LoadCursor(NULL, IDC_ARROW));
			}
			return 0;
		}

		void OnSize(UINT nType, int cx, int cy)
		{
			// CScrollView handles everything if zoomed in.
			if (m_nZoomState == _AFX_ZOOM_OUT)
			{
				// Force recalc of scale ratios on next draw
				for (UINT i = 0; i < m_nMaxPages; i++)
					m_pPageInfo[i].sizeScaleRatio.cx = 0;           // zero scale ratios

				CView::OnSize(nType, cx, cy);       // No scroll functionality
			}
			else
			{
				// adjust scroll size to size of page
				m_pageDev.cx = cx;
				m_pageDev.cy = cy;
				m_lineDev.cx = cx / 10;
				m_lineDev.cy = cy / 10;
				CScrollView::OnSize(nType, cx, cy);
			}
		}

		void OnUpdateNextPage(CCmdUI* pCmdUI)
		{
			// enable if not showing last page
			pCmdUI->Enable(m_nCurrentPage + m_nPages - 1 < m_pPreviewInfo->GetMaxPage());
		}

		void OnUpdateNumPageChange(CCmdUI* pCmdUI)
		{
			// set text of button to opposite of current state
			CString text;
			UINT nPages = m_nZoomState == _AFX_ZOOM_OUT ? m_nPages : m_nZoomOutPages;
			ENSURE(text.LoadString(nPages == 1 ? AFX_IDS_TWOPAGE : AFX_IDS_ONEPAGE));
			pCmdUI->SetText(text);

			// enable it only if valid to display another page and not zoomed
			pCmdUI->Enable(m_nZoomState == _AFX_ZOOM_OUT && m_nMaxPages != 1 &&
				(m_pPreviewInfo->GetMaxPage() > 1 || m_nPages > 1));
		}

		void OnUpdatePrevPage(CCmdUI* pCmdUI)
		{
			// enable if not showing First page
			pCmdUI->Enable(m_nCurrentPage > m_pPreviewInfo->GetMinPage());
		}

		void OnUpdateZoomIn(CCmdUI* pCmdUI)
		{
			pCmdUI->Enable(m_nZoomState != _AFX_ZOOM_IN);
		}

		void OnUpdateZoomOut(CCmdUI* pCmdUI)
		{
			pCmdUI->Enable(m_nZoomState != _AFX_ZOOM_OUT);
		}

		void OnZoomIn()
		{
			if (m_nZoomState != _AFX_ZOOM_IN)
				SetZoomState(m_nZoomState + 1, 0, CPoint(0, 0));
		}

		void OnZoomOut()
		{
			if (m_nZoomState != _AFX_ZOOM_OUT)
				SetZoomState(m_nZoomState - 1, 0, CPoint(0, 0));
		}

		void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
		{
			if (m_nZoomState != _AFX_ZOOM_OUT)
			{
				CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
				return;
			}

			switch (nSBCode)
			{
				case SB_BOTTOM:
					SetCurrentPage(m_pPreviewInfo->GetMaxPage(), TRUE);
					break;

				case SB_TOP:
					SetCurrentPage(m_pPreviewInfo->GetMinPage(), TRUE);
					break;

				case SB_PAGEDOWN:
					SetCurrentPage(m_nCurrentPage +
						(m_pPreviewInfo->GetMaxPage() - m_pPreviewInfo->GetMinPage() + 9) / 10, TRUE);
					break;

				case SB_PAGEUP:
					SetCurrentPage(m_nCurrentPage -
						(m_pPreviewInfo->GetMaxPage() - m_pPreviewInfo->GetMinPage() + 9) / 10, TRUE);
					break;

				case SB_LINEDOWN:
					SetCurrentPage(m_nCurrentPage + 1, TRUE);
					break;

				case SB_LINEUP:
					SetCurrentPage(m_nCurrentPage - 1, TRUE);
					break;

				case SB_THUMBPOSITION:
					SetCurrentPage(nPos, TRUE);
					break;
			}
		}



		CSize CalcPageDisplaySize()
			// calculate the current page size
			//  set 'm_nSecondPageOffset' to start of second page
			// return size of current page less margins
		{
			CSize windowSize, scrollSize;
			GetTrueClientSize(windowSize, scrollSize);

			// subtract out vertical scrollbar if zoomed out and page range is known
			// and there is more than one page.
			if (m_nZoomState == _AFX_ZOOM_OUT && (m_pPreviewInfo->GetMaxPage() != 0xffff) &&
				(m_pPreviewInfo->GetMaxPage() - m_pPreviewInfo->GetMinPage() != 0))
				windowSize.cx -= scrollSize.cx;

			m_nSecondPageOffset = (windowSize.cx - PREVIEW_MARGIN) / 2;

			windowSize.cx = (m_nPages == 2) ? (windowSize.cx - 3 * PREVIEW_MARGIN) / 2 :
				windowSize.cx - 2 * PREVIEW_MARGIN;

			windowSize.cy -= 2 * PREVIEW_MARGIN;
			return windowSize;
		}

		// Return is actually the fraction cx/cy. Simply using CSize for convenience
		virtual CSize CalcScaleRatio(CSize screenSize, CSize actualSize)
		{
			// Test ratio based on vertical dimension to see if it is the one to use
			int nNum = screenSize.cy;
			int nDen = actualSize.cy;

			// If scaled width too large, choose width as primary dimension
			if (MulDiv(actualSize.cx, nNum, nDen) > screenSize.cx)
			{
				// wrong ratio--base on width
				nNum = screenSize.cx;
				nDen = actualSize.cx;
			}
			CSize ratio(nNum, nDen);
			return ratio;
		}

		// Actual zoom code.
		void DoZoom(UINT nPage, CPoint point)
		{
			if (m_nZoomState == _AFX_ZOOM_OUT)
			{
				// taking over scroll bars
				m_nPages = m_nZoomOutPages;
				ShowScrollBar(SB_HORZ, FALSE);      //hide the horizontal bar

				BOOL bShowBar = m_pPreviewInfo->GetMaxPage() < 0x8000 &&
					m_pPreviewInfo->GetMaxPage() - m_pPreviewInfo->GetMinPage() <= 32767U;

				ShowScrollBar(SB_VERT, bShowBar);       //Show the vertical bar

				if (bShowBar)
				{
					SCROLLINFO info;
					info.fMask = SIF_PAGE | SIF_RANGE;
					info.nMin = m_pPreviewInfo->GetMinPage();
					info.nMax = m_pPreviewInfo->GetMaxPage();
					info.nPage = 1;
					if (!SetScrollInfo(SB_VERT, &info, FALSE))
						SetScrollRange(SB_VERT, info.nMin, info.nMax, FALSE);
				}

				SetCurrentPage(m_nCurrentPage, TRUE);
			}
			else
			{
				m_nPages = 1;       // only one page in zoomed states

				m_pPageInfo[0].sizeZoomOutRatio = m_pPageInfo[nPage].sizeZoomOutRatio;
				m_pPageInfo[0].sizeUnscaled = m_pPageInfo[nPage].sizeUnscaled;

				// Sets the printer page
				SetCurrentPage(m_nCurrentPage + nPage, FALSE);

				SetScaledSize(0);

				CSize* pRatio = &m_pPageInfo[nPage].sizeScaleRatio;

				// convert Hit Point from screen 1:1
				point.x = MulDiv(point.x, pRatio->cx, pRatio->cy);
				point.y = MulDiv(point.y, pRatio->cx, pRatio->cy);

				// Adjust point for page position
				point += (CSize)m_pPageInfo[0].rectScreen.TopLeft();

				// Scroll to center
				CenterOnPoint(point);
			}
		}

		// Finds page pointed to and convert to 1:1 screen device units
		BOOL FindPageRect(CPoint& point, UINT& nPage)
		{
			if (m_nZoomState != _AFX_ZOOM_OUT)
				point += (CSize)GetDeviceScrollPosition();

			CRect rectClient;
			this->GetClientRect(&rectClient);

			for (nPage = 0; nPage < m_nPages; nPage++)
			{
				CRect rectScreen(m_pPageInfo[nPage].rectScreen);
				/* When zoom state is not _AFX_ZOOM_OUT, only one page can be displayed.
				Two-page is available only on state _AFX_ZOOM_OUT

				When zoom state is not _AFX_ZOOM_OUT, the rectScreen need to be adjusted for point testing */
				if (m_nZoomState != _AFX_ZOOM_OUT)
				{
					int nDeltaX = (rectClient.Size().cx - rectScreen.Size().cx - 2 * PREVIEW_MARGIN) / 2 - 1;
					int nDeltaY = (rectClient.Size().cy - rectScreen.Size().cy - 2 * PREVIEW_MARGIN) / 2 - 1;

					// When delta is less than 0, don't need to change the value.
					nDeltaX = nDeltaX < 0 ? 0 : nDeltaX;
					nDeltaY = nDeltaY < 0 ? 0 : nDeltaY;
					rectScreen.OffsetRect(nDeltaX, nDeltaY);
				}
				if (rectScreen.PtInRect(point))
				{
					// adjust point for page position
					point -= (CSize)m_pPageInfo[nPage].rectScreen.TopLeft();

					// convert to 1:1
					point.x = MulDiv(point.x, m_pPageInfo[nPage].sizeScaleRatio.cy, m_pPageInfo[nPage].sizeScaleRatio.cx);
					point.y = MulDiv(point.y, m_pPageInfo[nPage].sizeScaleRatio.cy, m_pPageInfo[nPage].sizeScaleRatio.cx);
					return TRUE;
				}
			}
			return FALSE;
		}

		virtual void PositionPage(UINT nPage)
		{
			CSize windowSize = CalcPageDisplaySize();

			VERIFY(m_dcPrint.Escape(GETPHYSPAGESIZE, 0, NULL,
				(LPVOID)&m_pPageInfo[nPage].sizeUnscaled));

			CSize* pSize = &m_pPageInfo[nPage].sizeUnscaled;

			// Convert page size to screen coordinates
			pSize->cx = MulDiv(pSize->cx, afxData.cxPixelsPerInch, m_sizePrinterPPI.cx);
			pSize->cy = MulDiv(pSize->cy, afxData.cyPixelsPerInch, m_sizePrinterPPI.cy);

			m_pPageInfo[nPage].sizeZoomOutRatio = CalcScaleRatio(windowSize, *pSize);

			SetScaledSize(nPage);
		}

		void SetCurrentPage(UINT nPage, BOOL bClearRatios)
		{
			m_nCurrentPage = nPage;
			if (m_nCurrentPage > m_pPreviewInfo->GetMaxPage())
				m_nCurrentPage = m_pPreviewInfo->GetMaxPage();
			if (m_nCurrentPage < m_pPreviewInfo->GetMinPage())
				m_nCurrentPage = m_pPreviewInfo->GetMinPage();


			if (m_nZoomState == _AFX_ZOOM_OUT)
				SetScrollPos(SB_VERT, m_nCurrentPage);

			if (bClearRatios)
			{
				// Force Recalc of layout
				for (UINT i = 0; i < m_nMaxPages; i++)
					m_pPageInfo[i].sizeScaleRatio.cx = 0;           // zero scale ratios
			}

			Invalidate(TRUE);
		}

		BOOL SetPrintView(CView* pPrintView)
		{
			ASSERT_VALID(pPrintView);

			m_pPrintView = pPrintView;

			// allocate preview info
			m_pPreviewInfo = new CPrintInfo;
			m_pPreviewInfo->m_pPD->m_pd.Flags |= PD_PRINTSETUP;
			m_pPreviewInfo->m_pPD->m_pd.Flags &= ~PD_RETURNDC;

			m_pPreviewInfo->m_bPreview = TRUE;  // signal that this is preview
			ASSERT(m_pPreviewInfo->m_pPD != NULL);

			m_pPreviewDC = new CPreviewDC;      // must be created before any
			// possible error returns

			if (!m_pPrintView->OnPreparePrinting(m_pPreviewInfo))
				return FALSE;

			m_dcPrint.Attach(m_pPreviewInfo->m_pPD->m_pd.hDC);
			m_pPreviewDC->SetAttribDC(m_pPreviewInfo->m_pPD->m_pd.hDC);
			m_pPreviewDC->m_bPrinting = TRUE;
			m_dcPrint.m_bPrinting = TRUE;

			m_dcPrint.SaveDC();     // Save pristine state of DC

			HDC hDC = ::GetDC(m_hWnd);
			m_pPreviewDC->SetOutputDC(hDC);
			m_pPrintView->OnBeginPrinting(m_pPreviewDC, m_pPreviewInfo);
			m_pPreviewDC->ReleaseOutputDC();
			::ReleaseDC(m_hWnd, hDC);

			m_dcPrint.RestoreDC(-1);    // restore to untouched state

			// Get Pixels per inch from Printer
			m_sizePrinterPPI.cx = m_dcPrint.GetDeviceCaps(LOGPIXELSX);
			m_sizePrinterPPI.cy = m_dcPrint.GetDeviceCaps(LOGPIXELSY);

			m_nPages = m_pPreviewInfo->m_nNumPreviewPages;
			if (m_nPages == 0)
				m_nPages = 1;
			else if (m_nPages > m_nMaxPages)
				m_nPages = m_nMaxPages;     // Sanity Check!

			m_nZoomOutPages = m_nPages;

			SetScrollSizes(MM_TEXT, CSize(1, 1));   // initialize mapping mode only

			if (m_pPreviewInfo->GetMaxPage() < 0x8000 &&
				m_pPreviewInfo->GetMaxPage() - m_pPreviewInfo->GetMinPage() <= 32767U)
			{
				SCROLLINFO info;
				info.fMask = SIF_PAGE | SIF_RANGE;
				info.nMin = m_pPreviewInfo->GetMinPage();
				info.nMax = m_pPreviewInfo->GetMaxPage();
				info.nPage = 1;
				if (!SetScrollInfo(SB_VERT, &info, FALSE))
					SetScrollRange(SB_VERT, info.nMin, info.nMax, FALSE);
			}
			else
				ShowScrollBar(SB_VERT, FALSE);      // if no range specified, or too
			// large don't show

			SetCurrentPage(m_pPreviewInfo->m_nCurPage, TRUE);
			return TRUE;
		}

		void SetScaledSize(UINT nPage)
		{
			CSize* pSize = &m_pPageInfo[nPage].sizeUnscaled;
			CSize* pRatio = &m_pPageInfo[nPage].sizeScaleRatio;
			CSize* pZoomOutRatio = &m_pPageInfo[nPage].sizeZoomOutRatio;
			CSize windowSize = CalcPageDisplaySize();
			BOOL bPaperLarger = pZoomOutRatio->cx < pZoomOutRatio->cy;
			// whether the paper is larger than the screen, or vice versa

			switch (m_nZoomState)
			{
				case _AFX_ZOOM_OUT:
					*pRatio = *pZoomOutRatio;
					break;

				case _AFX_ZOOM_MIDDLE:
					// the middle zoom state is a ratio between cx/cy and
					// 1/1 (or cy/cy).  It is, therefore:
					//
					// (cx + cy)/2
					// -----------
					//     cy
					//
					// if the paper is larger than the screen, or
					//
					// (3*cx - cy)/2
					// -------------
					//      cy
					//
					// if the paper is smaller than the screen.
					if (bPaperLarger)
					{
						pRatio->cy = pZoomOutRatio->cy;
						pRatio->cx = (pZoomOutRatio->cx + pRatio->cy) / 2;
					}
					else
					{
						pRatio->cy = pZoomOutRatio->cy;
						pRatio->cx = (3 * pZoomOutRatio->cx - pRatio->cy) / 2;
					}
					break;

				case _AFX_ZOOM_IN:
					if (bPaperLarger)
						pRatio->cx = pRatio->cy = 1;
					else
					{
						// if the paper is smaller than the screen space we're displaying
						// it in, then using a ratio of 1/1 will result in a smaller image
						// on the screen, not a larger one. To get a larger image in this
						// case we double the zoom out ratio.
						pRatio->cy = pZoomOutRatio->cy;
						pRatio->cx = 2 * pZoomOutRatio->cx - pZoomOutRatio->cy;
					}
					break;

				default:
					ASSERT(FALSE);
			}

			// Convert to scaled size
			CSize scaledSize;
			scaledSize.cx = MulDiv(pSize->cx, pRatio->cx, pRatio->cy);
			scaledSize.cy = MulDiv(pSize->cy, pRatio->cx, pRatio->cy);

			CRect* pRect = &m_pPageInfo[nPage].rectScreen;
			pRect->SetRect(PREVIEW_MARGIN, PREVIEW_MARGIN,
							scaledSize.cx + PREVIEW_MARGIN + 3,
							scaledSize.cy + PREVIEW_MARGIN + 3);

			if (m_nZoomState == _AFX_ZOOM_OUT)
			{
				pRect->OffsetRect((windowSize.cx - pRect->Size().cx) / 2 - 1,
									(windowSize.cy - pRect->Size().cy) / 2 - 1);

				if (nPage == 1)
					pRect->OffsetRect(m_nSecondPageOffset, 0);
			}
			else
			{
				// set up scroll size

				SetScrollSizes(MM_TEXT, pRect->Size() + CSize(PREVIEW_MARGIN * 2, PREVIEW_MARGIN * 2), windowSize);
			}
		}

		void SetZoomState(UINT nNewState, UINT nPage, CPoint point)
		{
			if (m_nZoomState != nNewState)
			{
				m_nZoomState = nNewState;
				DoZoom(nPage, point);
			}
		}


};


BEGIN_MESSAGE_MAP(CPreviewView, CScrollView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()        // overriding CScrollView
	ON_WM_VSCROLL()

	ON_COMMAND(AFX_ID_PREVIEW_CLOSE, &CPreviewView::OnPreviewClose)

	ON_COMMAND(AFX_ID_PREVIEW_NUMPAGE, &CPreviewView::OnNumPageChange)
	ON_UPDATE_COMMAND_UI(AFX_ID_PREVIEW_NUMPAGE, &CPreviewView::OnUpdateNumPageChange)
	ON_COMMAND(AFX_ID_PREVIEW_NEXT, &CPreviewView::OnNextPage)
	ON_UPDATE_COMMAND_UI(AFX_ID_PREVIEW_NEXT, &CPreviewView::OnUpdateNextPage)
	ON_COMMAND(AFX_ID_PREVIEW_PREV, &CPreviewView::OnPrevPage)
	ON_UPDATE_COMMAND_UI(AFX_ID_PREVIEW_PREV, &CPreviewView::OnUpdatePrevPage)

	ON_COMMAND(AFX_ID_PREVIEW_PRINT, &CPreviewView::OnPreviewPrint)

	ON_COMMAND(AFX_ID_PREVIEW_ZOOMIN, &CPreviewView::OnZoomIn)
	ON_UPDATE_COMMAND_UI(AFX_ID_PREVIEW_ZOOMIN, &CPreviewView::OnUpdateZoomIn)
	ON_COMMAND(AFX_ID_PREVIEW_ZOOMOUT, &CPreviewView::OnZoomOut)
	ON_UPDATE_COMMAND_UI(AFX_ID_PREVIEW_ZOOMOUT, &CPreviewView::OnUpdateZoomOut)

END_MESSAGE_MAP()


_INLINE BOOL CALLBACK CPrintPreviewState::_AfxPreviewCloseProc(CFrameWnd* pFrameWnd)
{
	ASSERT_VALID(pFrameWnd);
	CPreviewView* pView = (CPreviewView*)pFrameWnd->GetDlgItem(AFX_IDW_PANE_FIRST);
	ASSERT_KINDOF(CPreviewView, pView);

	pView->OnPreviewClose();
	return FALSE;
}

_INLINE void CView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT, CPreviewView* pView)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pView);

	if (pView->m_pPrintView != NULL)
		pView->m_pPrintView->OnEndPrinting(pDC, pInfo);

	CWnd* pMainWnd = GetParentFrame();
	if (DYNAMIC_DOWNCAST(CFrameWnd, pMainWnd) == NULL)
	{
		// if it's not a frame, we'll try the main window
		pMainWnd = AfxGetMainWnd();
	}

	CFrameWnd* pParent = (CFrameWnd*)pMainWnd;
	ASSERT_VALID(pParent);

	// restore the old main window
	pParent->OnSetPreviewMode(FALSE, pView->m_pPreviewState);

	// Force active view back to old one
	pParent->SetActiveView(pView->m_pPreviewState->pViewActiveOld);
	if (pParent != GetParentFrame())
		OnActivateView(TRUE, this, this);   // re-activate view in real frame
	pView->DestroyWindow();     // destroy preview view
	// C++ object will be deleted in PostNcDestroy

	// restore main frame layout and idle message
	pParent->RecalcLayout();
	pParent->SendMessage(WM_SETMESSAGESTRING, (WPARAM)AFX_IDS_IDLEMESSAGE, 0L);
	pParent->UpdateWindow();
}

_INLINE void CView::OnFilePrintPreview()
{
	// In derived classes, implement special window handling here
	// Be sure to Unhook Frame Window close if hooked.

	// must not create this on the frame.  Must outlive this function
	CPrintPreviewState* pState = new CPrintPreviewState;

	TRY
	{
		// DoPrintPreview's return value does not necessarily indicate that
		// Print preview succeeded or failed, but rather what actions are necessary
		// at this point.  If DoPrintPreview returns TRUE, it means that
		// OnEndPrintPreview will be (or has already been) called and the
		// pState structure will be/has been deleted.
		// If DoPrintPreview returns FALSE, it means that OnEndPrintPreview
		// WILL NOT be called and that cleanup, including deleting pState
		// must be done here.

		if (!DoPrintPreview(AFX_IDD_PREVIEW_TOOLBAR, this, RUNTIME_CLASS(CPreviewView), pState))
		{
			// In derived classes, reverse special window handling here for
			// Preview failure case

			TRACE(traceAppMsg, 0, "Error: DoPrintPreview failed.\n");
			AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
			delete pState;      // preview failed to initialize, delete State now
		}
	}
	CATCH_ALL(e)
	{
		delete pState;
		THROW_LAST();
	}
	END_CATCH_ALL
}


_INLINE BOOL CView::DoPrintPreview(UINT nIDResource, CView* pPrintView, CRuntimeClass* pPreviewViewClass, CPrintPreviewState* pState)
{
	ASSERT_VALID_IDR(nIDResource);
	ASSERT_VALID(pPrintView);
	ASSERT(pPreviewViewClass != NULL);
	ASSERT(pPreviewViewClass->IsDerivedFrom(RUNTIME_CLASS(CPreviewView)));
	ASSERT(pState != NULL);

	CWnd* pMainWnd = GetParentFrame();
	if (DYNAMIC_DOWNCAST(CFrameWnd, pMainWnd) == NULL)
	{
		// if it's not a frame, we'll try the main window
		pMainWnd = AfxGetMainWnd();
	}

	CFrameWnd* pParent = (CFrameWnd*)pMainWnd;
	ASSERT_VALID(pParent);

	CCreateContext context;
	context.m_pCurrentFrame = pParent;
	context.m_pCurrentDoc = GetDocument();
	context.m_pLastView = this;

	// Create the preview view object
	CPreviewView* pView = (CPreviewView*)pPreviewViewClass->CreateObject();
	if (pView == NULL)
	{
		TRACE(traceAppMsg, 0, "Error: Failed to create preview view.\n");
		return FALSE;
	}
	ASSERT_KINDOF(CPreviewView, pView);
	pView->m_pPreviewState = pState;        // save pointer

	pParent->OnSetPreviewMode(TRUE, pState);    // Take over Frame Window

	// Create the toolbar from the dialog resource
	pView->m_pToolBar = new CDialogBar;

	CFrameWnd *pParentFrame = pParent->GetActiveFrame();
	ASSERT(pParentFrame);

	if (!pView->m_pToolBar->Create(pParent, ATL_MAKEINTRESOURCE(nIDResource), CBRS_TOP, AFX_IDW_PREVIEW_BAR))
	{
		TRACE(traceAppMsg, 0, "Error: Preview could not create toolbar dialog.\n");
		pParent->OnSetPreviewMode(FALSE, pState);   // restore Frame Window
		delete pView->m_pToolBar;       // not autodestruct yet
		pView->m_pToolBar = NULL;
		pView->m_pPreviewState = NULL;  // do not delete state structure
		delete pView;
		return FALSE;
	}
	pView->m_pToolBar->m_bAutoDelete = TRUE;    // automatic cleanup

	// Create the preview view as a child of the App Main Window.  This
	// is a sibling of this view if this is an SDI app.  This is NOT a sibling
	// if this is an MDI app.

	if (!pView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), pParent, AFX_IDW_PANE_FIRST, &context))
	{
		TRACE(traceAppMsg, 0, "Error: couldn't create preview view for frame.\n");
		pParent->OnSetPreviewMode(FALSE, pState);   // restore Frame Window
		pView->m_pPreviewState = NULL;  // do not delete state structure
		delete pView;
		return FALSE;
	}

	// Preview window shown now
	pState->pViewActiveOld = pParent->GetActiveView();
	CView* pActiveView = pParent->GetActiveFrame()->GetActiveView();

	if (pActiveView != NULL)
		pActiveView->OnActivateView(FALSE, pActiveView, pActiveView);

	if (!pView->SetPrintView(pPrintView))
	{
		pView->OnPreviewClose();
		return TRUE;            // signal that OnEndPrintPreview was called
	}

	pParent->SetActiveView(pView);  // set active view - even for MDI

	pView->m_pToolBar->SendMessage(WM_IDLEUPDATECMDUI, (WPARAM)TRUE);
	pParent->RecalcLayout();            // position and size everything
	pParent->UpdateWindow();

	return TRUE;
}

#endif // MTL_REMOVE_CFRAMEWND_VIEW


