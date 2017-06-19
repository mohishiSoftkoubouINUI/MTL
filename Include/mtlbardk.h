




class CDockBar : public CControlBar
{
	DECLARE_DYNAMIC(CDockBar)

	public:
		CDockBar::CDockBar(BOOL bFloating = FALSE)   // TRUE if attached to CMiniDockFrameWnd
		{
			m_bFloating = bFloating;
			m_bAutoDelete = TRUE;
			m_arrBars.Add(NULL);
			m_bLayoutQuery = FALSE;
			m_rectLayout.SetRectEmpty();

			// assume no margins
			m_cxLeftBorder = m_cxRightBorder = m_cyBottomBorder = m_cyTopBorder = 0;
		}

		virtual ~CDockBar()
		{
			for (int i = 0; i < m_arrBars.GetSize(); i++)
			{
				CControlBar* pBar = GetDockedControlBar(i);
				if (pBar != NULL && pBar->m_pDockBar == this)
					pBar->m_pDockBar = NULL;
			}
		}

	public:
		DECLARE_MESSAGE_MAP()

		// Attributes
		BOOL m_bFloating;

		CPtrArray m_arrBars;    // each element is a CControlBar

		BOOL m_bLayoutQuery;
		CRect m_rectLayout;

	public:
		void OnNcCalcSize(BOOL /*bCalcValidRects*/, NCCALCSIZE_PARAMS* lpncsp)
		{
			// calculate border space (will add to top/bottom, subtract from right/bottom)
			CRect rect;
			rect.SetRectEmpty();
			CalcInsideRect(rect, m_dwStyle & CBRS_ORIENT_HORZ);

			// adjust non-client area for border space
			lpncsp->rgrc[0].left += rect.left;
			lpncsp->rgrc[0].top += rect.top;
			lpncsp->rgrc[0].right += rect.right;
			lpncsp->rgrc[0].bottom += rect.bottom;
		}

		void OnNcPaint()
		{
			EraseNonClient();
		}

		LRESULT OnSizeParent(WPARAM wParam, LPARAM lParam)
		{
			AFX_SIZEPARENTPARAMS* lpLayout = (AFX_SIZEPARENTPARAMS*)lParam;

			// set m_bLayoutQuery to TRUE if lpLayout->hDWP == NULL
			BOOL bLayoutQuery = m_bLayoutQuery;
			CRect rectLayout = m_rectLayout;
			m_bLayoutQuery = (lpLayout->hDWP == NULL);
			m_rectLayout = lpLayout->rect;
			LRESULT lResult = CControlBar::OnSizeParent(wParam, lParam);
			// restore m_bLayoutQuery
			m_bLayoutQuery = bLayoutQuery;
			m_rectLayout = rectLayout;

			return lResult;
		}

		void OnWindowPosChanging(LPWINDOWPOS lpWndPos)
		{
			// not necessary to invalidate the borders
			DWORD dwStyle = m_dwStyle;
			m_dwStyle &= ~(CBRS_BORDER_ANY);
			CControlBar::OnWindowPosChanging(lpWndPos);
			m_dwStyle = dwStyle;
		}

		virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL /*bDisableIfNoHndler*/) { }


		/////////////////////////////////////////////////////////////////////////////
		// CDockBar layout

		virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz) ;
	

		BOOL Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID)
		{
			ENSURE_VALID(pParentWnd);

			// save the style
			m_dwStyle = (dwStyle & CBRS_ALL);

			VERIFY(AfxDeferRegisterClass(AFX_WNDCONTROLBAR_REG));

			// create the HWND
			CRect rect;
			rect.SetRectEmpty();

			// Note: Parent must resize itself for control bar to be resized
			return CWnd::Create(_afxWndControlBar, NULL, dwStyle, rect, pParentWnd, nID);
		}

		void DockControlBar(CControlBar* pBar, LPCRECT lpRect = NULL) ;

		int FindBar(CControlBar* pBar, int nPosExclude = -1)
		{
			for (int nPos = 0; nPos< m_arrBars.GetSize(); nPos++)
			{
				if (nPos != nPosExclude && m_arrBars[nPos] == pBar)
					return nPos;
			}
			return -1;
		}

		int GetDockedCount() const
		{
			int nCount = 0;
			for (int i = 0; i < m_arrBars.GetSize(); i++)
			{
				if (GetDockedControlBar(i) != NULL)
					nCount++;
			}
			return nCount;
		}

		CControlBar* GetDockedControlBar(int nPos) const
		{
			CControlBar* pResult = (CControlBar*)m_arrBars[nPos];
			if (DWORD_PTR(pResult) <= 0xffff)
				return NULL;
			return pResult;
		}

		virtual int GetDockedVisibleCount() const
		{
			int nCount = 0;
			for (int i = 0; i < m_arrBars.GetSize(); i++)
			{
				CControlBar* pBar = (CControlBar*)GetDockedControlBar(i);
				if (pBar != NULL && pBar->IsVisible())
					nCount++;
			}
			return nCount;
		}


		// implementation helpers
		int Insert(CControlBar* pBarIns, CRect rect, CPoint ptMid)
		{
			ENSURE_VALID(this);
			ENSURE_VALID(pBarIns);

			int nPos = 0;
			int nPosInsAfter = 0;
			int nWidth = 0;
			int nTotalWidth = 0;
			BOOL bHorz = m_dwStyle & CBRS_ORIENT_HORZ;

			for (nPos = 0; nPos < m_arrBars.GetSize(); nPos++)
			{
				CControlBar* pBar = GetDockedControlBar(nPos);
				if (pBar != NULL && pBar->IsVisible())
				{
					CRect rectBar;
					pBar->GetWindowRect(&rectBar);
					ScreenToClient(&rectBar);
					nWidth = max(nWidth,
						bHorz ? rectBar.Size().cy : rectBar.Size().cx - 1);
					if (bHorz ? rect.left > rectBar.left : rect.top > rectBar.top)
						nPosInsAfter = nPos;
				}
				else
				{
					if (pBar == NULL && m_arrBars[nPos] == NULL) // end of row
					{
						if (0 != nWidth) // if some visible elements in the row
						{
							nTotalWidth += nWidth - afxData.cyBorder2;
						}

						nWidth = 0;
						if ((bHorz ? ptMid.y : ptMid.x) < nTotalWidth)
						{
							if (nPos == 0) // first section
								m_arrBars.InsertAt(nPosInsAfter + 1, (CObject*)NULL);
							m_arrBars.InsertAt(nPosInsAfter + 1, pBarIns);
							return nPosInsAfter + 1;
						}
						nPosInsAfter = nPos;
					}
					// invisible toolbars are ignored
				}
			}

			// create a new row
			m_arrBars.InsertAt(nPosInsAfter + 1, (CObject*)NULL);
			m_arrBars.InsertAt(nPosInsAfter + 1, pBarIns);

			return nPosInsAfter + 1;
		}

		BOOL IsDockBar() const { return TRUE; }

		void ReDockControlBar(CControlBar* pBar, LPCRECT lpRect = NULL) ;
			
		BOOL RemoveControlBar(CControlBar* pBar, int nPosExclude = -1, int nAddPlaceHolder = 0) ;

		void RemovePlaceHolder(CControlBar* pBar)
		{
			// remove remembered docking position
			if (DWORD_PTR(pBar) > 0xffff)
				pBar = (CControlBar*)(DWORD_PTR)_AfxGetDlgCtrlID(pBar->m_hWnd);
			int nOldPos = FindBar(pBar);
			if (nOldPos > 0)
			{
				m_arrBars.RemoveAt(nOldPos);

				// remove section indicator (NULL) if nothing else in section
				if (m_arrBars[nOldPos - 1] == NULL && m_arrBars[nOldPos] == NULL)
					m_arrBars.RemoveAt(nOldPos);
			}
		}

		void ShowAll(BOOL bShow) ;
};

BEGIN_MESSAGE_MAP(CDockBar, CControlBar)
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_WINDOWPOSCHANGING()

	ON_MESSAGE(WM_SIZEPARENT, &CDockBar::OnSizeParent)
END_MESSAGE_MAP()


_INLINE CDockContext::~CDockContext()
{
	ASSERT(m_pBar != NULL);
	if (m_pBar->m_pDockBar != NULL)
		m_pBar->m_pDockBar->RemoveControlBar(m_pBar, -1, FALSE);
}


_INLINE BOOL CControlBar::IsFloating() const
{
	if (IsDockBar())
		return ((CDockBar*)this)->m_bFloating;
	else
		return m_pDockBar != NULL && m_pDockBar->m_bFloating;
}



struct CToolBarData
{
	WORD wVersion;
	WORD wWidth;
	WORD wHeight;
	WORD wItemCount;
	//WORD aItems[wItemCount]

	WORD* items()
	{
		return (WORD*)(this + 1);
	}
};

/*
DIBs use RGBQUAD format:
0xbb 0xgg 0xrr 0x00

Reasonably efficient code to convert a COLORREF into an
RGBQUAD is byte-order-dependent, so we need different
code depending on the byte order we're targeting.
*/
#define AFX_RGB_TO_RGBQUAD(r,g,b)   (RGB(b,g,r))
#define AFX_CLR_TO_RGBQUAD(clr)     (RGB(GetBValue(clr), GetGValue(clr), GetRValue(clr)))

struct AFX_COLORMAP
{
	// use DWORD instead of RGBQUAD so we can compare two RGBQUADs easily
	DWORD rgbqFrom;
	INT iSysColorTo;
};


_INLINE HBITMAP AfxLoadSysColorBitmap(HINSTANCE hInst, HRSRC hRsrc, BOOL bMono = FALSE)
{
	static const AFX_COLORMAP _afxSysColorMap[] =
	{
		// mapping from color in DIB to system color
		{ AFX_RGB_TO_RGBQUAD(0x00, 0x00, 0x00), COLOR_BTNTEXT },       // black
		{ AFX_RGB_TO_RGBQUAD(0x80, 0x80, 0x80), COLOR_BTNSHADOW },     // dark gray
		{ AFX_RGB_TO_RGBQUAD(0xC0, 0xC0, 0xC0), COLOR_BTNFACE },       // bright gray
		{ AFX_RGB_TO_RGBQUAD(0xFF, 0xFF, 0xFF), COLOR_BTNHIGHLIGHT }   // white
	};


	HGLOBAL hglb;
	if ((hglb = LoadResource(hInst, hRsrc)) == NULL)
	{
		return NULL;
	}

	LPBITMAPINFOHEADER lpBitmap = (LPBITMAPINFOHEADER)LockResource(hglb);
	if ((lpBitmap == NULL) || (lpBitmap->biBitCount > 8))
	{
		return NULL;
	}

	// make copy of BITMAPINFOHEADER so we can modify the color table
	const int nColorTableSize = 16;
	UINT nSize = lpBitmap->biSize + nColorTableSize * sizeof(RGBQUAD);
	LPBITMAPINFOHEADER lpBitmapInfo = (LPBITMAPINFOHEADER)::malloc(nSize);
	if (lpBitmapInfo == NULL)
	{
		return NULL;
	}

	memcpy(lpBitmapInfo, lpBitmap, nSize);

	// color table is in RGBQUAD DIB format
	DWORD* pColorTable =(DWORD*)(((LPBYTE)lpBitmapInfo) + (UINT)lpBitmapInfo->biSize);

	for (int iColor = 0; iColor < nColorTableSize; iColor++)
	{
		// look for matching RGBQUAD color in original
		for (int i = 0; i < _countof(_afxSysColorMap); i++)
		{
			if (pColorTable[iColor] == _afxSysColorMap[i].rgbqFrom)
			{
				if (bMono)
				{
					// all colors except text become white
					if (_afxSysColorMap[i].iSysColorTo != COLOR_BTNTEXT)
						pColorTable[iColor] = AFX_RGB_TO_RGBQUAD(255, 255, 255);
				}
				else
					pColorTable[iColor] = AFX_CLR_TO_RGBQUAD(::GetSysColor(_afxSysColorMap[i].iSysColorTo));
				break;
			}
		}
	}

	int nWidth = (int)lpBitmapInfo->biWidth;
	int nHeight = (int)lpBitmapInfo->biHeight;
	HDC hDCScreen = ::GetDC(NULL);
	HBITMAP hbm = ::CreateCompatibleBitmap(hDCScreen, nWidth, nHeight);

	if (hbm != NULL)
	{
		HDC hDCGlyphs = ::CreateCompatibleDC(hDCScreen);
		HBITMAP hbmOld = (HBITMAP)::SelectObject(hDCGlyphs, hbm);

		LPBYTE lpBits = (LPBYTE)(lpBitmap + 1);
		lpBits += ((size_t)1 << (lpBitmapInfo->biBitCount)) * sizeof(RGBQUAD);

		StretchDIBits(hDCGlyphs, 0, 0, nWidth, nHeight, 0, 0, nWidth, nHeight,
						lpBits, (LPBITMAPINFO)lpBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
		SelectObject(hDCGlyphs, hbmOld);
		::DeleteDC(hDCGlyphs);
	}
	::ReleaseDC(NULL, hDCScreen);

	// free copy of bitmap info struct and resource itself
	::free(lpBitmapInfo);
	::FreeResource(hglb);

	return hbm;
}




/////////////////////////////////////////////////////////////////////////////
// CToolBar idle update through CToolCmdUI class

#define CX_OVERLAP  0

#define STRING_REFRESH_OFFSET 1000000

class CToolCmdUI : public CCmdUI        // class private to this file !
{
	public: // re-implementations only
		virtual void Enable(BOOL bOn);
		virtual void SetCheck(int nCheck);
		virtual void SetText(LPCTSTR lpszText);
};

struct _AFX_CONTROLPOS
{
	int nIndex, nID;
	CRect rectOldPos;
};

class CToolBar : public CControlBar
{
	DECLARE_DYNAMIC(CToolBar)

	public:
		CToolBar() : CControlBar()
		{
			// initialize state
			//m_pStringMap = NULL;
			m_hRsrcImageWell = NULL;
			m_hInstImageWell = NULL;
			m_hbmImageWell = NULL;
			m_bDelayedButtonLayout = TRUE;

			// default image sizes
			m_sizeImage.cx = 16;
			m_sizeImage.cy = 15;

			// default button sizes
			m_sizeButton.cx = 23;
			m_sizeButton.cy = 22;

			// top and bottom borders are 1 larger than default for ease of grabbing
			m_cyTopBorder = 3;
			m_cyBottomBorder = 3;

			m_pStringMap = NULL ;
		}

	public:
		DECLARE_MESSAGE_MAP()

		//CMapStringToPtr* m_pStringMap;  // used as CMapStringToUInt

		HRSRC m_hRsrcImageWell; // handle to loaded resource for image well
		HINSTANCE m_hInstImageWell; // instance handle to load image well from
		HBITMAP m_hbmImageWell; // contains color mapped button images
		BOOL m_bDelayedButtonLayout; // used to manage when button layout should be done

		CSize m_sizeImage;  // current image size
		CSize m_sizeButton; // current button size

		CMapStringToPtr* m_pStringMap;  // used as CMapStringToUInt

	public:
		void OnBarStyleChange(DWORD dwOldStyle, DWORD dwNewStyle)
		{
			// a dynamically resizeable toolbar can not have the CBRS_FLOAT_MULTI
			ASSERT(!((dwNewStyle & CBRS_SIZE_DYNAMIC) && (m_dwDockStyle & CBRS_FLOAT_MULTI)));

			// a toolbar can not be both dynamic and fixed in size
			ASSERT(!((dwNewStyle & CBRS_SIZE_FIXED) && (dwNewStyle & CBRS_SIZE_DYNAMIC)));

			// CBRS_SIZE_DYNAMIC can not be disabled once it has been enabled
			ASSERT(((dwOldStyle & CBRS_SIZE_DYNAMIC) == 0) || ((dwNewStyle & CBRS_SIZE_DYNAMIC) != 0));

			if (m_hWnd != NULL &&
				((dwOldStyle & CBRS_BORDER_ANY) != (dwNewStyle & CBRS_BORDER_ANY)))
			{
				// recalc non-client area when border styles change
				SetWindowPos(NULL, 0, 0, 0, 0,
					SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME);
			}
			m_bDelayedButtonLayout = TRUE;
		}

		LRESULT OnNcHitTest(CPoint)
		{
			return HTCLIENT;
		}

		void OnPaint()
		{
			if (m_bDelayedButtonLayout)
				Layout();

			Default();
		}

		LRESULT OnPreserveSizingPolicyHelper(WPARAM, LPARAM)
		{
			//	NOTE: in comctl32 version 6.00 when using XP Look combined with TBSTYLE_EX_DRAWDDARROWS 
			//	style minimal sizing policy was changed to be
			//			button.height >= image.height + 13
			//			button.width >= image.width + 16
			//	this force buttons to be bigger then usual
			//	To override this behavior we should remove TBSTYLE_DROPDOWN from all buttons prior to setimagelist 
			//  operations

			UINT uiButtonNum = GetToolBarCtrl().GetButtonCount();
			DWORD* pdwStyles = new DWORD[uiButtonNum];
			ASSERT(pdwStyles);
			for (UINT i = 0; i < uiButtonNum; i++)
			{
				pdwStyles[i] = GetButtonStyle(i);
				SetButtonStyle(i, pdwStyles[i] & ~TBSTYLE_DROPDOWN);
			}

			LRESULT lResult = Default();

			if (pdwStyles)
			{
				for (UINT i = 0; i< uiButtonNum; i++)
				{
					SetButtonStyle(i, pdwStyles[i]);
				}
				delete[] pdwStyles;
			}

			return lResult;
		}

		LRESULT OnPreserveZeroBorderHelper(WPARAM, LPARAM)
		{
			DWORD dwStyle = GetStyle();
			BOOL bModify = ModifyStyle(0, TBSTYLE_TRANSPARENT | TBSTYLE_FLAT);
			DWORD dwStyleEx = GetToolBarCtrl().SetExtendedStyle(GetToolBarCtrl().GetExtendedStyle() &~TBSTYLE_EX_DRAWDDARROWS);

			LRESULT lResult = Default();

			if (bModify)
			{
				::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
			}
			if (dwStyleEx & TBSTYLE_EX_DRAWDDARROWS)
			{
				GetToolBarCtrl().SetExtendedStyle(dwStyleEx);
			}

			return lResult;
		}

		LRESULT OnSetButtonSize(WPARAM, LPARAM lParam)
		{
			return OnSetSizeHelper(m_sizeButton, lParam);
		}

		LRESULT OnSetBitmapSize(WPARAM, LPARAM lParam)
		{
			return OnSetSizeHelper(m_sizeImage, lParam);
		}

		LRESULT OnSetSizeHelper(CSize& size, LPARAM lParam)
		{
			//  The IE4 version of COMCTL32.DLL supports a zero border, but 
			//	only if TBSTYLE_TRANSPARENT is on during the the TB_SETBITMAPSIZE
			//	and/or TB_SETBUTTONSIZE messages.  In order to enable this feature
			//	all the time (so we get consistent border behavior, dependent only
			//	on the version of COMCTL32.DLL) we turn on TBSTYLE_TRANSPARENT 
			//	whenever these messages go through.  It would be nice that in a
			//	future version, the system toolbar would just allow you to set
			//	the top and left borders to anything you please.

			//  In comctl32 version 6.00 when using XP Look combined with TBSTYLE_EX_DRAWDDARROWS 
			//	style minimal sizing policy was changed to be
			//			button.height >= image.height + 13
			//			button.width >= image.width + 16
			//	this force buttons to be bigger then usual
			//	To override this behavior we should remove TBSTYLE_EX_DRAWDDARROWS prior to sizing operations

			DWORD dwStyle = GetStyle();
			BOOL bModify = ModifyStyle(0, TBSTYLE_TRANSPARENT | TBSTYLE_FLAT);
			DWORD dwStyleEx = GetToolBarCtrl().SetExtendedStyle(GetToolBarCtrl().GetExtendedStyle() &~TBSTYLE_EX_DRAWDDARROWS);

			LRESULT lResult = Default();
			if (lResult)
			{
				size = DWORD(lParam);
			}

			if (bModify)
			{
				::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
			}
			if (dwStyleEx)
			{
				GetToolBarCtrl().SetExtendedStyle(dwStyleEx);
			}

			return lResult;
		}

		INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const
		{
			ASSERT_VALID(this);
			ASSERT(::IsWindow(m_hWnd));

			// check child windows first by calling CControlBar
			INT_PTR nHit = CControlBar::OnToolHitTest(point, pTI);
			if (nHit != -1)
				return nHit;

			// now hit test against CToolBar buttons
			CToolBar* pBar = (CToolBar*)this;
			int nButtons = (int)pBar->DefWindowProc(TB_BUTTONCOUNT, 0, 0);
			for (int i = 0; i < nButtons; i++)
			{
				CRect rect;
				TBBUTTON button;
				if (pBar->DefWindowProc(TB_GETITEMRECT, i, (LPARAM)&rect))
				{
					++rect.bottom;
					++rect.right;
					if (rect.PtInRect(point) &&
						pBar->DefWindowProc(TB_GETBUTTON, i, (LPARAM)&button) &&
						!(button.fsStyle & TBSTYLE_SEP))
					{
						UINT_PTR uHit = GetItemID(i);
						if (pTI != NULL && pTI->cbSize >= sizeof(AFX_OLDTOOLINFO))
						{
							pTI->hwnd = m_hWnd;
							pTI->rect = rect;
							pTI->uId = uHit;
							pTI->lpszText = LPSTR_TEXTCALLBACK;
						}
						// found matching rect, return the ID of the button
						return uHit != 0 ? (INT_PTR)(uHit) : (INT_PTR)(-1);
					}
				}
			}
			return -1;
		}


		virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
		{
			CToolCmdUI state;
			state.m_pOther = this;

			state.m_nIndexMax = (UINT)DefWindowProc(TB_BUTTONCOUNT, 0, 0);
			for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax; state.m_nIndex++)
			{
				// get buttons state
				TBBUTTON button;
				_GetButton(state.m_nIndex, &button);
				state.m_nID = button.idCommand;

				// ignore separators
				if (!(button.fsStyle & TBSTYLE_SEP))
				{
					// allow reflections
					if (CWnd::OnCmdMsg(0, MAKELONG(CN_UPDATE_COMMAND_UI & 0xffff, WM_COMMAND + WM_REFLECT_BASE), &state, NULL))
						continue;

					// allow the toolbar itself to have update handlers
					if (CWnd::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
						continue;

					// allow the owner to process the update
					state.DoUpdate((CCmdTarget*)pTarget, bDisableIfNoHndler);
				}
			}

			// update the dialog controls added to the toolbar
			UpdateDialogControls((CCmdTarget*)pTarget, bDisableIfNoHndler);
		}

		void OnNcCalcSize(BOOL /*bCalcValidRects*/, NCCALCSIZE_PARAMS* lpncsp)
		{
			// calculate border space (will add to top/bottom, subtract from right/bottom)
			CRect rect; rect.SetRectEmpty();
			BOOL bHorz = (m_dwStyle & CBRS_ORIENT_HORZ) != 0;
			CControlBar::CalcInsideRect(rect, bHorz);

			// adjust non-client area for border space
			lpncsp->rgrc[0].left += rect.left;
			lpncsp->rgrc[0].top += rect.top;
			lpncsp->rgrc[0].right += rect.right;
			lpncsp->rgrc[0].bottom += rect.bottom;
		}

		BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct)
		{
			if (!CControlBar::OnNcCreate(lpCreateStruct))
				return FALSE;

			// if the owner was set before the toolbar was created, set it now
			if (m_hWndOwner != NULL)
				DefWindowProc(TB_SETPARENT, (WPARAM)m_hWndOwner, 0);

			DefWindowProc(TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

			return TRUE;
		}

		void OnNcPaint()
		{
			EraseNonClient();
		}

		void OnWindowPosChanging(LPWINDOWPOS lpWndPos)
		{
			// not necessary to invalidate the borders
			DWORD dwStyle = m_dwStyle;
			m_dwStyle &= ~(CBRS_BORDER_ANY);
			CControlBar::OnWindowPosChanging(lpWndPos);
			m_dwStyle = dwStyle;

			// If we can resize while floating
			if (dwStyle & CBRS_SIZE_DYNAMIC)
			{
				// And we are resizing
				if (lpWndPos->flags & SWP_NOSIZE)
					return;

				// Then redraw the buttons
				Invalidate();
			}
		}


		BOOL AddReplaceBitmap(HBITMAP hbmImageWell)
		{
			// need complete bitmap size to determine number of images
			BITMAP bitmap;
			VERIFY(::GetObject(hbmImageWell, sizeof(BITMAP), &bitmap));

			// add the bitmap to the common control toolbar
			BOOL bResult;
			if (m_hbmImageWell == NULL)
			{
				TBADDBITMAP addBitmap;
				addBitmap.hInst = NULL; // makes TBADDBITMAP::nID behave a HBITMAP
				addBitmap.nID = (UINT_PTR)hbmImageWell;
				bResult = DefWindowProc(TB_ADDBITMAP,
					bitmap.bmWidth / m_sizeImage.cx, (LPARAM)&addBitmap) == 0;
			}
			else
			{
				TBREPLACEBITMAP replaceBitmap;
				replaceBitmap.hInstOld = NULL;
				replaceBitmap.nIDOld = (UINT_PTR)m_hbmImageWell;
				replaceBitmap.hInstNew = NULL;
				replaceBitmap.nIDNew = (UINT_PTR)hbmImageWell;
				replaceBitmap.nButtons = bitmap.bmWidth / m_sizeImage.cx;
				bResult = (BOOL)DefWindowProc(TB_REPLACEBITMAP, 0,
					(LPARAM)&replaceBitmap);
			}
			// remove old bitmap, if present
			if (bResult)
			{
				AfxDeleteObject((HGDIOBJ*)&m_hbmImageWell);
				m_hbmImageWell = hbmImageWell;
			}

			return bResult;
		}

		virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode)
		{
			if ((nLength == -1) && !(dwMode & LM_MRUWIDTH) && !(dwMode & LM_COMMIT) &&
				((dwMode & LM_HORZDOCK) || (dwMode & LM_VERTDOCK)))
			{
				return CalcFixedLayout(dwMode & LM_STRETCH, dwMode & LM_HORZDOCK);
			}
			return CalcLayout(dwMode, nLength);
		}

		virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz)
		{
			DWORD dwMode = bStretch ? LM_STRETCH : 0;
			dwMode |= bHorz ? LM_HORZ : 0;

			return CalcLayout(dwMode);
		}

		CSize CalcLayout(DWORD dwMode, int nLength = -1)
		{
			ASSERT_VALID(this);
			ASSERT(::IsWindow(m_hWnd));
			if (dwMode & LM_HORZDOCK)
				ASSERT(dwMode & LM_HORZ);

			int nCount;
			TBBUTTON* pData = NULL;
			CSize sizeResult(0, 0);

			//BLOCK: Load Buttons
			{
				nCount = int(DefWindowProc(TB_BUTTONCOUNT, 0, 0));
				if (nCount != 0)
				{
					int i;
					pData = new TBBUTTON[nCount];
					for (i = 0; i < nCount; i++)
						_GetButton(i, &pData[i]);
				}
			}

			if (nCount > 0)
			{
				if (!(m_dwStyle & CBRS_SIZE_FIXED))
				{
					BOOL bDynamic = m_dwStyle & CBRS_SIZE_DYNAMIC;

					if (bDynamic && (dwMode & LM_MRUWIDTH))
						SizeToolBar(pData, nCount, m_nMRUWidth);
					else if (bDynamic && (dwMode & LM_HORZDOCK))
						SizeToolBar(pData, nCount, 32767);
					else if (bDynamic && (dwMode & LM_VERTDOCK))
						SizeToolBar(pData, nCount, 0);
					else if (bDynamic && (nLength != -1))
					{
						CRect rect; rect.SetRectEmpty();
						CalcInsideRect(rect, (dwMode & LM_HORZ));
						BOOL bVert = (dwMode & LM_LENGTHY);
						int nLen = nLength + (bVert ? rect.Height() : rect.Width());

						SizeToolBar(pData, nCount, nLen, bVert);
					}
					else if (bDynamic && (m_dwStyle & CBRS_FLOATING))
						SizeToolBar(pData, nCount, m_nMRUWidth);
					else
						SizeToolBar(pData, nCount, (dwMode & LM_HORZ) ? 32767 : 0);
				}

				sizeResult = CalcSize(pData, nCount);

				if (dwMode & LM_COMMIT)
				{
					_AFX_CONTROLPOS* pControl = NULL;
					int nControlCount = 0;
					BOOL bIsDelayed = m_bDelayedButtonLayout;
					m_bDelayedButtonLayout = FALSE;
					int i;

					for (i = 0; i < nCount; i++)
						if ((pData[i].fsStyle & TBSTYLE_SEP) && (pData[i].idCommand != 0))
							nControlCount++;

					if (nControlCount > 0)
					{
						pControl = new _AFX_CONTROLPOS[nControlCount];
						int nControlAlloc = nControlCount;
						nControlCount = 0;

						for (i = 0; i < nCount && nControlCount < nControlAlloc; i++)
						{
							if ((pData[i].fsStyle & TBSTYLE_SEP) && (pData[i].idCommand != 0))
							{
								pControl[nControlCount].nIndex = i;
								pControl[nControlCount].nID = pData[i].idCommand;

								CRect rect;
								GetItemRect(i, &rect);
								ClientToScreen(&rect);
								pControl[nControlCount].rectOldPos = rect;

								nControlCount++;
							}
						}
					}

					if ((m_dwStyle & CBRS_FLOATING) && (m_dwStyle & CBRS_SIZE_DYNAMIC))
						m_nMRUWidth = sizeResult.cx;
					for (i = 0; i < nCount; i++)
						_SetButton(i, &pData[i]);

					if (nControlCount > 0)
					{
						for (i = 0; i < nControlCount; i++)
						{
							CWnd* pWnd = GetDlgItem(pControl[i].nID);
							if (pWnd != NULL)
							{
								CRect rect;
								pWnd->GetWindowRect(&rect);
								CPoint pt = rect.TopLeft() - pControl[i].rectOldPos.TopLeft();
								GetItemRect(pControl[i].nIndex, &rect);
								pt = rect.TopLeft() + pt;
								pWnd->SetWindowPos(NULL, pt.x, pt.y, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
							}
						}
						delete[] pControl;
					}
					m_bDelayedButtonLayout = bIsDelayed;
				}
				delete[] pData;
			}

			//BLOCK: Adjust Margins
			{
				CRect rect; rect.SetRectEmpty();
				CalcInsideRect(rect, (dwMode & LM_HORZ));
				sizeResult.cy -= rect.Height();
				sizeResult.cx -= rect.Width();

				CSize size = CControlBar::CalcFixedLayout((dwMode & LM_STRETCH), (dwMode & LM_HORZ));
				sizeResult.cx = max(sizeResult.cx, size.cx);
				sizeResult.cy = max(sizeResult.cy, size.cy);
			}
			return sizeResult;
		}

		static int _AfxGetDropDownWidth()
		{
			static int _afxDropDownWidth = -1;

			// return cached width if already determined...
			if (_afxDropDownWidth != -1)
				return _afxDropDownWidth;

			// otherwise calculate it...
			HDC hDC = ::GetDC(NULL);
			ASSERT(hDC != NULL);
			HFONT hFont = NULL, hFontOld = NULL;
			if ((hFont = ::CreateFont(GetSystemMetrics(SM_CYMENUCHECK), 0, 0, 0,
										FW_NORMAL, 0, 0, 0, SYMBOL_CHARSET, 0, 0, 0, 0, _T("Marlett"))) != NULL)
				hFontOld = (HFONT)::SelectObject(hDC, hFont);
			VERIFY(GetCharWidth(hDC, '6', '6', &_afxDropDownWidth));
			if (hFont != NULL)
			{
				SelectObject(hDC, hFontOld);
				DeleteObject(hFont);
			}
			::ReleaseDC(NULL, hDC);
			ASSERT(_afxDropDownWidth != -1);
			return _afxDropDownWidth;
		}

		CSize CalcSize(TBBUTTON* pData, int nCount)
		{
			ASSERT(pData != NULL && nCount > 0);

			CPoint cur(0, 0);
			CSize sizeResult(0, 0);

			DWORD dwExtendedStyle = DWORD(DefWindowProc(TB_GETEXTENDEDSTYLE, 0, 0));

			for (int i = 0; i < nCount; i++)
			{
				//  The IE4 version of COMCTL32.DLL calculates the separation
				//	on a TBSTYLE_WRAP button as 100% of the value in iBitmap compared
				//	to the other versions which calculate it at 2/3 of that value.
				//	This is actually a bug which should be fixed in IE 4.01, so we
				//	only do the 100% calculation specifically for IE4.
				int cySep = pData[i].iBitmap;
				if (!(GetStyle() & TBSTYLE_FLAT))
					cySep = cySep * 2 / 3;

				if (pData[i].fsState & TBSTATE_HIDDEN)
					continue;

				int cx = m_sizeButton.cx;
				if (pData[i].fsStyle & TBSTYLE_SEP)
				{
					// a separator represents either a height or width
					if (pData[i].fsState & TBSTATE_WRAP)
						sizeResult.cy = max(cur.y + m_sizeButton.cy + cySep, sizeResult.cy);
					else
						sizeResult.cx = max(cur.x + pData[i].iBitmap, sizeResult.cx);
				}
				else
				{
					// check for dropdown style, but only if the buttons are being drawn
					if ((pData[i].fsStyle & TBSTYLE_DROPDOWN) &&
						(dwExtendedStyle & TBSTYLE_EX_DRAWDDARROWS))
					{
						// add size of drop down
						cx += _AfxGetDropDownWidth();
					}
					sizeResult.cx = max(cur.x + cx, sizeResult.cx);
					sizeResult.cy = max(cur.y + m_sizeButton.cy, sizeResult.cy);
				}

				if (pData[i].fsStyle & TBSTYLE_SEP)
					cur.x += pData[i].iBitmap;
				else
					cur.x += cx - CX_OVERLAP;

				if (pData[i].fsState & TBSTATE_WRAP)
				{
					cur.x = 0;
					cur.y += m_sizeButton.cy;
					if (pData[i].fsStyle & TBSTYLE_SEP)
						cur.y += cySep;
				}
			}
			return sizeResult;
		}

		int CommandToIndex(UINT nIDFind) const
		{
			ASSERT_VALID(this);
			ASSERT(::IsWindow(m_hWnd));

			CToolBar* pBar = (CToolBar*)this;
			return (int)pBar->DefWindowProc(TB_COMMANDTOINDEX, nIDFind, 0);
		}

		virtual BOOL Create(CWnd* pParentWnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP, UINT nID = AFX_IDW_TOOLBAR)
		{
			return CreateEx(pParentWnd, 0, dwStyle,
							CRect(m_cxLeftBorder, m_cyTopBorder, m_cxRightBorder, m_cyBottomBorder), nID);
		}

		virtual BOOL CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle = TBSTYLE_FLAT, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP,
								CRect rcBorders = CRect(0, 0, 0, 0), UINT nID = AFX_IDW_TOOLBAR)
		{
			ASSERT_VALID(pParentWnd);   // must have a parent
			ASSERT(!((dwStyle & CBRS_SIZE_FIXED) && (dwStyle & CBRS_SIZE_DYNAMIC)));

			SetBorders(rcBorders);

			// save the style
			m_dwStyle = (dwStyle & CBRS_ALL);
			if (nID == AFX_IDW_TOOLBAR)
				m_dwStyle |= CBRS_HIDE_INPLACE;

			dwStyle &= ~CBRS_ALL;
			dwStyle |= CCS_NOPARENTALIGN | CCS_NOMOVEY | CCS_NODIVIDER | CCS_NORESIZE;
			dwStyle |= dwCtrlStyle;

			// initialize common controls
			VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_BAR_REG));

			_AfxGetDropDownWidth() ;

			// create the HWND
			CRect rect; rect.SetRectEmpty();
			if (!CWnd::Create(TOOLBARCLASSNAME, NULL, dwStyle, rect, pParentWnd, nID))
				return FALSE;

			// sync up the sizes
			SetSizes(m_sizeButton, m_sizeImage);

			// Note: Parent must resize itself for control bar to be resized

			return TRUE;
		}

		void _GetButton(int nIndex, TBBUTTON* pButton) const
		{
			CToolBar* pBar = (CToolBar*)this;
			VERIFY(pBar->DefWindowProc(TB_GETBUTTON, nIndex, (LPARAM)pButton));
			// TBSTATE_ENABLED == TBBS_DISABLED so invert it
			pButton->fsState ^= TBSTATE_ENABLED;
		}

		void GetButtonInfo(int nIndex, UINT& nID, UINT& nStyle, int& iImage) const
		{
			ASSERT_VALID(this);
			ASSERT(::IsWindow(m_hWnd));

			TBBUTTON button;
			_GetButton(nIndex, &button);
			nID = button.idCommand;
			nStyle = MAKELONG(button.fsStyle, button.fsState);
			iImage = button.iBitmap;
		}

		UINT GetButtonStyle(int nIndex) const
		{
			ASSERT_VALID(this);
			ASSERT(::IsWindow(m_hWnd));

			TBBUTTON button;
			_GetButton(nIndex, &button);
			return MAKELONG(button.fsStyle, button.fsState);
		}

		CString CToolBar::GetButtonText(int nIndex) const
		{
			CString strResult;
			GetButtonText(nIndex, strResult);
			return strResult;
		}

		void CToolBar::GetButtonText(int nIndex, CString& rString) const
		{
			if (m_pStringMap != NULL)
			{
				// get button information (need button.iString)
				TBBUTTON button;
				_GetButton(nIndex, &button);

				// look in map for matching iString
				POSITION pos = m_pStringMap->GetStartPosition();
				CString str; void* p;
				while (pos)
				{
					m_pStringMap->GetNextAssoc(pos, str, p);
					if ((INT_PTR)p == button.iString)
					{
						rString = str;
						return;
					}
				}
			}
			rString.Empty();
		}

		// NOTE: The cast in GetToolBarCtrl is ugly, but must be preserved for compatibility.
		// CToolBarCtrl is not related to CToolBar by inheritance so we must be careful to ensure 
		// that CToolBarCtrl remains a binary compatible subset of CToolBar.
		CToolBarCtrl& GetToolBarCtrl() const { return *(CToolBarCtrl*)this; }

		UINT GetItemID(int nIndex) const
		{
			ASSERT_VALID(this);
			ASSERT(::IsWindow(m_hWnd));

			TBBUTTON button;
			_GetButton(nIndex, &button);
			return button.idCommand;
		}

		void GetItemRect(int nIndex, LPRECT lpRect) const
		{
			ASSERT_VALID(this);
			ASSERT(::IsWindow(m_hWnd));

			// handle any delayed layout
			if (m_bDelayedButtonLayout)
				((CToolBar*)this)->Layout();

			// now it is safe to get the item rectangle
			CToolBar* pBar = (CToolBar*)this;
			if (!pBar->DefWindowProc(TB_GETITEMRECT, nIndex, (LPARAM)lpRect))
				SetRectEmpty(lpRect);
		}

		void Layout()
		{
			ASSERT(m_bDelayedButtonLayout);

			m_bDelayedButtonLayout = FALSE;

			BOOL bHorz = (m_dwStyle & CBRS_ORIENT_HORZ) != 0;
			if ((m_dwStyle & CBRS_FLOATING) && (m_dwStyle & CBRS_SIZE_DYNAMIC))
				((CToolBar*)this)->CalcDynamicLayout(0, LM_HORZ | LM_MRUWIDTH | LM_COMMIT);
			else if (bHorz)
				((CToolBar*)this)->CalcDynamicLayout(0, LM_HORZ | LM_HORZDOCK | LM_COMMIT);
			else
				((CToolBar*)this)->CalcDynamicLayout(0, LM_VERTDOCK | LM_COMMIT);
		}

		BOOL LoadBitmap(LPCTSTR lpszResourceName);
		BOOL LoadBitmap(UINT nIDResource) { return LoadBitmap(MAKEINTRESOURCE(nIDResource)); }

		BOOL LoadToolBar(LPCTSTR lpszResourceName);
		BOOL LoadToolBar(UINT nIDResource) { return LoadToolBar(MAKEINTRESOURCE(nIDResource)); }

		BOOL SetBitmap(HBITMAP hbmImageWell)
		{
			ASSERT_VALID(this);
			ASSERT(hbmImageWell != NULL);

			// the caller must manage changing system colors
			m_hInstImageWell = NULL;
			m_hRsrcImageWell = NULL;

			// tell common control toolbar about the new bitmap
			return AddReplaceBitmap(hbmImageWell);
		}

		void _SetButton(int nIndex, TBBUTTON* pButton)
		{
			// get original button state
			TBBUTTON button;
			VERIFY(DefWindowProc(TB_GETBUTTON, nIndex, (LPARAM)&button));

			// prepare for old/new button comparsion
			button.bReserved[0] = 0;
			button.bReserved[1] = 0;
			// TBSTATE_ENABLED == TBBS_DISABLED so invert it
			pButton->fsState ^= TBSTATE_ENABLED;
			pButton->bReserved[0] = 0;
			pButton->bReserved[1] = 0;

			// nothing to do if they are the same
			if (memcmp(pButton, &button, sizeof(TBBUTTON)) != 0)
			{
				// don't redraw everything while setting the button
				DWORD dwStyle = GetStyle();
				ModifyStyle(WS_VISIBLE, 0);
				VERIFY(DefWindowProc(TB_DELETEBUTTON, nIndex, 0));

				// Force a recalc of the toolbar's layout to work around a comctl bug
				if (pButton->iString < -1)
				{
					INT iTextRows = (int)::SendMessage(m_hWnd, TB_GETTEXTROWS, 0, 0);
					::SendMessage(m_hWnd, WM_SETREDRAW, FALSE, 0);
					::SendMessage(m_hWnd, TB_SETMAXTEXTROWS, iTextRows + 1, 0);
					::SendMessage(m_hWnd, TB_SETMAXTEXTROWS, iTextRows, 0);
					::SendMessage(m_hWnd, WM_SETREDRAW, TRUE, 0);
					pButton->iString += STRING_REFRESH_OFFSET;
				}

				VERIFY(DefWindowProc(TB_INSERTBUTTON, nIndex, (LPARAM)pButton));
				ModifyStyle(0, dwStyle & WS_VISIBLE);

				// invalidate appropriate parts
				if (((pButton->fsStyle ^ button.fsStyle) & TBSTYLE_SEP) ||
					((pButton->fsStyle & TBSTYLE_SEP) && pButton->iBitmap != button.iBitmap))
				{
					// changing a separator
					Invalidate();
				}
				else
				{
					// invalidate just the button
					CRect rect;
					if (DefWindowProc(TB_GETITEMRECT, nIndex, (LPARAM)&rect))
						InvalidateRect(rect);
				}
			}
		}

		void SetButtonInfo(int nIndex, UINT nID, UINT nStyle, int iImage)
		{
			ASSERT_VALID(this);

			TBBUTTON button;
			_GetButton(nIndex, &button);
			TBBUTTON save;

			memcpy(&save, &button, sizeof(TBBUTTON));
			button.idCommand = nID;
			button.iBitmap = iImage;
			button.fsStyle = (BYTE)LOWORD(nStyle);
			button.fsState = (BYTE)HIWORD(nStyle);
			if (memcmp(&save, &button, sizeof(save)) != 0)
			{
				_SetButton(nIndex, &button);
				m_bDelayedButtonLayout = TRUE;
			}
		}

		BOOL SetButtons(const UINT* lpIDArray, int nIDCount)
		{
			ASSERT_VALID(this);
			ASSERT(nIDCount >= 1);  // must be at least one of them

			// delete all existing buttons
			int nCount = (int)DefWindowProc(TB_BUTTONCOUNT, 0, 0);
			while (nCount--)
				VERIFY(DefWindowProc(TB_DELETEBUTTON, 0, 0));

			TBBUTTON button; memset(&button, 0, sizeof(TBBUTTON));
			button.iString = -1;
			if (lpIDArray != NULL)
			{
				// add new buttons to the common control
				int iImage = 0;
				for (int i = 0; i < nIDCount; i++)
				{
					button.fsState = TBSTATE_ENABLED;
					if ((button.idCommand = *lpIDArray++) == 0)
					{
						// separator
						button.fsStyle = TBSTYLE_SEP;
						// width of separator includes 8 pixel overlap
						if (GetStyle() & TBSTYLE_FLAT)
							button.iBitmap = 6;
						else
							button.iBitmap = 8;
					}
					else
					{
						// a command button with image
						button.fsStyle = TBSTYLE_BUTTON;
						button.iBitmap = iImage++;
					}
					if (!DefWindowProc(TB_ADDBUTTONS, 1, (LPARAM)&button))
						return FALSE;
				}
			}
			else
			{
				// add 'blank' buttons
				button.fsState = TBSTATE_ENABLED;
				for (int i = 0; i < nIDCount; i++)
				{
					ASSERT(button.fsStyle == TBSTYLE_BUTTON);
					if (!DefWindowProc(TB_ADDBUTTONS, 1, (LPARAM)&button))
						return FALSE;
				}
			}
			m_nCount = (int)DefWindowProc(TB_BUTTONCOUNT, 0, 0);
			m_bDelayedButtonLayout = TRUE;

			return TRUE;
		}

		void SetButtonStyle(int nIndex, UINT nStyle)
		{
			ASSERT_VALID(this);
			ASSERT(::IsWindow(m_hWnd));

			TBBUTTON button;
			_GetButton(nIndex, &button);
			if (button.fsStyle != (BYTE)LOWORD(nStyle) || button.fsState != (BYTE)HIWORD(nStyle))
			{
				button.fsStyle = (BYTE)LOWORD(nStyle);
				button.fsState = (BYTE)HIWORD(nStyle);
				_SetButton(nIndex, &button);
				m_bDelayedButtonLayout = TRUE;
			}
		}

		BOOL SetButtonText(int nIndex, LPCTSTR lpszText)
		{
			// attempt to lookup string index in map
			INT_PTR nString = -1;
			void* p;
			if (m_pStringMap != NULL && m_pStringMap->Lookup(lpszText, p))
				nString = (INT_PTR)p;

			// add new string if not already in map
			if (nString == -1)
			{
				// initialize map if necessary
				if (m_pStringMap == NULL)
					m_pStringMap = new CMapStringToPtr;

				// add new string to toolbar list
				CString strTemp(lpszText, lstrlen(lpszText) + 1);
				nString = (INT_PTR)DefWindowProc(TB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strTemp);
				if (nString == -1)
					return FALSE;

				// cache string away in string map
				m_pStringMap->SetAt(lpszText, (void*)nString);
				ASSERT(m_pStringMap->Lookup(lpszText, p));
			}

			// change the toolbar button description
			TBBUTTON button;
			_GetButton(nIndex, &button);
			button.iString = nString - STRING_REFRESH_OFFSET;
			_SetButton(nIndex, &button);

			return TRUE;
		}

		void SetHeight(int cyHeight)
		{
			ASSERT_VALID(this);

			int nHeight = cyHeight;
			if (m_dwStyle & CBRS_BORDER_TOP)
				cyHeight -= afxData.cyBorder2;
			if (m_dwStyle & CBRS_BORDER_BOTTOM)
				cyHeight -= afxData.cyBorder2;
			m_cyBottomBorder = (cyHeight - m_sizeButton.cy) / 2;
			// if there is an extra pixel, m_cyTopBorder will get it
			m_cyTopBorder = cyHeight - m_sizeButton.cy - m_cyBottomBorder;
			if (m_cyTopBorder < 0)
			{
				TRACE(traceAppMsg, 0, "Warning: CToolBar::SetHeight(%d) is smaller than button.\n", nHeight);
				m_cyBottomBorder += m_cyTopBorder;
				m_cyTopBorder = 0;  // will clip at bottom
			}

			// recalculate the non-client region
			SetWindowPos(NULL, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
			Invalidate();   // just to be nice if called when toolbar is visible
		}

		void SetSizes(SIZE sizeButton, SIZE sizeImage)
		{
			ASSERT_VALID(this);

			// sizes must be non-zero and positive
			ASSERT(sizeButton.cx > 0 && sizeButton.cy > 0);
			ASSERT(sizeImage.cx > 0 && sizeImage.cy > 0);

			// button must be big enough to hold image
			//   + 7 pixels on x
			//   + 6 pixels on y
			ASSERT(sizeButton.cx >= sizeImage.cx + 7);
			ASSERT(sizeButton.cy >= sizeImage.cy + 6);

			if (::IsWindow(m_hWnd))
			{
				// set the sizes via TB_SETBITMAPSIZE and TB_SETBUTTONSIZE
				VERIFY(SendMessage(TB_SETBITMAPSIZE, 0, MAKELONG(sizeImage.cx, sizeImage.cy)));
				VERIFY(SendMessage(TB_SETBUTTONSIZE, 0, MAKELONG(sizeButton.cx, sizeButton.cy)));
				Invalidate();   // just to be nice if called when toolbar is visible
			}
			else
			{
				// just set our internal values for later
				m_sizeButton = sizeButton;
				m_sizeImage = sizeImage;
			}
		}

		void SizeToolBar(TBBUTTON* pData, int nCount, int nLength, BOOL bVert = FALSE)
		{
			ASSERT(pData != NULL && nCount > 0);

			if (!bVert)
			{
				// Wrap ToolBar as specified
				int nMax = nLength;
				int nTarget = WrapToolBar(pData, nCount, nMax);

				// Wrap ToolBar vertically
				int nMin = 0;
				int nCurrent = WrapToolBar(pData, nCount, nMin);

				if (nCurrent != nTarget)
				{
					while (nMin < nMax)
					{
						int nMid = (nMin + nMax) / 2;
						nCurrent = WrapToolBar(pData, nCount, nMid);

						if (nCurrent == nTarget)
							nMax = nMid;
						else
						{
							if (nMin == nMid)
							{
								WrapToolBar(pData, nCount, nMax);
								break;
							}
							nMin = nMid;
						}
					}
				}
				CSize size = CalcSize(pData, nCount);
				WrapToolBar(pData, nCount, size.cx);
			}
			else
			{
				CSize sizeMax, sizeMin, sizeMid;

				// Wrap ToolBar vertically
				WrapToolBar(pData, nCount, 0);
				sizeMin = CalcSize(pData, nCount);

				// Wrap ToolBar horizontally
				WrapToolBar(pData, nCount, 32767);
				sizeMax = CalcSize(pData, nCount);

				while (sizeMin.cx < sizeMax.cx)
				{
					sizeMid.cx = (sizeMin.cx + sizeMax.cx) / 2;
					WrapToolBar(pData, nCount, sizeMid.cx);
					sizeMid = CalcSize(pData, nCount);

					if (nLength < sizeMid.cy)
					{
						if (sizeMin == sizeMid)
						{
							WrapToolBar(pData, nCount, sizeMax.cx);
							return;
						}
						sizeMin = sizeMid;
					}
					else if (nLength > sizeMid.cy)
					{
						if (sizeMax == sizeMid)
						{
							WrapToolBar(pData, nCount, sizeMin.cx);
							return;
						}
						sizeMax = sizeMid;
					}
					else
						return;
				}
			}
		}

		int WrapToolBar(TBBUTTON* pData, int nCount, int nWidth)
		{
			ASSERT(pData != NULL && nCount > 0);

			int nResult = 0;
			int x = 0;
			for (int i = 0; i < nCount; i++)
			{
				pData[i].fsState &= ~TBSTATE_WRAP;

				if (pData[i].fsState & TBSTATE_HIDDEN)
					continue;

				int dx, dxNext;
				if (pData[i].fsStyle & TBSTYLE_SEP)
				{
					dx = pData[i].iBitmap;
					dxNext = dx;
				}
				else
				{
					dx = m_sizeButton.cx;
					dxNext = dx - CX_OVERLAP;
				}

				if (x + dx > nWidth)
				{
					BOOL bFound = FALSE;
					for (int j = i; j >= 0 && !(pData[j].fsState & TBSTATE_WRAP); j--)
					{
						// Find last separator that isn't hidden
						// a separator that has a command ID is not
						// a separator, but a custom control.
						if ((pData[j].fsStyle & TBSTYLE_SEP) && (pData[j].idCommand == 0) && !(pData[j].fsState & TBSTATE_HIDDEN))
						{
							bFound = TRUE; i = j; x = 0;
							pData[j].fsState |= TBSTATE_WRAP;
							nResult++;
							break;
						}
					}
					if (!bFound)
					{
						for (int j = i - 1; j >= 0 && !(pData[j].fsState & TBSTATE_WRAP); j--)
						{
							// Never wrap anything that is hidden,
							// or any custom controls
							if ((pData[j].fsState & TBSTATE_HIDDEN) || ((pData[j].fsStyle & TBSTYLE_SEP) && (pData[j].idCommand != 0)))
								continue;

							bFound = TRUE; i = j; x = 0;
							pData[j].fsState |= TBSTATE_WRAP;
							nResult++;
							break;
						}
						if (!bFound)
							x += dxNext;
					}
				}
				else
					x += dxNext;
			}
			return nResult + 1;
		}


};

BEGIN_MESSAGE_MAP(CToolBar, CControlBar)
	ON_WM_NCCREATE()
	ON_WM_NCCALCSIZE()
	ON_WM_NCHITTEST()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_WINDOWPOSCHANGING()

	ON_MESSAGE(TB_SETBITMAPSIZE, &CToolBar::OnSetBitmapSize)
	ON_MESSAGE(TB_SETBUTTONSIZE, &CToolBar::OnSetButtonSize)
	ON_MESSAGE(TB_SETDISABLEDIMAGELIST, &CToolBar::OnPreserveSizingPolicyHelper)
	ON_MESSAGE(WM_SETFONT, &CToolBar::OnPreserveZeroBorderHelper)
	ON_MESSAGE(TB_SETHOTIMAGELIST, &CToolBar::OnPreserveSizingPolicyHelper)
	ON_MESSAGE(TB_SETIMAGELIST, &CToolBar::OnPreserveSizingPolicyHelper)
	ON_MESSAGE(WM_SETTINGCHANGE, &CToolBar::OnPreserveZeroBorderHelper)
END_MESSAGE_MAP()

_INLINE void CControlBar::DrawBorders(CDC* pDC, CRect& rect)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	DWORD dwStyle = m_dwStyle;
	if (!(dwStyle & CBRS_BORDER_ANY))
		return;

	// prepare for dark lines
	ASSERT(rect.top == 0 && rect.left == 0);
	CRect rect1 = rect;
	CRect rect2 = rect;
	COLORREF clr = afxData.clrBtnShadow;


	// draw dark line one pixel back/up
	if (dwStyle & CBRS_BORDER_3D)
	{
		rect1.right -= AFX_CX_BORDER;
		rect1.bottom -= AFX_CY_BORDER;
	}
	if (dwStyle & CBRS_BORDER_TOP)
		rect2.top += afxData.cyBorder2;
	if (dwStyle & CBRS_BORDER_BOTTOM)
		rect2.bottom -= afxData.cyBorder2;

	if (IsKindOf(RUNTIME_CLASS(CToolBar)) && (dwStyle & TBSTYLE_FLAT) && (dwStyle & CBRS_BORDER_ANY))
	{
		CRect rctBk;
		rctBk.left = 0;
		rctBk.right = rect.right;
		rctBk.top = rect.top;
		rctBk.bottom = rect.bottom;

		pDC->FillSolidRect((LPRECT)rctBk, pDC->GetBkColor());
	}

	// draw left and top
	if (dwStyle & CBRS_BORDER_LEFT)
		pDC->FillSolidRect(0, rect2.top, AFX_CX_BORDER, rect2.Height(), clr);
	if (dwStyle & CBRS_BORDER_TOP)
		pDC->FillSolidRect(0, 0, rect.right, AFX_CY_BORDER, clr);

	// draw right and bottom
	if (dwStyle & CBRS_BORDER_RIGHT)
		pDC->FillSolidRect(rect1.right, rect2.top, -AFX_CX_BORDER, rect2.Height(), clr);
	if (dwStyle & CBRS_BORDER_BOTTOM)
		pDC->FillSolidRect(0, rect1.bottom, rect.right, -AFX_CY_BORDER, clr);


	if (dwStyle & CBRS_BORDER_3D)
	{
		// prepare for hilite lines
		clr = afxData.clrBtnHilite;

		// draw left and top
		if (dwStyle & CBRS_BORDER_LEFT)
			pDC->FillSolidRect(1, rect2.top, AFX_CX_BORDER, rect2.Height(), clr);
		if (dwStyle & CBRS_BORDER_TOP)
			pDC->FillSolidRect(0, 1, rect.right, AFX_CY_BORDER, clr);

		// draw right and bottom
		if (dwStyle & CBRS_BORDER_RIGHT)
			pDC->FillSolidRect(rect.right, rect2.top, -AFX_CX_BORDER, rect2.Height(), clr);
		if (dwStyle & CBRS_BORDER_BOTTOM)
			pDC->FillSolidRect(0, rect.bottom, rect.right, -AFX_CY_BORDER, clr);
	}

	if (dwStyle & CBRS_BORDER_LEFT)
		rect.left += afxData.cxBorder2;
	if (dwStyle & CBRS_BORDER_TOP)
		rect.top += afxData.cyBorder2;
	if (dwStyle & CBRS_BORDER_RIGHT)
		rect.right -= afxData.cxBorder2;
	if (dwStyle & CBRS_BORDER_BOTTOM)
		rect.bottom -= afxData.cyBorder2;

}


_INLINE void CToolCmdUI::Enable(BOOL bOn)
{
	m_bEnableChanged = TRUE;
	CToolBar* pToolBar = (CToolBar*)m_pOther;
	ASSERT(pToolBar != NULL);
	ASSERT(m_nIndex < m_nIndexMax);

	UINT nNewStyle = pToolBar->GetButtonStyle(m_nIndex) & ~TBBS_DISABLED;
	if (!bOn)
	{
		nNewStyle |= TBBS_DISABLED;
		// If a button is currently pressed and then is disabled
		// COMCTL32.DLL does not unpress the button, even after the mouse
		// button goes up!  We work around this bug by forcing TBBS_PRESSED
		// off when a button is disabled.
		nNewStyle &= ~TBBS_PRESSED;
	}
	ASSERT(!(nNewStyle & TBBS_SEPARATOR));
	pToolBar->SetButtonStyle(m_nIndex, nNewStyle);
}

_INLINE void CToolCmdUI::SetCheck(int nCheck)
{
	ASSERT(nCheck >= 0 && nCheck <= 2); // 0=>off, 1=>on, 2=>indeterminate
	CToolBar* pToolBar = (CToolBar*)m_pOther;
	ASSERT(pToolBar != NULL);
	ASSERT(m_nIndex < m_nIndexMax);

	UINT nNewStyle = pToolBar->GetButtonStyle(m_nIndex) &
		~(TBBS_CHECKED | TBBS_INDETERMINATE);
	if (nCheck == 1)
		nNewStyle |= TBBS_CHECKED;
	else if (nCheck == 2)
		nNewStyle |= TBBS_INDETERMINATE;
	ASSERT(!(nNewStyle & TBBS_SEPARATOR));
	pToolBar->SetButtonStyle(m_nIndex, nNewStyle | TBBS_CHECKBOX);
}

_INLINE void CToolCmdUI::SetText(LPCTSTR)
{
	// ignore it
}

