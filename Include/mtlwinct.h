







class CAnimateCtrlMTL : public CAnimateCtrlT<CWnd>
{
public:
public:
public:
};

class CButtonMTL : public CButtonT<CWnd>
{
	public:
		virtual ~CButton() { DestroyWindow(); }

	public:
	public:
		BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
		{
			CWnd* pWnd = this;
			return pWnd->Create(_T("BUTTON"), lpszCaption, dwStyle, rect, pParentWnd, nID);
		}
};

class CComboBoxMTL : public CComboBoxT<CWnd>
{
	public:
		virtual ~CComboBox() { DestroyWindow(); }

	public:
	public:
		BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
		{
			CWnd* pWnd = this;
			return pWnd->Create(_T("COMBOBOX"), NULL, dwStyle, rect, pParentWnd, nID);
		}

		void GetLBText(int nIndex, CString& rString) const
		{
			ASSERT(::IsWindow(m_hWnd));
			CComboBoxT<CWnd>::GetLBText(nIndex, rString.GetBufferSetLength(GetLBTextLen(nIndex)));
			rString.ReleaseBuffer();
		}

};

class CComboBoxExMTL : public CComboBoxExT<CWnd>
{
public:
public:
public:
};

class CDateTimePickerCtrlMTL : public CDateTimePickerCtrlT<CWnd>
{
public:
public:
public:
};


class CDragListBoxMTL : public CDragListBoxT<CWnd>
{
public:
public:
public:
};

class CEditMTL : public CEditT<CWnd>
{
	public:
		virtual ~CEdit() { DestroyWindow(); }

	public:
	public:
		BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
		{
			CWnd* pWnd = this;
			return pWnd->Create(_T("EDIT"), NULL, dwStyle, rect, pParentWnd, nID);
		}
};

class CFlatScrollBarMTL : public CFlatScrollBarT<CWnd>
{
public:
public:
public:
};

class CHeaderCtrlMTL : public CHeaderCtrlT<CWnd>
{
public:
public:
public:
};

class CHotKeyCtrlMTL : public CHotKeyCtrlT<CWnd>
{
public:
public:
public:
};

class CIPAddressCtrlMTL : public CIPAddressCtrlT<CWnd>
{
public:
public:
public:
};

class CListBoxMTL : public CListBoxT<CWnd>
{
	public:
		virtual ~CListBox() { DestroyWindow(); }

	public:
	public:
		BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
		{
			CWnd* pWnd = this;
			return pWnd->Create(_T("LISTBOX"), NULL, dwStyle, rect, pParentWnd, nID);
		}

		int GetText(int nIndex, LPTSTR lpszBuffer) const
		{
			return CListBoxT<CWnd>::GetText(nIndex, lpszBuffer);
		}

		void GetText(int nIndex, CString& rString) const
		{
			ASSERT(::IsWindow(m_hWnd));
			CListBoxT<CWnd>::GetText(nIndex, rString.GetBufferSetLength(GetTextLen(nIndex)));
			rString.ReleaseBuffer();
		}
};

#ifndef _MTL_ATL3
class CLinkCtrlMTL : public CLinkCtrlT<CWnd>
{
public:
public:
public:
};
#endif // _MTL_ATL3


class CListCtrlMTL : public CListViewCtrlT<CWnd>
{
	public:
		virtual ~CListCtrlMTL() { DestroyWindow(); }
	
	public:
	public:
		DWORD GetExtendedStyle() const
		{
			ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
		}

		DWORD SetExtendedStyle(DWORD dwNewStyle)
		{
			ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)dwNewStyle);
		}

		POSITION GetFirstSelectedItemPosition() const
		{
			ASSERT(::IsWindow(m_hWnd)); return (POSITION)(DWORD_PTR)(1 + GetNextItem(-1, LVIS_SELECTED));
		}

		CHeaderCtrl* GetHeaderCtrl() const
		{
			ASSERT(::IsWindow(m_hWnd));

			HWND hWnd = (HWND) ::SendMessage(m_hWnd, LVM_GETHEADER, 0, 0);
			if (hWnd == NULL)
				return NULL;

			return (CHeaderCtrl*)CHeaderCtrl::FromHandle(hWnd);
		}

		CString GetItemText(int nItem, int nSubItem) const
		{
			ASSERT(::IsWindow(m_hWnd));
			LVITEM lvi;
			memset(&lvi, 0, sizeof(LVITEM));
			lvi.iSubItem = nSubItem;
			CString str;
			int nLen = 128;
			int nRes;
			do
			{
				nLen *= 2;
				lvi.cchTextMax = nLen;
				lvi.pszText = str.GetBufferSetLength(nLen);
				nRes = (int)::SendMessage(m_hWnd, LVM_GETITEMTEXT, (WPARAM)nItem, (LPARAM)&lvi);
			} while (nRes >= nLen - 1);
			str.ReleaseBuffer();
			return str;
		}

		int GetNextSelectedItem(POSITION& pos) const
		{
			ASSERT(::IsWindow(m_hWnd)); DWORD_PTR nOldPos = (DWORD_PTR)pos - 1; pos = (POSITION)(DWORD_PTR)(1 + GetNextItem((UINT)nOldPos, LVIS_SELECTED)); return (UINT)nOldPos;
		}


};

class CMonthCalendarCtrlMTL : public CMonthCalendarCtrlT<CWnd>
{
public:
public:
public:
};

class CRichEditCtrlMTL : public CRichEditCtrlT<CWnd>
{
public:
public:
public:
};

class CProgressCtrlMTL : public CProgressBarCtrlT<CWnd>
{
public:
public:
public:
};

class CProgressBarCtrlMTL : public CProgressBarCtrlT<CWnd>
{
public:
public:
public:
};

class CScrollBarMTL : public CScrollBarT<CWnd>
{
	DECLARE_DYNAMIC(CScrollBar)

	public:
		virtual ~CScrollBar() { DestroyWindow(); }

	public:
	public:
		BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
		{
			CWnd* pWnd = this;
			return pWnd->Create(_T("SCROLLBAR"), NULL, dwStyle, rect, pParentWnd, nID);
		}
};

class CSliderCtrlMTL : public CTrackBarCtrlT<CWnd>
{
public:
public:
public:
};

class CStaticMTL : public CStaticT<CWnd>
{
	public:
		virtual ~CStatic() { DestroyWindow(); }

	public:
	public:
		BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
		{
			CWnd* pWnd = this;
			return pWnd->Create(_T("STATIC"), lpszText, dwStyle, rect, pParentWnd, nID);
		}

};

class CStatusBarCtrlMTL : public CStatusBarCtrlT<CWnd>
{
public:
public:
public:
};

class CTabCtrlMTL : public CTabCtrlT<CWnd>
{
	DECLARE_DYNAMIC(CTabCtrlMTL)

	public:
		virtual ~CTabCtrlMTL() { DestroyWindow(); }

	public:
	public:
		BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
		{
			// initialize common controls
			VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_TAB_REG));

			CWnd* pWnd = this;
			return pWnd->Create(WC_TABCONTROL, NULL, dwStyle, rect, pParentWnd, nID);
		}

		BOOL CTabCtrl::CreateEx(DWORD dwExStyle, DWORD dwStyle, const RECT& rect,
			CWnd* pParentWnd, UINT nID)
		{
			BOOL bRet = Create(dwStyle, rect, pParentWnd, nID);
			if (bRet && dwExStyle != 0)
			{
				bRet = ModifyStyleEx(0, dwExStyle);
			}
			return bRet;
		}

		DWORD GetExtendedStyle() const
		{
			ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, TCM_GETEXTENDEDSTYLE, 0, 0);
		}

		CImageList* GetImageList() const
		{
			ASSERT(::IsWindow(m_hWnd)); return CImageList::FromHandle((HIMAGELIST)::SendMessage(m_hWnd, TCM_GETIMAGELIST, 0, 0L));
		}

		DWORD SetExtendedStyle(DWORD dwNewStyle, DWORD dwExMask /*= 0*/)
		{
			ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, TCM_SETEXTENDEDSTYLE, dwExMask, dwNewStyle);
		}

		CImageList* SetImageList(CImageList* pImageList)
		{
			ASSERT(::IsWindow(m_hWnd)); return CImageList::FromHandle((HIMAGELIST)::SendMessage(m_hWnd, TCM_SETIMAGELIST, 0, (LPARAM)pImageList->GetSafeHandle()));
		}
};

class CToolBarCtrlMTL : public CToolBarCtrlT<CWnd>
{
	public:
		virtual ~CToolBarCtrlMTL() { DestroyWindow(); }

	public:
	public:
		DWORD GetExtendedStyle() const
		{
			ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, TB_GETEXTENDEDSTYLE, 0, 0L);
		}
		DWORD SetExtendedStyle(DWORD dwExStyle)
		{
			ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, TB_SETEXTENDEDSTYLE, 0, dwExStyle);
		}
};


class CToolTipCtrlMTL : public CToolTipCtrlT<CWnd>
{
	DECLARE_DYNAMIC(CToolTipCtrlMTL)
	
	public:
		~CToolTipCtrl() { DestroyWindow(); }

	public:
	public:
		BOOL Create(CWnd* pParentWnd, DWORD dwStyle /* = 0 */)
		{
			// initialize common controls
			VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_BAR_REG));

			BOOL bResult = CWnd::CreateEx(NULL, TOOLTIPS_CLASS, NULL,
				WS_POPUP | dwStyle, // force WS_POPUP
				CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
				pParentWnd->GetSafeHwnd(), NULL, NULL);

			if (bResult)
				SetOwner(pParentWnd);
			return bResult;
		}
};

class CTrackBarCtrlMTL : public CTrackBarCtrlT<CWnd>
{
public:
public:
public:
};

class CTreeCtrlMTL : public CTreeViewCtrlT<CWnd>
{
	public:
		~CTreeCtrlMTL() { DestroyWindow(); }

	public:
	public:
		CImageList* CreateDragImage(HTREEITEM hItem)
		{
			ASSERT(::IsWindow(m_hWnd));

			HIMAGELIST hImageList = (HIMAGELIST)::SendMessage(m_hWnd, TVM_CREATEDRAGIMAGE, 0, (LPARAM)hItem);
			if (hImageList == NULL)
				return NULL;

			CImageList* pImageList = new CImageList;
			VERIFY(pImageList->Attach(hImageList));
			return pImageList;
		}

		CEdit* GetEditControl() const
		{
			ASSERT(::IsWindow(m_hWnd)); return (CEdit*)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, TVM_GETEDITCONTROL, 0, 0));
		}

		DWORD GetExtendedStyle() const
		{
			ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, TVM_GETEXTENDEDSTYLE, (WPARAM)0, 0);
		}

		CImageList* GetImageList(int nImageList) const
		{
			ASSERT(::IsWindow(m_hWnd)); return CImageList::FromHandle((HIMAGELIST)::SendMessage(m_hWnd, TVM_GETIMAGELIST, (WPARAM)nImageList, 0));
		}

		CString GetItemText(HTREEITEM hItem) const
		{
			ASSERT(::IsWindow(m_hWnd));
			TVITEM item;
			item.hItem = hItem;
			item.mask = TVIF_TEXT;
			CString str;
			int nLen = 128;
			int nRes;
			do
			{
				nLen *= 2;
				item.pszText = str.GetBufferSetLength(nLen);
				item.cchTextMax = nLen;
				::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item);
				nRes = lstrlen(item.pszText);
			} while (nRes >= nLen - 1);
			str.ReleaseBuffer();
			return str;
		}

		CToolTipCtrl* GetToolTips() const
		{
			ASSERT(::IsWindow(m_hWnd)); return (CToolTipCtrl*)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, TVM_GETTOOLTIPS, 0, 0L));
		}

		DWORD SetExtendedStyle(DWORD dwExMask, DWORD dwExStyles)
		{
			ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, TVM_SETEXTENDEDSTYLE, (WPARAM)dwExMask, (LPARAM)dwExStyles);
		}

		CImageList* SetImageList(CImageList* pImageList, int nImageList)
		{
			ASSERT(::IsWindow(m_hWnd)); return CImageList::FromHandle((HIMAGELIST)::SendMessage(m_hWnd, TVM_SETIMAGELIST, (WPARAM)nImageList, (LPARAM)pImageList->GetSafeHandle()));
		}

		CToolTipCtrl* SetToolTips(CToolTipCtrl* pWndTip)
		{
			ASSERT(::IsWindow(m_hWnd)); return (CToolTipCtrl*)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, TVM_SETTOOLTIPS, (WPARAM)pWndTip->GetSafeHwnd(), 0L));
		}

};

class CTreeItemMTL : public CTreeItemT<CWnd>
{
public:
public:
public:
};

class CTreeCtrlExMTL : public CTreeViewCtrlExT<CWnd>
{
public:
public:
public:
};

class CUpDownCtrlMTL : public CUpDownCtrlT<CWnd>
{
public:
public:
public:
};

class CReBarCtrlMTL : public CReBarCtrlT<CWnd>
{
public:
public:
public:
};

class CPagerCtrlMTL : public CPagerCtrlT<CWnd>
{
public:
public:
public:
};

class CSpinButtonCtrlMTL : public CUpDownCtrlT<CWnd>
{
	public:
		~CSpinButtonCtrlMTL() { DestroyWindow(); }

	public:
	public:
		CWnd* GetBuddy() const
		{
			ASSERT(::IsWindow(m_hWnd)); return CWnd::FromHandle((HWND) ::SendMessage(m_hWnd, UDM_GETBUDDY, 0, 0l));
		}

		CWnd* SetBuddy(CWnd* pWndBuddy)
		{
			ASSERT(::IsWindow(m_hWnd)); return CWnd::FromHandle((HWND) ::SendMessage(m_hWnd, UDM_SETBUDDY, (WPARAM)pWndBuddy->GetSafeHwnd(), 0L));
		}

};




