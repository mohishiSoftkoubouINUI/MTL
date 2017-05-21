







class CAnimateCtrlMTL : public CAnimateCtrlT<CWnd>
{
	public:
		virtual ~CAnimateCtrlMTL() { DestroyWindow(); }

	public:
	public:
		BOOL CAnimateCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
		{
			// initialize common controls
			VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_ANIMATE_REG));

			CWnd* pWnd = this;
			return pWnd->Create(ANIMATE_CLASS, NULL, dwStyle, rect, pParentWnd, nID);
		}

};

class CButtonMTL : public CButtonT<CWnd>
{
	public:
		virtual ~CButtonMTL() { DestroyWindow(); }

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
		virtual ~CComboBoxMTL() { DestroyWindow(); }

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
		virtual ~CComboBoxExMTL() { DestroyWindow(); }

	public:
	public:
		BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
		{
			// initialize common controls
			VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_USEREX_REG));

			CWnd* pWnd = this;
			return pWnd->Create(WC_COMBOBOXEX, NULL, dwStyle, rect, pParentWnd, nID);
		}

		CComboBoxMTL* GetComboBoxCtrl() const
		{
			ASSERT(::IsWindow(m_hWnd)); return (CComboBoxMTL*)CComboBox::FromHandle((HWND) ::SendMessage(m_hWnd, CBEM_GETCOMBOCONTROL, 0, 0));
		}

		CEditMTL* GetEditCtrl() const ;

		CImageListMTL* SetImageList(CImageListMTL* pImageList)
		{
			ASSERT(::IsWindow(m_hWnd)); return CImageListMTL::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, CBEM_SETIMAGELIST, 0, (LPARAM)pImageList->GetSafeHandle()));
		}

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
		virtual ~CDragListBoxMTL() { DestroyWindow(); }

	public:
	public:
};

class CEditMTL : public CEditT<CWnd>
{
	public:
		virtual ~CEditMTL() { DestroyWindow(); }

	public:
	public:
		BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
		{
			CWnd* pWnd = this;
			return pWnd->Create(_T("EDIT"), NULL, dwStyle, rect, pParentWnd, nID);
		}
};

_INLINE CEditMTL* CComboBoxExMTL::GetEditCtrl() const
{
	ASSERT(::IsWindow(m_hWnd)); return (CEditMTL*)CEditMTL::FromHandle((HWND) ::SendMessage(m_hWnd, CBEM_GETEDITCONTROL, 0, 0));
}

class CFlatScrollBarMTL : public CFlatScrollBarT<CWnd>
{
public:
public:
public:
};

class CHeaderCtrlMTL : public CHeaderCtrlT<CWnd>
{
	public:
		virtual ~CHeaderCtrlMTL() { DestroyWindow(); }

	public:
	public:
		BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
		{
			// initialize common controls
			VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_LISTVIEW_REG));

			CWnd* pWnd = this;
			return pWnd->Create(WC_HEADER, NULL, dwStyle, rect, pParentWnd, nID);
		}

		CImageListMTL* CreateDragImage(int nIndex)
		{
			ASSERT(::IsWindow(m_hWnd)); return CImageListMTL::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, HDM_CREATEDRAGIMAGE, nIndex, 0L));
		}

		CImageListMTL* GetImageList(int nImageList /* = HDSIL_NORMAL */) const
		{
			ASSERT(::IsWindow(m_hWnd)); return CImageListMTL::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, HDM_GETIMAGELIST, nImageList, 0L));
		}

		CImageListMTL* SetImageList(CImageListMTL* pImageList, int nImageList /* = HDSIL_NORMAL */)
		{
			ASSERT(::IsWindow(m_hWnd)); return CImageListMTL::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, HDM_SETIMAGELIST, nImageList, (LPARAM)pImageList->GetSafeHandle()));
		}

};

class CHotKeyCtrlMTL : public CHotKeyCtrlT<CWnd>
{
	public:
		virtual ~CHotKeyCtrlMTL() { DestroyWindow(); }
	
	public:
	public:
		BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
		{
			// initialize common controls
			VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_HOTKEY_REG));

			CWnd* pWnd = this;
			return pWnd->Create(HOTKEY_CLASS, NULL, dwStyle, rect, pParentWnd, nID);
		}

};

class CIPAddressCtrlMTL : public CIPAddressCtrlT<CWnd>
{
	public:
		virtual ~CIPAddressCtrlMTL() { DestroyWindow(); }

	public:
	public:
		BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
		{
			// initialize common controls
			VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_INTERNET_REG));

			// the IP Address Control must be a child
			ASSERT(dwStyle & WS_CHILD);

			CWnd* pWnd = this;
			return pWnd->Create(WC_IPADDRESS, NULL, dwStyle, rect, pParentWnd, nID);
		}

};

class CListBoxMTL : public CListBoxT<CWnd>
{
	public:
		virtual ~CListBoxMTL() { DestroyWindow(); }

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
		virtual ~CLinkCtrlMTL() { DestroyWindow(); }

	public:
	public:
		BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
		{
			// initialize common controls
			ENSURE(AfxDeferRegisterClass(AFX_WNDCOMMCTL_LINK_REG));

#ifndef _UNICODE
			ASSERT(0);
			dwStyle;
			rect;
			pParentWnd;
			nID;
			// Can only be used in UNICODE builds
			return FALSE;
#else
			CWnd* pWnd = this;
			return pWnd->Create(WC_LINK, NULL, dwStyle, rect, pParentWnd, nID);
#endif	// _UNICODE
		}

};
#endif // _MTL_ATL3


class CListCtrlMTL : public CListViewCtrlT<CWnd>
{
	public:
		virtual ~CListCtrlMTL() { DestroyWindow(); }
	
	public:
	public:
		CEdit* EditLabel(int nItem)
		{
			ASSERT(::IsWindow(m_hWnd)); return (CEdit*)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, LVM_EDITLABEL, nItem, 0L));
		}

		CEdit* GetEditControl() const
		{
			ASSERT(::IsWindow(m_hWnd)); return (CEdit*)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, LVM_GETEDITCONTROL, 0, 0L));
		}

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

			return (CHeaderCtrl*)FromHandle(hWnd);
		}

		CImageListMTL* GetImageList(int nImageList) const
		{
			ASSERT(::IsWindow(m_hWnd)); return CImageListMTL::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, LVM_GETIMAGELIST, nImageList, 0L));
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

		CToolTipCtrlMTL* GetToolTips() const
		{
			ASSERT(::IsWindow(m_hWnd)); return (CToolTipCtrlMTL*)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, LVM_GETTOOLTIPS, 0, 0L));
		}

		CImageListMTL* SetImageList(CImageListMTL* pImageList, int nImageList)
		{
			ASSERT(::IsWindow(m_hWnd)); return CImageListMTL::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, LVM_SETIMAGELIST, nImageList, (LPARAM)pImageList->GetSafeHandle()));
		}

		CToolTipCtrlMTL* SetToolTips(CToolTipCtrlMTL* pTip) ;
			

};

class CMonthCalendarCtrlMTL : public CMonthCalendarCtrlT<CWnd>
{
public:
public:
public:
};

class CPagerCtrlMTL : public CPagerCtrlT<CWnd>
{
	public:
		virtual ~CPagerCtrlMTL() { DestroyWindow(); }

	public:
	public:
		BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
		{
			// initialize common controls
			ENSURE(AfxDeferRegisterClass(AFX_WNDCOMMCTL_PAGER_REG));

			CWnd* pWnd = this;
			return pWnd->Create(WC_PAGESCROLLER, NULL, dwStyle, rect, pParentWnd, nID);
		}

};

class CRichEditCtrlMTL : public CRichEditCtrlT<CWnd>
{
	public:
		virtual ~CRichEditCtrlMTL() { DestroyWindow(); }

	public:
	public:
};

class CProgressCtrlMTL : public CProgressBarCtrlT<CWnd>
{
	public:
		virtual ~CProgressCtrlMTL() { DestroyWindow(); }
	
	public:
	public:
		BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
		{
			// initialize common controls
			VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_PROGRESS_REG));

			CWnd* pWnd = this;
			return pWnd->Create(PROGRESS_CLASS, NULL, dwStyle, rect, pParentWnd, nID);
		}

};

class CProgressBarCtrlMTL : public CProgressBarCtrlT<CWnd>
{
public:
public:
public:
};

class CReBarCtrlMTL : public CReBarCtrlT<CWnd>
{
	public:
		virtual ~CReBarCtrlMTL() { DestroyWindow(); }

	public:
	public:
		BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
		{
			// initialize common controls
			VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_COOL_REG));

			CWnd* pWnd = this;
			return pWnd->Create(REBARCLASSNAME, NULL, dwStyle, rect, pParentWnd, nID);
		}

		CPaletteMTL* GetPalette() const
		{
			ASSERT(::IsWindow(m_hWnd)); return CPaletteMTL::FromHandle((HPALETTE)::SendMessage(m_hWnd, RB_GETPALETTE, 0, 0L));
		}

		CToolTipCtrlMTL* GetToolTips() const
		{
			ASSERT(::IsWindow(m_hWnd)); return (CToolTipCtrlMTL*)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, RB_GETTOOLTIPS, 0, 0L));
		}

		CWnd* SetOwner(CWnd* pWnd)
		{
			ASSERT(::IsWindow(m_hWnd)); CWnd::SetOwner(pWnd);  return CWnd::FromHandle((HWND)::SendMessage(m_hWnd, RB_SETPARENT, (WPARAM)pWnd->m_hWnd, 0L));
		}

		CPaletteMTL* SetPalette(HPALETTE hPal)
		{
			ASSERT(::IsWindow(m_hWnd)); return CPaletteMTL::FromHandle((HPALETTE)::SendMessage(m_hWnd, RB_SETPALETTE, 0, (LPARAM)hPal));
		}

		void SetToolTips(CToolTipCtrlMTL* pTip) ;

};

class CScrollBarMTL : public CScrollBarT<CWnd>
{
	DECLARE_DYNAMIC(CScrollBar)

	public:
		virtual ~CScrollBarMTL() { DestroyWindow(); }

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
		virtual ~CSliderCtrlMTL() { DestroyWindow(); }

	public:
	public:
		BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
		{
			// initialize common controls
			VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_BAR_REG));

			CWnd* pWnd = this;
			return pWnd->Create(TRACKBAR_CLASS, NULL, dwStyle, rect, pParentWnd, nID);
		}

		CWnd* GetBuddy(BOOL fLocation) const
		{
			ASSERT(::IsWindow(m_hWnd)); return CWnd::FromHandle((HWND) ::SendMessage(m_hWnd, TBM_GETBUDDY, fLocation, 0l));
		}

		CToolTipCtrlMTL* GetToolTips() const
		{
			ASSERT(::IsWindow(m_hWnd)); return (CToolTipCtrlMTL*)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, TBM_GETTOOLTIPS, 0, 0L));
		}

		CWnd* SetBuddy(CWnd* pWndBuddy, BOOL fLocation)
		{
			ASSERT(::IsWindow(m_hWnd)); return CWnd::FromHandle((HWND) ::SendMessage(m_hWnd, TBM_SETBUDDY, fLocation, (LPARAM)pWndBuddy->GetSafeHwnd()));
		}

		void SetToolTips(CToolTipCtrlMTL* pTip) ;

};

class CSpinButtonCtrlMTL : public CUpDownCtrlT<CWnd>
{
	public:
		virtual ~CSpinButtonCtrlMTL() { DestroyWindow(); }

	public:
	public:
		BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
		{
			// initialize common controls
			VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_UPDOWN_REG));

			CWnd* pWnd = this;
			return pWnd->Create(UPDOWN_CLASS, NULL, dwStyle, rect, pParentWnd, nID);
		}

		CWnd* GetBuddy() const
		{
			ASSERT(::IsWindow(m_hWnd)); return CWnd::FromHandle((HWND) ::SendMessage(m_hWnd, UDM_GETBUDDY, 0, 0l));
		}

		CWnd* SetBuddy(CWnd* pWndBuddy)
		{
			ASSERT(::IsWindow(m_hWnd)); return CWnd::FromHandle((HWND) ::SendMessage(m_hWnd, UDM_SETBUDDY, (WPARAM)pWndBuddy->GetSafeHwnd(), 0L));
		}

};

class CStaticMTL : public CStaticT<CWnd>
{
	public:
		virtual ~CStaticMTL() { DestroyWindow(); }

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
		virtual ~CStatusBarCtrlMTL() { DestroyWindow(); }

	public:
	public:
		BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
		{
			// initialize common controls
			VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_BAR_REG));

			CWnd* pWnd = this;
			return pWnd->Create(STATUSCLASSNAME, NULL, dwStyle, rect, pParentWnd, nID);
		}

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

		BOOL CreateEx(DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
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

		CImageListMTL* GetImageList() const
		{
			ASSERT(::IsWindow(m_hWnd)); return CImageListMTL::FromHandle((HIMAGELIST)::SendMessage(m_hWnd, TCM_GETIMAGELIST, 0, 0L));
		}

		CToolTipCtrlMTL* GetToolTips() const
		{
			ASSERT(::IsWindow(m_hWnd)); return (CToolTipCtrlMTL*)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, TCM_GETTOOLTIPS, 0, 0L));
		}

		DWORD SetExtendedStyle(DWORD dwNewStyle, DWORD dwExMask /*= 0*/)
		{
			ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, TCM_SETEXTENDEDSTYLE, dwExMask, dwNewStyle);
		}

		CImageListMTL* SetImageList(CImageListMTL* pImageList)
		{
			ASSERT(::IsWindow(m_hWnd)); return CImageListMTL::FromHandle((HIMAGELIST)::SendMessage(m_hWnd, TCM_SETIMAGELIST, 0, (LPARAM)pImageList->GetSafeHandle()));
		}

		void SetToolTips(CToolTipCtrlMTL* pWndTip) ;

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

		CImageListMTL* GetDisabledImageList() const
		{
			ASSERT(::IsWindow(m_hWnd)); return CImageListMTL::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, TB_GETDISABLEDIMAGELIST, 0, 0));
		}

		CImageListMTL* GetHotImageList() const
		{
			ASSERT(::IsWindow(m_hWnd)); return CImageListMTL::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, TB_GETHOTIMAGELIST, 0, 0));
		}

		CImageListMTL* GetImageList() const
		{
			ASSERT(::IsWindow(m_hWnd)); return CImageListMTL::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, TB_GETIMAGELIST, 0, 0));
		}

		CImageListMTL* SetDisabledImageList(CImageListMTL* pImageList)
		{
			ASSERT(::IsWindow(m_hWnd)); return CImageListMTL::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, TB_SETDISABLEDIMAGELIST, 0, (LPARAM)pImageList->GetSafeHandle()));
		}

		CImageListMTL* SetHotImageList(CImageListMTL* pImageList)
		{
			ASSERT(::IsWindow(m_hWnd)); return CImageListMTL::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, TB_SETHOTIMAGELIST, 0, (LPARAM)pImageList->GetSafeHandle()));
		}

		CImageListMTL* SetImageList(CImageListMTL* pImageList)
		{
			ASSERT(::IsWindow(m_hWnd)); return CImageListMTL::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, TB_SETIMAGELIST, 0, (LPARAM)pImageList->GetSafeHandle()));
		}

};


class CToolTipCtrlMTL : public CToolTipCtrlT<CWnd>
{
	DECLARE_DYNAMIC(CToolTipCtrlMTL)
	
	public:
		virtual ~CToolTipCtrlMTL() { DestroyWindow(); }

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

		CToolTipCtrlMTL* GetToolTips() const
		{
			ASSERT(::IsWindow(m_hWnd)); return (CToolTipCtrlMTL*)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, TB_GETTOOLTIPS, 0, 0L));
		}
		void SetToolTips(CToolTipCtrlMTL* pTip)
		{
			ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, TB_SETTOOLTIPS, (WPARAM)pTip->GetSafeHwnd(), 0L);
		}
		void SetOwner(CWnd* pWnd)
		{
			ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, TB_SETPARENT, (WPARAM)pWnd->GetSafeHwnd(), 0L);
		}
};

_INLINE CToolTipCtrlMTL* CListCtrlMTL::SetToolTips(CToolTipCtrlMTL* pTip)
{
	ASSERT(::IsWindow(m_hWnd)); return (CToolTipCtrlMTL*)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, LVM_SETTOOLTIPS, 0, (LPARAM)pTip->GetSafeHwnd()));
}

_INLINE void CReBarCtrlMTL::SetToolTips(CToolTipCtrlMTL* pTip)
{
	ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, RB_SETTOOLTIPS, (WPARAM)pTip->m_hWnd, 0L);
}

_INLINE void CSliderCtrlMTL::SetToolTips(CToolTipCtrlMTL* pTip)
{
	ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, TBM_SETTOOLTIPS, (WPARAM)pTip->GetSafeHwnd(), 0L);
}

_INLINE void CTabCtrlMTL::SetToolTips(CToolTipCtrlMTL* pWndTip)
{
	ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, TCM_SETTOOLTIPS, (WPARAM)pWndTip->GetSafeHwnd(), 0L);
}

class CTrackBarCtrlMTL : public CTrackBarCtrlT<CWnd>
{
public:
public:
public:
};

class CTreeCtrlMTL : public CTreeViewCtrlT<CWnd>
{
	public:
		virtual ~CTreeCtrlMTL() { DestroyWindow(); }

	public:
	public:
		BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
		{
			// initialize common controls
			VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_TREEVIEW_REG));

			CWnd* pWnd = this;
			return pWnd->Create(WC_TREEVIEW, NULL, dwStyle, rect, pParentWnd, nID);
		}

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

		CEditMTL* EditLabel(HTREEITEM hItem)
		{
			ASSERT(::IsWindow(m_hWnd)); return (CEdit*)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, TVM_EDITLABEL, 0, (LPARAM)hItem));
		}

		CEditMTL* GetEditControl() const
		{
			ASSERT(::IsWindow(m_hWnd)); return (CEdit*)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, TVM_GETEDITCONTROL, 0, 0));
		}

		DWORD GetExtendedStyle() const
		{
			ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, TVM_GETEXTENDEDSTYLE, (WPARAM)0, 0);
		}

		CImageListMTL* GetImageList(int nImageList) const
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

		CToolTipCtrlMTL* GetToolTips() const
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

		CToolTipCtrlMTL* SetToolTips(CToolTipCtrl* pWndTip)
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




