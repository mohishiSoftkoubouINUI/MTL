



#define IMPLEMENT_HANDLE_CREATE(class_name, handle_name) \
	static void* classHandleFunc##class_name(HANDLE hHandle, void* pData) \
		{ if (pData == NULL) return (void*)(new class_name((handle_name)hHandle)) ; \
			delete (class_name*)pData ; return NULL ;} \
	static class_name* FromHandle(handle_name hHnd, class_name* pClass = NULL, HWND hWnd = (HWND)HANDLE_ADDRESS_INVALID, \
									HANDLE_FUNC_NEW pFuncNew = &class_name::classHandleFunc##class_name) ; \
	static class_name* FromHandlePermanent(handle_name hHnd) { return FromHandle(hHnd, NULL, NULL, NULL) ; } \
	static handle_name RemoveHandle(handle_name hHnd) ;

#define CLASS_HANDLE_CREATE(class_name, handle_name, class_base, class_ext) \
	_INLINE class_name* class_name::FromHandle(handle_name hHnd, class_name* pClass, HWND hWndOwner, HANDLE_FUNC_NEW pFuncNew) \
		{ return (class_name*)AfxGetApp()->FromHandle##class_ext(hHnd, (class_base*)pClass, hWndOwner, pFuncNew) ; } \
	_INLINE handle_name class_name::RemoveHandle(handle_name hHnd) \
		{ return (handle_name)AfxGetApp()->RemoveHandle##class_ext(hHnd); }


#define FROM_HANDLE_PWND(t, h) t::FromHandle(h, NULL, m_hWnd)


/*============================================================================*/
// CGdiObject abstract class for CDC SelectObject

class CGdiobj
{
	public:
		CGdiobj(HGDIOBJ hGdiobj = NULL) 	// must Create a derived class object
		{
			m_hGdiObject = hGdiobj;
		}

		~CGdiobj()
		{
			if (m_hGdiObject != NULL)
				DeleteObject();
		}

	public:
		// Attributes
		HGDIOBJ m_hGdiObject;                  // must be first data member

	public:
		operator HGDIOBJ() const { return m_hGdiObject; }

		BOOL Attach(HGDIOBJ hObject)
		{
			ASSERT(m_hGdiObject == NULL);      // only attach once, detach on destroy
			if (hObject == NULL)
				return FALSE;

			if (m_hGdiObject != NULL&& m_hGdiObject != hObject)
				::DeleteObject(m_hGdiObject);

			// remember early to avoid leak
			m_hGdiObject = hObject;
			return TRUE;
		}

		HGDIOBJ Detach()
		{
			HGDIOBJ hObject = m_hGdiObject;
			m_hGdiObject = NULL;
			return hObject;
		}

		BOOL DeleteObject()
		{
			if (m_hGdiObject == NULL)
				return FALSE;
			return ::DeleteObject(Detach());
		}

};


template<class C, class H, class B>
class ATL_NO_VTABLE CHandleWrapper : public B
{
	public:
		CHandleWrapper(H hHnd = NULL) : B(hHnd), m_bFromHandle(hHnd != NULL) { }

		virtual ~CHandleWrapper()
		{
			if (m_bFromHandle)
			{
				B::Detach();
				return ;
			}

			H hHandle = GetSafeHandle() ;
			if (hHandle != NULL)
				C::RemoveHandle(hHandle);
		}


		const BOOL m_bFromHandle;

	public:
		operator H() const
		{
			return this == NULL ? NULL : (H)(*(B*)this);
		}

		BOOL operator==(const C& rc) const { return ((H)rc) == (H)*this; }
		BOOL operator!=(const C& rc) const { return ((H)rc) != (H)*this; }


		BOOL Attach(H hObject)
		{
			ASSERT(GetSafeHandle() == NULL);      // only attach once, detach on destroy
			if (hObject == NULL)
			{
				return FALSE;
			}

			B::Attach(hObject);

			C::FromHandle(hObject, (C*)this, NULL);
			return TRUE;
		}

		H Detach()
		{
			H hObject = B::Detach();
			if (hObject != NULL)
			{
				C::RemoveHandle(hObject);
			}

			return hObject;
		}


		const H GetSafeHandle() const { return (this == NULL) ? NULL : (H)*this; }

};

class CGdiObject : public CHandleWrapper<CGdiObject, HGDIOBJ, CGdiobj>
{
	public:
		CGdiObject(HGDIOBJ hHnd = NULL) : CHandleWrapper<CGdiObject, HGDIOBJ, CGdiobj>(hHnd) { }

	public:
		IMPLEMENT_HANDLE_CREATE(CGdiObject, HGDIOBJ)

};


class CBitmapMTL : public CHandleWrapper<CBitmapMTL, HBITMAP, CBitmap>
{
	public:
		CBitmapMTL(HBITMAP hHnd = NULL) : CHandleWrapper<CBitmapMTL, HBITMAP, CBitmap>(hHnd) { }

	public:
		IMPLEMENT_HANDLE_CREATE(CBitmapMTL, HBITMAP)

};


class CBrushMTL : public CHandleWrapper<CBrushMTL, HBRUSH, CBrush>
{
	public:
		CBrushMTL(HBRUSH hHnd = NULL) : CHandleWrapper<CBrushMTL, HBRUSH, CBrush>(hHnd) { }

		CBrushMTL(COLORREF crColor)
		{
			CBrush::CreateSolidBrush(crColor);
		}

		CBrushMTL(int nIndex, COLORREF crColor)
		{
			CBrush::CreateHatchBrush(nIndex, crColor);
		}


	public:
		IMPLEMENT_HANDLE_CREATE(CBrushMTL, HBRUSH)

};


class CFontMTL : public CHandleWrapper<CFontMTL, HFONT, CFont>
{
	public:
		CFontMTL(HFONT hHnd = NULL) : CHandleWrapper<CFontMTL, HFONT, CFont>(hHnd) { }

	public:
		IMPLEMENT_HANDLE_CREATE(CFontMTL, HFONT)

};


class CPaletteMTL : public CHandleWrapper<CPaletteMTL, HPALETTE, CPalette>
{
	public:
		CPaletteMTL(HPALETTE hHnd = NULL) : CHandleWrapper<CPaletteMTL, HPALETTE, CPalette>(hHnd) { }

	public:
		IMPLEMENT_HANDLE_CREATE(CPaletteMTL, HPALETTE)

};


class CPenMTL : public CHandleWrapper<CPenMTL, HPEN, CPen>
{
	public:
		CPenMTL(HPEN hHnd = NULL) : CHandleWrapper<CPenMTL, HPEN, CPen>(hHnd) { }

		CPenMTL(int nPenStyle, int nWidth, COLORREF crColor)
		{
			if (!Attach(::CreatePen(nPenStyle, nWidth, crColor)))
				AfxThrowResourceException();
		}


	public:
		IMPLEMENT_HANDLE_CREATE(CPenMTL, HPEN)

};


class CRgnMTL : public CHandleWrapper<CRgnMTL, HRGN, CRgn>
{
	public:
		CRgnMTL(HRGN hHnd = NULL) : CHandleWrapper<CRgnMTL, HRGN, CRgn>(hHnd) { }

	public:
		IMPLEMENT_HANDLE_CREATE(CRgnMTL, HRGN)


	public:
		int CombineRgn(const CRgnMTL* pRgn1, const CRgnMTL* pRgn2, int nCombineMode)
		{
			ASSERT(m_hRgn != NULL);
			return ::CombineRgn((HRGN)m_hRgn, (HRGN)pRgn1->GetSafeHandle(), (HRGN)pRgn2->GetSafeHandle(), nCombineMode);
		}

		int CopyRgn(const CRgnMTL* pRgnSrc)
		{
			ASSERT(m_hRgn != NULL); return ::CombineRgn((HRGN)m_hRgn, (HRGN)pRgnSrc->GetSafeHandle(), NULL, RGN_COPY);
		}

		BOOL EqualRgn(const CRgnMTL* pRgn) const
		{
			ASSERT(m_hRgn != NULL); return ::EqualRgn((HRGN)m_hRgn, (HRGN)pRgn->GetSafeHandle());
		}

};



#if 1
#define CBitmap CBitmapMTL
#define CBrush CBrushMTL
#define CFont CFontMTL
#define CPalette CPaletteMTL
#define CPen CPenMTL
#define CRgn CRgnMTL
#endif




class CDCMTL : public CHandleWrapper<CDCMTL, HDC, CDC>
{
	public:
		CDCMTL(HDC hDC = NULL) : CHandleWrapper<CDCMTL, HDC, CDC>(hDC)
		{
			m_hAttribDC = NULL;
			m_bPrinting = FALSE;
		}


public:
		IMPLEMENT_HANDLE_CREATE(CDCMTL, HDC)


		HDC m_hAttribDC;    // The Attribute DC
		BOOL m_bPrinting;

	public:
		operator HDC() const
		{
			ASSERT(this != NULL && m_hDC != NULL);
			return this == NULL ? NULL : m_hDC;
		}


		BOOL CreateDC(LPCTSTR lpszDriverName, LPCTSTR lpszDeviceName, LPCTSTR lpszOutput, const void* lpInitData)
		{
			return Attach(::CreateDC(lpszDriverName, lpszDeviceName, lpszOutput, (const DEVMODE*)lpInitData));
		}

		BOOL CreateIC(LPCTSTR lpszDriverName, LPCTSTR lpszDeviceName, LPCTSTR lpszOutput, const void* lpInitData)
		{
			return Attach(::CreateIC(lpszDriverName, lpszDeviceName, lpszOutput, (const DEVMODE*)lpInitData));
		}

		HDC GetSafeHdc() const { return this == NULL ? NULL : m_hDC; }

		BOOL IsPrinting() const { return m_bPrinting; }

		// m_hAttribDC

		virtual void SetAttribDC(HDC hDC) { m_hAttribDC = hDC; }  // Set the Attribute DC
		virtual void ReleaseAttribDC() { m_hAttribDC = NULL; }    // Release the Attribute DC

		virtual int SaveDC()
		{
			ASSERT(m_hDC != NULL);
			int nRetVal = 0;
			if (m_hAttribDC != NULL)
				nRetVal = ::SaveDC(m_hAttribDC);
			if (m_hDC != m_hAttribDC && ::SaveDC(m_hDC) != 0)
				nRetVal = -1;   // -1 is the only valid restore value for complex DCs
			return nRetVal;
		}

		virtual BOOL RestoreDC(int nSavedDC)
		{
			// if two distinct DCs, nSavedDC can only be -1
			ASSERT(m_hDC != NULL);
			ASSERT(m_hDC == m_hAttribDC || nSavedDC == -1);

			BOOL bRetVal = TRUE;
			if (m_hDC != m_hAttribDC)
				bRetVal = ::RestoreDC(m_hDC, nSavedDC);
			if (m_hAttribDC != NULL)
				bRetVal = (bRetVal && ::RestoreDC(m_hAttribDC, nSavedDC));
			return bRetVal;
		}

		CGdiObject* SelectStockObject(int nIndex)
		{
			ASSERT(m_hDC != NULL);

			HGDIOBJ hObject = ::GetStockObject(nIndex);
			HGDIOBJ hOldObj = NULL;

			ASSERT(hObject != NULL);
			if (m_hDC != m_hAttribDC)
				hOldObj = ::SelectObject(m_hDC, hObject);
			if (m_hAttribDC != NULL)
				hOldObj = ::SelectObject(m_hAttribDC, hObject);
			return CGdiObject::FromHandle(hOldObj);
		}

		CPen* SelectObject(CPen* pPen)
		{
			ASSERT(m_hDC != NULL);
			HGDIOBJ hOldObj = NULL;

			if (m_hDC != m_hAttribDC)
				hOldObj = ::SelectObject(m_hDC, pPen->GetSafeHandle());
			if (m_hAttribDC != NULL)
				hOldObj = ::SelectObject(m_hAttribDC, pPen->GetSafeHandle());
			return (CPen*)CGdiObject::FromHandle(hOldObj);
		}

		CBrush* SelectObject(CBrush* pBrush)
		{
			ASSERT(m_hDC != NULL);
			HGDIOBJ hOldObj = NULL;

			if (m_hDC != m_hAttribDC)
				hOldObj = ::SelectObject(m_hDC, pBrush->GetSafeHandle());
			if (m_hAttribDC != NULL)
				hOldObj = ::SelectObject(m_hAttribDC, pBrush->GetSafeHandle());
			return (CBrush*)CGdiObject::FromHandle(hOldObj);
		}

		CFont* SelectObject(CFont* pFont)
		{
			ASSERT(m_hDC != NULL);
			HGDIOBJ hOldObj = NULL;

			if (m_hDC != m_hAttribDC)
				hOldObj = ::SelectObject(m_hDC, pFont->GetSafeHandle());
			if (m_hAttribDC != NULL)
				hOldObj = ::SelectObject(m_hAttribDC, pFont->GetSafeHandle());
			return (CFont*)CGdiObject::FromHandle(hOldObj);
		}

		int SelectObject(CRgn* pRgn)
		{
			ASSERT(m_hDC != NULL);
			int nRetVal = GDI_ERROR;

			if (m_hDC != m_hAttribDC)
				nRetVal = (int)(INT_PTR)::SelectObject(m_hDC, pRgn->GetSafeHandle());
			if (m_hAttribDC != NULL)
				nRetVal = (int)(INT_PTR)::SelectObject(m_hAttribDC, pRgn->GetSafeHandle());
			return nRetVal;
		}

		CPalette* SelectPalette(CPalette* pPalette, BOOL bForceBackground)
		{
			ASSERT(m_hDC != NULL);

			return (CPalette*)CGdiObject::FromHandle(::SelectPalette(m_hDC,
								(HPALETTE)pPalette->GetSafeHandle(), bForceBackground));
		}

		COLORREF SetBkColor(COLORREF crColor)
		{
			ASSERT(m_hDC != NULL);
			COLORREF crRetVal = CLR_INVALID;

			if (m_hDC != m_hAttribDC)
				crRetVal = ::SetBkColor(m_hDC, crColor);
			if (m_hAttribDC != NULL)
				crRetVal = ::SetBkColor(m_hAttribDC, crColor);
			return crRetVal;
		}

		int SetBkMode(int nBkMode)
		{
			ASSERT(m_hDC != NULL);
			int nRetVal = 0;

			if (m_hDC != m_hAttribDC)
				nRetVal = ::SetBkMode(m_hDC, nBkMode);
			if (m_hAttribDC != NULL)
				nRetVal = ::SetBkMode(m_hAttribDC, nBkMode);
			return nRetVal;
		}

		int SetPolyFillMode(int nPolyFillMode)
		{
			ASSERT(m_hDC != NULL);
			int nRetVal = 0;

			if (m_hDC != m_hAttribDC)
				nRetVal = ::SetPolyFillMode(m_hDC, nPolyFillMode);
			if (m_hAttribDC != NULL)
				nRetVal = ::SetPolyFillMode(m_hAttribDC, nPolyFillMode);
			return nRetVal;
		}

		int SetROP2(int nDrawMode)
		{
			ASSERT(m_hDC != NULL);
			int nRetVal = 0;

			if (m_hDC != m_hAttribDC)
				nRetVal = ::SetROP2(m_hDC, nDrawMode);
			if (m_hAttribDC != NULL)
				nRetVal = ::SetROP2(m_hAttribDC, nDrawMode);
			return nRetVal;
		}

		int SetStretchBltMode(int nStretchMode)
		{
			ASSERT(m_hDC != NULL);
			int nRetVal = 0;

			if (m_hDC != m_hAttribDC)
				nRetVal = ::SetStretchBltMode(m_hDC, nStretchMode);
			if (m_hAttribDC != NULL)
				nRetVal = ::SetStretchBltMode(m_hAttribDC, nStretchMode);
			return nRetVal;
		}

		COLORREF SetTextColor(COLORREF crColor)
		{
			ASSERT(m_hDC != NULL);
			COLORREF crRetVal = CLR_INVALID;

			if (m_hDC != m_hAttribDC)
				crRetVal = ::SetTextColor(m_hDC, crColor);
			if (m_hAttribDC != NULL)
				crRetVal = ::SetTextColor(m_hAttribDC, crColor);
			return crRetVal;
		}

		int SetGraphicsMode(int iMode)
		{
			ASSERT(m_hDC != NULL);
			int nRetVal = 0;

			if (m_hDC != m_hAttribDC)
			{
				nRetVal = ::SetGraphicsMode(m_hDC, iMode);
			}

			if (m_hAttribDC != NULL)
			{
				nRetVal = ::SetGraphicsMode(m_hAttribDC, iMode);
			}

			return nRetVal;
		}

		BOOL SetWorldTransform(const XFORM* pXform)
		{
			ASSERT(m_hDC != NULL);
			BOOL nRetVal = 0;

			if (m_hDC != m_hAttribDC)
			{
				nRetVal = ::SetWorldTransform(m_hDC, pXform);
			}

			if (m_hAttribDC != NULL)
			{
				nRetVal = ::SetWorldTransform(m_hAttribDC, pXform);
			}

			return nRetVal;
		}

		BOOL ModifyWorldTransform(const XFORM* pXform, DWORD iMode)
		{
			ASSERT(m_hDC != NULL);
			BOOL nRetVal = 0;

			if (m_hDC != m_hAttribDC)
			{
				nRetVal = ::ModifyWorldTransform(m_hDC, pXform, iMode);
			}

			if (m_hAttribDC != NULL)
			{
				nRetVal = ::ModifyWorldTransform(m_hAttribDC, pXform, iMode);
			}

			return nRetVal;
		}

		int SetMapMode(int nMapMode)
		{
			ASSERT(m_hDC != NULL);
			int nRetVal = 0;

			if (m_hDC != m_hAttribDC)
				nRetVal = ::SetMapMode(m_hDC, nMapMode);
			if (m_hAttribDC != NULL)
				nRetVal = ::SetMapMode(m_hAttribDC, nMapMode);
			return nRetVal;
		}

		CPoint SetViewportOrg(int x, int y)
		{
			ASSERT(m_hDC != NULL);
			CPoint point;

			if (m_hDC != m_hAttribDC)
				VERIFY(::SetViewportOrgEx(m_hDC, x, y, &point));
			if (m_hAttribDC != NULL)
				VERIFY(::SetViewportOrgEx(m_hAttribDC, x, y, &point));
			return point;
		}

		CPoint OffsetViewportOrg(int nWidth, int nHeight)
		{
			ASSERT(m_hDC != NULL);
			CPoint point;

			if (m_hDC != m_hAttribDC)
				VERIFY(::OffsetViewportOrgEx(m_hDC, nWidth, nHeight, &point));
			if (m_hAttribDC != NULL)
				VERIFY(::OffsetViewportOrgEx(m_hAttribDC, nWidth, nHeight, &point));
			return point;
		}

		CSize SetViewportExt(int x, int y)
		{
			ASSERT(m_hDC != NULL);
			CSize size;

			if (m_hDC != m_hAttribDC)
				VERIFY(::SetViewportExtEx(m_hDC, x, y, &size));
			if (m_hAttribDC != NULL)
				VERIFY(::SetViewportExtEx(m_hAttribDC, x, y, &size));
			return size;
		}

		CSize ScaleViewportExt(int xNum, int xDenom, int yNum, int yDenom)
		{
			ASSERT(m_hDC != NULL);
			CSize size;

			if (m_hDC != m_hAttribDC)
				VERIFY(::ScaleViewportExtEx(m_hDC, xNum, xDenom, yNum, yDenom, &size));
			if (m_hAttribDC != NULL)
				VERIFY(::ScaleViewportExtEx(m_hAttribDC, xNum, xDenom, yNum, yDenom, &size));
			return size;
		}

		CPoint SetWindowOrg(int x, int y)
		{
			ASSERT(m_hDC != NULL);
			CPoint point;

			if (m_hDC != m_hAttribDC)
				VERIFY(::SetWindowOrgEx(m_hDC, x, y, &point));
			if (m_hAttribDC != NULL)
				VERIFY(::SetWindowOrgEx(m_hAttribDC, x, y, &point));
			return point;
		}

		CPoint OffsetWindowOrg(int nWidth, int nHeight)
		{
			ASSERT(m_hDC != NULL);
			CPoint point;

			if (m_hDC != m_hAttribDC)
				VERIFY(::OffsetWindowOrgEx(m_hDC, nWidth, nHeight, &point));
			if (m_hAttribDC != NULL)
				VERIFY(::OffsetWindowOrgEx(m_hAttribDC, nWidth, nHeight, &point));
			return point;
		}

		CSize SetWindowExt(int x, int y)
		{
			ASSERT(m_hDC != NULL);
			CSize size;

			if (m_hDC != m_hAttribDC)
				VERIFY(::SetWindowExtEx(m_hDC, x, y, &size));
			if (m_hAttribDC != NULL)
				VERIFY(::SetWindowExtEx(m_hAttribDC, x, y, &size));
			return size;
		}

		CSize ScaleWindowExt(int xNum, int xDenom, int yNum, int yDenom)
		{
			ASSERT(m_hDC != NULL);
			CSize size;

			if (m_hDC != m_hAttribDC)
				VERIFY(::ScaleWindowExtEx(m_hDC, xNum, xDenom, yNum, yDenom, &size));
			if (m_hAttribDC != NULL)
				VERIFY(::ScaleWindowExtEx(m_hAttribDC, xNum, xDenom, yNum, yDenom, &size));
			return size;
		}

		int SelectClipRgn(CRgn* pRgn)
		{
			ASSERT(m_hDC != NULL);
			int nRetVal = ERROR;

			if (m_hDC != m_hAttribDC)
				nRetVal = ::SelectClipRgn(m_hDC, (HRGN)pRgn->GetSafeHandle());
			if (m_hAttribDC != NULL)
				nRetVal = ::SelectClipRgn(m_hAttribDC, (HRGN)pRgn->GetSafeHandle());
			return nRetVal;
		}

		int ExcludeClipRect(int x1, int y1, int x2, int y2)
		{
			ASSERT(m_hDC != NULL);
			int nRetVal = ERROR;

			if (m_hDC != m_hAttribDC)
				nRetVal = ::ExcludeClipRect(m_hDC, x1, y1, x2, y2);
			if (m_hAttribDC != NULL)
				nRetVal = ::ExcludeClipRect(m_hAttribDC, x1, y1, x2, y2);
			return nRetVal;
		}

		int ExcludeClipRect(LPCRECT lpRect)
		{
			ASSERT(m_hDC != NULL);
			int nRetVal = ERROR;

			if (m_hDC != m_hAttribDC)
				nRetVal = ::ExcludeClipRect(m_hDC, lpRect->left, lpRect->top,
				lpRect->right, lpRect->bottom);
			if (m_hAttribDC != NULL)
				nRetVal = ::ExcludeClipRect(m_hAttribDC, lpRect->left, lpRect->top,
				lpRect->right, lpRect->bottom);
			return nRetVal;
		}

		int IntersectClipRect(int x1, int y1, int x2, int y2)
		{
			ASSERT(m_hDC != NULL);
			int nRetVal = ERROR;

			if (m_hDC != m_hAttribDC)
				nRetVal = ::IntersectClipRect(m_hDC, x1, y1, x2, y2);
			if (m_hAttribDC != NULL)
				nRetVal = ::IntersectClipRect(m_hAttribDC, x1, y1, x2, y2);
			return nRetVal;
		}

		int IntersectClipRect(LPCRECT lpRect)
		{
			ASSERT(m_hDC != NULL);
			int nRetVal = ERROR;

			if (m_hDC != m_hAttribDC)
				nRetVal = ::IntersectClipRect(m_hDC, lpRect->left, lpRect->top,
				lpRect->right, lpRect->bottom);
			if (m_hAttribDC != NULL)
				nRetVal = ::IntersectClipRect(m_hAttribDC, lpRect->left, lpRect->top,
				lpRect->right, lpRect->bottom);
			return nRetVal;
		}

		int OffsetClipRgn(int x, int y)
		{
			ASSERT(m_hDC != NULL);
			int nRetVal = ERROR;

			if (m_hDC != m_hAttribDC)
				nRetVal = ::OffsetClipRgn(m_hDC, x, y);
			if (m_hAttribDC != NULL)
				nRetVal = ::OffsetClipRgn(m_hAttribDC, x, y);
			return nRetVal;
		}

		int OffsetClipRgn(SIZE size)
		{
			ASSERT(m_hDC != NULL);
			int nRetVal = ERROR;

			if (m_hDC != m_hAttribDC)
				nRetVal = ::OffsetClipRgn(m_hDC, size.cx, size.cy);
			if (m_hAttribDC != NULL)
				nRetVal = ::OffsetClipRgn(m_hAttribDC, size.cx, size.cy);
			return nRetVal;
		}

		CPoint MoveTo(int x, int y)
		{
			ASSERT(m_hDC != NULL);
			CPoint point;

			if (m_hDC != m_hAttribDC)
				VERIFY(::MoveToEx(m_hDC, x, y, &point));
			if (m_hAttribDC != NULL)
				VERIFY(::MoveToEx(m_hAttribDC, x, y, &point));
			return point;
		}

		CPoint MoveTo(POINT point) { ASSERT(m_hDC != NULL); return MoveTo(point.x, point.y); }

		BOOL LineTo(int x, int y)
		{
			ASSERT(m_hDC != NULL);
			if (m_hAttribDC != NULL && m_hDC != m_hAttribDC)
				::MoveToEx(m_hAttribDC, x, y, NULL);
			return ::LineTo(m_hDC, x, y);
		}

		BOOL LineTo(POINT point) { ASSERT(m_hDC != NULL); return LineTo(point.x, point.y); }

		UINT SetTextAlign(UINT nFlags)
		{
			ASSERT(m_hDC != NULL);
			UINT nRetVal = GDI_ERROR;

			if (m_hDC != m_hAttribDC)
				::SetTextAlign(m_hDC, nFlags);
			if (m_hAttribDC != NULL)
				nRetVal = ::SetTextAlign(m_hAttribDC, nFlags);
			return nRetVal;
		}

		int SetTextJustification(int nBreakExtra, int nBreakCount)
		{
			ASSERT(m_hDC != NULL);
			int nRetVal = 0;

			if (m_hDC != m_hAttribDC)
				nRetVal = ::SetTextJustification(m_hDC, nBreakExtra, nBreakCount);
			if (m_hAttribDC != NULL)
				nRetVal = ::SetTextJustification(m_hAttribDC, nBreakExtra, nBreakCount);
			return nRetVal;
		}

		int SetTextCharacterExtra(int nCharExtra)
		{
			ASSERT(m_hDC != NULL);
			int nRetVal = 0x8000000;
			if (m_hDC != m_hAttribDC)
				nRetVal = ::SetTextCharacterExtra(m_hDC, nCharExtra);
			if (m_hAttribDC != NULL)
				nRetVal = ::SetTextCharacterExtra(m_hAttribDC, nCharExtra);
			return nRetVal;
		}

		DWORD SetMapperFlags(DWORD dwFlag)
		{
			ASSERT(m_hDC != NULL);
			DWORD dwRetVal = GDI_ERROR;
			if (m_hDC != m_hAttribDC)
				dwRetVal = ::SetMapperFlags(m_hDC, dwFlag);
			if (m_hAttribDC != NULL)
				dwRetVal = ::SetMapperFlags(m_hAttribDC, dwFlag);
			return dwRetVal;
		}

		BOOL ArcTo(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
		{
			ASSERT(m_hDC != NULL);
			BOOL bResult = ::ArcTo(m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
			if (m_hDC != m_hAttribDC)
			{
				CPoint pt;
				VERIFY(::GetCurrentPositionEx(m_hDC, &pt));
				VERIFY(::MoveToEx(m_hAttribDC, pt.x, pt.y, NULL));
			}
			return bResult;
		}

		int SetArcDirection(int nArcDirection)
		{
			ASSERT(m_hDC != NULL);
			int nResult = 0;
			if (m_hDC != m_hAttribDC)
				nResult = ::SetArcDirection(m_hDC, nArcDirection);
			if (m_hAttribDC != NULL)
				nResult = ::SetArcDirection(m_hAttribDC, nArcDirection);
			return nResult;
		}

		BOOL PolyDraw(const POINT* lpPoints, const BYTE* lpTypes, int nCount)
		{
			ASSERT(m_hDC != NULL);
			BOOL bResult = ::PolyDraw(m_hDC, lpPoints, lpTypes, nCount);
			if (m_hDC != m_hAttribDC)
			{
				CPoint pt;
				VERIFY(::GetCurrentPositionEx(m_hDC, &pt));
				VERIFY(::MoveToEx(m_hAttribDC, pt.x, pt.y, NULL));
			}
			return bResult;
		}

		BOOL PolylineTo(const POINT* lpPoints, int nCount)
		{
			ASSERT(m_hDC != NULL);
			BOOL bResult = ::PolylineTo(m_hDC, lpPoints, nCount);
			if (m_hDC != m_hAttribDC)
			{
				CPoint pt;
				VERIFY(::GetCurrentPositionEx(m_hDC, &pt));
				VERIFY(::MoveToEx(m_hAttribDC, pt.x, pt.y, NULL));
			}
			return bResult;
		}

		BOOL SetColorAdjustment(const COLORADJUSTMENT* lpColorAdjust)
		{
			ASSERT(m_hDC != NULL);
			BOOL bResult = FALSE;
			if (m_hDC != m_hAttribDC)
				bResult = ::SetColorAdjustment(m_hDC, lpColorAdjust);
			if (m_hAttribDC != NULL)
				bResult = ::SetColorAdjustment(m_hAttribDC, lpColorAdjust);
			return bResult;
		}

		BOOL PolyBezierTo(const POINT* lpPoints, int nCount)
		{
			ASSERT(m_hDC != NULL);
			BOOL bResult = ::PolyBezierTo(m_hDC, lpPoints, nCount);
			if (m_hDC != m_hAttribDC)
			{
				CPoint pt;
				VERIFY(::GetCurrentPositionEx(m_hDC, &pt));
				VERIFY(::MoveToEx(m_hAttribDC, pt.x, pt.y, NULL));
			}
			return bResult;
		}

		BOOL SelectClipPath(int nMode)
		{
			ASSERT(m_hDC != NULL);

			// output DC always holds the current path
			if (!::SelectClipPath(m_hDC, nMode))
				return FALSE;

			// transfer clipping region into the attribute DC
			BOOL bResult = TRUE;
			if (m_hDC != m_hAttribDC)
			{
				HRGN hRgn = ::CreateRectRgn(0, 0, 0, 0);
				if (::GetClipRgn(m_hDC, hRgn) < 0 || !::SelectClipRgn(m_hAttribDC, hRgn))
				{
					TRACE(traceAppMsg, 0, "Error: unable to transfer clip region in SelectClipPath!\n");
					bResult = FALSE;
				}
				DeleteObject(hRgn);
			}
			return bResult;
		}

		int SelectClipRgn(CRgn* pRgn, int nMode)
		{
			ASSERT(m_hDC != NULL);
			int nRetVal = ERROR;
			if (m_hDC != m_hAttribDC)
				nRetVal = ::ExtSelectClipRgn(m_hDC, (HRGN)pRgn->GetSafeHandle(), nMode);
			if (m_hAttribDC != NULL)
				nRetVal = ::ExtSelectClipRgn(m_hAttribDC, (HRGN)pRgn->GetSafeHandle(), nMode);
			return nRetVal;
		}


		// m_hDC


		// Escape helpers
		int StartDoc(LPDOCINFO lpDocInfo) { ASSERT(m_hDC != NULL); return ::StartDoc(m_hDC, lpDocInfo); }
		int StartPage() { ASSERT(m_hDC != NULL); return ::StartPage(m_hDC); }
		int EndPage() { ASSERT(m_hDC != NULL); return ::EndPage(m_hDC); }
		int SetAbortProc(BOOL(CALLBACK* lpfn)(HDC, int)) { ASSERT(m_hDC != NULL); return ::SetAbortProc(m_hDC, (ABORTPROC)lpfn); }
		int AbortDoc() { ASSERT(m_hDC != NULL); return ::AbortDoc(m_hDC); }
		int EndDoc() { ASSERT(m_hDC != NULL); return ::EndDoc(m_hDC); }

		int StartDoc(LPCTSTR lpszDocName)
		{
			DOCINFO di;
			memset(&di, 0, sizeof(DOCINFO));
			di.cbSize = sizeof(DOCINFO);
			di.lpszDocName = lpszDocName;
			return StartDoc(&di);
		}



		void DrawDragRect(LPCRECT lpRect, SIZE size, LPCRECT lpRectLast, SIZE sizeLast, CBrush* pBrush, CBrush* pBrushLast)
		{
			// first, determine the update region and select it
			CRgn rgnNew;
			CRgn rgnOutside, rgnInside;
			rgnOutside.CreateRectRgnIndirect(lpRect);
			CRect rect = *lpRect;
			rect.InflateRect(-size.cx, -size.cy);
			rect.IntersectRect(rect, lpRect);
			rgnInside.CreateRectRgnIndirect(rect);
			rgnNew.CreateRectRgn(0, 0, 0, 0);
			rgnNew.CombineRgn(&rgnOutside, &rgnInside, RGN_XOR);

			CBrush* pBrushOld = NULL;
			if (pBrush == NULL)
				pBrush = GetHalftoneBrush();

			ENSURE(pBrush);

			if (pBrushLast == NULL)
				pBrushLast = pBrush;

			CRgn rgnLast, rgnUpdate;
			if (lpRectLast != NULL)
			{
				// find difference between new region and old region
				rgnLast.CreateRectRgn(0, 0, 0, 0);
				rgnOutside.SetRectRgn(lpRectLast);
				rect = *lpRectLast;
				rect.InflateRect(-sizeLast.cx, -sizeLast.cy);
				rect.IntersectRect(rect, lpRectLast);
				rgnInside.SetRectRgn(rect);
				rgnLast.CombineRgn(&rgnOutside, &rgnInside, RGN_XOR);

				// only diff them if brushes are the same
				if (pBrush->m_hBrush == pBrushLast->m_hBrush)
				{
					rgnUpdate.CreateRectRgn(0, 0, 0, 0);
					rgnUpdate.CombineRgn(&rgnLast, &rgnNew, RGN_XOR);
				}
			}
			if (pBrush->m_hBrush != pBrushLast->m_hBrush && lpRectLast != NULL)
			{
				// brushes are different -- erase old region first
				SelectClipRgn(&rgnLast);
				GetClipBox(&rect);
				pBrushOld = SelectObject(pBrushLast);
				PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATINVERT);
				SelectObject(pBrushOld);
				pBrushOld = NULL;
			}

			// draw into the update/new region
			SelectClipRgn(rgnUpdate.m_hRgn != NULL ? &rgnUpdate : &rgnNew);
			GetClipBox(&rect);
			pBrushOld = SelectObject(pBrush);
			PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATINVERT);

			// cleanup DC
			if (pBrushOld != NULL)
				SelectObject(pBrushOld);
			SelectClipRgn(NULL);
		}

		static CBrush* GetHalftoneBrush()
		{
			HBRUSH hHalftoneBrush = CAfx::HalftoneBrush();

			return CBrush::FromHandle(hHalftoneBrush);
		}

		CSize GetTextExtent(const CString& str) const
		{
			SIZE size;
			VERIFY(::GetTextExtentPoint32(m_hDC, str, (int)str.GetLength(), &size));
			return size;
		}




		void SetOutputDC(HDC hDC) { m_hDC = hDC; }  // Set the Output DC

		void ReleaseOutputDC() { m_hDC = NULL; }     // Release the Output DC


		CGdiObject* SelectGdiObject(HDC hDC, HGDIOBJ h)
		{
			return CGdiObject::FromHandle(::SelectObject(hDC, h));
		}


#ifndef _MTL_ATL3
		DWORD GetLayout() const
		{
			ASSERT(m_hDC != NULL);

			DWORD dwGetLayout = ::GetLayout(m_hDC);

			return dwGetLayout;
		}

		DWORD SetLayout(DWORD dwSetLayout)
		{
			ASSERT(m_hDC != NULL);

			DWORD dwGetLayout = ::SetLayout(m_hDC, dwSetLayout);

			return dwGetLayout;
		}
#else

#pragma warning(disable:4191)

		typedef DWORD (CALLBACK* AFX_GDIGETLAYOUTPROC)(HDC);
		typedef DWORD (CALLBACK* AFX_GDISETLAYOUTPROC)(HDC, DWORD);

		DWORD GetLayout() const
		{
			ASSERT(m_hDC != NULL);

			HINSTANCE hInst = ::GetModuleHandleA("GDI32.DLL");
			ASSERT(hInst != NULL);

			DWORD dwGetLayout = LAYOUT_LTR;

			AFX_GDIGETLAYOUTPROC pfn = (AFX_GDIGETLAYOUTPROC) GetProcAddress(hInst, "GetLayout");

			// if they API is available, just call it. If it is not
			// available, indicate an error.

			if (pfn != NULL)
				dwGetLayout = (*pfn)(m_hDC);
			else
			{
				dwGetLayout = GDI_ERROR;
				SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
			}

			return dwGetLayout;
		}

		DWORD SetLayout(DWORD dwSetLayout)
		{
			ASSERT(m_hDC != NULL);

			HINSTANCE hInst = ::GetModuleHandleA("GDI32.DLL");
			ASSERT(hInst != NULL);

			DWORD dwGetLayout = LAYOUT_LTR;

			AFX_GDISETLAYOUTPROC pfn = (AFX_GDISETLAYOUTPROC) GetProcAddress(hInst, "SetLayout");

			// If the API is availalbe, just call it. If it's not available,
			// setting anything other than LAYOUT_LTR is an error.

			if (pfn != NULL)
				dwGetLayout = (*pfn)(m_hDC, dwSetLayout);
			else if (dwSetLayout != LAYOUT_LTR)
			{
				dwGetLayout = GDI_ERROR;
				SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
			}

			return dwGetLayout;
		}
#endif

		/////////////////////////////////////////////////////////////////////////////
		// Advanced Win32 GDI functions


		CWnd* GetWindow() const ;
		int ExcludeUpdateRgn(CWnd* pWnd) ;

		BOOL CreateCompatibleDC(CDCMTL* pDC) { return Attach(::CreateCompatibleDC(pDC->GetSafeHdc())); }

		CPoint GetBrushOrg() const
		{
			ASSERT(m_hDC != NULL);
			POINT point;
			VERIFY(::GetBrushOrgEx(m_hDC, &point));
			return point;
		}
		CPoint SetBrushOrg(int x, int y)
		{
			ASSERT(m_hDC != NULL);
			POINT point;
			VERIFY(::SetBrushOrgEx(m_hDC, x, y, &point));
			return point;
		}
		CPoint SetBrushOrg(POINT point)
		{
			ASSERT(m_hDC != NULL);
			VERIFY(::SetBrushOrgEx(m_hDC, point.x, point.y, &point));
			return point;
		}


		CBitmap* SelectObject(CBitmap* pBitmap) { ASSERT(m_hDC != NULL); return (CBitmap*) SelectGdiObject(m_hDC, pBitmap->GetSafeHandle()); }
		CGdiObject* SelectObject(CGdiObject* pObject) { ASSERT(m_hDC != NULL); return SelectGdiObject(m_hDC, pObject->GetSafeHandle()); }
		HGDIOBJ SelectObject(HGDIOBJ hObject) // Safe for NULL handles
		{ return (hObject != NULL) ? ::SelectObject(m_hDC, hObject) : NULL; }


		CPoint GetViewportOrg() const
		{
			ASSERT(m_hDC != NULL);
			POINT point;
			VERIFY(::GetViewportOrgEx(m_hDC, &point));
			return point;
		}
		CSize GetViewportExt() const
		{
			ASSERT(m_hDC != NULL);
			SIZE size;
			VERIFY(::GetViewportExtEx(m_hDC, &size));
			return size;
		}
		CPoint GetWindowOrg() const
		{
			ASSERT(m_hDC != NULL);
			POINT point;
			VERIFY(::GetWindowOrgEx(m_hDC, &point));
			return point;
		}
		CSize GetWindowExt() const
		{
			ASSERT(m_hDC != NULL);
			SIZE size;
			VERIFY(::GetWindowExtEx(m_hDC, &size));
			return size;
		}

		// non-virtual helpers calling virtual mapping functions
		CPoint SetViewportOrg(POINT point) { ASSERT(m_hDC != NULL); return SetViewportOrg(point.x, point.y); }
		CSize SetViewportExt(SIZE size) { ASSERT(m_hDC != NULL); return SetViewportExt(size.cx, size.cy); }
		CPoint SetWindowOrg(POINT point) { ASSERT(m_hDC != NULL); return SetWindowOrg(point.x, point.y); }
		CSize SetWindowExt(SIZE size) { ASSERT(m_hDC != NULL); return SetWindowExt(size.cx, size.cy); }


		BOOL FillRgn(CRgn* pRgn, CBrush* pBrush) { ASSERT(m_hDC != NULL); return ::FillRgn(m_hDC, (HRGN)pRgn->GetSafeHandle(), (HBRUSH)pBrush->GetSafeHandle()); }
		BOOL FrameRgn(CRgn* pRgn, CBrush* pBrush, int nWidth, int nHeight)
		{ ASSERT(m_hDC != NULL); return ::FrameRgn(m_hDC, (HRGN)pRgn->GetSafeHandle(), (HBRUSH)pBrush->GetSafeHandle(), nWidth, nHeight); }
		BOOL InvertRgn(CRgn* pRgn) { ASSERT(m_hDC != NULL); return ::InvertRgn(m_hDC, (HRGN)pRgn->GetSafeHandle()); }
		BOOL PaintRgn(CRgn* pRgn) { ASSERT(m_hDC != NULL); return ::PaintRgn(m_hDC, (HRGN)pRgn->GetSafeHandle()); }

		BOOL BitBlt(int x, int y, int nWidth, int nHeight, CDCMTL* pSrcDC, int xSrc, int ySrc, DWORD dwRop)
		{
			ASSERT(m_hDC != NULL); return ::BitBlt(m_hDC, x, y, nWidth, nHeight,
												pSrcDC->GetSafeHdc(), xSrc, ySrc, dwRop);
		}
		BOOL StretchBlt(int x, int y, int nWidth, int nHeight, CDCMTL* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop)
		{
			ASSERT(m_hDC != NULL); return ::StretchBlt(m_hDC, x, y, nWidth, nHeight,
												pSrcDC->GetSafeHdc(), xSrc, ySrc, nSrcWidth, nSrcHeight, dwRop);
		}

		CPoint GetCurrentPosition() const
		{
			ASSERT(m_hDC != NULL);
			POINT point;
			VERIFY(::GetCurrentPositionEx(m_hDC, &point));
			return point;
		}

		void FillRect(LPCRECT lpRect, CBrush* pBrush) { ASSERT(m_hDC != NULL); ::FillRect(m_hDC, lpRect, (HBRUSH)pBrush->GetSafeHandle()); }
		void FrameRect(LPCRECT lpRect, CBrush* pBrush) { ASSERT(m_hDC != NULL); ::FrameRect(m_hDC, lpRect, (HBRUSH)pBrush->GetSafeHandle()); }

		BOOL DrawState(CPoint pt, CSize size, HBITMAP hBitmap, UINT nFlags, HBRUSH hBrush)
		{ ASSERT(m_hDC != NULL); return ::DrawState(m_hDC, hBrush, 
													NULL, (LPARAM)hBitmap, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_BITMAP); }
		BOOL DrawState(CPoint pt, CSize size, CBitmap* pBitmap, UINT nFlags, CBrush* pBrush)
		{ ASSERT(m_hDC != NULL); return ::DrawState(m_hDC, (HBRUSH)pBrush->GetSafeHandle(),
													NULL, (LPARAM)pBitmap->GetSafeHandle(), 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_BITMAP); }
		BOOL DrawState(CPoint pt, CSize size, HICON hIcon, UINT nFlags, HBRUSH hBrush)
		{ ASSERT(m_hDC != NULL); return ::DrawState(m_hDC, hBrush, NULL,
													(LPARAM)hIcon, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_ICON); }
		BOOL DrawState(CPoint pt, CSize size, HICON hIcon, UINT nFlags, CBrush* pBrush)
		{ ASSERT(m_hDC != NULL); return ::DrawState(m_hDC, (HBRUSH)pBrush->GetSafeHandle(), NULL,
													(LPARAM)hIcon, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_ICON); }
		BOOL DrawState(CPoint pt, CSize size, LPCTSTR lpszText, UINT nFlags, BOOL bPrefixText, int nTextLen, HBRUSH hBrush)
		{ ASSERT(m_hDC != NULL); return ::DrawState(m_hDC, hBrush,
													NULL, (LPARAM)lpszText, (WPARAM)nTextLen, pt.x, pt.y, size.cx, size.cy, nFlags|(bPrefixText ? DST_PREFIXTEXT : DST_TEXT)); }
		BOOL DrawState(CPoint pt, CSize size, LPCTSTR lpszText, UINT nFlags, BOOL bPrefixText, int nTextLen, CBrush* pBrush)
		{ ASSERT(m_hDC != NULL); return ::DrawState(m_hDC, (HBRUSH)pBrush->GetSafeHandle(),
													NULL, (LPARAM)lpszText, (WPARAM)nTextLen, pt.x, pt.y, size.cx, size.cy, nFlags|(bPrefixText ? DST_PREFIXTEXT : DST_TEXT)); }
		BOOL DrawState(CPoint pt, CSize size, DRAWSTATEPROC lpDrawProc, LPARAM lData, UINT nFlags, HBRUSH hBrush)
		{ ASSERT(m_hDC != NULL); return ::DrawState(m_hDC, hBrush,
													lpDrawProc, lData, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_COMPLEX); }
		BOOL DrawState(CPoint pt, CSize size, DRAWSTATEPROC lpDrawProc, LPARAM lData, UINT nFlags, CBrush* pBrush)
		{ ASSERT(m_hDC != NULL); return ::DrawState(m_hDC, (HBRUSH)pBrush->GetSafeHandle(),
													lpDrawProc, lData, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_COMPLEX); }


		virtual BOOL TextOut(int x, int y, LPCTSTR lpszString, int nCount) { ASSERT(m_hDC != NULL); return ::TextOut(m_hDC, x, y, lpszString, nCount); } // call virtual
		BOOL TextOut(int x, int y, const CString& str) { ASSERT(m_hDC != NULL); return ::TextOut(m_hDC, x, y, (LPCTSTR)str, (int)str.GetLength()); }
				
		BOOL ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect, const CString& str, LPINT lpDxWidths)
		{ ASSERT(m_hDC != NULL); return ::ExtTextOut(m_hDC, x, y, nOptions, lpRect,
													str, (UINT)str.GetLength(), lpDxWidths); }
		CSize TabbedTextOut(int x, int y, LPCTSTR lpszString, int nCount, int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin)
		{ ASSERT(m_hDC != NULL); return ::TabbedTextOut(m_hDC, x, y, lpszString, nCount,
														nTabPositions, lpnTabStopPositions, nTabOrigin); }
		CSize TabbedTextOut(int x, int y, const CString& str, int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin)
		{ ASSERT(m_hDC != NULL); return ::TabbedTextOut(m_hDC, x, y, str, (int)str.GetLength(),
														nTabPositions, lpnTabStopPositions, nTabOrigin); }
		int DrawText(const CString& str, LPRECT lpRect, UINT nFormat)
		{ ASSERT(m_hDC != NULL);
			// these flags would modify the string
			ASSERT((nFormat & (DT_END_ELLIPSIS | DT_MODIFYSTRING)) != (DT_END_ELLIPSIS | DT_MODIFYSTRING));
			ASSERT((nFormat & (DT_PATH_ELLIPSIS | DT_MODIFYSTRING)) != (DT_PATH_ELLIPSIS | DT_MODIFYSTRING));
			return ::DrawText(m_hDC, (LPCTSTR)str, (int)str.GetLength(), lpRect, nFormat); }

		int DrawTextEx(const CString& str, LPRECT lpRect, UINT nFormat, LPDRAWTEXTPARAMS lpDTParams)
		{ ASSERT(m_hDC != NULL);
			// these flags would modify the string
			ASSERT((nFormat & (DT_END_ELLIPSIS | DT_MODIFYSTRING)) != (DT_END_ELLIPSIS | DT_MODIFYSTRING));
			ASSERT((nFormat & (DT_PATH_ELLIPSIS | DT_MODIFYSTRING)) != (DT_PATH_ELLIPSIS | DT_MODIFYSTRING));
			return ::DrawTextEx(m_hDC, (LPTSTR)((LPCTSTR)str), (int)str.GetLength(), lpRect, nFormat, lpDTParams); }

		int GetTextFace(int nCount, LPTSTR lpszFacename) const 
		{
			ASSERT(m_hAttribDC != NULL); return ::GetTextFace(m_hAttribDC, nCount, lpszFacename);
		}

		int GetTextFace(CString& rString) const
		{
			ASSERT(m_hAttribDC != NULL); int nResult = ::GetTextFace(m_hAttribDC, 256, rString.GetBuffer(256)); rString.ReleaseBuffer();
			return nResult;
		}

		CSize GetTextExtent(LPCTSTR lpszString, int nCount) const
		{
			ASSERT(m_hDC != NULL);
			SIZE size;
			VERIFY(::GetTextExtentPoint32(m_hDC, lpszString, nCount, &size));
			return size;
		}

		CSize GetOutputTextExtent(LPCTSTR lpszString, int nCount) const
		{
			ASSERT(m_hDC != NULL);
			SIZE size;
			VERIFY(::GetTextExtentPoint32(m_hDC, lpszString, nCount, &size));
			return size;
		}
		CSize GetOutputTextExtent(const CString& str) const
		{
			ASSERT(m_hDC != NULL);
			SIZE size;
			VERIFY(::GetTextExtentPoint32(m_hDC, str, (int)str.GetLength(), &size));
			return size;
		}

		CSize GetTabbedTextExtent(LPCTSTR lpszString, int nCount, int nTabPositions, LPINT lpnTabStopPositions) const
		{ ASSERT(m_hDC != NULL); return ::GetTabbedTextExtent(m_hDC, lpszString, nCount,nTabPositions, lpnTabStopPositions); }
		 CSize GetTabbedTextExtent(const CString& str, int nTabPositions, LPINT lpnTabStopPositions) const
		{ ASSERT(m_hDC != NULL); return ::GetTabbedTextExtent(m_hDC, str, (int)str.GetLength(), nTabPositions, lpnTabStopPositions); }
		CSize GetOutputTabbedTextExtent(LPCTSTR lpszString, int nCount, int nTabPositions, LPINT lpnTabStopPositions) const
		{ ASSERT(m_hDC != NULL); return ::GetTabbedTextExtent(m_hDC, lpszString, nCount, nTabPositions, lpnTabStopPositions); }
		 CSize GetOutputTabbedTextExtent(const CString& str, int nTabPositions, LPINT lpnTabStopPositions) const
		{ ASSERT(m_hDC != NULL); return ::GetTabbedTextExtent(m_hDC, str, (int)str.GetLength(), nTabPositions, lpnTabStopPositions); }
		BOOL GrayString(CBrush* pBrush, BOOL (CALLBACK* lpfnOutput)(HDC, LPARAM, int), LPARAM lpData, int nCount, int x, int y, int nWidth, int nHeight)
		{ ASSERT(m_hDC != NULL); return ::GrayString(m_hDC, (HBRUSH)pBrush->GetSafeHandle(),
			(GRAYSTRINGPROC)lpfnOutput, lpData, nCount, x, y, nWidth, nHeight); }

		BOOL GetOutputCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const { ASSERT(m_hDC != NULL); return ::GetCharWidth(m_hDC, nFirstChar, nLastChar, lpBuffer); }
		DWORD GetFontLanguageInfo() const { ASSERT(m_hDC != NULL); return ::GetFontLanguageInfo(m_hDC); }

		DWORD GetCharacterPlacement(LPCTSTR lpString, int nCount, int nMaxExtent, LPGCP_RESULTS lpResults, DWORD dwFlags) const { ASSERT(m_hDC != NULL); return ::GetCharacterPlacement(m_hDC, lpString, nCount, nMaxExtent, lpResults, dwFlags); }
		DWORD GetCharacterPlacement(CString& str, int nMaxExtent, LPGCP_RESULTS lpResults, DWORD dwFlags) const { ASSERT(m_hDC != NULL); return ::GetCharacterPlacement(m_hDC, (LPCTSTR)str, str.GetLength(), nMaxExtent, lpResults, dwFlags); }


		CSize GetAspectRatioFilter() const
		{
			ASSERT(m_hDC != NULL);
			SIZE size;
			VERIFY(::GetAspectRatioFilterEx(m_hDC, &size));
			return size;
		}
		BOOL ScrollDC(int dx, int dy, LPCRECT lpRectScroll, LPCRECT lpRectClip, CRgn* pRgnUpdate, LPRECT lpRectUpdate)
		{ ASSERT(m_hDC != NULL); return ::ScrollDC(m_hDC, dx, dy, lpRectScroll, lpRectClip, (HRGN)pRgnUpdate->GetSafeHandle(), lpRectUpdate); }

		BOOL MaskBlt(int x, int y, int nWidth, int nHeight, CDCMTL* pSrcDC, int xSrc, int ySrc, CBitmap& maskBitmap, int xMask, int yMask, DWORD dwRop)
		{
			ASSERT(m_hDC != NULL); return ::MaskBlt(m_hDC, x, y, nWidth, nHeight, pSrcDC->GetSafeHdc(),
													xSrc, ySrc, (HBITMAP)maskBitmap.m_hBitmap, xMask, yMask, dwRop);
		}
		BOOL PlgBlt(LPPOINT lpPoint, CDCMTL* pSrcDC, int xSrc, int ySrc, int nWidth, int nHeight, CBitmap& maskBitmap, int xMask, int yMask)
		{
			ASSERT(m_hDC != NULL); return ::PlgBlt(m_hDC, lpPoint, pSrcDC->GetSafeHdc(), xSrc, ySrc, nWidth,
													nHeight, (HBITMAP)maskBitmap.m_hBitmap, xMask, yMask);
		}

		CPen* GetCurrentPen() { ASSERT(m_hDC != NULL); return CPen::FromHandle((HPEN)::GetCurrentObject(m_hDC, OBJ_PEN)); }
		CBrush* GetCurrentBrush() { ASSERT(m_hDC != NULL); return CBrush::FromHandle((HBRUSH)::GetCurrentObject(m_hDC, OBJ_BRUSH)); }
		CPalette* GetCurrentPalette() { ASSERT(m_hDC != NULL); return CPalette::FromHandle((HPALETTE)::GetCurrentObject(m_hDC, OBJ_PAL)); }
		CFont* GetCurrentFont() { ASSERT(m_hDC != NULL); return CFont::FromHandle((HFONT)::GetCurrentObject(m_hDC, OBJ_FONT)); }
		CBitmap* GetCurrentBitmap() { ASSERT(m_hDC != NULL); return CBitmap::FromHandle((HBITMAP)::GetCurrentObject(m_hDC, OBJ_BITMAP)); }

		float GetMiterLimit() const
		{ ASSERT(m_hDC != NULL); float fMiterLimit; VERIFY(::GetMiterLimit(m_hDC, &fMiterLimit)); return fMiterLimit; }


};


#define CDC CDCMTL

class CPreviewDC : public CDCMTL
{
	public:
		CPreviewDC() : afxData(CAfx::GetAfxData())
		{
			// Initial scale factor and top-left offset
			m_nScaleNum = m_nScaleDen = 1;
			m_sizeTopLeft.cx = m_sizeTopLeft.cy = 8;
			m_hFont = m_hPrinterFont = NULL;
		}

		virtual ~CPreviewDC()
		{
			ASSERT(m_hDC == NULL);      // Should not have a screen DC at this time
			AfxDeleteObject((HGDIOBJ*)&m_hFont);
		}

	public:
		const struct _AUX_DATA& afxData;

		int m_nScaleNum;    // Scale ratio Numerator
		int m_nScaleDen;    // Scale ratio Denominator
		int m_nSaveDCIndex; // DC Save index when Screen DC Attached
		int m_nSaveDCDelta; // delta between Attrib and output restore indices
		CSize m_sizeTopLeft;// Offset for top left corner of page
		HFONT m_hFont;      // Font selected into the screen DC (NULL if none)
		HFONT m_hPrinterFont; // Font selected into the print DC

		CSize m_sizeWinExt; // cached window extents computed for screen
		CSize m_sizeVpExt;  // cached viewport extents computed for screen

	public:
		static long _AfxMultMultDivDiv(int factor, int num1, int num2, int den1, int den2)
		{
			__int64 numerator = (__int64)num1 * (__int64)num2;   // no overflow
			__int64 denominator = (__int64)den1 * (__int64)den2; // no overflow
			__int64 temp;

			temp = numerator < 0 ? -numerator : numerator;
			int nBitsInNum;
			for (nBitsInNum = 0; temp != 0; nBitsInNum++)
				temp >>= 1;

			temp = factor < 0 ? -factor : factor;
			int nBitsInFactor;
			for (nBitsInFactor = 0; temp != 0; nBitsInFactor++)
				temp >>= 1;

			nBitsInNum += nBitsInFactor;

			//
			// normalizing the denominator to positive results in an easier
			// determination of whether there is overflow
			//
			if (denominator < 0)
			{
				denominator = -denominator;
				numerator = -numerator;
			}

			// Get the product of factor * numerator representable in a long/__int64
			// while distributing loss of presision across all three numerator terms
			// Adjust denominator as well
			//
			while (nBitsInNum-- > 31)
			{
				numerator >>= 1;
				denominator >>= 1;
				if (nBitsInNum-- <= 31)
					break;
				numerator >>= 1;
				denominator >>= 1;
				if (nBitsInNum-- <= 31)
					break;
				factor >>= 1;
				denominator >>= 1;
			}
			numerator *= factor;

			if (denominator == 0)
				return numerator < 0 ? LONG_MIN : LONG_MAX;

			return (long)((numerator + denominator / 2) / denominator);
		}


		void ClipToPage()
		{
			ASSERT(m_hAttribDC != NULL);
			ASSERT(m_hDC != NULL);
			// Create a rect in Screen Device coordinates that is one pixel larger
			// on all sides than the actual page.  This is to hide the fact that
			// the printer to screen mapping mode is approximate and may result
			// in rounding error.

			CPoint pt(::GetDeviceCaps(m_hAttribDC, HORZRES), ::GetDeviceCaps(m_hAttribDC, VERTRES));
			PrinterDPtoScreenDP(&pt);

			// Set the screen dc to MM_TEXT and no WindowOrg for the interesection

			::SetMapMode(m_hDC, MM_TEXT);
			::SetWindowOrgEx(m_hDC, 0, 0, NULL);
			::SetViewportOrgEx(m_hDC, m_sizeTopLeft.cx, m_sizeTopLeft.cy, NULL);
			::IntersectClipRect(m_hDC, -1, -1, pt.x + 2, pt.y + 2);

			// Resynchronize the mapping mode
			MirrorMappingMode(FALSE);
		}

		void MirrorFont()
		{
			if (m_hAttribDC == NULL)
				return;         // Can't do anything without Attrib DC

			if (m_hPrinterFont == NULL)
			{
				SelectStockObject(DEVICE_DEFAULT_FONT); // will recurse
				return;
			}

			if (m_hDC == NULL)
				return;         // can't mirror font without a screen DC

			LOGFONT logFont;
			// Fill the logFont structure with the original info
			::GetObject(m_hPrinterFont, sizeof(LOGFONT), (LPVOID)&logFont);

			TEXTMETRIC tm;

			GetTextFace(LF_FACESIZE, (LPTSTR)&logFont.lfFaceName[0]);
			GetTextMetrics(&tm);

			// Set real values based on the Printer's text metrics.

			if (tm.tmHeight < 0)
				logFont.lfHeight = tm.tmHeight;
			else
				logFont.lfHeight = -(tm.tmHeight - tm.tmInternalLeading);

			logFont.lfWidth = tm.tmAveCharWidth;
			logFont.lfWeight = tm.tmWeight;
			logFont.lfItalic = tm.tmItalic;
			logFont.lfUnderline = tm.tmUnderlined;
			logFont.lfStrikeOut = tm.tmStruckOut;
			logFont.lfCharSet = tm.tmCharSet;
			logFont.lfPitchAndFamily = tm.tmPitchAndFamily;

			HFONT hNewFont = ::CreateFontIndirect(&logFont);
			::SelectObject(m_hDC, hNewFont);

			::GetTextMetrics(m_hDC, &tm);

			// Is the displayed font too large?

			int cyDesired = -logFont.lfHeight;
			int cyActual;
			if (tm.tmHeight < 0)
				cyActual = -tm.tmHeight;
			else
				cyActual = tm.tmHeight - tm.tmInternalLeading;

			CSize sizeWinExt;
			VERIFY(::GetWindowExtEx(m_hDC, &sizeWinExt));
			CSize sizeVpExt;
			VERIFY(::GetViewportExtEx(m_hDC, &sizeVpExt));

			// Only interested in Extent Magnitudes, not direction
			if (sizeWinExt.cy < 0)
				sizeWinExt.cy = -sizeWinExt.cy;
			if (sizeVpExt.cy < 0)
				sizeVpExt.cy = -sizeVpExt.cy;

			// Convert to screen device coordinates to eliminate rounding
			// errors as a source of SmallFont aliasing

			cyDesired = MulDiv(cyDesired, sizeVpExt.cy, sizeWinExt.cy);
			cyActual = MulDiv(cyActual, sizeVpExt.cy, sizeWinExt.cy);

			ASSERT(cyDesired >= 0 && cyActual >= 0);

			if (cyDesired < cyActual)
			{
				logFont.lfFaceName[0] = 0;      // let the mapper find a good fit

				if ((logFont.lfPitchAndFamily & 0xf0) == FF_DECORATIVE)
					logFont.lfPitchAndFamily = DEFAULT_PITCH | FF_DECORATIVE;
				else
					logFont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

				HFONT hTempFont = ::CreateFontIndirect(&logFont);
				::SelectObject(m_hDC, hTempFont);           // Select it in.
				::DeleteObject(hNewFont);
				hNewFont = hTempFont;
			}

			AfxDeleteObject((HGDIOBJ*)&m_hFont);  // delete the old logical font
			m_hFont = hNewFont;         // save the new one
		}

		void MirrorMappingMode(BOOL bCompute)
		{
			ASSERT(m_hAttribDC != NULL);
			if (bCompute)
			{
				//
				// The following formula is used to compute the screen's viewport extent
				// From the printer and screen information and the Printer's Viewport
				// Extents.  (Note:  This formula is used twice, once for horizontal
				// and once for vertical)
				//
				// It is assumed that the Window Extents are maintained as equal.
				//
				//                  m * LogPixPerInch(Screen) * VpExt(Printer)
				// VpExt(Screen) = -------------------------------------------------
				//                          n * LogPixPerInch(Printer)
				//
				// Where m/n is the scaling factor.  (m/n > 1 is expansion)
				//

				VERIFY(::GetViewportExtEx(m_hAttribDC, &m_sizeVpExt));
				VERIFY(::GetWindowExtEx(m_hAttribDC, &m_sizeWinExt));

				while (m_sizeWinExt.cx > -0x4000 && m_sizeWinExt.cx < 0x4000 &&
						m_sizeVpExt.cx  > -0x4000 && m_sizeVpExt.cx  < 0x4000)
				{
					m_sizeWinExt.cx <<= 1;
					m_sizeVpExt.cx <<= 1;
				}

				while (m_sizeWinExt.cy > -0x4000 && m_sizeWinExt.cy < 0x4000 &&
					m_sizeVpExt.cy  > -0x4000 && m_sizeVpExt.cy  < 0x4000)
				{
					m_sizeWinExt.cy <<= 1;
					m_sizeVpExt.cy <<= 1;
				}

				long lTempExt = _AfxMultMultDivDiv(m_sizeVpExt.cx,
									m_nScaleNum, afxData.cxPixelsPerInch,
									m_nScaleDen, ::GetDeviceCaps(m_hAttribDC, LOGPIXELSX));

				ASSERT(m_sizeWinExt.cx != 0);
				m_sizeVpExt.cx = (int)lTempExt;

				lTempExt = _AfxMultMultDivDiv(m_sizeVpExt.cy,
					m_nScaleNum, afxData.cyPixelsPerInch,
					m_nScaleDen, ::GetDeviceCaps(m_hAttribDC, LOGPIXELSY));

				ASSERT(m_sizeWinExt.cy != 0);
				m_sizeVpExt.cy = (int)lTempExt;
			}

			if (m_hDC != NULL)
			{
				::SetMapMode(m_hDC, MM_ANISOTROPIC);
				::SetWindowExtEx(m_hDC, m_sizeWinExt.cx, m_sizeWinExt.cy, NULL);
				::SetViewportExtEx(m_hDC, m_sizeVpExt.cx, m_sizeVpExt.cy, NULL);

				// Now that the Logical Units are synchronized, we can set the Viewport Org
				MirrorViewportOrg();
			}
		}

		void MirrorViewportOrg()
		{
			if (m_hAttribDC == NULL || m_hDC == NULL)
				return;

			CPoint ptVpOrg;
			VERIFY(::GetViewportOrgEx(m_hAttribDC, &ptVpOrg));
			PrinterDPtoScreenDP(&ptVpOrg);
			ptVpOrg += m_sizeTopLeft;
			::SetViewportOrgEx(m_hDC, ptVpOrg.x, ptVpOrg.y, NULL);

			CPoint ptWinOrg;
			VERIFY(::GetWindowOrgEx(m_hAttribDC, &ptWinOrg));
			::SetWindowOrgEx(m_hDC, ptWinOrg.x, ptWinOrg.y, NULL);
		}

		void PrinterDPtoScreenDP(LPPOINT lpPoint) const
		{
			ASSERT(m_hAttribDC != NULL);

			CSize sizePrinterVpExt;
			VERIFY(::GetViewportExtEx(m_hAttribDC, &sizePrinterVpExt));
			CSize sizePrinterWinExt;
			VERIFY(::GetWindowExtEx(m_hAttribDC, &sizePrinterWinExt));

			long xScreen = _AfxMultMultDivDiv(lpPoint->x,
				sizePrinterWinExt.cx, m_sizeVpExt.cx,
				sizePrinterVpExt.cx, m_sizeWinExt.cx);

			lpPoint->x = (int)xScreen;

			long yScreen = _AfxMultMultDivDiv(lpPoint->y,
				sizePrinterWinExt.cy, m_sizeVpExt.cy,
				sizePrinterVpExt.cy, m_sizeWinExt.cy);

			lpPoint->y = (int)yScreen;
		}

		int SaveDC()
		{
			ASSERT(m_hAttribDC != NULL);
			int nAttribIndex = ::SaveDC(m_hAttribDC);
			if (m_hDC != NULL)
			{
				// remove font from object
				::SelectObject(m_hDC, GetStockObject(SYSTEM_FONT));
				m_nSaveDCDelta = ::SaveDC(m_hDC) - nAttribIndex;
				// Select font back in after save
				::SelectObject(m_hDC, m_hFont);
			}
			else
			{
				m_nSaveDCDelta = 0x7fff;        // impossibly high value
			}
			return nAttribIndex;
		}

		BOOL RestoreDC(int nSavedDC)
		{
			ASSERT(m_hAttribDC != NULL);
			BOOL bSuccess = ::RestoreDC(m_hAttribDC, nSavedDC);
			if (bSuccess)
			{
				if (m_nSaveDCDelta != 0x7fff)
				{
					ASSERT(m_hDC != NULL);      // removed Output DC after save

					if (nSavedDC != -1)
						nSavedDC += m_nSaveDCDelta;
					bSuccess = ::RestoreDC(m_hDC, nSavedDC);
					MirrorFont();               // mirror the font
				}
				else
				{
					ASSERT(m_hDC == NULL);      // Added the Output DC after save
				}
			}
			return bSuccess;
		}

		CFont* SelectObject(CFont* pFont)
		{
			if (pFont == NULL)
				return NULL;

			ASSERT(m_hAttribDC != NULL);
			ASSERT_VALID(pFont);

			CFont* pOldFont = (CFont*)CFont::FromHandle((HFONT)::SelectObject(m_hAttribDC, pFont->m_hFont));

			// If same as already selected, don't re-mirror screen font
			if (m_hPrinterFont != pFont->m_hFont)
			{
				m_hPrinterFont = (HFONT)pFont->m_hFont;
				MirrorFont();
			}

			return pOldFont;
		}

		CGdiObject* SelectStockObject(int nIndex)
		{
			ASSERT(m_hAttribDC != NULL);

			HGDIOBJ hObj = ::GetStockObject(nIndex);

			switch (nIndex)
			{
			case ANSI_FIXED_FONT:
			case ANSI_VAR_FONT:
			case DEVICE_DEFAULT_FONT:
			case OEM_FIXED_FONT:
			case SYSTEM_FONT:
			case SYSTEM_FIXED_FONT:
			case DEFAULT_GUI_FONT:
				// Handle the stock fonts correctly
			{
				CGdiObject* pObject = CGdiObject::FromHandle(
					::SelectObject(m_hAttribDC, hObj));

				// Don't re-mirror screen font if this is the same font.
				if (m_hPrinterFont == (HFONT)hObj)
					return pObject;

				m_hPrinterFont = (HFONT)hObj;

				ASSERT(m_hPrinterFont != NULL); // Do not allow infinite recursion

				MirrorFont();
				return pObject;
			}

			default:
				if (m_hDC != NULL)
					::SelectObject(m_hDC, hObj);
				return CGdiObject::FromHandle(::SelectObject(m_hAttribDC, hObj));
			}
		}

		void SetScaleRatio(int nNumerator, int nDenominator)
		{
			m_nScaleNum = nNumerator;
			m_nScaleDen = nDenominator;
			if (m_hAttribDC != NULL)
			{
				MirrorMappingMode(TRUE);
				MirrorFont();
			}
		}

		void SetTopLeftOffset(CSize sizeTopLeft)
		{
			ASSERT(m_hAttribDC != NULL);
			m_sizeTopLeft = sizeTopLeft;
			MirrorViewportOrg();
		}

		/////////////////////////////////////////////////////////////////////////////
		// Drawing-Attribute Functions

		COLORREF SetBkColor(COLORREF crColor)
		{
			ASSERT(m_hAttribDC != NULL);
			if (m_hDC != NULL)
				::SetBkColor(m_hDC, ::GetNearestColor(m_hAttribDC, crColor));
			return ::SetBkColor(m_hAttribDC, crColor);
		}

		COLORREF SetTextColor(COLORREF crColor)
		{
			ASSERT(m_hAttribDC != NULL);
			if (m_hDC != NULL)
				::SetTextColor(m_hDC, ::GetNearestColor(m_hAttribDC, crColor));
			return ::SetTextColor(m_hAttribDC, crColor);
		}

		int SetMapMode(int nMapMode)
		{
			ASSERT(m_hAttribDC != NULL);
			int nModeOld = ::SetMapMode(m_hAttribDC, nMapMode);
			MirrorMappingMode(TRUE);
			return nModeOld;
		}

		CPoint SetViewportOrg(int x, int y)
		{
			ASSERT(m_hAttribDC != NULL);
			CPoint ptOrgOld;
			VERIFY(::SetViewportOrgEx(m_hAttribDC, x, y, &ptOrgOld));
			MirrorViewportOrg();
			return ptOrgOld;
		}

		CPoint OffsetViewportOrg(int nWidth, int nHeight)
		{
			ASSERT(m_hAttribDC != NULL);
			CPoint ptOrgOld;
			VERIFY(::OffsetViewportOrgEx(m_hAttribDC, nWidth, nHeight, &ptOrgOld));
			MirrorViewportOrg();
			return ptOrgOld;
		}

		CSize SetViewportExt(int x, int y)
		{
			ASSERT(m_hAttribDC != NULL);
			CSize sizeExtOld;
			VERIFY(::SetViewportExtEx(m_hAttribDC, x, y, &sizeExtOld));
			MirrorMappingMode(TRUE);
			return sizeExtOld;
		}

		CSize ScaleViewportExt(int xNum, int xDenom, int yNum, int yDenom)
		{
			ASSERT(m_hAttribDC != NULL);
			CSize sizeExtOld;
			VERIFY(::ScaleViewportExtEx(m_hAttribDC, xNum, xDenom,
				yNum, yDenom, &sizeExtOld));
			MirrorMappingMode(TRUE);
			return sizeExtOld;
		}

		CSize SetWindowExt(int x, int y)
		{
			ASSERT(m_hAttribDC != NULL);
			CSize sizeExtOld;
			VERIFY(::SetWindowExtEx(m_hAttribDC, x, y, &sizeExtOld));
			MirrorMappingMode(TRUE);
			return sizeExtOld;
		}

		CSize ScaleWindowExt(int xNum, int xDenom, int yNum, int yDenom)
		{
			ASSERT(m_hAttribDC != NULL);
			CSize sizeExtOld;
			VERIFY(::ScaleWindowExtEx(m_hAttribDC, xNum, xDenom, yNum, yDenom, &sizeExtOld));
			MirrorMappingMode(TRUE);
			return sizeExtOld;
		}

		/////////////////////////////////////////////////////////////////////////////
		// Text Functions

		// private helpers for TextOut functions

		static int _AfxComputeNextTab(int x, UINT nTabStops, LPINT lpnTabStops, int nTabOrigin, int nTabWidth)
		{
			ENSURE(nTabWidth != 0);
			x -= nTabOrigin;        // normalize position to tab origin
			for (UINT i = 0; i < nTabStops; i++, lpnTabStops++)
			{
				if (*lpnTabStops > x)
				{
					return *lpnTabStops + nTabOrigin;
				}
			}
			return (x / nTabWidth + 1) * nTabWidth + nTabOrigin;
		}

		// Compute a character delta table for correctly positioning the screen
		// font characters where the printer characters will appear on the page
		CSize ComputeDeltas(int& x, LPCTSTR lpszString, UINT &nCount,
							BOOL bTabbed, UINT nTabStops, LPINT lpnTabStops, int nTabOrigin, LPTSTR lpszOutputString, int* pnDxWidths, int& nRightFixup)
		{
			ASSERT_VALID(this);

			TEXTMETRIC tmAttrib;
			TEXTMETRIC tmScreen;
			::GetTextMetrics(m_hAttribDC, &tmAttrib);
			::GetTextMetrics(m_hDC, &tmScreen);

			CSize sizeExtent;
			::GetTextExtentPoint32W(m_hAttribDC, L"A", 1, &sizeExtent);

			CPoint ptCurrent;
			UINT nAlignment = ::GetTextAlign(m_hAttribDC);
			BOOL bUpdateCP = (nAlignment & TA_UPDATECP) != 0;
			if (bUpdateCP)
			{
				::GetCurrentPositionEx(m_hDC, &ptCurrent);
				x = ptCurrent.x;
			}

			LPCTSTR lpszCurChar = lpszString;
			LPCTSTR lpszStartRun = lpszString;
			int* pnCurDelta = pnDxWidths;
			int nStartRunPos = x;
			int nCurrentPos = x;
			int nStartOffset = 0;

			int nTabWidth = 0;
			if (bTabbed)
			{
				if (nTabStops == 1)
				{
					nTabWidth = lpnTabStops[0];
				}
				else
				{
					// Get default size of a tab
					nTabWidth = LOWORD(::GetTabbedTextExtentW(m_hAttribDC,
						L"\t", 1, 0, NULL));
				}
			}

			for (UINT i = 0; i < nCount; i++)
			{
				BOOL bSpace = ((_TUCHAR)*lpszCurChar == (_TUCHAR)tmAttrib.tmBreakChar);
				if (bSpace || (bTabbed && *lpszCurChar == '\t'))
				{
					// bSpace will be either TRUE (==1) or FALSE (==0).  For spaces
					// we want the space included in the GetTextExtent, for tabs we
					// do not want the tab included
					int nRunLength = (int)(lpszCurChar - lpszStartRun) + bSpace;

					CSize sizeExtent;
					::GetTextExtentPoint32(m_hAttribDC, lpszStartRun, nRunLength,
						&sizeExtent);
					int nNewPos = nStartRunPos + sizeExtent.cx
						- tmAttrib.tmOverhang;

					// now, if this is a Tab (!bSpace), compute the next tab stop
					if (!bSpace)
					{
						nNewPos = _AfxComputeNextTab(nNewPos, nTabStops, lpnTabStops,
							nTabOrigin, nTabWidth);
					}

					// Add this width to previous width
					if (pnCurDelta == pnDxWidths)
						nStartOffset += nNewPos - nCurrentPos;
					else
						*(pnCurDelta - 1) += nNewPos - nCurrentPos;

					nCurrentPos = nNewPos;

					nStartRunPos = nCurrentPos;     // set start of run
					// *lpszCurChar must be SBC: tmBreakChar or '\t'
					lpszStartRun = lpszCurChar + 1;
				}
				else
				{
					// For the non-tabbed or non-tab-character case
					int cxScreen;
					if (_istlead(*lpszCurChar) && i + 1 < nCount)
					{
						SIZE size;
						if (::GetTextExtentPoint(m_hDC, lpszCurChar, 2, &size))
							cxScreen = size.cx / 2;   // average of the 2 chars
						else
							cxScreen = tmScreen.tmAveCharWidth; // revert to default average

						if (::GetTextExtentPoint(m_hAttribDC, lpszCurChar, 2, &size))
							*pnCurDelta = size.cx / 2; // average of the 2 chars
						else
							*pnCurDelta = tmAttrib.tmAveCharWidth; // revert to default average
					}
					else
					{
						::GetCharWidth(m_hDC, (_TUCHAR)*lpszCurChar,
							(_TUCHAR)*lpszCurChar, &cxScreen);
						if (!::GetCharWidth(m_hAttribDC, (_TUCHAR)*lpszCurChar,
							(_TUCHAR)*lpszCurChar, pnCurDelta))
						{
							// If printer driver fails the above call, use the average width
							*pnCurDelta = tmAttrib.tmAveCharWidth;
						}
					}
					*pnCurDelta -= tmAttrib.tmOverhang;
					cxScreen -= tmScreen.tmOverhang;
					nCurrentPos += *pnCurDelta;     // update current position

					// Center character in allotted space
					if (pnCurDelta != pnDxWidths)
					{
						int diff = (*pnCurDelta - cxScreen) / 2;
						*pnCurDelta -= diff;
						*(pnCurDelta - 1) += diff;
					}
					*lpszOutputString++ = *lpszCurChar;
					if (_istlead(*lpszCurChar) && i + 1 < nCount)
					{
						*lpszOutputString++ = *(lpszCurChar + 1); // copy trailing byte
						*(pnCurDelta + 1) = *pnCurDelta;        // double wide
						nCurrentPos += *pnCurDelta;
						pnCurDelta++;
						i++;
					}
					pnCurDelta++;
				}
				lpszCurChar = _tcsinc(lpszCurChar);
			}

			nAlignment &= TA_CENTER | TA_RIGHT;
			sizeExtent.cx = nCurrentPos - x;
			nRightFixup = 0;

			if (nAlignment == TA_LEFT)
				x += nStartOffset;      // Full left side adjustment
			else if (nAlignment == TA_CENTER)
				x += nStartOffset / 2;    // Adjust Center by 1/2 left side adjustment
			else if (nAlignment == TA_RIGHT)
				nRightFixup = nStartOffset; // Right adjust needed later if TA_UPDATECP

			if (bUpdateCP)
				::MoveToEx(m_hDC, x, ptCurrent.y, NULL);

			nCount = (UINT)(pnCurDelta - pnDxWidths);   // number of characters output
			return sizeExtent;
		}

		BOOL TextOut(int x, int y, LPCTSTR lpszString, int nCount)
		{
			return ExtTextOut(x, y, 0, NULL, lpszString, nCount, NULL);
		}

		BOOL ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect, LPCTSTR lpszString, UINT nCount, LPINT lpDxWidths)
		{
			ASSERT(m_hDC != NULL);
			ASSERT(m_hAttribDC != NULL);
			ASSERT(lpszString != NULL);

			CAlloc<int> spDeltas;
			CAlloc<TCHAR> spOutputString;
			int nRightFixup = 0;

			if (lpDxWidths == NULL)
			{
				if (nCount == 0)    // Do nothing
					return TRUE;

				if (!spDeltas.Allocate(nCount) ||!spOutputString.Allocate(nCount))
				{
					return FALSE;
				}

				ComputeDeltas(x, (LPTSTR)lpszString, nCount, FALSE, 0, NULL, 0, spOutputString, spDeltas, nRightFixup);

				lpDxWidths = spDeltas;
				lpszString = spOutputString;
			}

			BOOL bSuccess = ::ExtTextOut(m_hDC, x, y, nOptions, lpRect, lpszString, nCount, lpDxWidths);
			if (nRightFixup != 0 && bSuccess && (GetTextAlign() & TA_UPDATECP))
			{
				CPoint pt;
				::GetCurrentPositionEx(m_hDC, &pt);
				MoveTo(pt.x - nRightFixup, pt.y);
			}

			return bSuccess;
		}

		CSize TabbedTextOut(int x, int y, LPCTSTR lpszString, int nCount, int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin)
		{
			ASSERT(m_hAttribDC != NULL);
			ASSERT(m_hDC != NULL);
			ASSERT(lpszString != NULL);

			if (nCount <= 0)
				return 0;         // nCount is zero, there is nothing to print

			int nRightFixup;

			CAlloc<int> spDeltas;
			CAlloc<TCHAR> spOutputString;
			if (!spDeltas.Allocate(nCount) || !spOutputString.Allocate(nCount))
			{
				return 0;
			}

			UINT uCount = nCount;
			CSize sizeFinalExtent = ComputeDeltas(x, lpszString, uCount, TRUE,
													nTabPositions, lpnTabStopPositions, nTabOrigin,
													spOutputString, spDeltas, nRightFixup);

			BOOL bSuccess = ExtTextOut(x, y, 0, NULL, spOutputString, uCount, spDeltas);

			if (bSuccess && (GetTextAlign() & TA_UPDATECP))
			{
				CPoint pt;
				::GetCurrentPositionEx(m_hDC, &pt);
				MoveTo(pt.x - nRightFixup, pt.y);
			}

			return sizeFinalExtent;
		}

		// This one is too complicated to do character-by-character output positioning
		// All we really need to do here is mirror the current position
		int DrawText(LPCTSTR lpszString, int nCount, LPRECT lpRect, UINT nFormat)
		{
			ASSERT(m_hAttribDC != NULL);
			ASSERT(m_hDC != NULL);
			ASSERT(lpszString != NULL);
			ASSERT(lpRect != NULL);

			int retVal = ::DrawText(m_hDC, lpszString, nCount, lpRect, nFormat);

			CPoint pos;
			::GetCurrentPositionEx(m_hDC, &pos);
			::MoveToEx(m_hAttribDC, pos.x, pos.y, NULL);
			return retVal;
		}

		int DrawTextEx(LPTSTR lpszString, int nCount, LPRECT lpRect,
			UINT nFormat, LPDRAWTEXTPARAMS lpDTParams)
		{
			ASSERT(m_hAttribDC != NULL);
			ASSERT(m_hDC != NULL);
			ASSERT(lpszString != NULL);
			ASSERT(lpRect != NULL);

			int retVal = ::DrawTextEx(m_hDC, lpszString, nCount, lpRect, nFormat, lpDTParams);

			CPoint pos;
			::GetCurrentPositionEx(m_hDC, &pos);
			::MoveToEx(m_hAttribDC, pos.x, pos.y, NULL);
			return retVal;
		}

		BOOL GrayString(CBrush*, BOOL(CALLBACK *)(HDC, LPARAM, int), LPARAM lpData, int nCount, int x, int y, int, int)
		{
			TRACE(traceAppMsg, 0, "TextOut() substituted for GrayString() in Print Preview.\n");
			return TextOut(x, y, (LPCTSTR)lpData, nCount);
		}

		int Escape(int nEscape, int nCount, LPCSTR lpszInData, void* lpOutData)
		{
			// The tact here is to NOT allow any of the document control escapes
			// to be passed through.  Elimination of StartDoc and EndDoc should
			// eliminate anything actually going to the printer.  Also anything
			// that actually draws something will be filtered.

			ASSERT(m_hAttribDC != NULL);

			switch (nEscape)
			{
				case NEXTBAND:
				case SETCOLORTABLE:
				case GETCOLORTABLE:
				case FLUSHOUTPUT:
				case DRAFTMODE:
				case QUERYESCSUPPORT:
				case GETPHYSPAGESIZE:
				case GETPRINTINGOFFSET:
				case GETSCALINGFACTOR:
				case GETPENWIDTH:
				case SETCOPYCOUNT:
				case SELECTPAPERSOURCE:
				case GETTECHNOLOGY:
				case SETLINECAP:
				case SETLINEJOIN:
				case SETMITERLIMIT:
				case BANDINFO:
				case GETVECTORPENSIZE:
				case GETVECTORBRUSHSIZE:
				case ENABLEDUPLEX:
				case GETSETPAPERBINS:
				case GETSETPRINTORIENT:
				case ENUMPAPERBINS:
				case SETDIBSCALING:
				case ENUMPAPERMETRICS:
				case GETSETPAPERMETRICS:
				case GETEXTENDEDTEXTMETRICS:
				case GETEXTENTTABLE:
				case GETPAIRKERNTABLE:
				case GETTRACKKERNTABLE:
				case ENABLERELATIVEWIDTHS:
				case ENABLEPAIRKERNING:
				case SETKERNTRACK:
				case SETALLJUSTVALUES:
				case SETCHARSET:
				case SET_BACKGROUND_COLOR:
				case SET_SCREEN_ANGLE:
				case SET_SPREAD:
					return ::Escape(m_hAttribDC, nEscape, nCount, lpszInData, lpOutData);

				default:
					return 0;
				}
		}

};


class CImage : public CBitmapMTL
{
	public:
		CImage() : m_pBits(NULL),
					m_hDC(NULL),
					m_nDCRefCount(0),
					m_hOldBitmap(NULL),
					m_nWidth(0),
					m_nHeight(0),
					m_nPitch(0),
					m_nBPP(0),
					m_iTransparentColor(-1),
					m_clrTransparentColor((COLORREF)-1),
					m_bHasAlphaChannel(false),
					m_bIsDIBSection(false), CBitmapMTL()
		{
		}

		~CImage()
		{
			Destroy();
		}

	public:
		enum DIBOrientation
		{
			DIBOR_DEFAULT,
			DIBOR_TOPDOWN,
			DIBOR_BOTTOMUP
		};

		enum ImageConst
		{
			createAlphaChannel = 0x01,

			excludeGIF = 0x01,
			excludeBMP = 0x02,
			excludeEMF = 0x04,
			excludeWMF = 0x08,
			excludeJPEG = 0x10,
			excludePNG = 0x20,
			excludeTIFF = 0x40,
			excludeIcon = 0x80,
			excludeOther = 0x80000000,
			excludeDefaultLoad = 0,
			excludeDefaultSave = excludeIcon|excludeEMF|excludeWMF,
			excludeValid = 0x800000ff
		};

		HDC m_hDC;
		int m_nDCRefCount;
		HBITMAP m_hOldBitmap;

		void* m_pBits;
		int m_nWidth;
		int m_nHeight;
		int m_nPitch;
		int m_nBPP;
		bool m_bIsDIBSection;
		bool m_bHasAlphaChannel;
		LONG m_iTransparentColor;
		COLORREF m_clrTransparentColor;

	public:
		void Attach(HBITMAP hBitmap, DIBOrientation eOrientation)
		{
			ASSERT(m_hBitmap == NULL);
			ATLASSERT(hBitmap != NULL);

			CBitmapMTL::Attach(hBitmap);

			UpdateBitmapInfo(eOrientation);
		}

		HBITMAP Detach()
		{
			ASSERT(m_hBitmap != NULL);
			ASSERT(m_hDC == NULL);

			HBITMAP hBitmap = CBitmapMTL::Detach();
			m_pBits = NULL;
			m_nWidth = 0;
			m_nHeight = 0;
			m_nBPP = 0;
			m_nPitch = 0;
			m_iTransparentColor = -1;
			m_clrTransparentColor = (COLORREF)-1;
			m_bHasAlphaChannel = false;
			m_bIsDIBSection = false;

			return(hBitmap);
		}

		static int ComputePitch(int nWidth, int nBPP) { return((((nWidth*nBPP) + 31) / 32) * 4); }

		BOOL CreateEx(int nWidth, int nHeight, int nBPP, DWORD eCompression, const DWORD* pdwBitfields, DWORD dwFlags)
		{
			LPBITMAPINFO pbmi;
			HBITMAP hBitmap;

			ATLASSERT((eCompression == BI_RGB) || (eCompression == BI_BITFIELDS));
			if (dwFlags & createAlphaChannel)
			{
				ATLASSERT((nBPP == 32) && (eCompression == BI_RGB));
			}

			UCHAR ucaBmi[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256] ;
			pbmi = (LPBITMAPINFO)ucaBmi ;

			memset(&pbmi->bmiHeader, 0, sizeof(pbmi->bmiHeader));
			pbmi->bmiHeader.biSize = sizeof(pbmi->bmiHeader);
			pbmi->bmiHeader.biWidth = nWidth;
			pbmi->bmiHeader.biHeight = nHeight;
			pbmi->bmiHeader.biPlanes = 1;
			pbmi->bmiHeader.biBitCount = USHORT(nBPP);
			pbmi->bmiHeader.biCompression = eCompression;
			if (nBPP <= 8)
			{
				ATLASSERT(eCompression == BI_RGB);
				memset(pbmi->bmiColors, 0, 256 * sizeof(RGBQUAD));
			}

			else
			{
				if (eCompression == BI_BITFIELDS)
				{
					ASSERT(pdwBitfields != NULL);
					memcpy(pbmi->bmiColors, pdwBitfields, 3 * sizeof(DWORD));
				}
			}

			hBitmap = ::CreateDIBSection(NULL, pbmi, DIB_RGB_COLORS, &m_pBits, NULL, 0);
			if (hBitmap == NULL)
			{
				return(FALSE);
			}

			Attach(hBitmap, (nHeight < 0) ? DIBOR_TOPDOWN : DIBOR_BOTTOMUP);

			if (dwFlags&createAlphaChannel)
			{
				m_bHasAlphaChannel = true;
			}

			return(TRUE);
		}

		void Destroy()
		{
			CBitmapMTL::DeleteObject() ;
		}

		void UpdateBitmapInfo(DIBOrientation eOrientation)
		{
			DIBSECTION dibsection;
			int nBytes;

			nBytes = ::GetObject(m_hBitmap, sizeof(DIBSECTION), &dibsection);
			if (nBytes == sizeof(DIBSECTION))
			{
				m_bIsDIBSection = true;
				m_nWidth = dibsection.dsBmih.biWidth;
				m_nHeight = abs(dibsection.dsBmih.biHeight);
				m_nBPP = dibsection.dsBmih.biBitCount;
				m_nPitch = ComputePitch(m_nWidth, m_nBPP);
				m_pBits = dibsection.dsBm.bmBits;
				if (eOrientation == DIBOR_DEFAULT)
				{
					eOrientation = (dibsection.dsBmih.biHeight > 0) ? DIBOR_BOTTOMUP : DIBOR_TOPDOWN;
				}
				if (eOrientation == DIBOR_BOTTOMUP)
				{
					m_pBits = LPBYTE(m_pBits) + ((m_nHeight - 1)*m_nPitch);
					m_nPitch = -m_nPitch;
				}
			}
			else
			{
				// Non-DIBSection
				ATLASSERT(nBytes == sizeof(BITMAP));
				m_bIsDIBSection = false;
				m_nWidth = dibsection.dsBm.bmWidth;
				m_nHeight = dibsection.dsBm.bmHeight;
				m_nBPP = dibsection.dsBm.bmBitsPixel;
				m_nPitch = 0;
				m_pBits = 0;
			}
			m_iTransparentColor = -1;
			m_bHasAlphaChannel = false;
		}

} ;


class CImageListMTL : public CHandleWrapper<CImageListMTL, HIMAGELIST, CImageListT<TRUE> >
{
	public:
		CImageListMTL(HIMAGELIST hHnd = NULL) : CHandleWrapper<CImageListMTL, HIMAGELIST, CImageListT<TRUE> >(hHnd) { }

	public:
		IMPLEMENT_HANDLE_CREATE(CImageListMTL, HIMAGELIST)

	public:
		BOOL Copy(int iDst, CImageListMTL* pSrc, int iSrc, UINT uFlags = ILCF_MOVE)
		{
			ASSERT(m_hImageList != NULL); ASSERT(pSrc != NULL && (HIMAGELIST)*pSrc != NULL); return ImageList_Copy(m_hImageList, iDst, *pSrc, iSrc, uFlags);
		}

};


