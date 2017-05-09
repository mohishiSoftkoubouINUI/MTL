





/*============================================================================*/
// class CCtrlView allows almost any control to be a view

class ATL_NO_VTABLE CCtrlView : public CView
{
	DECLARE_DYNAMIC(CCtrlView)

	public:
		CCtrlView(LPCTSTR lpszClass, DWORD dwStyle)
		{
			m_strClass = lpszClass;
			m_dwDefaultStyle = dwStyle;
		}

		// Attributes
	public:
		DECLARE_MESSAGE_MAP()


		CString m_strClass;
		DWORD m_dwDefaultStyle;

		// Implementation
	public:
		void OnPaint()
		{
			// this is done to avoid CView::OnPaint
			Default();
		}

		LRESULT OnPrintClient(CDC* /*pDC*/, UINT /*nFlags*/)
		{
			return Default();
		}

		// Overrides

		virtual BOOL PreCreateWindow(CREATESTRUCT& cs)
		{
			ASSERT(cs.lpszClass == NULL);
			cs.lpszClass = m_strClass;

			// initialize common controls
			VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTLS_REG));
			AfxDeferRegisterClass(AFX_WNDCOMMCTLSNEW_REG);

			// map default CView style to default style
			// WS_BORDER is insignificant
			if ((cs.style | WS_BORDER) == AFX_WS_DEFAULT_VIEW)
				cs.style = m_dwDefaultStyle & (cs.style | ~WS_BORDER);

			return CView::PreCreateWindow(cs);
		}


};

BEGIN_MESSAGE_MAP(CCtrlView, CView)
	ON_WM_PAINT()
	ON_WM_PRINTCLIENT()
END_MESSAGE_MAP()


/*============================================================================*/
// CEditView - simple text editor view

class CEditView : public CCtrlView
{
	DECLARE_DYNAMIC(CEditView)

		// Construction
	public:
		CEditView() : CCtrlView(_T("EDIT"), NULL), afxData(CAfx::GetAfxData())
		{
			m_nTabStops = 8 * 4;  // default 8 character positions
			m_hPrinterFont = NULL;
			m_hMirrorFont = NULL;
			m_pShadowBuffer = NULL;
			m_nShadowSize = 0;
		}

		virtual ~CEditView()
		{
			ASSERT(m_hWnd == NULL);
			delete[] m_pShadowBuffer;
		}

		// Attributes
	public:

		DECLARE_MESSAGE_MAP()


		const struct _AUX_DATA& afxData;


		int m_nTabStops;            // tab stops in dialog units
		LPTSTR m_pShadowBuffer;     // special shadow buffer only used in Win95
		UINT m_nShadowSize;

		CArray<UINT> m_aPageStart;    // array of starting pages
		HFONT m_hPrinterFont;       // if NULL, mirror display font
		HFONT m_hMirrorFont;        // font object used when mirroring

	public:
		static UINT _AfxEndOfLine(LPCTSTR lpszText, UINT nLen, UINT nIndex)
		{
			ASSERT(AfxIsValidAddress(lpszText, nLen, FALSE));
			ENSURE_ARG(lpszText != NULL);
			LPCTSTR lpsz = lpszText + nIndex;
			LPCTSTR lpszStop = lpszText + nLen;
			while (lpsz < lpszStop && *lpsz != '\r')
				++lpsz;
			return int(lpsz - lpszText);
		}


		//virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);

		int OnCreate(LPCREATESTRUCT lpcs)
		{
			if (CCtrlView::OnCreate(lpcs) != 0)
				return -1;

			// Set arbitrary max size. Otherwise, default max size would be too small(32K).
			// Call SetLimitText() in OnCreate() to set your own max size.
			// Refer to documentation for EM_LIMITTEXT for max sizes for your target OS.

			// Arbitrary max size. Otherwise, default max size would be too small(32K).
			const UINT nMaxSize = 1024U * 1024U - 1;
			GetEditCtrl().LimitText(nMaxSize);
			GetEditCtrl().SetTabStops(m_nTabStops);

			return 0;
		}

		BOOL OnEditChange() ;

		void OnEditClear() { GetEditCtrl().Clear(); }
		void OnEditCopy() { GetEditCtrl().Copy(); }
		void OnEditCut() { GetEditCtrl().Cut(); ASSERT_VALID(this); }

		//void OnEditFind();
		//void OnEditFindReplace(BOOL bFindOnly);

		void OnEditPaste() {GetEditCtrl().Paste(); }

		//void OnEditRepeat();
		//void OnEditReplace();

		void OnEditSelectAll() { GetEditCtrl().SetSel(0, -1); }
		void OnEditUndo() { GetEditCtrl().Undo(); }

		//virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo = NULL);

		//virtual void OnFindNext(LPCTSTR lpszFind, BOOL bNext, BOOL bCase);
		//LRESULT OnFindReplaceCmd(WPARAM wParam, LPARAM lParam);

		//virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo);
		//virtual BOOL OnPreparePrinting(CPrintInfo* pInfo) { return DoPreparePrinting(pInfo); }
		//virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

		//virtual void OnReplaceSel(LPCTSTR lpszFind, BOOL bNext, BOOL bCase, LPCTSTR lpszReplace);
		//virtual void OnReplaceAll(LPCTSTR lpszFind, LPCTSTR lpszReplace, BOOL bCase);

		void OnSetFont(CFont* pFont, BOOL bRedraw) { Default(); GetEditCtrl().SetTabStops(m_nTabStops); }

		//virtual void OnTextNotFound(LPCTSTR lpszFind);

		void OnUpdateNeedSel(CCmdUI* pCmdUI)
		{
			ASSERT_VALID(this);
			ENSURE_ARG(pCmdUI != NULL);
			int nStartChar, nEndChar;
			GetEditCtrl().GetSel(nStartChar, nEndChar);
			pCmdUI->Enable(nStartChar != nEndChar);
			ASSERT_VALID(this);
		}

		void OnUpdateNeedClip(CCmdUI* pCmdUI)
		{
			ASSERT_VALID(this);
			ENSURE_ARG(pCmdUI != NULL);
			pCmdUI->Enable(::IsClipboardFormatAvailable(CF_TEXT));
			ASSERT_VALID(this);
		}

		void OnUpdateNeedText(CCmdUI* pCmdUI)
		{
			ASSERT_VALID(this);
			ENSURE_ARG(pCmdUI != NULL);
			pCmdUI->Enable(GetWindowTextLength() != 0);
			ASSERT_VALID(this);
		}

		//void OnUpdateNeedFind(CCmdUI* pCmdUI) ;

		void OnUpdateEditUndo(CCmdUI* pCmdUI)
		{
			ASSERT_VALID(this);
			ENSURE_ARG(pCmdUI != NULL);
			pCmdUI->Enable(GetEditCtrl().CanUndo());
			ASSERT_VALID(this);
		}

		// EDIT controls always turn off WS_BORDER and draw it themselves
		virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder)
		{
			ENSURE_ARG(lpClientRect != NULL);
			if (nAdjustType != 0)
			{
				// default behavior for in-place editing handles scrollbars
				DWORD dwStyle = GetStyle();
				if (dwStyle & WS_VSCROLL)
					lpClientRect->right += afxData.cxVScroll - AFX_CX_BORDER;
				if (dwStyle & WS_HSCROLL)
					lpClientRect->bottom += afxData.cyHScroll - AFX_CY_BORDER;
				return;
			}

			::AdjustWindowRectEx(lpClientRect, GetStyle() | WS_BORDER, FALSE, GetExStyle() & ~(WS_EX_CLIENTEDGE));
		}

		virtual void DeleteContents()
		{
			ASSERT_VALID(this);
			ASSERT(m_hWnd != NULL);
			SetWindowText(NULL);
			ASSERT_VALID(this);
		}

		//BOOL FindText(LPCTSTR lpszFind, BOOL bNext = TRUE, BOOL bCase = TRUE);

		// this function returns the length in characters
		UINT GetBufferLength() const
		{
			ASSERT_VALID(this);
			ASSERT(m_hWnd != NULL);
			LPCTSTR lpszText = LockBuffer();
			UINT nLen = lstrlen(lpszText);
			UnlockBuffer();
			return nLen;
		}

		// CEdit control access
		CEdit& GetEditCtrl() const { return *(CEdit*)this; }

		// presentation attributes
		//CFont* GetPrinterFont() const;

		// other attributes
		void GetSelectedText(CString& strResult) const
		{
			ASSERT_VALID(this);
			int nStartChar, nEndChar;
			GetEditCtrl().GetSel(nStartChar, nEndChar);
			ASSERT((UINT)nEndChar <= GetBufferLength());
			LPCTSTR lpszText = ((CEditView*)this)->LockBuffer();
			UINT nLen = _AfxEndOfLine(lpszText, nEndChar, nStartChar) - nStartChar;
			memcpy(strResult.GetBuffer(nLen), lpszText + nStartChar, nLen * sizeof(TCHAR));
			strResult.ReleaseBuffer(nLen);
			UnlockBuffer();
			ASSERT_VALID(this);
		}

		//BOOL InitializeReplace();

		// buffer access
		LPCTSTR LockBuffer() const
		{
			ASSERT_VALID(this);
			ASSERT(m_hWnd != NULL);

#ifndef _UNICODE
			// with commctrl6, it is necessary to maintain a shadow buffer
			//  it is only updated when the control contents have been changed.
			if (m_pShadowBuffer == NULL || GetEditCtrl().GetModify())
			{
				ASSERT(m_pShadowBuffer != NULL || m_nShadowSize == 0);
				UINT nSize = GetWindowTextLength() + 1;
				if (nSize > m_nShadowSize)
				{
					// need more room for shadow buffer
					CEditView* pThis = (CEditView*)this;
					delete[] m_pShadowBuffer;
					pThis->m_pShadowBuffer = NULL;
					pThis->m_nShadowSize = 0;
					pThis->m_pShadowBuffer = new TCHAR[nSize];
					pThis->m_nShadowSize = nSize;
				}

				// update the shadow buffer with GetWindowText
				ASSERT(m_nShadowSize >= nSize);
				ASSERT(m_pShadowBuffer != NULL);
				GetWindowText(m_pShadowBuffer, nSize);

				// turn off edit control's modify bit
				GetEditCtrl().SetModify(FALSE);
			}
			return m_pShadowBuffer;
#else
			// else -- running under non-subset Win32 system
			HLOCAL hLocal = GetEditCtrl().GetHandle();
			ASSERT(hLocal != NULL);
			LPCTSTR lpszText = (LPCTSTR)LocalLock(hLocal);
			ASSERT(lpszText != NULL);
			ASSERT_VALID(this);
			return lpszText;
#endif
		}

		//BOOL PaginateTo(CDC* pDC, CPrintInfo* pInfo);

		virtual BOOL PreCreateWindow(CREATESTRUCT& cs)
		{
			const DWORD dwStyleDefault = AFX_WS_DEFAULT_VIEW |
											WS_HSCROLL | WS_VSCROLL |
											ES_AUTOHSCROLL | ES_AUTOVSCROLL |
											ES_MULTILINE | ES_NOHIDESEL;

			m_dwDefaultStyle = dwStyleDefault;
			return CCtrlView::PreCreateWindow(cs);
		}

		//UINT PrintInsideRect(CDC* pDC, RECT& rectLayout, UINT nIndexStart, UINT nIndexStop);

		//void ReadFromArchive(CArchive& ar, UINT nLen);

		//BOOL SameAsSelected(LPCTSTR lpszCompare, BOOL bCase);

		//virtual void Serialize(CArchive& ar);
		//void SerializeRaw(CArchive& ar);

		//void SetPrinterFont(CFont* pFont);

		void SetTabStops(int nTabStops)
		{
			ASSERT_VALID(this);
			m_nTabStops = nTabStops;
			GetEditCtrl().SetTabStops(m_nTabStops);
			Invalidate();
			ASSERT_VALID(this);
		}

		void UnlockBuffer() const
		{
			ASSERT_VALID(this);
			ASSERT(m_hWnd != NULL);

#ifndef _UNICODE
			return;
#else
			HLOCAL hLocal = GetEditCtrl().GetHandle();
			ASSERT(hLocal != NULL);
			LocalUnlock(hLocal);
#endif
		}

		//void WriteToArchive(CArchive& ar);
};

BEGIN_MESSAGE_MAP(CEditView, CCtrlView)
	ON_WM_CREATE()
	//ON_WM_DESTROY()
	ON_WM_SETFONT()

	ON_COMMAND(ID_EDIT_CLEAR, &CEditView::OnEditClear)
	ON_COMMAND(ID_EDIT_CUT, &CEditView::OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, &CEditView::OnEditCopy)
	//ON_COMMAND(ID_EDIT_FIND, &CEditView::OnEditFind)
	ON_COMMAND(ID_EDIT_PASTE, &CEditView::OnEditPaste)
	//ON_COMMAND(ID_EDIT_REPEAT, &CEditView::OnEditRepeat)
	//ON_COMMAND(ID_EDIT_REPLACE, &CEditView::OnEditReplace)
	//ON_COMMAND(ID_FILE_PRINT, &CCtrlView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_DIRECT, &CCtrlView::OnFilePrint)
	ON_COMMAND(ID_EDIT_SELECT_ALL, &CEditView::OnEditSelectAll)
	ON_COMMAND(ID_EDIT_UNDO, &CEditView::OnEditUndo)

	ON_CONTROL_REFLECT_EX(EN_CHANGE, &CEditView::OnEditChange)

	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, &CEditView::OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CEditView::OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CEditView::OnUpdateNeedSel)
	//ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, &CEditView::OnUpdateNeedText)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CEditView::OnUpdateNeedClip)
	//ON_UPDATE_COMMAND_UI(ID_EDIT_REPEAT, &CEditView::OnUpdateNeedFind)
	//ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, &CEditView::OnUpdateNeedText)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, &CEditView::OnUpdateNeedText)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CEditView::OnUpdateEditUndo)

	//ON_REGISTERED_MESSAGE(_afxMsgFindReplace, &CEditView::OnFindReplaceCmd)
END_MESSAGE_MAP()


/*============================================================================*/
// CListView

class CListView : public CCtrlView
{
	DECLARE_DYNAMIC(CListView)

		// Construction
	public:
		CListView() : CCtrlView(WC_LISTVIEW, AFX_WS_DEFAULT_VIEW) { }

		// Attributes
	public:
		DECLARE_MESSAGE_MAP()

	public:
		virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
		{
			if (message != WM_DRAWITEM)
				return CCtrlView::OnChildNotify(message, wParam, lParam, pResult);

			ASSERT(pResult == NULL);       // no return value expected
			UNUSED(pResult); // unused in release builds

			DrawItem((LPDRAWITEMSTRUCT)lParam);
			return TRUE;
		}

		void OnNcDestroy()
		{
			RemoveImageList(LVSIL_NORMAL);
			RemoveImageList(LVSIL_SMALL);
			RemoveImageList(LVSIL_STATE);

			CCtrlView::OnNcDestroy();
		}

		// Overridables
		virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) {}

		CListCtrl& GetListCtrl() const { return *(CListCtrl*)this; }

		void RemoveImageList(int nImageList) { SendMessage(LVM_SETIMAGELIST, (WPARAM)nImageList, NULL); }


};

BEGIN_MESSAGE_MAP(CListView, CCtrlView)
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()




/*============================================================================*/
// CTreeView

class CTreeView : public CCtrlView
{
	DECLARE_DYNCREATE(CTreeView)

		// Construction
	public:
		CTreeView() : CCtrlView(WC_TREEVIEW, AFX_WS_DEFAULT_VIEW) { }

		// Attributes
	public:
		DECLARE_MESSAGE_MAP()


	public:
		void OnDestroy()
		{
			RemoveImageList(LVSIL_NORMAL);
			RemoveImageList(LVSIL_STATE);

			CCtrlView::OnDestroy();
		}

		CTreeCtrl& GetTreeCtrl() const { return *(CTreeCtrl*)this; }


		void RemoveImageList(int nImageList) { SendMessage(TVM_SETIMAGELIST, (WPARAM)nImageList, NULL); }

};

BEGIN_MESSAGE_MAP(CTreeView, CCtrlView)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


