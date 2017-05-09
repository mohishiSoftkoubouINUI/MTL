
	
	
class CMenuMTL : public CHandleWrapper<CMenuMTL, HMENU, CMenu>
{
	public:
		CMenuMTL(HMENU hHnd = NULL) : CHandleWrapper<CMenuMTL, HMENU, CMenu>(hHnd) { }

	public:
		IMPLEMENT_HANDLE_CREATE(CMenuMTL, HMENU)


	public:
		operator HMENU() const
		{
			ASSERT(this == NULL || m_hMenu == NULL || ::IsMenu(m_hMenu));
			return this == NULL ? NULL : m_hMenu;
		}


		BOOL DestroyMenu()
		{
			if (m_hMenu == NULL)
				return FALSE;
			return ::DestroyMenu(Detach());
		}

		int GetMenuString(UINT nIDItem, CString& rString, UINT nFlags) const
		{
			ASSERT(::IsMenu(m_hMenu));

			// offer no buffer first
			int nStringLen = ::GetMenuString(m_hMenu, nIDItem, NULL, 0, nFlags);

			// use exact buffer length
			if (nStringLen > 0)
			{
				LPTSTR pstrString = rString.GetBufferSetLength(nStringLen + 1);
				::GetMenuString(m_hMenu, nIDItem, pstrString, nStringLen + 1, nFlags);
				rString.ReleaseBuffer();
			}
			else
				rString.Empty();

			return nStringLen;
		}


		HMENU GetSafeHmenu() const
		{
			ASSERT(this == NULL || m_hMenu == NULL || ::IsMenu(m_hMenu));
			return this == NULL ? NULL : m_hMenu;
		}

		//BOOL CreateMenu() { return Attach(::CreateMenu()); }
		//BOOL CreatePopupMenu() { return Attach(::CreatePopupMenu()); }
		//BOOL DeleteMenu(UINT nPosition, UINT nFlags) { ASSERT(::IsMenu(m_hMenu)); return ::DeleteMenu(m_hMenu, nPosition, nFlags); }
		BOOL AppendMenu(UINT nFlags, UINT_PTR nIDNewItem = 0, LPCTSTR lpszNewItem = NULL) { ASSERT(::IsMenu(m_hMenu)); return ::AppendMenu(m_hMenu, nFlags, nIDNewItem, lpszNewItem); }
		BOOL AppendMenu(UINT nFlags, UINT_PTR nIDNewItem, CBitmap* pBmp) { ASSERT(::IsMenu(m_hMenu)); return ::AppendMenu(m_hMenu, nFlags | MF_BITMAP, nIDNewItem, (LPCTSTR)pBmp->GetSafeHandle()); }
		//UINT CheckMenuItem(UINT nIDCheckItem, UINT nCheck) { ASSERT(::IsMenu(m_hMenu)); return (UINT)::CheckMenuItem(m_hMenu, nIDCheckItem, nCheck); }
		//UINT EnableMenuItem(UINT nIDEnableItem, UINT nEnable) { ASSERT(::IsMenu(m_hMenu)); return ::EnableMenuItem(m_hMenu, nIDEnableItem, nEnable); }
		BOOL SetDefaultItem(UINT uItem, BOOL fByPos = FALSE) { ASSERT(::IsMenu(m_hMenu)); return ::SetMenuDefaultItem(m_hMenu, uItem, fByPos); }
		UINT GetDefaultItem(UINT gmdiFlags, BOOL fByPos = FALSE) { ASSERT(::IsMenu(m_hMenu)); return ::GetMenuDefaultItem(m_hMenu, fByPos, gmdiFlags); }
		//int GetMenuItemCount() const { ASSERT(::IsMenu(m_hMenu)); return ::GetMenuItemCount(m_hMenu); }
		//UINT GetMenuItemID(int nPos) const { ASSERT(::IsMenu(m_hMenu)); return ::GetMenuItemID(m_hMenu, nPos); }
		//UINT GetMenuState(UINT nID, UINT nFlags) const { ASSERT(::IsMenu(m_hMenu)); return ::GetMenuState(m_hMenu, nID, nFlags); }
		//int GetMenuString(_In_ UINT nIDItem, _Out_writes_z_(nMaxCount) LPTSTR lpString, _In_ int nMaxCount, _In_ UINT nFlags) const { ASSERT(::IsMenu(m_hMenu)); return ::GetMenuString(m_hMenu, nIDItem, lpString, nMaxCount, nFlags); }
		BOOL GetMenuItemInfo(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos)
		{
			ASSERT(::IsMenu(m_hMenu));
			return ::GetMenuItemInfo(m_hMenu, uItem, fByPos, lpMenuItemInfo);
		}
		BOOL SetMenuItemInfo(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos)
		{
			ASSERT(::IsMenu(m_hMenu));
			return ::SetMenuItemInfo(m_hMenu, uItem, fByPos, lpMenuItemInfo);
		}
		CMenuMTL* GetSubMenu(int nPos) { ASSERT(::IsMenu(m_hMenu)); return FromHandle(::GetSubMenu(m_hMenu, nPos)); }
		BOOL InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, LPCTSTR lpszNewItem) { ASSERT(::IsMenu(m_hMenu)); return ::InsertMenu(m_hMenu, nPosition, nFlags, nIDNewItem, lpszNewItem); }
		BOOL InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, CBitmap* pBmp)
		{
			ASSERT(::IsMenu(m_hMenu)); return ::InsertMenu(m_hMenu, nPosition, nFlags | MF_BITMAP, nIDNewItem, (LPCTSTR)pBmp->GetSafeHandle());
		}
		BOOL InsertMenuItem(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos)
		{
			ASSERT(::IsMenu(m_hMenu));
			return ::InsertMenuItem(m_hMenu, uItem, fByPos, lpMenuItemInfo);
		}
		BOOL ModifyMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, LPCTSTR lpszNewItem) { ASSERT(::IsMenu(m_hMenu)); return ::ModifyMenu(m_hMenu, nPosition, nFlags, nIDNewItem, lpszNewItem); }
		BOOL ModifyMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, CBitmap* pBmp)
		{
			ASSERT(::IsMenu(m_hMenu)); return ::ModifyMenu(m_hMenu, nPosition, nFlags | MF_BITMAP, nIDNewItem,
				(LPCTSTR)pBmp->GetSafeHandle());
		}
		BOOL RemoveMenu(UINT nPosition, UINT nFlags) { ASSERT(::IsMenu(m_hMenu)); return ::RemoveMenu(m_hMenu, nPosition, nFlags); }
		BOOL SetMenuItemBitmaps(UINT nPosition, UINT nFlags, CBitmap* pBmpUnchecked, CBitmap* pBmpChecked)
		{
			ASSERT(::IsMenu(m_hMenu)); return ::SetMenuItemBitmaps(m_hMenu, nPosition, nFlags,
				(HBITMAP)pBmpUnchecked->GetSafeHandle(), (HBITMAP)pBmpChecked->GetSafeHandle());
		}

		BOOL TrackPopupMenu(UINT nFlags, int x, int y, CWnd* pWnd, LPCRECT lpRect = 0) ;
			
		BOOL LoadMenu(LPCTSTR lpszResourceName) ;
		BOOL LoadMenu(UINT nIDResource) ; 
		BOOL LoadMenuIndirect(const void* lpMenuTemplate) { return Attach(::LoadMenuIndirect(lpMenuTemplate)); }

		// Win4
		//BOOL SetMenuContextHelpId(DWORD dwContextHelpId) { return ::SetMenuContextHelpId(m_hMenu, dwContextHelpId); }
		//DWORD GetMenuContextHelpId() const { return ::GetMenuContextHelpId(m_hMenu); }
		//BOOL CheckMenuRadioItem(UINT nIDFirst, UINT nIDLast, UINT nIDItem, UINT nFlags) { return ::CheckMenuRadioItem(m_hMenu, nIDFirst, nIDLast, nIDItem, nFlags); }

};

#define CMenu CMenuMTL


