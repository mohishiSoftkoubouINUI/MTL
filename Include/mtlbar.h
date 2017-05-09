



/*============================================================================*/
// toolbar docking support

class CControlBar ;
class CDockBar ;

#define HORZF(dw) (dw & CBRS_ORIENT_HORZ)
#define VERTF(dw) (dw & CBRS_ORIENT_VERT)

#define m_rectRequestedSize     m_rectDragHorz
#define m_rectActualSize        m_rectDragVert
#define m_rectActualFrameSize   m_rectFrameDragHorz
#define m_rectFrameBorders      m_rectFrameDragVert

class CDockContext
{
	public:
		// Construction
		CDockContext(CControlBar* pBar);

		virtual ~CDockContext() ;
			

	public:
		const struct _AUX_DATA& afxData;

		// Attributes
		CPoint m_ptLast;            // last mouse position during drag
		CRect m_rectLast;
		CSize m_sizeLast;
		BOOL m_bDitherLast;

		// Rectangles used during dragging or resizing
		CRect m_rectDragHorz;
		CRect m_rectDragVert;
		CRect m_rectFrameDragHorz;
		CRect m_rectFrameDragVert;

		CControlBar* m_pBar;        // the toolbar that created this context
		CFrameWnd* m_pDockSite;     // the controlling frame of the CControlBar
		DWORD m_dwDockStyle;        // allowable dock styles for bar
		DWORD m_dwOverDockStyle;    // style of dock that rect is over
		DWORD m_dwStyle;            // style of control bar
		BOOL m_bFlip;               // if shift key is down
		BOOL m_bForceFrame;         // if ctrl key is down

		CDC* m_pDC;                 // where to draw during drag
		BOOL m_bDragging;
		int m_nHitTest;

		UINT m_uMRUDockID;
		CRect m_rectMRUDockPos;

		DWORD m_dwMRUFloatStyle;
		CPoint m_ptMRUFloatPos;

	public:
		static void _AfxAdjustRectangle(CRect& rect, CPoint pt)
		{
			int nXOffset = (pt.x < rect.left) ? (pt.x - rect.left) : (pt.x > rect.right) ? (pt.x - rect.right) : 0;
			int nYOffset = (pt.y < rect.top) ? (pt.y - rect.top) : (pt.y > rect.bottom) ? (pt.y - rect.bottom) : 0;
			rect.OffsetRect(nXOffset, nYOffset);
		}


		DWORD CanDock() ;

		void CancelLoop()
		{
			DrawFocusRect(TRUE);    // gets rid of focus rect
			ReleaseCapture();

			CWnd* pWnd = CWnd::GetDesktopWindow();
			pWnd->UnlockWindowUpdate();
			if (m_pDC != NULL)
			{
				pWnd->ReleaseDC(m_pDC);
				m_pDC = NULL;
			}
		}

		void DrawFocusRect(BOOL bRemoveRect = FALSE)
		{
			ASSERT(m_pDC != NULL);

			// default to thin frame
			CSize size(AFX_CX_BORDER, AFX_CY_BORDER);

			// determine new rect and size
			CRect rect;
			CBrush* pWhiteBrush = CBrush::FromHandle((HBRUSH)::GetStockObject(WHITE_BRUSH));
			CBrush* pDitherBrush = CDC::GetHalftoneBrush();
			CBrush* pBrush = pWhiteBrush;

			if (HORZF(m_dwOverDockStyle))
				rect = m_rectDragHorz;
			else if (VERTF(m_dwOverDockStyle))
				rect = m_rectDragVert;
			else
			{
				// use thick frame instead
				size.cx = GetSystemMetrics(SM_CXFRAME) - AFX_CX_BORDER;
				size.cy = GetSystemMetrics(SM_CYFRAME) - AFX_CY_BORDER;
				if ((HORZF(m_dwStyle) && !m_bFlip) || (VERTF(m_dwStyle) && m_bFlip))
					rect = m_rectFrameDragHorz;
				else
					rect = m_rectFrameDragVert;
				pBrush = pDitherBrush;
			}
			if (bRemoveRect)
				size.cx = size.cy = 0;

			if ((HORZF(m_dwOverDockStyle) || VERTF(m_dwOverDockStyle)))
			{
				// looks better one pixel in (makes the bar look pushed down)
				rect.InflateRect(-AFX_CX_BORDER, -AFX_CY_BORDER);
			}
#if 0
			TRACE("DrawFocusRect(%d), rect(%d,%d,%d,%d), size(%d,%d), m_rectLast(%d,%d,%d,%d), m_sizeLast(%d,%d)\n",
				bRemoveRect, rect.left, rect.top, rect.Width(), rect.Height(), size.cx, size.cy,
				m_rectLast.left, m_rectLast.top, m_rectLast.Width(), m_rectLast.Height(), m_sizeLast.cx, m_sizeLast.cy);
#endif
			// draw it and remember last size
			m_pDC->DrawDragRect(&rect, size, &m_rectLast, m_sizeLast, pBrush, m_bDitherLast ? pDitherBrush : pWhiteBrush);
			m_rectLast = rect;
			m_sizeLast = size;
			m_bDitherLast = (pBrush == pDitherBrush);

		}

		void EndDrag() ;
			
		void EndResize() ;
			
		CDockBar* GetDockBar(DWORD dwOverDockStyle) ; 
			
		void InitLoop() ;

		void Move(CPoint pt)
		{
			CPoint ptOffset = pt - m_ptLast;

			// offset all drag rects to new position
			m_rectDragHorz.OffsetRect(ptOffset);
			m_rectFrameDragHorz.OffsetRect(ptOffset);
			m_rectDragVert.OffsetRect(ptOffset);
			m_rectFrameDragVert.OffsetRect(ptOffset);
			m_ptLast = pt;

			// if control key is down don't dock
			m_dwOverDockStyle = m_bForceFrame ? 0 : CanDock();

			// update feedback
			DrawFocusRect();
		}

		void OnKey(int nChar, BOOL bDown)
		{
			if (nChar == VK_CONTROL)
				UpdateState(&m_bForceFrame, bDown);
			if (nChar == VK_SHIFT)
				UpdateState(&m_bFlip, bDown);
		}

		// Drag Operations
		virtual void StartDrag(CPoint pt) ;

		virtual void StartResize(int nHitTest, CPoint pt) ;
			
		void Stretch(CPoint pt) ;

		// Double Click Operations
		void ToggleDocking() ;

		BOOL Track() ;

		void UpdateState(BOOL* pFlag, BOOL bNewValue)
		{
			if (*pFlag != bNewValue)
			{
				*pFlag = bNewValue;
				m_bFlip = (HORZF(m_dwDockStyle) && VERTF(m_dwDockStyle) && m_bFlip); // shift key
				m_dwOverDockStyle = (m_bForceFrame) ? 0 : CanDock();
				DrawFocusRect();
			}
		}

};


class CControlBar : public CWnd
{
	DECLARE_DYNAMIC(CControlBar)

	public:
		CControlBar() : afxData(CAfx::GetAfxData())
		{
			m_bAutoDelete = FALSE ;

			// no elements contained in the control bar yet
			m_nCount = 0;
			m_pData = NULL;

			// set up some default border spacings
			m_cxLeftBorder = m_cxRightBorder = 6;
			m_cxDefaultGap = 2;
			m_cyTopBorder = m_cyBottomBorder = 1;
			//m_bAutoDelete = FALSE;
			m_nStateFlags = 0;
			m_pDockSite = NULL;
			m_pDockBar = NULL;
			m_pDockContext = NULL;
			m_dwStyle = 0;
			m_dwDockStyle = 0;
			m_nMRUWidth = 32767;
			//m_pInPlaceOwner = NULL;

		}

		virtual ~CControlBar() ;
			
	public:
		DECLARE_MESSAGE_MAP()

		const struct _AUX_DATA& afxData ;

		BOOL m_bAutoDelete;

		// info about bar (for status bar and toolbar)
		int m_cxLeftBorder, m_cxRightBorder;
		int m_cyTopBorder, m_cyBottomBorder;
		int m_cxDefaultGap;         // default gap value
		UINT m_nMRUWidth;   // For dynamic resizing.

		// array of elements
		int m_nCount;
		void* m_pData;        // m_nCount elements - type depends on derived class


		// support for delayed hide/show
		enum StateFlags
		{
			delayHide = 1, delayShow = 2, tempHide = 4, statusSet = 8
		};
		UINT m_nStateFlags;

		// support for docking
		DWORD m_dwStyle;    // creation style (used for layout)
		DWORD m_dwDockStyle;// indicates how bar can be docked
		CFrameWnd* m_pDockSite; // current dock site, if dockable
		CDockBar* m_pDockBar;   // current dock bar, if dockable
		CDockContext* m_pDockContext;   // used during dragging

	public:
		virtual void OnBarStyleChange(DWORD dwOldStyle, DWORD dwNewStyle) {}

		int OnCreate(LPCREATESTRUCT lpcs) ;
		void OnDestroy() ;

		LRESULT OnHelpHitTest(WPARAM, LPARAM lParam)
		{
			ASSERT_VALID(this);

			INT_PTR nID = OnToolHitTest((DWORD)lParam, NULL);
			if (nID != -1)
				return HID_BASE_COMMAND + nID;

			nID = _AfxGetDlgCtrlID(m_hWnd);
			return nID != 0 ? HID_BASE_CONTROL + nID : 0;
		}

		LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM) ;
		
		void OnInitialUpdate()
		{
			// update the indicators before becoming visible
			OnIdleUpdateCmdUI(TRUE, 0L);
		}

		void OnLButtonDown(UINT nFlags, CPoint pt)
		{
			// only start dragging if clicked in "void" space
			if (m_pDockBar != NULL && OnToolHitTest(pt, NULL) == -1)
			{
				// start the drag
				ASSERT(m_pDockContext != NULL);
				ClientToScreen(&pt);
				m_pDockContext->StartDrag(pt);
			}
			else
			{
				CWnd::OnLButtonDown(nFlags, pt);
			}
		}

		void OnLButtonDblClk(UINT nFlags, CPoint pt)
		{
			// only toggle docking if clicked in "void" space
			if (m_pDockBar != NULL && OnToolHitTest(pt, NULL) == -1)
			{
				// start the drag
				ASSERT(m_pDockContext != NULL);
				m_pDockContext->ToggleDocking();
			}
			else
			{
				CWnd::OnLButtonDblClk(nFlags, pt);
			}
		}

		LRESULT OnSizeParent(WPARAM, LPARAM lParam)
		{
			AFX_SIZEPARENTPARAMS* lpLayout = (AFX_SIZEPARENTPARAMS*)lParam;
			DWORD dwStyle = RecalcDelayShow(lpLayout);

			if ((dwStyle & WS_VISIBLE) && (dwStyle & CBRS_ALIGN_ANY) != 0)
			{
				// align the control bar
				CRect rect;
				rect.CopyRect(&lpLayout->rect);

				CSize sizeAvail = rect.Size();  // maximum size available

				// get maximum requested size
				DWORD dwMode = lpLayout->bStretch ? LM_STRETCH : 0;
				if ((m_dwStyle & CBRS_SIZE_DYNAMIC) && m_dwStyle & CBRS_FLOATING)
					dwMode |= LM_HORZ | LM_MRUWIDTH;
				else if (dwStyle & CBRS_ORIENT_HORZ)
					dwMode |= LM_HORZ | LM_HORZDOCK;
				else
					dwMode |= LM_VERTDOCK;

				CSize size = CalcDynamicLayout(-1, dwMode);

				size.cx = min(size.cx, sizeAvail.cx);
				size.cy = min(size.cy, sizeAvail.cy);

				if (dwStyle & CBRS_ORIENT_HORZ)
				{
					lpLayout->sizeTotal.cy += size.cy;
					lpLayout->sizeTotal.cx = max(lpLayout->sizeTotal.cx, size.cx);
					if (dwStyle & CBRS_ALIGN_TOP)
						lpLayout->rect.top += size.cy;
					else if (dwStyle & CBRS_ALIGN_BOTTOM)
					{
						rect.top = rect.bottom - size.cy;
						lpLayout->rect.bottom -= size.cy;
					}
				}
				else if (dwStyle & CBRS_ORIENT_VERT)
				{
					lpLayout->sizeTotal.cx += size.cx;
					lpLayout->sizeTotal.cy = max(lpLayout->sizeTotal.cy, size.cy);
					if (dwStyle & CBRS_ALIGN_LEFT)
						lpLayout->rect.left += size.cx;
					else if (dwStyle & CBRS_ALIGN_RIGHT)
					{
						rect.left = rect.right - size.cx;
						lpLayout->rect.right -= size.cx;
					}
				}
				else
				{
					ASSERT(FALSE);      // can never happen
				}

				rect.right = rect.left + size.cx;
				rect.bottom = rect.top + size.cy;

				// only resize the window if doing layout and not just rect query
				if (lpLayout->hDWP != NULL)
					AfxRepositionWindow(lpLayout, m_hWnd, &rect);
			}
			return 0;
		}

		void OnTimer(UINT_PTR nIDEvent) ;

		// updating
		virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler) = 0;

		virtual LRESULT WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
		{
			LRESULT lResult;

			switch (nMsg)
			{
				case WM_NOTIFY:
				case WM_COMMAND:
				case WM_DRAWITEM:
				case WM_MEASUREITEM:
				case WM_DELETEITEM:
				case WM_COMPAREITEM:
				case WM_VKEYTOITEM:
				case WM_CHARTOITEM:
					// send these messages to the owner if not handled
					if (OnWndMsg(nMsg, wParam, lParam, &lResult))
						return lResult;

					lResult = GetOwner()->SendMessage(nMsg, wParam, lParam);

					// special case for TTN_NEEDTEXTA and TTN_NEEDTEXTW
					if (nMsg == WM_NOTIFY)
					{
						NMHDR* pNMHDR = (NMHDR*)lParam;
						if (pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW)
						{
							TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
							TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;

							if (pNMHDR->code == TTN_NEEDTEXTA)
							{
								if (pTTTA->hinst == 0 && (!pTTTA->lpszText || !*pTTTA->lpszText))
								{
									// not handled by owner, so let bar itself handle it
									lResult = CWnd::WindowProc(nMsg, wParam, lParam);
								}
							}
							else if (pNMHDR->code == TTN_NEEDTEXTW)
							{
								if (pTTTW->hinst == 0 && (!pTTTW->lpszText || !*pTTTW->lpszText))
								{
									// not handled by owner, so let bar itself handle it
									lResult = CWnd::WindowProc(nMsg, wParam, lParam);
								}
							}
						}
					}
					return lResult;
			}

			// otherwise, just handle in default way
			lResult = CWnd::WindowProc(nMsg, wParam, lParam);
			return lResult;
		}



		BOOL AllocElements(int nElements, int cbElement)
		{
			ASSERT_VALID(this);
			ENSURE_ARG(nElements >= 0 && cbElement >= 0);
			ENSURE(m_pData != NULL || m_nCount == 0);

			// allocate new data if necessary
			void* pData = NULL;
			if (nElements > 0)
			{
				ENSURE_ARG(cbElement > 0);
				if ((pData = calloc(nElements, cbElement)) == NULL)
					return FALSE;
			}
			free(m_pData);      // free old data

			// set new data and elements
			m_pData = pData;
			m_nCount = nElements;

			return TRUE;
		}

		virtual CSize CalcDynamicLayout(int, DWORD nMode)
		{
			return CalcFixedLayout(nMode & LM_STRETCH, nMode & LM_HORZ);
		}

		virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz)
		{
			CSize size;
			size.cx = (bStretch && bHorz ? 32767 : 0);
			size.cy = (bStretch && !bHorz ? 32767 : 0);
			return size;
		}

		// input CRect should be client rectangle size
		void CalcInsideRect(CRect& rect, BOOL bHorz) const
		{
			ASSERT_VALID(this);
			DWORD dwStyle = m_dwStyle;

			if (dwStyle & CBRS_BORDER_LEFT)
				rect.left += afxData.cxBorder2;
			if (dwStyle & CBRS_BORDER_TOP)
				rect.top += afxData.cyBorder2;
			if (dwStyle & CBRS_BORDER_RIGHT)
				rect.right -= afxData.cxBorder2;
			if (dwStyle & CBRS_BORDER_BOTTOM)
				rect.bottom -= afxData.cyBorder2;

			// inset the top and bottom.
			if (bHorz)
			{
				rect.left += m_cxLeftBorder;
				rect.top += m_cyTopBorder;
				rect.right -= m_cxRightBorder;
				rect.bottom -= m_cyBottomBorder;
				if ((m_dwStyle & (CBRS_GRIPPER | CBRS_FLOATING)) == CBRS_GRIPPER)
					rect.left += AFX_CX_BORDER_GRIPPER + AFX_CX_GRIPPER + AFX_CX_BORDER_GRIPPER;
			}
			else
			{
				rect.left += m_cyTopBorder;
				rect.top += m_cxLeftBorder;
				rect.right -= m_cyBottomBorder;
				rect.bottom -= m_cxRightBorder;
				if ((m_dwStyle & (CBRS_GRIPPER | CBRS_FLOATING)) == CBRS_GRIPPER)
					rect.top += AFX_CY_BORDER_GRIPPER + AFX_CY_GRIPPER + AFX_CY_BORDER_GRIPPER;
			}
		}

		void DelayShow(BOOL bShow)
		{
			m_nStateFlags &= ~(delayHide | delayShow);
			if (bShow && (GetStyle() & WS_VISIBLE) == 0)
				m_nStateFlags |= delayShow;
			else if (!bShow && (GetStyle() & WS_VISIBLE) != 0)
				m_nStateFlags |= delayHide;
		}

		BOOL DestroyWindow() ;

		void DrawBorders(CDC* pDC, CRect& rect) ;

		void DrawNCGripper(CDC* pDC, const CRect& rect)
		{
			ENSURE_ARG(pDC);

			if ((m_dwStyle & (CBRS_GRIPPER | CBRS_FLOATING)) == CBRS_GRIPPER)
			{
				DrawNonThemedGripper(pDC, rect);
			}
		}

		BOOL DrawNonThemedGripper(CDC* pDC, const CRect& rect)
		{
			ENSURE_ARG(pDC);

			// draw the gripper in the border
			if (m_dwStyle & CBRS_ORIENT_HORZ)
			{
				pDC->Draw3dRect(rect.left + AFX_CX_BORDER_GRIPPER,
					rect.top + m_cyTopBorder,
					AFX_CX_GRIPPER, rect.Height() - m_cyTopBorder - m_cyBottomBorder,
					afxData.clrBtnHilite, afxData.clrBtnShadow);
			}
			else
			{
				pDC->Draw3dRect(rect.left + m_cyTopBorder,
					rect.top + AFX_CY_BORDER_GRIPPER,
					rect.Width() - m_cyTopBorder - m_cyBottomBorder, AFX_CY_GRIPPER,
					afxData.clrBtnHilite, afxData.clrBtnShadow);
			}
			return TRUE;
		}

		void EnableDocking(DWORD dwDockStyle)
		{
			// must be CBRS_ALIGN_XXX or CBRS_FLOAT_MULTI only
			ASSERT((dwDockStyle & ~(CBRS_ALIGN_ANY | CBRS_FLOAT_MULTI)) == 0);
			// CBRS_SIZE_DYNAMIC toolbar cannot have the CBRS_FLOAT_MULTI style
			ASSERT(((dwDockStyle & CBRS_FLOAT_MULTI) == 0) || ((m_dwStyle & CBRS_SIZE_DYNAMIC) == 0));

			m_dwDockStyle = dwDockStyle;
			if (m_pDockContext == NULL)
				m_pDockContext = new CDockContext(this);

			// permanently wire the bar's owner to its current parent
			if (m_hWndOwner == NULL)
				m_hWndOwner = ::GetParent(m_hWnd);
		}

		void EraseNonClient()
		{
			// get window DC that is clipped to the non-client area
			CWindowDC dc(this);
			CRect rectClient;
			GetClientRect(rectClient);
			CRect rectWindow;
			GetWindowRect(rectWindow);
			ScreenToClient(rectWindow);
			rectClient.OffsetRect(-rectWindow.left, -rectWindow.top);
			dc.ExcludeClipRect(rectClient);

			// draw borders in non-client area
			rectWindow.OffsetRect(-rectWindow.left, -rectWindow.top);
			DrawBorders(&dc, rectWindow);

			// erase parts not drawn
			dc.IntersectClipRect(rectWindow);
			SendMessage(WM_ERASEBKGND, (WPARAM)dc.m_hDC);

			// draw gripper in non-client area
			DrawNCGripper(&dc, rectWindow);
		}

		CRect GetBorders() const {  return CRect(m_cxLeftBorder, m_cyTopBorder, m_cxRightBorder, m_cyBottomBorder);  }

		int GetCount() const { return m_nCount; }

		DWORD GetBarStyle() { return m_dwStyle; }

		CFrameWnd* GetDockingFrame()
		{
			CFrameWnd* pFrameWnd = GetParentFrame();
			if (pFrameWnd == NULL)
				pFrameWnd = m_pDockSite;

			ASSERT(pFrameWnd != NULL);
			return pFrameWnd;
		}

		virtual BOOL IsDockBar() const { return FALSE; }

		BOOL IsFloating() const ;

		virtual BOOL IsVisible() const
		{
			if (m_nStateFlags & delayHide)
				return FALSE;

			if ((m_nStateFlags & delayShow) || ((GetStyle() & WS_VISIBLE) != 0))
				return TRUE;

			return FALSE;
		}

		void PostNcDestroy()
		{
			if (m_bAutoDelete)      // Automatic cleanup?
				delete this ;
		}

		BOOL PreCreateWindow(CREATESTRUCT& cs)
		{
			if (!CWnd::PreCreateWindow(cs))
				return FALSE;

			// force clipsliblings (otherwise will cause repaint problems)
			cs.style |= WS_CLIPSIBLINGS;

			// default border style translation for Win4
			//  (you can turn off this translation by setting CBRS_BORDER_3D)
			if ((m_dwStyle & CBRS_BORDER_3D) == 0)
			{
				DWORD dwNewStyle = 0;
				switch (m_dwStyle & (CBRS_BORDER_ANY | CBRS_ALIGN_ANY))
				{
					case CBRS_LEFT:
						dwNewStyle = CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM;
						break;
					case CBRS_TOP:
						dwNewStyle = CBRS_BORDER_TOP;
						break;
					case CBRS_RIGHT:
						dwNewStyle = CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM;
						break;
					case CBRS_BOTTOM:
						dwNewStyle = CBRS_BORDER_BOTTOM;
						break;
				}

				// set new style if it matched one of the predefined border types
				if (dwNewStyle != 0)
				{
					m_dwStyle &= ~(CBRS_BORDER_ANY);
					m_dwStyle |= (dwNewStyle | CBRS_BORDER_3D);
				}
			}

			return TRUE;
		}

		/////////////////////////////////////////////////////////////////////////////
		// Default control bar processing

		BOOL PreTranslateMessage(MSG* pMsg) ;

		DWORD RecalcDelayShow(AFX_SIZEPARENTPARAMS* lpLayout)
		{
			ENSURE_ARG(lpLayout != NULL);
			// resize and reposition this control bar based on styles
			DWORD dwStyle = (m_dwStyle & (CBRS_ALIGN_ANY | CBRS_BORDER_ANY)) |
				(GetStyle() & WS_VISIBLE);

			// handle delay hide/show
			if (m_nStateFlags & (delayHide | delayShow))
			{
				UINT swpFlags = 0;
				if (m_nStateFlags & delayHide)
				{
					ASSERT((m_nStateFlags & delayShow) == 0);
					if (dwStyle & WS_VISIBLE)
						swpFlags = SWP_HIDEWINDOW;
				}
				else
				{
					ASSERT(m_nStateFlags & delayShow);
					if ((dwStyle & WS_VISIBLE) == 0)
						swpFlags = SWP_SHOWWINDOW;
				}
				if (swpFlags != 0)
				{
					// make the window seem visible/hidden
					dwStyle ^= WS_VISIBLE;
					if (lpLayout->hDWP != NULL)
					{
						// clear delay flags
						m_nStateFlags &= ~(delayShow | delayHide);
						// hide/show the window if actually doing layout
						lpLayout->hDWP = ::DeferWindowPos(lpLayout->hDWP, m_hWnd, NULL,
															0, 0, 0, 0, swpFlags |
															SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
					}
				}
				else
				{
					// clear delay flags -- window is already in correct state
					m_nStateFlags &= ~(delayShow | delayHide);
				}
			}
			return dwStyle; // return new style
		}

		void ResetTimer(UINT_PTR nEvent, UINT nTime)
		{
			KillTimer(AFX_TIMER_ID_TIMER_WAIT);
			KillTimer(AFX_TIMER_ID_TIMER_CHECK);
			VERIFY(SetTimer(nEvent, nTime, NULL));
		}

		void SetBorders(LPCRECT lpRect) { SetBorders(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom); }

		void SetBorders(int cxLeft, int cyTop, int cxRight, int cyBottom)
		{
			ASSERT(cxLeft >= 0);
			ASSERT(cyTop >= 0);
			ASSERT(cxRight >= 0);
			ASSERT(cyBottom >= 0);

			m_cxLeftBorder = cxLeft;
			m_cyTopBorder = cyTop;
			m_cxRightBorder = cxRight;
			m_cyBottomBorder = cyBottom;
		}

		void SetBarStyle(DWORD dwStyle)
		{
			ASSERT((dwStyle & CBRS_ALL) == dwStyle);

			EnableToolTips(dwStyle & CBRS_TOOLTIPS);

			if (m_dwStyle != dwStyle)
			{
				DWORD dwOldStyle = m_dwStyle;
				m_dwStyle = dwStyle;
				OnBarStyleChange(dwOldStyle, dwStyle);
			}
		}

		virtual BOOL SetStatusText(INT_PTR nHit) ;
			

};

BEGIN_MESSAGE_MAP(CControlBar, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()

	ON_MESSAGE(WM_IDLEUPDATECMDUI, &CControlBar::OnIdleUpdateCmdUI)
	ON_MESSAGE(WM_SIZEPARENT, &CControlBar::OnSizeParent)
	ON_MESSAGE_VOID(WM_INITIALUPDATE, &CControlBar::OnInitialUpdate)
END_MESSAGE_MAP()




_INLINE void CDockContext::InitLoop()
{
	// handle pending WM_PAINT messages
	MSG msg;
	while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
	{
		if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
			return;
		DispatchMessage(&msg);
	}

	// get styles from bar
	m_dwDockStyle = m_pBar->m_dwDockStyle;
	m_dwStyle = m_pBar->m_dwStyle & CBRS_ALIGN_ANY;
	ASSERT(m_dwStyle != 0);

	// initialize state
	m_rectLast.SetRectEmpty();
	m_sizeLast.cx = m_sizeLast.cy = 0;
	m_bForceFrame = m_bFlip = m_bDitherLast = FALSE;

	// lock window update while dragging
	ASSERT(m_pDC == NULL);
	CWnd* pWnd = CWnd::GetDesktopWindow();
	if (pWnd->LockWindowUpdate())
		m_pDC = pWnd->GetDCEx(NULL, DCX_WINDOW | DCX_CACHE | DCX_LOCKWINDOWUPDATE);
	else
		m_pDC = pWnd->GetDCEx(NULL, DCX_WINDOW | DCX_CACHE);
	ASSERT(m_pDC != NULL);
}



#define CX_PANE_BORDER 6    // 3 pixels on each side of each pane

#define SBPF_UPDATE 0x0001  // pending update of text

// Styles for status bar panes
#define SBPS_NORMAL     0x0000
#define SBPS_NOBORDERS  SBT_NOBORDERS
#define SBPS_POPOUT     SBT_POPOUT
#define SBPS_OWNERDRAW  SBT_OWNERDRAW
#define SBPS_DISABLED   0x04000000
#define SBPS_STRETCH    0x08000000  // stretch to fill status bar

struct AFX_STATUSPANE
{
	UINT    nID;        // IDC of indicator: 0 => normal text area
	int     cxText;     // width of string area in pixels
	//   on both sides there is a 3 pixel gap and
	//   a one pixel border, making a pane 6 pixels wider
	UINT    nStyle;     // style flags (SBPS_*)
	UINT    nFlags;     // state flags (SBPF_*)
	CString strText;    // text in the pane
};

/////////////////////////////////////////////////////////////////////////////
// CStatusBar idle update through CStatusCmdUI class

class CStatusCmdUI : public CCmdUI      // class private to this file!
{
	public:

	public: // re-implementations only
		virtual void Enable(BOOL bOn);
		virtual void SetCheck(int nCheck);
		virtual void SetText(LPCTSTR lpszText);

};



class CStatusBar : public CControlBar
{
	//DECLARE_DYNAMIC(CStatusBar)

	public:
		CStatusBar()
		{
			// setup default border/margin depending on type of system
			m_cyTopBorder = 2;
			m_cxLeftBorder = 0;
			m_cxRightBorder = 0;
			m_cyBottomBorder = 0;

			// minimum height set with SB_SETMINHEIGHT is cached
			m_nMinHeight = 0;
		}

	public:
		DECLARE_MESSAGE_MAP()

		int m_nMinHeight;

	public:
		int OnGetText(int nMaxChars, LPTSTR lpszText)
		{
			ASSERT_VALID(this);
			ASSERT(::IsWindow(m_hWnd));

			if (nMaxChars == 0)
				return 0;       // nothing copied

			INT_PTR nLen = 0;
			int nIndex = CommandToIndex(0); // use pane with ID zero
			if (nIndex >= 0)
			{
				AFX_STATUSPANE* pSBP = _GetPanePtr(nIndex);
				nLen = pSBP->strText.GetLength();
				if (nLen > nMaxChars)
					nLen = nMaxChars - 1; // number of characters to copy (less term.)

				memcpy(lpszText, (LPCTSTR)pSBP->strText, nLen*sizeof(TCHAR));
			}
			lpszText[nLen] = '\0';
			return (int)nLen + 1;      // number of bytes copied
		}

		UINT OnGetTextLength()
		{
			ASSERT_VALID(this);
			ASSERT(::IsWindow(m_hWnd));

			INT_PTR nLen = 0;
			int nIndex = CommandToIndex(0); // use pane with ID zero
			if (nIndex >= 0)
			{
				AFX_STATUSPANE* pSBP = _GetPanePtr(nIndex);
				nLen = pSBP->strText.GetLength();
			}
			return (UINT)nLen;
		}

		virtual void OnBarStyleChange(DWORD dwOldStyle, DWORD dwNewStyle)
		{
			if (m_hWnd != NULL &&
				((dwOldStyle & CBRS_BORDER_ANY) != (dwNewStyle & CBRS_BORDER_ANY)))
			{
				// recalc non-client area when border styles change
				SetWindowPos(NULL, 0, 0, 0, 0,
					SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME);
			}
		}

		void OnNcCalcSize(BOOL /*bCalcValidRects*/, NCCALCSIZE_PARAMS* lpncsp)
		{
			// calculate border space (will add to top/bottom, subtract from right/bottom)
			CRect rect; rect.SetRectEmpty();
			CControlBar::CalcInsideRect(rect, TRUE);

			// adjust non-client area for border space
			lpncsp->rgrc[0].left += rect.left;
			lpncsp->rgrc[0].top += rect.top - 2;
			lpncsp->rgrc[0].right += rect.right;
			lpncsp->rgrc[0].bottom += rect.bottom;
		}

		void OnNcPaint()
		{
			EraseNonClient();
		}

		void OnPaint()
		{
			UpdateAllPanes(FALSE, TRUE);

			Default();
		}

		LRESULT OnSetMinHeight(WPARAM wParam, LPARAM)
		{
			LRESULT lResult = Default();
			m_nMinHeight = (int)wParam;
			return lResult;
		}

		int OnSetText(LPCTSTR lpszText)
		{
			ASSERT_VALID(this);
			ASSERT(::IsWindow(m_hWnd));

			int nIndex = CommandToIndex(0);
			if (nIndex < 0)
				return -1;
			return SetPaneText(nIndex, lpszText) ? 0 : -1;
		}

		void OnSize(UINT nType, int cx, int cy)
		{
			ASSERT_VALID(this);
			ASSERT(::IsWindow(m_hWnd));

			CControlBar::OnSize(nType, cx, cy);

			// need to adjust pane right edges (because of stretchy pane)
			UpdateAllPanes(TRUE, FALSE);
		}

		void OnWindowPosChanging(LPWINDOWPOS lpWndPos)
		{
			// not necessary to invalidate the borders
			DWORD dwStyle = m_dwStyle;
			m_dwStyle &= ~(CBRS_BORDER_ANY);
			CControlBar::OnWindowPosChanging(lpWndPos);
			m_dwStyle = dwStyle;
		}

		virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
		{
			CStatusCmdUI state;
			state.m_pOther = this;
			state.m_nIndexMax = (UINT)m_nCount;
			for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax; state.m_nIndex++)
			{
				state.m_nID = _GetPanePtr(state.m_nIndex)->nID;

				// allow the statusbar itself to have update handlers
				if (CWnd::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
					continue;

				// allow target (owner) to handle the remaining updates
				state.DoUpdate((CCmdTarget*)pTarget, FALSE);
			}

			// update the dialog controls added to the status bar
			UpdateDialogControls((CCmdTarget*)pTarget, bDisableIfNoHndler);
		}



		BOOL AllocElements(int nElements, int cbElement)
		{
			int i;

			// destruct old elements
			AFX_STATUSPANE* pSBP = _GetPanePtr(0);
			for (i = 0; i < m_nCount; i++)
			{
				pSBP->strText.~CString();
				++pSBP;
			}

			// allocate new elements
			if (!CControlBar::AllocElements(nElements, cbElement))
				return FALSE;

			// construct new elements
			pSBP = _GetPanePtr(0);
			for (i = 0; i < m_nCount; i++)
			{
				new(&pSBP->strText) CString;
				//memcpy(&pSBP->strText, &CAfx::GetCStringNil(), sizeof(CString));
				++pSBP;
			}
			return TRUE;
		}

		virtual CSize CalcFixedLayout(BOOL, BOOL bHorz)
		{
			ASSERT_VALID(this);
			ASSERT(::IsWindow(m_hWnd));

			// determinme size of font being used by the status bar
			TEXTMETRIC tm;
			{
				CClientDC dc(NULL);
				HFONT hFont = (HFONT)SendMessage(WM_GETFONT);
				HGDIOBJ hOldFont = NULL;
				if (hFont != NULL)
					hOldFont = dc.SelectObject(hFont);
				VERIFY(dc.GetTextMetrics(&tm));
				if (hOldFont != NULL)
					dc.SelectObject(hOldFont);
			}

			// get border information
			CRect rect; rect.SetRectEmpty();
			CalcInsideRect(rect, bHorz);
			int rgBorders[3];
			DefWindowProc(SB_GETBORDERS, 0, (LPARAM)rgBorders);

			// determine size, including borders
			CSize size;
			size.cx = 32767;
			size.cy = tm.tmHeight - tm.tmInternalLeading - 1
						+ rgBorders[1] * 2 + ::GetSystemMetrics(SM_CYBORDER) * 2 - rect.Height();
			if (size.cy < m_nMinHeight)
				size.cy = m_nMinHeight;

			return size;
		}

		void CalcInsideRect(CRect& rect, BOOL bHorz) const
		{
			ASSERT_VALID(this);
			ASSERT(bHorz);  // vertical status bar not supported

			// subtract standard CControlBar borders
			CControlBar::CalcInsideRect(rect, bHorz);

			// subtract size grip if present
			if ((GetStyle() & SBARS_SIZEGRIP) && !::IsZoomed(::GetParent(m_hWnd)))
			{
				// get border metrics from common control
				int rgBorders[3];
				CStatusBar* pBar = (CStatusBar*)this;
				pBar->DefWindowProc(SB_GETBORDERS, 0, (LPARAM)rgBorders);

				// size grip uses a border + size of scrollbar + cx border
				rect.right -= rgBorders[0] + ::GetSystemMetrics(SM_CXVSCROLL) + ::GetSystemMetrics(SM_CXBORDER) * 2;
			}
		}

		int CommandToIndex(UINT nIDFind) const
		{
			ASSERT_VALID(this);

			if (m_nCount <= 0)
				return -1;

			AFX_STATUSPANE* pSBP = _GetPanePtr(0);
			for (int i = 0; i < m_nCount; i++, pSBP++)
				if (pSBP->nID == nIDFind)
					return i;

			return -1;
		}

		virtual BOOL Create(CWnd* pParentWnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_BOTTOM, UINT nID = AFX_IDW_STATUS_BAR)
		{
			return CreateEx(pParentWnd, 0, dwStyle, nID);
		}

		virtual BOOL CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle = 0, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_BOTTOM, UINT nID = AFX_IDW_STATUS_BAR)
		{
			ASSERT_VALID(pParentWnd);   // must have a parent

			// save the style (some of these style bits are MFC specific)
			m_dwStyle = (dwStyle & CBRS_ALL);

			// translate MFC style bits to windows style bits
			dwStyle &= ~CBRS_ALL;
			dwStyle |= CCS_NOPARENTALIGN | CCS_NOMOVEY | CCS_NODIVIDER | CCS_NORESIZE;
			if (pParentWnd->GetStyle() & WS_THICKFRAME)
				dwStyle |= SBARS_SIZEGRIP;
			dwStyle |= dwCtrlStyle;

			// initialize common controls
			VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_BAR_REG));

			// create the HWND
			CRect rect; rect.SetRectEmpty();
			return CWnd::Create(STATUSCLASSNAME, NULL, dwStyle, rect, pParentWnd, nID);
		}

		UINT GetItemID(int nIndex) const
		{
			ASSERT_VALID(this);
			return _GetPanePtr(nIndex)->nID;
		}

		void GetItemRect(int nIndex, LPRECT lpRect) const
		{
			ASSERT_VALID(this);
			ASSERT(::IsWindow(m_hWnd));

			CStatusBar* pBar = (CStatusBar*)this;
			if (!pBar->DefWindowProc(SB_GETRECT, nIndex, (LPARAM)lpRect))
				::SetRectEmpty(lpRect);
		}

		AFX_STATUSPANE* _GetPanePtr(int nIndex) const
		{
			ASSERT((nIndex >= 0 && nIndex < m_nCount) || m_nCount == 0);
			return ((AFX_STATUSPANE*)m_pData) + nIndex;
		}

		void GetPaneInfo(int nIndex, UINT& nID, UINT& nStyle, int& cxWidth) const
		{
			ASSERT_VALID(this);

			AFX_STATUSPANE* pSBP = _GetPanePtr(nIndex);
			nID = pSBP->nID;
			nStyle = pSBP->nStyle;
			cxWidth = pSBP->cxText;
		}

		UINT GetPaneStyle(int nIndex) const
		{
			return _GetPanePtr(nIndex)->nStyle;
		}

		void GetPaneText(int nIndex, CString& s) const
		{
			ASSERT_VALID(this);

			AFX_STATUSPANE* pSBP = _GetPanePtr(nIndex);
			s = pSBP->strText;
		}

		CString GetPaneText(int nIndex) const
		{
			ASSERT_VALID(this);

			AFX_STATUSPANE* pSBP = _GetPanePtr(nIndex);
			return pSBP->strText;
		}


		// NOTE: The cast in GetStatusBarCtrl is ugly, but must be preserved for compatibility.
		// CStatusBarCtrl is not related to CStatusBar by inheritance so we must be careful to ensure 
		// that CStatusBarCtrl remains a binary compatible subset of CStatusBar.
		CStatusBarCtrl& GetStatusBarCtrl() const { return *(CStatusBarCtrl*)this; }

		BOOL PreCreateWindow(CREATESTRUCT& cs)
		{
			// in Win4, status bars do not have a border at all, since it is
			//  provided by the client area.
			if ((m_dwStyle & (CBRS_ALIGN_ANY | CBRS_BORDER_ANY)) == CBRS_BOTTOM)
				m_dwStyle &= ~(CBRS_BORDER_ANY | CBRS_BORDER_3D);

			return CControlBar::PreCreateWindow(cs);
		}


		void SetBorders(LPCRECT lpRect) { SetBorders(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom); }
		void SetBorders(int cxLeft, int cyTop, int cxRight, int cyBottom)
		{
			ASSERT(cyTop >= 2); CControlBar::SetBorders(cxLeft, cyTop, cxRight, cyBottom);
		}

		BOOL SetIndicators(const UINT* lpIDArray, int nIDCount)
		{
			ASSERT_VALID(this);
			ASSERT(nIDCount >= 1);  // must be at least one of them

			// first allocate array for panes and copy initial data
			if (!AllocElements(nIDCount, sizeof(AFX_STATUSPANE)))
				return FALSE;
			ASSERT(nIDCount == m_nCount);

			// copy initial data from indicator array
			BOOL bResult = TRUE;
			if (lpIDArray != NULL)
			{
				HFONT hFont = (HFONT)SendMessage(WM_GETFONT);
				CClientDC dcScreen(NULL);
				HGDIOBJ hOldFont = NULL;
				if (hFont != NULL)
					hOldFont = dcScreen.SelectObject(hFont);

				AFX_STATUSPANE* pSBP = _GetPanePtr(0);
				for (int i = 0; i < nIDCount; i++)
				{
					pSBP->nID = *lpIDArray++;
					pSBP->nFlags |= SBPF_UPDATE;
					if (pSBP->nID != 0)
					{
						if (!pSBP->strText.LoadString(pSBP->nID))
						{
							TRACE(traceAppMsg, 0, "Warning: failed to load indicator string 0x%04X.\n", pSBP->nID);
							bResult = FALSE;
							break;
						}
						pSBP->cxText = dcScreen.GetTextExtent(pSBP->strText).cx;
						ASSERT(pSBP->cxText >= 0);
						if (!SetPaneText(i, pSBP->strText, FALSE))
						{
							bResult = FALSE;
							break;
						}
					}
					else
					{
						// no indicator (must access via index)
						// default to 1/4 the screen width (first pane is stretchy)
						pSBP->cxText = ::GetSystemMetrics(SM_CXSCREEN) / 4;
						if (i == 0)
							pSBP->nStyle |= (SBPS_STRETCH | SBPS_NOBORDERS);
					}
					++pSBP;
				}
				if (hOldFont != NULL)
					dcScreen.SelectObject(hOldFont);
			}
			UpdateAllPanes(TRUE, TRUE);

			return bResult;
		}

		void SetPaneInfo(int nIndex, UINT nID, UINT nStyle, int cxWidth)
		{
			ASSERT_VALID(this);

			BOOL bChanged = FALSE;
			AFX_STATUSPANE* pSBP = _GetPanePtr(nIndex);
			pSBP->nID = nID;
			if (pSBP->nStyle != nStyle)
			{
				if ((pSBP->nStyle ^ nStyle) & SBPS_STRETCH)
					bChanged = TRUE;
				else
				{
					pSBP->nStyle = nStyle;
					pSBP->nFlags |= SBPF_UPDATE;
					SetPaneText(nIndex, pSBP->strText);
				}
				pSBP->nStyle = nStyle;
			}
			if (cxWidth != pSBP->cxText)
			{
				// change width of one pane -> invalidate the entire status bar
				pSBP->cxText = cxWidth;
				bChanged = TRUE;
			}
			if (bChanged)
				UpdateAllPanes(TRUE, FALSE);
		}

		void SetPaneStyle(int nIndex, UINT nStyle)
		{
			AFX_STATUSPANE* pSBP = _GetPanePtr(nIndex);
			if (pSBP->nStyle != nStyle)
			{
				// if the pane is changing SBPS_STRETCH, then...
				if ((pSBP->nStyle ^ nStyle) & SBPS_STRETCH)
				{
					// ... we need to re-layout the panes
					pSBP->nStyle = nStyle;
					UpdateAllPanes(TRUE, FALSE);
				}

				// use SetPaneText, since it updates the style and text
				pSBP->nStyle = nStyle;
				pSBP->nFlags |= SBPF_UPDATE;
				SetPaneText(nIndex, pSBP->strText);
			}
		}

		BOOL SetPaneText(int nIndex, LPCTSTR lpszNewText, BOOL bUpdate = TRUE)
		{
			ASSERT_VALID(this);

			AFX_STATUSPANE* pSBP = _GetPanePtr(nIndex);

			if (!(pSBP->nFlags & SBPF_UPDATE) &&
				((lpszNewText == NULL && pSBP->strText.IsEmpty()) ||
				(lpszNewText != NULL && pSBP->strText.Compare(lpszNewText) == 0)))
			{
				// nothing to change
				return TRUE;
			}

			if (lpszNewText != NULL)
				pSBP->strText = lpszNewText;
			else
				pSBP->strText.Empty();

			if (!bUpdate)
			{
				// can't update now, wait until later
				pSBP->nFlags |= SBPF_UPDATE;
				return TRUE;
			}

			pSBP->nFlags &= ~SBPF_UPDATE;
			DefWindowProc(SB_SETTEXT, ((WORD)pSBP->nStyle) | nIndex,
							(pSBP->nStyle & SBPS_DISABLED) ? NULL : (LPARAM)(LPCTSTR)pSBP->strText);

			return TRUE;
		}

		void UpdateAllPanes(BOOL bUpdateRects, BOOL bUpdateText)
		{
			ASSERT_VALID(this);

			int i;

			// update the status pane locations
			if (bUpdateRects)
			{

				// get border information and client work area
				CRect rect; GetWindowRect(rect);
				rect.OffsetRect(-rect.left, -rect.top);
				CalcInsideRect(rect, TRUE);
				int rgBorders[3];
				VERIFY((BOOL)DefWindowProc(SB_GETBORDERS, 0, (LPARAM)rgBorders));

				// determine extra space for stretchy pane
				int cxExtra = rect.Width() + rgBorders[2];
				int nStretchyCount = 0;
				AFX_STATUSPANE* pSBP = _GetPanePtr(0);
				for (i = 0; i < m_nCount; i++)
				{
					if (pSBP->nStyle & SBPS_STRETCH)
						++nStretchyCount;
					cxExtra -= (pSBP->cxText + CX_PANE_BORDER + rgBorders[2]);
					++pSBP;
				}

				// determine right edge of each pane
				CArray<int> rgRights;
				rgRights.SetSize(m_nCount);
				int right = rgBorders[0];
				pSBP = _GetPanePtr(0);
				for (i = 0; i < m_nCount; i++)
				{
					// determine size of the pane
					ASSERT(pSBP->cxText >= 0);
					right += pSBP->cxText + CX_PANE_BORDER;
					if ((pSBP->nStyle & SBPS_STRETCH) && cxExtra > 0)
					{
						ASSERT(nStretchyCount != 0);
						int cxAddExtra = cxExtra / nStretchyCount;
						right += cxAddExtra;
						--nStretchyCount;
						cxExtra -= cxAddExtra;
					}
					rgRights[i] = right;

					// next pane
					++pSBP;
					right += rgBorders[2];
				}

				// set new right edges for all panes
				DefWindowProc(SB_SETPARTS, m_nCount,(LPARAM)(rgRights.GetData()));
			}

			// update text in the status panes if specified
			if (bUpdateText)
			{
				AFX_STATUSPANE* pSBP = _GetPanePtr(0);
				for (i = 0; i < m_nCount; i++)
				{
					if (pSBP->nFlags & SBPF_UPDATE)
						SetPaneText(i, pSBP->strText);
					++pSBP;
				}
			}
		}

};

BEGIN_MESSAGE_MAP(CStatusBar, CControlBar)
	ON_WM_GETTEXT()
	ON_WM_GETTEXTLENGTH()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_SETTEXT()
	ON_WM_SIZE()
	ON_WM_WINDOWPOSCHANGING()

	ON_MESSAGE(SB_SETMINHEIGHT, &CStatusBar::OnSetMinHeight)
END_MESSAGE_MAP()



_INLINE void CStatusCmdUI::Enable(BOOL bOn)
{
	m_bEnableChanged = TRUE;
	CStatusBar* pStatusBar = (CStatusBar*)m_pOther;
	ASSERT(pStatusBar != NULL);
	ASSERT(m_nIndex < m_nIndexMax);

	UINT nNewStyle = pStatusBar->GetPaneStyle(m_nIndex) & ~SBPS_DISABLED;
	if (!bOn)
		nNewStyle |= SBPS_DISABLED;
	pStatusBar->SetPaneStyle(m_nIndex, nNewStyle);
}

_INLINE void CStatusCmdUI::SetCheck(int nCheck) // "checking" will pop out the text
{
	CStatusBar* pStatusBar = (CStatusBar*)m_pOther;
	ASSERT(pStatusBar != NULL);
	ASSERT(m_nIndex < m_nIndexMax);

	UINT nNewStyle = pStatusBar->GetPaneStyle(m_nIndex) & ~SBPS_POPOUT;
	if (nCheck != 0)
		nNewStyle |= SBPS_POPOUT;
	pStatusBar->SetPaneStyle(m_nIndex, nNewStyle);
}

_INLINE void CStatusCmdUI::SetText(LPCTSTR lpszText)
{
	CStatusBar* pStatusBar = (CStatusBar*)m_pOther;
	ASSERT(pStatusBar != NULL);
	ASSERT(m_nIndex < m_nIndexMax);

	pStatusBar->SetPaneText(m_nIndex, lpszText);
}



class CDialogBar : public CControlBar
{
	//DECLARE_DYNAMIC(CDialogBar)

	public:
		CDialogBar()
		{
			m_lpszTemplateName = NULL;
		}

		virtual ~CDialogBar()
		{
			DestroyWindow();    // avoid PostNcDestroy problems
		}


	public:
		//DECLARE_MESSAGE_MAP()

		CSize m_sizeDefault;

		LPCTSTR m_lpszTemplateName;

	public:
		void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
		{
			UpdateDialogControls((CCmdTarget*)pTarget, bDisableIfNoHndler);
		}

		virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
		{
			return Create(pParentWnd, MAKEINTRESOURCE(nIDTemplate), nStyle, nID);
		}

		virtual BOOL Create(CWnd* pParentWnd, LPCTSTR lpszTemplateName, UINT nStyle, UINT nID)
		{
			ASSERT(pParentWnd != NULL);
			ASSERT(lpszTemplateName != NULL);

			// allow chance to modify styles
			m_dwStyle = (nStyle & CBRS_ALL);
			CREATESTRUCT cs;
			memset(&cs, 0, sizeof(cs));
			cs.lpszClass = _afxWndControlBar;
			cs.style = (DWORD)nStyle | WS_CHILD;
			cs.hMenu = (HMENU)(UINT_PTR)nID;
			cs.hInstance = AfxGetInstanceHandle();
			cs.hwndParent = pParentWnd->GetSafeHwnd();
			if (!PreCreateWindow(cs))
				return FALSE;

			// create a modeless dialog

			m_lpszTemplateName = lpszTemplateName;

			// initialize common controls
			VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTLS_REG));
			AfxDeferRegisterClass(AFX_WNDCOMMCTLSNEW_REG);

			BOOL bSuccess = CreateDlg(lpszTemplateName, pParentWnd);

			m_lpszTemplateName = NULL;

			if (!bSuccess)
				return FALSE;

			// dialog template MUST specify that the dialog
			//  is an invisible child window
			SetDlgCtrlID(nID);
			CRect rect;
			GetWindowRect(&rect);
			m_sizeDefault = rect.Size();    // set fixed size

			// force WS_CLIPSIBLINGS
			ModifyStyle(0, WS_CLIPSIBLINGS);

			if (!ExecuteDlgInit(lpszTemplateName))
				return FALSE;

			// force the size to zero - resizing bar will occur later
			SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);

			return TRUE;
		}

		virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz)
		{
			if (bStretch) // if not docked stretch to fit
				return CSize(bHorz ? 32767 : m_sizeDefault.cx, bHorz ? m_sizeDefault.cy : 32767);
			else
				return m_sizeDefault;
		}

};

//BEGIN_MESSAGE_MAP(CDialogBar, CControlBar)
//END_MESSAGE_MAP()
